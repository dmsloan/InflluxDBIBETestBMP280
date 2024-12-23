# Repo to test connecting esp32 bme280 to influxdb2
### 

## Contents
- [Notes](#notes)
- [Installation Instructions](#installation-instructions)
- [Esp32-devkitC](#esp32-devkitC-v4)
- [BME280/BMP280 Pressure/Temperature/Humidity sensor](#BME280/BMP280)
- [MPRLS Pressure sensor](#MPRLS)

## Notes
- You may have to push the button to upload to the board

## Installation Instructions
- Use VS Code and PlatformIO

## esp32-devkitC-v4
Board name esp32-devkitC v4. For PlatformIO use "esp32dev" for the board type.<br/>
Purchased several of these. They have a connection for an external antenna.<br/>

![Pin Functions](docs/ESP32-DEV-KIT-DevKitC-v4-pinout-mischianti.png)
![Pin Functions](docs/esp32-devkitC-v4-pinout.png)

## BME280/BMP280
I2C address is 0x76 or 0x77 if you cut the trace between the left two pads and add a jumper to the right two pads.<br/>
Supply Range: 1.8V to 5V DC<br/>
The BME280 has Pressure, Temperature, and Humidity. The BMP only has Pressure and Temperature.<br/>
![Pin Functions](docs/BMP280.jpg)<br/>
Go to [Adafruit](https://learn.adafruit.com/adafruit-bme280-humidity-barometric-pressure-temperature-sensor-breakout) for more information.<br/>

## MPRLS
I2C address is 0x18 and cannot be changed.<br/>
Supply Range: 2.0V to 5.5V DC<br/>
The MPRLS has Pressure sensing of 0-25 PSI with a push on a tube connection.<br/>
![Pin Functions](docs/MPRLS3965-00.jpg)<br/>
Go to [Adafruit](https://www.adafruit.com/products/3965) or [Sparkfun](https://www.sparkfun.com/products/16476) for more information.<br/>

## Hint
Keep at it.