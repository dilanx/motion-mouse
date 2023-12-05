#pragma once

#include <stdint.h>

#include "nrf_twi_mngr.h"

uint8_t i2c_int_reg_read(uint8_t i2c_addr, uint8_t reg_addr);
void i2c_int_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data);
void i2c_int_reg_write_bits(uint8_t i2c_addr, uint8_t reg_addr, uint8_t mask, uint8_t starting_bit, uint8_t data);

uint8_t i2c_ext_reg_read(uint8_t i2c_addr, uint8_t reg_addr);
void i2c_ext_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data);
void i2c_ext_reg_write_bits(uint8_t i2c_addr, uint8_t reg_addr, uint8_t mask, uint8_t starting_bit, uint8_t data);

void i2c_init();
