# vaccimon-device
Software for the vaccine carrier monitor device.

## Setup
1. Clone this repository.
2. Install [Adruino IDE](https://www.arduino.cc/en/software).
3. Go to `Preferences > Additional Boards Manager URLs` and add the link below:
    * https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
4. Open the Boards Manager and install `esp32`.
5. Set `DOIT ESP32 DEVKIT V1` as your board.


## Requirements
### Software
* Arduino IDE
* Board:
    * ESP32
* Libraries:
    * DallasTemperature v3.9.0 by Miles Burton
    * BH1750 v1.3.0 by Christopher Laws
    * Adafruit MPU6050 v2.2.3 by Adafruit
    * Adafruit Unified Sensor v1.1.6 by Adafruit 
    * OneWire v2.3.7 by Paul Stoffegen
* VSCodium (optional)

### Hardware
* DOIT ESP32 DEVKIT V1
* DS18B20 Temperature Sensor
* GY-302 BH1750 Light Intensity Sensor
* GY-521 MPU-6050 Gyro Sensor
