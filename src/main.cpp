#include <Arduino.h>

#include <Adafruit_BME280.h>

#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32WBMP280"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "secrets.h"    //all the secret stuff goes here such as ssid, passwords, etc.

// InfluxDB v2 organization id (Use: InfluxDB UI -> User -> About -> Common Ids )
#define INFLUXDB_ORG "sloan"
// InfluxDB v2 bucket name (Use: InfluxDB UI ->  Data -> Buckets)
#define INFLUXDB_BUCKET "ESP32 Test"
//#define SEALEVELPRESSURE_HPA (1013.25) //this is the default
double SEALEVELPRESSURE_HPA (1015.8); // as reported at VNY 2023/03/18 11:00
const String sketchName = "InfluxDBIBETextBMP280";

// Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
// Examples:
//  Pacific Time: "PST8PDT"
//  Eastern: "EST5EDT"
#define TZ_INFO "PST8PDT"

// InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Data point
Point sensor("BMP280");

Adafruit_BME280 bme; // I2C
    unsigned status;
    unsigned long delayTime;


// weighted average
//
// Tracks a weighted average in order to smooth out the values that it is given.
// it takes about ten readings to sabilize.
double wtgAverage (double wtgReading, double reading){
  double average;
  reading = reading*0.1;
  wtgReading = wtgReading*0.9;
  average = wtgReading + reading;
  return average;
}

/*
 * @brief returns temperature or relative humidity
 * @param "type" is character
 *     C = Celsius
 *     K = Keliven
 *     F = Fahrenheit
 *     H = Humidity
 *     P = Pressure
 *     A = Altitude
 * @return returns one of the values above
 * Usage: to get Fahrenheit type: getHTU('F')
 * to print it on serial monitor Serial.println(getBME('F'));
 * Written by Ahmad Shamshiri on July 13, 2019. Update july 25, 2019
 * in Ajax, Ontario, Canada
 * www.Robojax.com 
 */
float getBME(char type)
{
   // Robojax.com BME280 Code
  float value;
    float temp = bme.readTemperature();// read temperature
    float pressure = bme.readPressure() / 100.0F; // read pressure
    float rel_hum = bme.readHumidity();// read humidity
    float alt =bme.readAltitude(SEALEVELPRESSURE_HPA);// read altitude
   if(type =='F')
   {
    value = temp *9/5 + 32;//convert to Fahrenheit 
   }else if(type =='K')
   {
    value = temp + 273.15;//convert to Kelvin
   }else if(type =='H')
   {
    value = rel_hum;//return relative humidity
   }else if(type =='P')
   {
    value = pressure;//return pressure
   }else if(type =='A')
   {
    value = alt;//return approximate altitude
   }else{
    value = temp;// return Celsius
   }
   return value;
    // Robojax.com BME280 Code
}

void setup() {
  Serial.begin(115200);
    while (!Serial){};
  Serial.println();
  Serial.print("Sketch is called ");
  Serial.println(sketchName);

  // Setup wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP32 by your DHCP server
  Serial.println();

  Wire.begin();

      // default settings
    status = bme.begin(0x76);  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor in space One, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        delay(5000);
    }
    else
    {
      Serial.println("Connected to the BME280.");
    }

    Serial.println(status);

    // indoor navigation
    Serial.println("-- Indoor Navigation Scenario --");
    Serial.println("normal mode, 16x pressure / 2x temperature / 1x humidity oversampling,");
    Serial.println("0.5ms standby period, filter 16x");
    bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                    Adafruit_BME280::SAMPLING_X2,  // temperature
                    Adafruit_BME280::SAMPLING_X16, // pressure
                    Adafruit_BME280::SAMPLING_X1,  // humidity
                    Adafruit_BME280::FILTER_X16,
                    Adafruit_BME280::STANDBY_MS_0_5 );
    // suggested rate is 1/60Hz (1m)
    delayTime = 1000; // in milliseconds
    Serial.println();

  // Add tags
  sensor.addTag("device", DEVICE);
//  sensor.addTag("SSID", WiFi.SSID());

  // Accurate time is necessary for certificate validation and writing in batches
  // For the fastest time sync find NTP servers in your area: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}


void loop() {
  // Clear fields for reusing the point. Tags will remain untouched
  sensor.clearFields();

  // Store measured value into point
  // Report RSSI of currently connected network
  sensor.addField("Temp", getBME('F'));
  sensor.addField("Humidity", getBME('H'));
  sensor.addField("Pressure", getBME('P'));

  // Print what are we exactly writing
  Serial.print("Writing: ");
  Serial.println(sensor.toLineProtocol());

  // Check WiFi connection and reconnect if needed
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi connection lost");
  }

  // Write point
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  Serial.println("Wait 10s");
  delay(10000);
}
