#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/interp.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#include "dev_pico.h"

int8_t do_calc(struct bme280_dev *bme_data0, struct bme280_dev *bme_data1);
void print_sensor_data(struct bme280_data *comp_data);

int main()
{
    stdio_init_all();

    int8_t odl = 10;
    while(odl >=0)
    {
        printf("Odliczanie %i\r\n", odl);
        --odl;
        sleep_ms(700);
    }

    i2c_init(I2C_PORT_0, 400*1000);
    i2c_init(I2C_PORT_1, 400*1000);
    
    gpio_set_function(I2C_SDA_0, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_0, GPIO_FUNC_I2C);
    //gpio_pull_up(I2C_SDA_0); // in my BME280 modules there are already pullup resistors
    //gpio_pull_up(I2C_SCL_0);

    gpio_set_function(I2C_SDA_1, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_1, GPIO_FUNC_I2C);
    //gpio_pull_up(I2C_SDA_1);
    //gpio_pull_up(I2C_SCL_1);

    printf("Init I2C done\r\n");

    identifier_t id0;
    struct bme280_dev dev0;
    identifier_t id1;
    struct bme280_dev dev1;

    id0.dev_addr = BME280_I2C_ADDR_PRIM;
    id0.i2c_ptr = i2c0;
    dev0.intf = BME280_I2C_INTF;
    dev0.read = user_i2c_read;
    dev0.write = user_i2c_write;
    dev0.delay_us = user_delay_us;
    dev0.intf_ptr = &id0;

    id1.dev_addr = BME280_I2C_ADDR_PRIM;
    id1.i2c_ptr = i2c1;
    dev1.intf = BME280_I2C_INTF;
    dev1.read = user_i2c_read;
    dev1.write = user_i2c_write;
    dev1.delay_us = user_delay_us;
    dev1.intf_ptr = &id1;

    printf("Checkpoint 1\r\n");
    sleep_ms(500);

    int8_t rslt = BME280_OK;
    rslt = bme280_init(&dev0);
    if (rslt != BME280_OK)
    {
        printf("Failed to initialize the device 0 (code %+d).\n", rslt);
        //exit(1);
    }
    rslt = bme280_init(&dev1);
    if (rslt != BME280_OK)
    {
        printf("Failed to initialize the device 1 (code %+d).\n", rslt);
        //exit(1);
    }
    printf("Checkpoint 10\r\n");
    sleep_ms(1000);
    do_calc(&dev0, &dev1);

    return 0;
}


int8_t do_calc(struct bme280_dev *bme_data0, struct bme280_dev *bme_data1)
{
    int8_t rslt = BME280_OK;
    uint8_t settings_sel = 0;
    uint32_t req_delay;
    struct bme280_data comp_data0;
    struct bme280_data comp_data1;
    bme_data0->settings.osr_h = BME280_OVERSAMPLING_1X;
    bme_data0->settings.osr_p = BME280_OVERSAMPLING_16X;
    bme_data0->settings.osr_t = BME280_OVERSAMPLING_2X;
    bme_data0->settings.filter = BME280_FILTER_COEFF_16;

    bme_data1->settings.osr_h = BME280_OVERSAMPLING_1X;
    bme_data1->settings.osr_p = BME280_OVERSAMPLING_16X;
    bme_data1->settings.osr_t = BME280_OVERSAMPLING_2X;
    bme_data1->settings.filter = BME280_FILTER_COEFF_16;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    rslt = bme280_set_sensor_settings(settings_sel, bme_data0);
    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to set sensor 0 settings (code %+d).", rslt);

        return rslt;
    }

    rslt = bme280_set_sensor_settings(settings_sel, bme_data1);
    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to set sensor 1 settings (code %+d).", rslt);

        return rslt;
    }

    req_delay = bme280_cal_meas_delay(&bme_data0->settings);

        /* Continuously stream sensor data */
    while (1)
    {
        /* Set the sensor to forced mode */
        rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, bme_data0);
        if (rslt != BME280_OK)
        {
            fprintf(stderr, "Failed to set sensor 0 mode (code %+d).", rslt);
            break;
        }
        rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, bme_data1);
        if (rslt != BME280_OK)
        {
            fprintf(stderr, "Failed to set sensor 1 mode (code %+d).", rslt);
            break;
        }

        /* Wait for the measurement to complete and print data */
        bme_data0->delay_us(req_delay, NULL);

        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data0, bme_data0);
        if (rslt != BME280_OK)
        {
            fprintf(stderr, "Failed to get sensor 0 data (code %+d).", rslt);
            break;
        }

        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data1, bme_data1);
        if (rslt != BME280_OK)
        {
            fprintf(stderr, "Failed to get sensor 1 data (code %+d).", rslt);
            break;
        }

        print_sensor_data(&comp_data0);
        sleep_ms(10);
        print_sensor_data(&comp_data1);
        sleep_ms(2000);
    }
}

void print_sensor_data(struct bme280_data *comp_data)
{
    float temp, press, hum;

#ifdef BME280_FLOAT_ENABLE
    temp = comp_data->temperature;
    press = 0.01 * comp_data->pressure;
    hum = comp_data->humidity;
#else
#ifdef BME280_64BIT_ENABLE
    temp = 0.01f * comp_data->temperature;
    press = 0.0001f * comp_data->pressure;
    hum = 1.0f / 1024.0f * comp_data->humidity;
#else
    temp = 0.01f * comp_data->temperature;
    press = 0.01f * comp_data->pressure;
    hum = 1.0f / 1024.0f * comp_data->humidity;
#endif
#endif
    printf("%0.2lf deg C, %0.2lf hPa, %0.2lf%%\r\n", temp, press, hum);
}