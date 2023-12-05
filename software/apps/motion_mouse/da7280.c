#include <math.h>

#include "da7280.h"
#include "i2c.h"

void da7280_set_actuator_type(da7280_actuator_t actuator_type) {
  i2c_ext_reg_write_bits(DA7280_ADDRESS, TOP_CFG1_REGISTER, 0xDF, 5, actuator_type);
}

void da7280_set_actuator_abs_volt(float voltage) {
  voltage = voltage / (23.4 * pow(10, -3));
  i2c_ext_reg_write(DA7280_ADDRESS, ACTUATOR2_REGISTER, (uint8_t) voltage);
}

void da7280_set_actuator_nom_volt(float voltage) {
  voltage = voltage / (23.4 * pow(10, -3));
  i2c_ext_reg_write(DA7280_ADDRESS, ACTUATOR1_REGISTER, (uint8_t) voltage);
}

void da7280_set_actuator_imax(float current) {
    current = (current - 28.6) / 7.2;
    i2c_ext_reg_write(DA7280_ADDRESS, ACTUATOR3_REGISTER, (uint8_t) current);
}

void da7280_set_actuator_impedance(float impedance) {
    
    uint8_t maxCurr = i2c_ext_reg_read(DA7280_ADDRESS, ACTUATOR3_REGISTER) | 0x1F;
    //calculating impedance with ~math~
    uint16_t v2i_factor = (impedance * (maxCurr + 4)) / 1.6104;
    uint8_t msb_impedance = (v2i_factor - (v2i_factor & 0x00FF)) / 256;
    uint8_t lsb_impedance = (v2i_factor - (256 * (v2i_factor & 0x00FF)));

    i2c_ext_reg_write(DA7280_ADDRESS, CALIB_V2I_L, lsb_impedance);
    i2c_ext_reg_write(DA7280_ADDRESS, CALIB_V2I_H, msb_impedance);
}

void da7280_set_actuator_lra_freq(float frequency) {
    uint16_t lra_period = 1 / (frequency * (1333.32 * pow(10, -9)));
    uint8_t msb_frequency = (lra_period - (lra_period & 0x007F)) / 128;
    uint8_t lsb_frequency = (lra_period - 128 * (lra_period & 0xFF00));
    
    i2c_ext_reg_write(DA7280_ADDRESS, FRQ_LRA_PER_H, msb_frequency);
    i2c_ext_reg_write(DA7280_ADDRESS, FRQ_LRA_PER_L, lsb_frequency);
}

void da7280_set_operation_mode(da7280_operation_mode_t operation_mode){
    //we are masking it here with the 0xF8 so we don't overwrite prev data 
    i2c_ext_reg_write_bits(DA7280_ADDRESS, TOP_CTL1_REGISTER, 0xF8, 0, operation_mode);
}

void da7280_enable_freq_track(bool enable){
    i2c_ext_reg_write_bits(DA7280_ADDRESS, TOP_CFG1_REGISTER, 0xF7, 3, enable);
}


void da7280_init(void) {
  da7280_set_actuator_type(LRA_TYPE);
  da7280_set_actuator_abs_volt(2.26);
  da7280_set_actuator_nom_volt(0);
  da7280_set_actuator_imax(165.4);
  da7280_set_actuator_impedance(13.8);
  da7280_set_actuator_lra_freq(170);

  da7280_enable_freq_track(false);

  da7280_set_operation_mode(DRO_MODE);
}

// max is 127 with acceleration on
void da7280_set_vibrate(uint8_t val) {
  i2c_ext_reg_write(DA7280_ADDRESS, TOP_CTL2_REGISTER, val);
}
