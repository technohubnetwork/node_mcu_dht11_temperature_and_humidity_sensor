#ifndef SENSOR_HELPER_H
#define SENSOR_HELPER_H

#include <Arduino.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <cmath>

uint32_t setup_serial_info_for_dht_and_get_min_delay(DHT_Unified dth);

void serial_log_temperature(sensors_event_t event);

void serial_log_humidity(sensors_event_t event);

void update_lcd_temperature(LiquidCrystal_I2C lcd, float temperature, float humidity);

#endif // SENSOR_HELPER_H