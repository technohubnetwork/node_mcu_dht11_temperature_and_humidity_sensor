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

  Serial.begin(9600);
  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print(F("Sensor Type: "));
  Serial.println(sensor.name);
  Serial.print(F("Driver Ver:  "));
  Serial.println(sensor.version);
  Serial.print(F("Unique ID:   "));
  Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value:   "));
  Serial.print(sensor.max_value);
  Serial.println(F("°C"));
  Serial.print(F("Min Value:   "));
  Serial.print(sensor.min_value);
  Serial.println(F("°C"));
  Serial.print(F("Resolution:  "));
  Serial.print(sensor.resolution);
  Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print(F("Sensor Type: "));
  Serial.println(sensor.name);
  Serial.print(F("Driver Ver:  "));
  Serial.println(sensor.version);
  Serial.print(F("Unique ID:   "));
  Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value:   "));
  Serial.print(sensor.max_value);
  Serial.println(F("%"));
  Serial.print(F("Min Value:   "));
  Serial.print(sensor.min_value);
  Serial.println(F("%"));
  Serial.print(F("Resolution:  "));
  Serial.print(sensor.resolution);
  Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop()
{
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  float new_temperature;
  float new_humidity;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
  }
  else
  {
    if (temperature != String(event.temperature))
    {
      temperature = String(event.temperature);
      serial_log_temperature(event);
      lcd.setCursor(0, 0);
      new_temperature = event.temperature;
      lcd.print("Temperature: " + String(new_temperature) + "C");
    }
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Error reading humidity!"));
  }
  else
  {
    if (humidity != String(event.relative_humidity))
    {
      humidity = String(event.relative_humidity);
      serial_log_humidity(event);
      new_humidity = event.relative_humidity;
      lcd.setCursor(0, 1);
      lcd.print("Humidity: " + String(new_humidity) + "%");
    }
  }

  if (String(new_temperature) != temperature || String(new_humidity) != humidity)
  {
    update_lcd_temperature(lcd, temperature.toFloat(), humidity.toFloat());
  }
}
