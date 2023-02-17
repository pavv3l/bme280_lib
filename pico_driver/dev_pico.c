#include "dev_pico.h"


// i2c_write_timeout_us and i2c_read_timeout_us are blocking!

void user_delay_us(uint32_t period, void *intf_ptr)
{
    sleep_us(period);
}

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    identifier_t* id_ptr = (identifier_t*)intf_ptr;
    int rslt = 0;

    rslt = i2c_write_timeout_us(id_ptr->i2c_ptr, id_ptr->dev_addr, &reg_addr, 1, true, MAX_I2C_TIMEOUT);
    if(rslt == PICO_ERROR_GENERIC)
    {
        if(rslt == PICO_ERROR_GENERIC)
        {
            printf("user_i2c_read address not ack\r\n");
        }
        else
        {
            printf("user_i2c_read error happen\r\n");
        }
        return -1;
    }
    rslt = i2c_read_timeout_us(id_ptr->i2c_ptr, id_ptr->dev_addr, data, len, false, MAX_I2C_TIMEOUT);
    if(rslt == PICO_ERROR_GENERIC)
    {
        if(rslt == PICO_ERROR_GENERIC)
        {
            printf("user_i2c_read address not ack\r\n");
        }
        else
        {
            printf("user_i2c_read error happen\r\n");
        }
        return -1;
    }
    return 0;
}

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    identifier_t* id_ptr = (identifier_t*)intf_ptr;
    uint8_t buf[len + 1];
    buf[0] = reg_addr;
    memcpy((buf + 1), data, len);
    int rslt = 0;
    rslt = i2c_write_timeout_us(id_ptr->i2c_ptr, id_ptr->dev_addr , buf, len + 1, true, MAX_I2C_TIMEOUT);
    if(rslt == PICO_ERROR_GENERIC)
    {
        if(rslt == PICO_ERROR_GENERIC)
        {
            printf("user_i2c_write address not ack\r\n");
        }
        else
        {
            printf("user_i2c_write error happen\r\n");
        }
        return -1;
    }
    return 0;
}