// Infrared Array Sensor Grid-EYE (AMG88)

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "nrf_twi_mngr.h"

static const uint8_t AMG88_ADDRESS = 0x69;

typedef struct {
  bool pixels[64];
} ir_measurement_t;

typedef struct {
  float temperature[64];
} ir_temp_measurement_t;

typedef enum {
  POWER_CONTROL_REGISTER = 0x00,
  RESET_REGISTER = 0x01,
  FRAMERATE_REGISTER = 0x02,
  INT_CONTROL_REGISTER = 0x03,
  STATUS_REGISTER = 0x04,
  STATUS_CLEAR_REGISTER = 0x05,
  AVERAGE_REGISTER = 0x07,
  INT_LEVEL_REGISTER_UPPER_LSB = 0x08,
  INT_LEVEL_REGISTER_UPPER_MSB = 0x09,
  INT_LEVEL_REGISTER_LOWER_LSB = 0x0A,
  INT_LEVEL_REGISTER_LOWER_MSB = 0x0B,
  INT_LEVEL_REGISTER_HYST_LSB = 0x0C,
  INT_LEVEL_REGISTER_HYST_MSB = 0x0D,
  THERMISTOR_REGISTER_LSB = 0x0E,
  THERMISTOR_REGISTER_MSB = 0x0F,
  INT_TABLE_REGISTER_INT0 = 0x10,
  RESERVED_AVERAGE_REGISTER = 0x1F,
  TEMPERATURE_REGISTER_START = 0x80,
} amg88_reg_t;

void amg88_init(void);

ir_measurement_t amg88_read_pixels(float threshold);
ir_temp_measurement_t amg88_read_pixels_temp(void);

void amg88_print_pixels(ir_measurement_t measurement);
