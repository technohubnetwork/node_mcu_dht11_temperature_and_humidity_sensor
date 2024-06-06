#include "sensor_helper.h"

uint32_t setup_serial_info_for_dht_and_get_min_delay(DHT_Unified dht)
{
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

    return sensor.min_delay;
}

void serial_log_temperature(sensors_event_t event)
{

    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
}

void serial_log_humidity(sensors_event_t event)
{
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
}

float calculateHeatIndex(float temperature_f, float humidity, float c1, float c2, float c3, float c4, float c5, float c6, float c7, float c8, float c9)
{
    float heat_index_f = (c1 + (c2 * temperature_f) + (c3 * humidity) +
                          (c4 * temperature_f * humidity) + (c5 * pow(temperature_f, 2)) +
                          (c6 * pow(humidity, 2)) + (c7 * pow(temperature_f, 2) * humidity) +
                          (c8 * temperature_f * pow(humidity, 2)) + (c9 * pow(temperature_f, 2) * pow(humidity, 2)));
    return heat_index_f;
}

float feels_like(float temperature, float humidity)
{
    float c1 = -42.379;
    float c2 = 2.04901523;
    float c3 = 10.14333127;
    float c4 = -0.22475541;
    float c5 = -0.00683783;
    float c6 = -0.05481717;
    float c7 = 0.00122874;
    float c8 = 0.00085282;
    float c9 = -0.00000199;

    float temperature_f = temperature * 9 / 5 + 32;

    float heat_index_f = calculateHeatIndex(temperature_f, humidity, c1, c2, c3, c4, c5, c6, c7, c8, c9);

    float heat_index_c = (heat_index_f - 32) * 5 / 9;

    return heat_index_c;
}

void update_lcd_temperature(LiquidCrystal_I2C lcd, float temperature, float humidity)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tempera.: " + String(temperature) + "C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: " + String(humidity) + "%");
    delay(1500);
    float feels_like_temperature = feels_like(temperature, humidity);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Feels Like: ");
    lcd.setCursor(0, 1);
    lcd.print(String(feels_like_temperature) + "C");
    delay(1500);
}