// LSM303AGR driver for Microbit_v2
//
// Initializes sensor and communicates over I2C
// Capable of reading temperature, acceleration, and magnetic field strength

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "lsm303agr.h"
#include "nrf_delay.h"
#include "i2c.h"

// Initialize and configure the LSM303AGR accelerometer/magnetometer
//
// i2c - pointer to already initialized and enabled twim instance
void lsm303agr_init() {
  // ---Initialize Accelerometer---

  // Reboot acclerometer
  i2c_int_reg_write(LSM303AGR_ACC_ADDRESS, CTRL_REG5_A, 0x80);
  nrf_delay_ms(100); // needs delay to wait for reboot

  // Enable Block Data Update
  // Only updates sensor data when both halves of the data has been read
  i2c_int_reg_write(LSM303AGR_ACC_ADDRESS, CTRL_REG4_A, 0x80);

  // Configure accelerometer at 100Hz, normal mode (10-bit)
  // Enable x, y and z axes
  i2c_int_reg_write(LSM303AGR_ACC_ADDRESS, CTRL_REG1_A, 0x57);

  // Read WHO AM I register
  // Always returns the same value if working
  uint8_t who_am_i_acc = i2c_int_reg_read(LSM303AGR_ACC_ADDRESS, WHO_AM_I_A);
  // printf("0x%x\n", who_am_i_acc);

  // ---Initialize Magnetometer---

  // Reboot magnetometer
  i2c_int_reg_write(LSM303AGR_MAG_ADDRESS, CFG_REG_A_M, 0x40);
  nrf_delay_ms(100); // needs delay to wait for reboot

  // Enable Block Data Update
  // Only updates sensor data when both halves of the data has been read
  i2c_int_reg_write(LSM303AGR_MAG_ADDRESS, CFG_REG_C_M, 0x10);

  // Configure magnetometer at 100Hz, continuous mode
  i2c_int_reg_write(LSM303AGR_MAG_ADDRESS, CFG_REG_A_M, 0x0C);

  // Read WHO AM I register
  uint8_t who_am_i_mag = i2c_int_reg_read(LSM303AGR_MAG_ADDRESS, WHO_AM_I_M);
  // printf("0x%x\n", who_am_i_mag);

  // Enable temperature sensor
  i2c_int_reg_write(LSM303AGR_ACC_ADDRESS, TEMP_CFG_REG_A, 0xC0);
}

// Read the internal temperature sensor
//
// Return measurement as floating point value in degrees C
float lsm303agr_read_temperature(void) {
  uint8_t temp_l = i2c_int_reg_read(LSM303AGR_ACC_ADDRESS, OUT_TEMP_L_A);
  uint8_t temp_h = i2c_int_reg_read(LSM303AGR_ACC_ADDRESS, OUT_TEMP_H_A);
  int16_t temp = (temp_h << 8) | temp_l;

  return ((float) temp * (1.0 / 256.0)) + 25.0;
}

lsm303agr_measurement_t lsm303agr_read_accelerometer(void) {
  lsm303agr_measurement_t measurement = {0};

  uint8_t x_l = i2c_int_reg_read(LSM303AGR_ACC_ADDRESS, OUT_X_L_A);
  uint8_t x_h = i2c_int_reg_read(LSM303AGR_ACC_ADDRESS, OUT_X_H_A);
  int16_t x = (x_h << 8) | x_l;
  measurement.x_axis = ((float) (x >> 6) * 3.9) / 1000.0;

  uint8_t y_l = i2c_int_reg_read(LSM303AGR_ACC_ADDRESS, OUT_Y_L_A);
  uint8_t y_h = i2c_int_reg_read(LSM303AGR_ACC_ADDRESS, OUT_Y_H_A);
  int16_t y = (y_h << 8) | y_l;
  measurement.y_axis = ((float) (y >> 6) * 3.9) / 1000.0;

  uint8_t z_l = i2c_int_reg_read(LSM303AGR_ACC_ADDRESS, OUT_Z_L_A);
  uint8_t z_h = i2c_int_reg_read(LSM303AGR_ACC_ADDRESS, OUT_Z_H_A);
  int16_t z = (z_h << 8) | z_l;
  measurement.z_axis = ((float) (z >> 6) * 3.9) / 1000.0;
  
  return measurement;
}

lsm303agr_measurement_t lsm303agr_read_magnetometer(void) {
  lsm303agr_measurement_t measurement = {0};

  uint8_t x_l = i2c_int_reg_read(LSM303AGR_MAG_ADDRESS, OUTX_L_REG_M);
  uint8_t x_h = i2c_int_reg_read(LSM303AGR_MAG_ADDRESS, OUTX_H_REG_M);
  int16_t x = (x_h << 8) | x_l;
  measurement.x_axis = ((float) (x) * 1.5) / 10.0;

  uint8_t y_l = i2c_int_reg_read(LSM303AGR_MAG_ADDRESS, OUTY_L_REG_M);
  uint8_t y_h = i2c_int_reg_read(LSM303AGR_MAG_ADDRESS, OUTY_H_REG_M);
  int16_t y = (y_h << 8) | y_l;
  measurement.y_axis = ((float) (y) * 1.5) / 10.0;

  uint8_t z_l = i2c_int_reg_read(LSM303AGR_MAG_ADDRESS, OUTZ_L_REG_M);
  uint8_t z_h = i2c_int_reg_read(LSM303AGR_MAG_ADDRESS, OUTZ_H_REG_M);
  int16_t z = (z_h << 8) | z_l;
  measurement.z_axis = ((float) (z) * 1.5) / 10.0;

  return measurement;
}

lsm303agr_measurement_t lsm303agr_calculate_tilt(lsm303agr_measurement_t acc) {
  lsm303agr_measurement_t measurement = {0};

  //measuring x-axis using theta
  float theta_numerator = acc.x_axis;
  float theta_denominator = sqrt(pow(acc.y_axis, 2) + pow(acc.z_axis, 2));
  float theta_angle_rad = atan(theta_numerator / theta_denominator);
  float theta = theta_angle_rad * (180.0 / M_PI);
  measurement.x_axis = theta;
  
  //measuring y-axis using phi
  float phi_numerator = sqrt(pow(acc.x_axis, 2) + pow(acc.y_axis, 2));
  float phi_denominator = acc.z_axis;
  float phi_angle_rad = atan(phi_numerator / phi_denominator);
  float phi = phi_angle_rad * (180.0 / M_PI);
  measurement.y_axis = phi;

  //measuring z-axis using psi
  float psi_numerator = acc.y_axis;
  float psi_denominator = sqrt(pow(acc.x_axis, 2) + pow(acc.z_axis, 2));
  float psi_angle_rad = atan (psi_numerator / psi_denominator);
  float psi = psi_angle_rad * (180.0 / M_PI);
  measurement.z_axis = psi;
  
  return measurement;
}
