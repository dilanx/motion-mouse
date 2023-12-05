// Haptic Driver - DA7280

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "nrf_twi_mngr.h"

static const uint8_t DA7280_ADDRESS = 0x4A;

typedef enum {
  CHIP_REV_REGISTER = 0x00,
  FRQ_LRA_PER_H = 0x0A,
  FRQ_LRA_PER_L = 0x0B,
  ACTUATOR1_REGISTER = 0x0C,
  ACTUATOR2_REGISTER = 0x0D,
  ACTUATOR3_REGISTER = 0x0E,
  CALIB_V2I_H = 0x0F,
  CALIB_V2I_L = 0x10,
  TOP_CFG1_REGISTER = 0x13,
  TOP_CTL1_REGISTER = 0x22,
  TOP_CTL2_REGISTER = 0x23,

} da7280_reg_t;

typedef enum {
  LRA_TYPE = 0x00,
  ERM_TYPE = 0x01,
} da7280_actuator_t;

typedef enum {
  INACTIVE = 0x00,
  DRO_MODE = 0x01,
  PWM_MODE = 0x02,
  RTWM_MODE = 0x03,
  ETWM_MODE = 0x04,
} da7280_operation_mode_t;

typedef enum {
  ENABLE = 0x01,
  DISABLE = 0x00,
} da7280_toggle_t;

void da7280_set_actuator_type(da7280_actuator_t actuator_type);
void da7280_set_actuator_abs_volt(float voltage);
void da7280_set_actuator_nom_volt(float voltage);
void da7280_set_actuator_imax(float current);
void da7280_set_actuator_impedance(float impedance);
void da7280_set_actuator_lra_freq(float frequency);

void da7280_enable_freq_track(bool enable);

void da7280_set_operation_mode(da7280_operation_mode_t operation_mode);

void da7280_init(void);

void da7280_set_vibrate(uint8_t val);
