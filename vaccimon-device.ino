/*
 * Vaccimon Monitoring Device
 * by Paul Harriet Asinero
 * v0.1
 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <BH1750.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Initialize Temperature Sensor
const int temperaturePin = 12;
OneWire temperatureOneWire(temperaturePin);
DallasTemperature temperatureSensor(&temperatureOneWire);

// Initialize Light Sensor
BH1750 lightSensor;

// Initilize Gyro Sensor
Adafruit_MPU6050 mpu6050;
Adafruit_MPU6050_Gyro gyroSensor(&mpu6050);


void setup() {
  // Setup Temperature Sensor
  Serial.begin(115200);
  temperatureSensor.begin();
  
  // Setup Light Sensor
  Wire.begin(21, 22);
  lightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire);

  // Setup Gyro Sensor
  mpu6050.begin();
  mpu6050.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu6050.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {

  // Read Temperature
  temperatureSensor.requestTemperatures();
  Serial.print("Temperature: ");
  Serial.println(temperatureSensor.getTempCByIndex(0));

  // Read Light
  float lightLevel = lightSensor.readLightLevel();
  Serial.print("Light: ");
  Serial.println(lightLevel);

  // Read Gyro
  sensors_event_t gyroRead;
  gyroSensor.getEvent(&gyroRead);
  Serial.print("Rotation X: ");
  Serial.print(gyroRead.gyro.x);
  Serial.print(", Y: ");
  Serial.print(gyroRead.gyro.y);
  Serial.print(", Z: ");
  Serial.print(gyroRead.gyro.z);
  Serial.println(" rad/s");

  Serial.println();
  delay(1000);
}
