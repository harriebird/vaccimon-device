/*
 * Vaccimon Monitoring Device
 * by Paul Harriet Asinero
 * v0.8
 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <esp_bt_device.h>
#include <esp_bt_main.h>
#include <WiFi.h>
#include "includes/Bitmaps.h"

// Initialize OLED Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define SSD1306_NO_SPLASH
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initialize Temperature Sensor
const int temperaturePin = 15;
OneWire temperatureOneWire(temperaturePin);
DallasTemperature temperatureSensor(&temperatureOneWire);
bool temperatureGood = false;

// Initilize Gyro Sensor
Adafruit_MPU6050 mpu6050;
Adafruit_MPU6050_Gyro gyroSensor(&mpu6050);
bool gyroGood = false;

// Initialize BLE
#define BLE_DEVICE_NAME "Vaccimon"
#define SERVICE_UUID "181A" // Environment Sensing Service
#define CHARACTERISTIC_UUID "2AF8" // Fixed String 8

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;
BLEAdvertising *pAdvertising;
String deviceName = BLE_DEVICE_NAME;
bool deviceConnected = false;
const uint8_t* macUint;
String toSend = "";

void displayBleStatus(bool status) {
  display.writeFillRect(0, 0, 128, 16, SSD1306_BLACK);
  display.display();
  if (status) {
    display.drawBitmap(0, 0, bmBleConnected, 17, 16, SSD1306_WHITE);
  } else {
    display.drawBitmap(0, 0, bmBleWaiting, 11, 16, SSD1306_WHITE);
  }
  String text = (status) ? "Connected" : "Waiting";
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setCursor(20, 0);
  display.println(text);
  display.display();
}

class VaccimonServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
    displayBleStatus(deviceConnected);
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
    // To Reconnect
    Serial.println("Client disconnected.");
    pServer->getAdvertising()->start();
    displayBleStatus(deviceConnected);
  }
};


void displayTemperature(float temperature) {
  display.writeFillRect(25, 17, 18*5, 24, SSD1306_BLACK);
  display.drawBitmap(0, 17, bmTemperatureIcon, 14, 24, SSD1306_WHITE);
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setCursor(23, 17);
  display.println(temperature);
  display.display();
}

String uintToHex(uint8_t val) {
  char result[3];
  sprintf(result, "%02X", (int)val);
  return result;
}

void checkSensors() {
  if(!gyroGood || !temperatureGood) {
    String warning = "Please check your\n sensors.";
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    display.setCursor(0, 0);
    display.println(warning);
    display.drawBitmap(37, 17, bmWarningIcon, 53, 48, SSD1306_WHITE);
    display.display();
    for(;;);
  }
}

void setup() {
  Serial.begin(115200);

  // Setup OLED Displa  y 
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed."));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.drawBitmap(0, 0, bmSplash, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.display();

  // Turn off WiFi
  WiFi.mode(WIFI_OFF);

  // Setup Temperature Sensor
  temperatureSensor.begin();
  temperatureGood = (temperatureSensor.getDeviceCount() > 0) ? true : false;
  
  // Setup Gyro Sensor
  gyroGood = mpu6050.begin();
  mpu6050.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu6050.setFilterBandwidth(MPU6050_BAND_21_HZ);

  btStart();
  esp_bluedroid_init();
  esp_bluedroid_enable();
  macUint = esp_bt_dev_get_address();
  deviceName.concat("-" + uintToHex(macUint[3]) + uintToHex(macUint[4]) + uintToHex(macUint[5]));

  // Setup BLE
  BLEDevice::init(deviceName.c_str());
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

  delay(3000);
  display.clearDisplay();
  checkSensors();
  displayBleStatus(deviceConnected);
}

void loop() {
  toSend = "";
  
  // Read Temperature
  temperatureSensor.requestTemperatures();
  float temperature = temperatureSensor.getTempCByIndex(0);

  // Read Gyro
  sensors_event_t gyroRead;
  gyroSensor.getEvent(&gyroRead);

  // Format into JSON-styled String
  toSend = "{\"temperature\":";
  toSend.concat(temperature);
  toSend.concat(", \"gyro\": {\"x\":");
  toSend.concat(gyroRead.gyro.x);
  toSend.concat(", \"y\":");
  toSend.concat(gyroRead.gyro.y);
  toSend.concat(", \"z\":");
  toSend.concat(gyroRead.gyro.z);
  toSend.concat("}}");
  
  // Sets the Characteristic updated value from sensor
  pCharacteristic->setValue(toSend.c_str());

  displayTemperature(temperature);
}
