#include "i2c.h"
#include "microbit_v2.h"

NRF_TWI_MNGR_DEF(twi_mngr_int_instance, 1, 0);
NRF_TWI_MNGR_DEF(twi_mngr_ext_instance, 1, 1);

static const nrf_twi_mngr_t* i2c_int_manager = NULL;
static const nrf_twi_mngr_t* i2c_ext_manager = NULL;

static uint8_t i2c_reg_read(const nrf_twi_mngr_t* mngr, uint8_t i2c_addr, uint8_t reg_addr) {
  uint8_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 1, 0),
  };

  ret_code_t result = nrf_twi_mngr_perform(mngr, NULL, read_transfer, 2, NULL);
  if (result != NRF_SUCCESS) {
    // Likely error codes:
    //  NRF_ERROR_INTERNAL            (0x0003) - something is wrong with the driver itself
    //  NRF_ERROR_INVALID_ADDR        (0x0010) - buffer passed was in Flash instead of RAM
    //  NRF_ERROR_BUSY                (0x0011) - driver was busy with another transfer still
    //  NRF_ERROR_DRV_TWI_ERR_OVERRUN (0x8200) - data was overwritten during the transaction
    //  NRF_ERROR_DRV_TWI_ERR_ANACK   (0x8201) - i2c device did not acknowledge its address
    //  NRF_ERROR_DRV_TWI_ERR_DNACK   (0x8202) - i2c device did not acknowledge a data byte
    printf("I2C transaction failed! Error: %lX\n", result);
  }

  return rx_buf;
}

static void i2c_reg_write(const nrf_twi_mngr_t* mngr, uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  //Note: there should only be a single two-byte transfer to be performed
  uint16_t transfer_buf = (data << 8) | reg_addr;
  nrf_twi_mngr_transfer_t const write_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, &transfer_buf, 2, 0),
  };

  ret_code_t result = nrf_twi_mngr_perform(mngr, NULL, write_transfer, 1, NULL);

  //uint16_t transfer_buf = (data << 8) | reg_addr;
  // nrf_twi_mngr_transfer_t const write_transfer[] = {
  //   NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
  //   NRF_TWI_MNGR_WRITE(i2c_addr, &data, 1, 0),
  // };

  // ret_code_t result = nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 2, NULL);

  if (result != NRF_SUCCESS) {
    // Likely error codes:
    //  NRF_ERROR_INTERNAL            (0x0003) - something is wrong with the driver itself
    //  NRF_ERROR_INVALID_ADDR        (0x0010) - buffer passed was in Flash instead of RAM
    //  NRF_ERROR_BUSY                (0x0011) - driver was busy with another transfer still
    //  NRF_ERROR_DRV_TWI_ERR_OVERRUN (0x8200) - data was overwritten during the transaction
    //  NRF_ERROR_DRV_TWI_ERR_ANACK   (0x8201) - i2c device did not acknowledge its address
    //  NRF_ERROR_DRV_TWI_ERR_DNACK   (0x8202) - i2c device did not acknowledge a data byte
    printf("I2C transaction failed! Error: %lX\n", result);
  }
}

// 1110 0101
// 0000 0000
// val |= 0 << 5;

// 1101 1111
// 1 << 5
// 0010 0000

// 1100 0101
// val |= 0 << 5;


static void i2c_reg_write_bits(
  const nrf_twi_mngr_t* mngr,
  uint8_t i2c_addr,
  uint8_t reg_addr,
  uint8_t mask,
  uint8_t starting_bit,
  uint8_t data
) {
  uint8_t val = i2c_reg_read(mngr, i2c_addr, reg_addr);
  val &= mask;
  val |= data << starting_bit;
  i2c_reg_write(mngr, i2c_addr, reg_addr, val);
}

uint8_t i2c_int_reg_read(uint8_t i2c_addr, uint8_t reg_addr) {
  return i2c_reg_read(i2c_int_manager, i2c_addr, reg_addr);
}

void i2c_int_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  i2c_reg_write(i2c_int_manager, i2c_addr, reg_addr, data);
}

void i2c_int_reg_write_bits(
  uint8_t i2c_addr,
  uint8_t reg_addr,
  uint8_t mask,
  uint8_t starting_bit,
  uint8_t data
) {
  i2c_reg_write_bits(i2c_int_manager, i2c_addr, reg_addr, mask, starting_bit, data);
}

uint8_t i2c_ext_reg_read(uint8_t i2c_addr, uint8_t reg_addr) {
  return i2c_reg_read(i2c_ext_manager, i2c_addr, reg_addr);
}

void i2c_ext_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  i2c_reg_write(i2c_ext_manager, i2c_addr, reg_addr, data);
}

void i2c_ext_reg_write_bits(
  uint8_t i2c_addr,
  uint8_t reg_addr,
  uint8_t mask,
  uint8_t starting_bit,
  uint8_t data
) {
  i2c_reg_write_bits(i2c_ext_manager, i2c_addr, reg_addr, mask, starting_bit, data);
}

void i2c_init() {
  nrf_drv_twi_config_t i2c_int_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_int_config.scl = I2C_SCL;
  i2c_int_config.sda = I2C_SDA;
  i2c_int_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_int_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_int_instance, &i2c_int_config);
  i2c_int_manager = &twi_mngr_int_instance;

  nrf_drv_twi_config_t i2c_ext_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_ext_config.scl = EDGE_P19;
  i2c_ext_config.sda = EDGE_P20;
  i2c_ext_config.frequency = NRF_TWIM_FREQ_400K;
  i2c_ext_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_ext_instance, &i2c_ext_config);
  i2c_ext_manager = &twi_mngr_ext_instance;
}
