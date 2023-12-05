#include "amg88.h"
#include "i2c.h"

static float amg88_read_pixel_temp(uint8_t pixel) {
  uint8_t lsb = i2c_ext_reg_read(AMG88_ADDRESS, TEMPERATURE_REGISTER_START + (pixel * 2));
  uint8_t msb = i2c_ext_reg_read(AMG88_ADDRESS, TEMPERATURE_REGISTER_START + (pixel * 2) + 1);
  int16_t temp = (msb << 8) | lsb;
  return temp * 0.25;
}

void amg88_init(void) {
  i2c_ext_reg_write(AMG88_ADDRESS, POWER_CONTROL_REGISTER, 0x00);
  i2c_ext_reg_write(AMG88_ADDRESS, RESET_REGISTER, 0x3F);
  i2c_ext_reg_write(AMG88_ADDRESS, FRAMERATE_REGISTER, 0x00);
}

ir_measurement_t amg88_read_pixels(float threshold) {
  ir_measurement_t measurement = {0};

  for (int i = 0; i < 64; i++) {
    float temp = amg88_read_pixel_temp(i);
    measurement.pixels[i] = temp > threshold;
  }

  return measurement;
}

ir_temp_measurement_t amg88_read_pixels_temp(void) {
  ir_temp_measurement_t measurement = {0};

  for (int i = 0; i < 64; i++) {
    measurement.temperature[i] = amg88_read_pixel_temp(i);
  }

  return measurement;
}


void amg88_print_pixels(ir_measurement_t measurement) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (measurement.pixels[i * 8 + j]) {
        printf("X");
      } else {
        printf(" ");
      }
      
    }
    printf("\n");
  }
  printf("\n\n");
}
