#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"

#include "InfluxDbClient.h"
#include "InfluxDbCloud.h"
#include "Wire.h"
#include "DFRobot_AHT20.h"
#include "DFRobot_ENS160.h"
#include "DFRobot_ADS1115.h"

// WiFi AP SSID
#define WIFI_SSID "Singtel-46KE"   
// WiFi password
#define WIFI_PASSWORD "38vduut64u"  
// InfluxDB v2 server url, e.g. https://eu-central-1-1.aws.cloud2.influxdata.com (Use: InfluxDB UI -> Load Data -> Client Libraries)
#define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
// InfluxDB v2 server or cloud API token (Use: InfluxDB UI -> Data -> API Tokens -> Generate API Token)
#define INFLUXDB_TOKEN "pjYYG0njgubqOSZjR-wcZWLJFBkw9vIPUjRerSuj8stY29ZLx4O8QIZQMrd4s0OeTDIGPdyfROiYpn4tQlM9cg=="
// InfluxDB v2 organization id (Use: InfluxDB UI -> User -> About -> Common Ids )
#define INFLUXDB_ORG "f6f659bfb99f5ddf"
// InfluxDB v2 bucket name (Use: InfluxDB UI ->  Data -> Buckets)
#define INFLUXDB_BUCKET "EzCompostr_Testing"

// Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
// Examples:
//  Pacific Time: "PST8PDT"
//  Eastern: "EST5EDT"
//  Japanesse: "JST-9"
//  Central Europe: "CET-1CEST,M3.5.0,M10.5.0/3"
#define TZ_INFO "<+08>-8"

// InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
// InfluxDB client instance without preconfigured InfluxCloud certificate for insecure connection
//InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

// Data point
Point sensor("wifi_status");

//define sensor address
DFRobot_AHT20 aht20;
DFRobot_ADS1115 ads(&Wire);

#define I2C_COMMUNICATION  //I2C communication. Comment out this line of code if you want to use SPI communication.
DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ 0x53);

float calibration_value = 21.34;

void setup() {
  //Setup I2C
  Wire.begin(41, 42);

  Serial.begin(115200);
  while (!Serial) {
  }

  //Setup temperature sensor
  aht20_init();
  ens160_init();
  adc_init();

  while (NO_ERR != ENS160.begin()) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  // Setup wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  // Add tags
  sensor.addTag("device", DEVICE);
  sensor.addTag("SSID", WiFi.SSID());

  // Alternatively, set insecure connection to skip server certificate validation
  //client.setInsecure();

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
  // Store measured value into point
  sensor.clearFields();
  // Report RSSI of currently connected network
  sensor.addField("rssi", WiFi.RSSI());

  //Add temperature and humidity
  if (aht20.startMeasurementReady(/* crcEn = */ true)) {
    sensor.addField("Temperature_C", aht20.getTemperature_C()-3);
    sensor.addField("Humidity_RH", aht20.getHumidity_RH());
  } else{
    aht20_init();
    delay(2000);
  }

  //Add CO2 and Organic Compound 
  if ((ENS160.getECO2() != 0) && (ENS160.getTVOC() != 0)){
    ENS160.setTempAndHum(aht20.getTemperature_C(), aht20.getHumidity_RH());
    sensor.addField("Organic_Compound_ppb", ENS160.getTVOC());
    sensor.addField("Co2_ppm", ENS160.getECO2());
    sensor.addField("Ethanol_ppb", ENS160.getETOH());
  } else{
    ens160_init();
    delay(2000);
  }
  
  if (ads.checkADS1115()) {
    int total = 0;
    for (int i = 0; i < 6; i++){
      total += ads.readVoltage(0);
    }

    int16_t pH_volt = total / 6;
    float pH = (-0.011 * pH_volt) + 40.397;
    sensor.addField("pH_voltage", pH_volt);
    sensor.addField("pH_level", pH);
  } else{
    adc_init();
    delay(2000);
  }

  // Print what are we exactly writing
  Serial.print("Writing: ");
  Serial.println(client.pointToLineProtocol(sensor));
  // If no Wifi signal, try to reconnect it
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi connection lost");
  }
  // Write point
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  //Wait 1s
  Serial.println("Wait 1s");
  delay(5000);
}


void aht20_init() {
  uint8_t status;
  while ((status = aht20.begin()) != 0) {
    Serial.print("AHT20 sensor initialization failed. error status : ");
    Serial.println(status);
    delay(1000);
  }
  Serial.println("aht ok!");
}

void ens160_init() {
  while (NO_ERR != ENS160.begin()) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("ens ok!");
  ENS160.setPWRMode(ENS160_STANDARD_MODE);
}

void adc_init() {
  ads.setAddr_ADS1115(0x48);      // 0x48
  ads.setGain(eGAIN_TWOTHIRDS);   // 2/3x gain
  ads.setMode(eMODE_SINGLE);      // single-shot mode
  ads.setRate(eRATE_128);         // 128SPS (default)
  ads.setOSMode(eOSMODE_SINGLE);  // Set to start a single-conversion
  ads.init();
}
