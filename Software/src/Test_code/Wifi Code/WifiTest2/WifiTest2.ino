#include "esp_wpa2.h"
#include <WiFi.h>
  String line;
const char *ssid = "NUS_STU";  // your ssid
#define EAP_ID "NUSSTU\E0774750" //e0774750@u.nus.edu
#define EAP_USERNAME "NUSSTU\E0774750" //e0774750@u.nus.edu
#define EAP_PASSWORD "NEwk5urKFnt8Mqd"
#ifdef __cplusplus
extern "C" {
#endif
  uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();
void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // WPA2 enterprise magic starts here
  WiFi.disconnect(true);

  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ID, strlen(EAP_ID));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
  esp_wifi_sta_wpa2_ent_enable(&config);
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());
  // WPA2 enterprise magic ends here
  WiFi.begin(ssid);
  WiFi.setHostname("PINGER");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

const char *host = "arduino.php5.sk";

void loop() {
  delay(5000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    WiFi.begin(ssid);
    WiFi.setHostname("PINGER");
  }
  Serial.print("connecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/rele/rele1.txt";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    line = client.readStringUntil('\n');
    Serial.println(line);
  }

  if (line == "VYP") {
    Serial.println("Vypnuty spotrebic");
  } else if (line == "ZAP") {
    Serial.println("Zapnuty spotrebic");
  }
  Serial.println(line.length());
  //Serial.println(velkost);
  Serial.println();
  Serial.println("closing connection");
  int measurement = 0;

  measurement = hallRead();

  Serial.print("Hall sensor measurement: ");
  Serial.println(measurement);
  Serial.print("Temperature: ");
  // Convert raw temperature in F to Celsius degrees
  Serial.print((temprature_sens_read() - 32) / 1.8);
  Serial.println(" C");
}