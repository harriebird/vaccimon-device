/*
 * Vaccimon Monitoring Device
 * by Paul Harriet Asinero
 * v0.5
 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <BH1750.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <WiFi.h>

// Initialize Temperature Sensor
const int temperaturePin = 12;
OneWire temperatureOneWire(temperaturePin);
DallasTemperature temperatureSensor(&temperatureOneWire);

// Initialize Light Sensor
BH1750 lightSensor;

// Initilize Gyro Sensor
Adafruit_MPU6050 mpu6050;
Adafruit_MPU6050_Gyro gyroSensor(&mpu6050);

// Initialize BLE
#define BLE_DEVICE_NAME "Vaccimon"
#define SERVICE_UUID "181A" // Environment Sensing Service
#define CHARACTERISTIC_UUID "2AF8" // Fixed String 8

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;
BLEAdvertising *pAdvertising;
bool deviceConnected = false;

String toSend = "";

class VaccimonServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
    // To Reconnect
    Serial.println("Client disconnected.");
    pServer->getAdvertising()->start();
  }
};

void setup() {
  // Built-in LED Indicator
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Turn off WiFi
  WiFi.mode(WIFI_OFF);

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

  // Setup BLE
  BLEDevice::init(BLE_DEVICE_NAME);
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new VaccimonServerCallbacks());
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pAdvertising = BLEDevice::getAdvertising();
  pService->start();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void loop() {
  if(deviceConnected) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
   
  
  toSend = "";
  
  // Read Temperature
  temperatureSensor.requestTemperatures();
  float temperature = temperatureSensor.getTempCByIndex(0);

  // Read Light
  float lightLevel = lightSensor.readLightLevel();

  // Read Gyro
  sensors_event_t gyroRead;
  gyroSensor.getEvent(&gyroRead);

  // Format into JSON-styled String
  toSend = "{\"temperature\":";
  toSend.concat(temperature);
  toSend.concat(", \"light\":");
  toSend.concat(lightLevel);
  toSend.concat(", \"gyro\": {\"x\":");
  toSend.concat(gyroRead.gyro.x);
  toSend.concat(", \"y\":");
  toSend.concat(gyroRead.gyro.y);
  toSend.concat(", \"z\":");
  toSend.concat(gyroRead.gyro.z);
  toSend.concat("}}");
  
  // Sets the Characteristic updated value from sensor
  pCharacteristic->setValue(toSend.c_str());
}
