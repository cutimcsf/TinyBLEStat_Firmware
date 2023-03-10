/*
 *  peripherals/i2c/i2c_lmp_config.c
 *
 *  Created on: January 21, 2023
 *      Author: Timothy C. Sweeney-Fanelli
 *              Affects AI
 *              tim@affects.ai
 *
 *  I2C Initialization Structures for the two LMP91000 chips.
 */

#include "i2c_lmp_config.h"
#include "../adc.h"
#include <em_adc.h>

sl_i2cspm_t *i2c_lmp1 = I2C_LMP1_PERIPHERAL;
sl_i2cspm_t *i2c_lmp2 = I2C_LMP2_PERIPHERAL;

I2CSPM_Init_TypeDef init_lmp1 = {
  .port = I2C_LMP1_PERIPHERAL,
  .sclPort = I2C_LMP1_SCL_PORT,
  .sclPin = I2C_LMP1_SCL_PIN,
  .sdaPort = I2C_LMP1_SDA_PORT,
  .sdaPin = I2C_LMP1_SDA_PIN,
  .portLocationScl = I2C_LMP1_SCL_LOC,
  .portLocationSda = I2C_LMP1_SDA_LOC,
  .i2cRefFreq = 0,
  .i2cMaxFreq = I2C_LMP1_MAX_FREQ,
  .i2cClhr = I2C_LMP1_HLR
};

I2CSPM_Init_TypeDef init_lmp2 = {
  .port = I2C_LMP2_PERIPHERAL,
  .sclPort = I2C_LMP2_SCL_PORT,
  .sclPin = I2C_LMP2_SCL_PIN,
  .sdaPort = I2C_LMP2_SDA_PORT,
  .sdaPin = I2C_LMP2_SDA_PIN,
  .portLocationScl = I2C_LMP2_SCL_LOC,
  .portLocationSda = I2C_LMP2_SDA_LOC,
  .i2cRefFreq = 0,
  .i2cMaxFreq = I2C_LMP2_MAX_FREQ,
  .i2cClhr = I2C_LMP2_HLR
};

LMP91000_Config lmp91000_config_1 = {
    .i2c_lmp = I2C_LMP1_PERIPHERAL,
    .i2c_init = &init_lmp1,
    .enlow_port = LMP91000_1_ENLOW_PORT,
    .enlow_pin = LMP91000_1_ENLOW_PIN,
    .temperature_port = LMP91000_1_TEMPERATURE_PORT,
    .temperature_pin = LMP91000_1_TEMPERATURE_PIN,
    .adcDataPin = LMP91000_1_DATA_PIN,
};

LMP91000_Config lmp91000_config_2 = {
    .i2c_lmp = I2C_LMP2_PERIPHERAL,
    .i2c_init = &init_lmp2,
    .enlow_port = LMP91000_2_ENLOW_PORT,
    .enlow_pin = LMP91000_2_ENLOW_PIN,
    .temperature_port = LMP91000_2_TEMPERATURE_PORT,
    .temperature_pin = LMP91000_2_TEMPERATURE_PIN,
    .adcDataPin = LMP91000_2_DATA_PIN,
};
