#ifndef __DEV_PICO_H__
#define __DEV_PICO_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib.h"
#include <stdio.h>
#include "string.h"
#include "hardware/i2c.h"
#include "bme280.h"

#define I2C_PORT_0 i2c0
#define I2C_SDA_0 8
#define I2C_SCL_0 9

#define I2C_PORT_1 i2c1
#define I2C_SDA_1 14
#define I2C_SCL_1 15

#define MAX_I2C_TIMEOUT 500000

// Structure that contains identifier details
typedef struct identifier
{
    uint8_t dev_addr;
    i2c_inst_t* i2c_ptr;
} identifier_t;

void user_delay_us(uint32_t period, void *intf_ptr);
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

#ifdef __cplusplus
}
#endif
#endif // __DEV_PICO_H__