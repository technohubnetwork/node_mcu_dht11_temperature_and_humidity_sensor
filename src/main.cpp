#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>
#include "sensor_helper.h"

#define DHT_TYPE DHT11

LiquidCrystal_I2C lcd(0x27, 16, 2);

DHT_Unified dht(D4, DHT_TYPE);

uint32_t delayMS;

String temperature;
String humidity;

void setup()
{
  Wire.begin(D2, D1); //(SDA, SCL)
  lcd.init();
  lcd.backlight();

  lcd.setCursor(3, 0);
  lcd.print("Techno Hub");
  lcd.setCursor(5, 1);
  lcd.print("Network");
  delay(500);

  delayMS = setup_serial_info_for_dht_and_get_min_delay(dht);
  // Set delay between sensor readings based on sensor details.
  delayMS = delayMS / 1000;
}

void loop()
{
  delay(delayMS);

  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
  }
  else
  {
    temperature = String(event.temperature);
    serial_log_temperature(event);
  }

  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Error reading humidity!"));
  }
  else
  {
    humidity = String(event.relative_humidity);
    serial_log_humidity(event);
  }

  update_lcd_temperature(lcd, temperature.toFloat(), humidity.toFloat());
}
