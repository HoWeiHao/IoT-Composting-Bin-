#include "WiFi.h"
#include "esp_wpa2.h"
#include "esp_wifi.h"

const char *ca_pem = "-----BEGIN CERTIFICATE-----\n"
                     "MIID3DCCA0WgAwIBAgIJANe5ZSCKoB8fMA0GCSqGSIb3DQEBCwUAMIGTMQswCQYD\n"
                     "VQQGEwJGUjEPMA0GA1UECAwGUmFkaXVzMRIwEAYDVQQHDAlTb21ld2hlcmUxFTAT\n"
                     "BgNVBAoMDEV4YW1wbGUgSW5jLjEgMB4GCSqGSIb3DQEJARYRYWRtaW5AZXhhbXBs\n"
                     "ZS5jb20xJjAkBgNVBAMMHUV4YW1wbGUgQ2VydGlmaWNhdGUgQXV0aG9yaXR5MB4X\n"
                     "DTE2MTEyMzAyNTUwN1oXDTE3MDEyMjAyNTUwN1owgZMxCzAJBgNVBAYTAkZSMQ8w\n"
                     "DQYDVQQIDAZSYWRpdXMxEjAQBgNVBAcMCVNvbWV3aGVyZTEVMBMGA1UECgwMRXhh\n"
                     "bXBsZSBJbmMuMSAwHgYJKoZIhvcNAQkBFhFhZG1pbkBleGFtcGxlLmNvbTEmMCQG\n"
                     "A1UEAwwdRXhhbXBsZSBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkwgZ8wDQYJKoZIhvcN\n"
                     "AQEBBQADgY0AMIGJAoGBAL03y7N2GvNDO9BN8fVtdNonp0bMiqpj1D0He5+OTM+9\n"
                     "3ZTIsJCNrbzhLQrRI3vMW7UDy8U7GeWORN9W4dWYlYiy/NFRp3hNMrbePhVmNIOV\n"
                     "ww4ovGzbD+Xo31gPVkhzQ8I5/jbOIQBmgKMAMZyOMlG9VD6yMmAeYqnZYz68WHKt\n"
                     "AgMBAAGjggE0MIIBMDAdBgNVHQ4EFgQUf1MLQIzAEZcRsgZlS8sosfmVI+UwgcgG\n"
                     "A1UdIwSBwDCBvYAUf1MLQIzAEZcRsgZlS8sosfmVI+WhgZmkgZYwgZMxCzAJBgNV\n"
                     "BAYTAkZSMQ8wDQYDVQQIDAZSYWRpdXMxEjAQBgNVBAcMCVNvbWV3aGVyZTEVMBMG\n"
                     "A1UECgwMRXhhbXBsZSBJbmMuMSAwHgYJKoZIhvcNAQkBFhFhZG1pbkBleGFtcGxl\n"
                     "LmNvbTEmMCQGA1UEAwwdRXhhbXBsZSBDZXJ0aWZpY2F0ZSBBdXRob3JpdHmCCQDX\n"
                     "uWUgiqAfHzAMBgNVHRMEBTADAQH/MDYGA1UdHwQvMC0wK6ApoCeGJWh0dHA6Ly93\n"
                     "d3cuZXhhbXBsZS5jb20vZXhhbXBsZV9jYS5jcmwwDQYJKoZIhvcNAQELBQADgYEA\n"
                     "GepHc7TE/P+5t/cZPn5TTQkWQ/4/1lgQd82lF36RYWSIW3BdAc0zwYWYZaWixxyp\n"
                     "s0YOqwz6PZAGRV+SlYO2f8Kf+C3aZs4YHB0GsmksmFOb8r9d7xcDuOKHoA+QV0Zw\n"
                     "RaK6pttsBAxy7rw3kX/CgTp0Y2puaLdMXv/v9FisCP8=\n"
                     "-----END CERTIFICATE-----";

const char *client_crt = "Certificate:\n"
                         "    Data:\n"
                         "        Version: 3 (0x2)\n"
                         "        Serial Number: 44 (0x2c)\n"
                         "    Signature Algorithm: sha1WithRSAEncryption\n"
                         "        Issuer: C=FR, ST=Radius, L=Somewhere, O=Example Inc./emailAddress=admin@example.com, CN=Example Certificate Authority\n"
                         "        Validity\n"
                         "            Not Before: Nov 23 02:55:07 2016 GMT\n"
                         "            Not After : Jan 22 02:55:07 2017 GMT\n"
                         "        Subject: C=FR, ST=Radius, O=Example Inc., CN=user@example.com/emailAddress=user@example.com\n"
                         "        Subject Public Key Info:\n"
                         "            Public Key Algorithm: rsaEncryption\n"
                         "                Public-Key: (2048 bit)\n"
                         "                Modulus:\n"
                         "                    00:ac:41:d4:a2:46:0c:dc:67:1d:7b:89:36:7c:15:\n"
                         "                    be:a2:c1:fe:4c:f2:fa:af:5d:76:0e:ee:b5:ca:d4:\n"
                         "                    d3:01:c8:6b:30:50:df:2d:57:17:f4:43:47:97:ca:\n"
                         "                    f1:8d:f7:c0:9d:56:b3:e7:17:7c:58:59:de:f3:be:\n"
                         "                    b5:08:5d:f8:3a:ad:83:44:0d:31:c9:f1:3d:f1:9a:\n"
                         "                    cf:84:0c:ad:d3:be:5c:bd:3d:58:b5:1d:2c:fe:70:\n"
                         "                    8d:c5:b0:17:87:d4:8e:85:f7:51:4c:0f:d1:e0:8c:\n"
                         "                    7b:a0:25:ab:91:7c:7f:eb:47:73:c9:4b:6c:8b:e6:\n"
                         "                    c1:06:d5:94:30:63:ec:45:1a:f5:7f:46:2f:b3:84:\n"
                         "                    78:5d:1c:37:1a:fa:57:ea:45:5e:45:40:ab:14:c7:\n"
                         "                    81:b0:26:3d:7e:cf:da:db:f0:f1:40:a7:a1:4b:54:\n"
                         "                    f3:96:1b:c9:30:3c:3c:d8:19:ba:c7:df:b1:ad:a2:\n"
                         "                    d6:17:0a:d6:ed:31:b5:cb:12:39:f5:6e:92:6b:85:\n"
                         "                    f2:9e:c7:06:6b:bb:89:ed:a7:5f:ec:56:12:46:fd:\n"
                         "                    3a:74:d1:d2:31:30:1d:58:19:25:33:ff:11:ea:3a:\n"
                         "                    52:33:b1:fb:d3:75:8d:1f:5e:36:a5:35:e0:11:5a:\n"
                         "                    4a:2d:97:58:2c:3d:62:3c:32:af:83:69:a9:1a:32:\n"
                         "                    1b:b7\n"
                         "                Exponent: 65537 (0x10001)\n"
                         "        X509v3 extensions:\n"
                         "            X509v3 Extended Key Usage: \n"
                         "                TLS Web Client Authentication\n"
                         "            X509v3 CRL Distribution Points: \n"
                         "\n"
                         "                Full Name:\n"
                         "                  URI:http://www.example.com/example_ca.crl\n"
                         "\n"
                         "    Signature Algorithm: sha1WithRSAEncryption\n"
                         "         8b:8d:b6:19:ce:6f:6b:9e:1d:03:8b:6b:10:fc:99:d0:7a:2f:\n"
                         "         e0:37:ce:b8:a4:e4:b9:a1:c2:36:ff:76:b2:ad:d7:d0:df:d1:\n"
                         "         03:27:93:a7:4e:1e:bf:ed:d2:b7:65:2a:c9:c3:ab:20:aa:e3:\n"
                         "         10:4c:75:3b:c4:02:ab:34:08:6e:61:91:cf:e3:02:35:6a:e5:\n"
                         "         f3:25:96:51:92:82:6e:52:81:c1:f1:7b:68:02:b0:ce:f4:ba:\n"
                         "         fd:6e:68:35:b3:7e:77:cb:a0:1e:11:5e:58:bf:f3:2a:ed:b3:\n"
                         "         4c:82:21:5e:1b:47:b6:2f:f3:f5:c9:1b:6a:70:44:6d:ff:ad:\n"
                         "         a6:e3\n"
                         "-----BEGIN CERTIFICATE-----\n"
                         "MIIDTjCCAregAwIBAgIBLDANBgkqhkiG9w0BAQUFADCBkzELMAkGA1UEBhMCRlIx\n"
                         "DzANBgNVBAgMBlJhZGl1czESMBAGA1UEBwwJU29tZXdoZXJlMRUwEwYDVQQKDAxF\n"
                         "eGFtcGxlIEluYy4xIDAeBgkqhkiG9w0BCQEWEWFkbWluQGV4YW1wbGUuY29tMSYw\n"
                         "JAYDVQQDDB1FeGFtcGxlIENlcnRpZmljYXRlIEF1dGhvcml0eTAeFw0xNjExMjMw\n"
                         "MjU1MDdaFw0xNzAxMjIwMjU1MDdaMHExCzAJBgNVBAYTAkZSMQ8wDQYDVQQIDAZS\n"
                         "YWRpdXMxFTATBgNVBAoMDEV4YW1wbGUgSW5jLjEZMBcGA1UEAwwQdXNlckBleGFt\n"
                         "cGxlLmNvbTEfMB0GCSqGSIb3DQEJARYQdXNlckBleGFtcGxlLmNvbTCCASIwDQYJ\n"
                         "KoZIhvcNAQEBBQADggEPADCCAQoCggEBAKxB1KJGDNxnHXuJNnwVvqLB/kzy+q9d\n"
                         "dg7utcrU0wHIazBQ3y1XF/RDR5fK8Y33wJ1Ws+cXfFhZ3vO+tQhd+Dqtg0QNMcnx\n"
                         "PfGaz4QMrdO+XL09WLUdLP5wjcWwF4fUjoX3UUwP0eCMe6Alq5F8f+tHc8lLbIvm\n"
                         "wQbVlDBj7EUa9X9GL7OEeF0cNxr6V+pFXkVAqxTHgbAmPX7P2tvw8UCnoUtU85Yb\n"
                         "yTA8PNgZusffsa2i1hcK1u0xtcsSOfVukmuF8p7HBmu7ie2nX+xWEkb9OnTR0jEw\n"
                         "HVgZJTP/Eeo6UjOx+9N1jR9eNqU14BFaSi2XWCw9Yjwyr4NpqRoyG7cCAwEAAaNP\n"
                         "ME0wEwYDVR0lBAwwCgYIKwYBBQUHAwIwNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDov\n"
                         "L3d3dy5leGFtcGxlLmNvbS9leGFtcGxlX2NhLmNybDANBgkqhkiG9w0BAQUFAAOB\n"
                         "gQCLjbYZzm9rnh0Di2sQ/JnQei/gN864pOS5ocI2/3ayrdfQ39EDJ5OnTh6/7dK3\n"
                         "ZSrJw6sgquMQTHU7xAKrNAhuYZHP4wI1auXzJZZRkoJuUoHB8XtoArDO9Lr9bmg1\n"
                         "s353y6AeEV5Yv/Mq7bNMgiFeG0e2L/P1yRtqcERt/62m4w==\n"
                         "-----END CERTIFICATE-----";

const char *client_key = "-----BEGIN RSA PRIVATE KEY-----\n"
                         "MIIEpgIBAAKCAQEArEHUokYM3Gcde4k2fBW+osH+TPL6r112Du61ytTTAchrMFDf\n"
                         "LVcX9ENHl8rxjffAnVaz5xd8WFne8761CF34Oq2DRA0xyfE98ZrPhAyt075cvT1Y\n"
                         "tR0s/nCNxbAXh9SOhfdRTA/R4Ix7oCWrkXx/60dzyUtsi+bBBtWUMGPsRRr1f0Yv\n"
                         "s4R4XRw3GvpX6kVeRUCrFMeBsCY9fs/a2/DxQKehS1TzlhvJMDw82Bm6x9+xraLW\n"
                         "FwrW7TG1yxI59W6Sa4XynscGa7uJ7adf7FYSRv06dNHSMTAdWBklM/8R6jpSM7H7\n"
                         "03WNH142pTXgEVpKLZdYLD1iPDKvg2mpGjIbtwIDAQABAoIBAQCMhO9GqUpYia2d\n"
                         "VyOhOcPX1dTzRMuHPwDN0aFvIwo2zB3UvkQxInkiA7hldWJz44W3VEFR5PDEyht8\n"
                         "Tzgy6SVUCLOqUfEpwag8bYOXPxiWQRY6Mc8pf/FyZrLgb3PilFznoAcru0QEn9VB\n"
                         "oTlCZ4OalSE5NlQIFGemgZhvmTPmcm4OwPW2diBjLtb3AA8eaaw8okWZwr8g4Bcd\n"
                         "el5KX6pZpDRpGQueh3iKaKxYWbxLYK+c30gKWD65tsAqKyVg2Tm1R2c+kFXgizZt\n"
                         "EexD95SGMjSkGg3R05sKv6m71iJhlOzVQ4ZCKm18Kqa7wZuZ4SIehVmKIV0gaupz\n"
                         "gjyr7+NBAoGBAOGjjGI3nxJTZY3O+KeaQo/jqrKowqZxzMOsCgAvW56xDuAvx9TJ\n"
                         "m4428NGubMl/0RwX6TnxJDm6oe+tnOxLIgE/VnsQLiNzQuFJxrs5JYctdGc4uvk2\n"
                         "KuXDr7tPEYlU/7OLRReov9emydIXJnsGejkIPllUj+DGNjNFqtXh2VoHAoGBAMNv\n"
                         "eSgJSkcM6AUaDuUKaXBL2nkKHNoTtRQ0eCEUds6arKyMo0mSP753FNEuOWToVz1O\n"
                         "oaddSFw81J9t+Xd6XSRbhMj63bQ9nvFKBA1lJfLu+xe3ts0f+vmp1PguOuUHsgNP\n"
                         "aAm/gLPSKUpBO46NG6KhUrZ2ej6AEg7SuGXrDITRAoGBAKK7s6m6d81dvGZ0GT23\n"
                         "sb3Y8ul7cTdd59JPp77OaQOgqxvhGfxLkxcUZMa1R9xjhMsAK8MQOZIxGk2kJwL8\n"
                         "hP/lUFfdKYmDvX6CGQQ6iOhfTg6MCb1m5bVkVr9+nSUw2mIBVclkeUftEK2m6Kfd\n"
                         "2hR774u5wzLXgYuk+TrcckfNAoGBAJ9X8hacjH0lnr8aIe7I8HLoxbZOcnuz+b4B\n"
                         "kbiW8M8++W6uNCw2G9b1THnJEG6fqRGJXPASdH8P8eQTTIUHtY2BOOCM+dqNK1xc\n"
                         "FrW9NJXAF+WcmmTgoEaTG9tGBirafV+JjK/1/b+fqJ6sVRzDHDcbBU9ThhQTY6XG\n"
                         "VSZz4H8hAoGBAMeQQjiUlKBnpGt1oTgKDZo58b7ui61yftg+dEAwIKs6eb5X20vZ\n"
                         "Ca4v/zg06k9lKTzyspQjJZuzpMjFUvDK4ReamEvmwQTIc+oYVJm9Af1HUytzrHJH\n"
                         "u0/dDt0eYpZpzrFqxlP+0oXxlegD8REMVvwNCy+4isyCvjogDaYRfJqi\n"
                         "-----END RSA PRIVATE KEY-----";

#define EAP_TLS 0
#define EAP_PEAP 1
#define EAP_TTLS 2

#define EXAMPLE_WIFI_SSID "NUS_STU"  //SSID (network name) for the example to connect to.

#define EXAMPLE_EAP_METHOD EAP_PEAP             //EAP method (TLS, PEAP or TTLS) for the example to use.
#define EXAMPLE_EAP_ID "NUSSTU\e0774750"        //Identity in phase 1 of EAP procedure.
#define EXAMPLE_EAP_USERNAME "NUSSTU\e0774750"  //Username for EAP method (PEAP and TTLS).
#define EXAMPLE_EAP_PASSWORD "NEwk5urKFnt8Mqd"  //Password for EAP method (PEAP and TTLS).

void initialise_wifi(void) {
  WiFi.begin(EXAMPLE_WIFI_SSID, NULL, 0, NULL, false);
  if (esp_wifi_sta_wpa2_ent_set_ca_cert((uint8_t *)ca_pem, strlen(ca_pem))) {
    Serial.println("Failed to set WPA2 CA Certificate");
    return;
  }
  if (esp_wifi_sta_wpa2_ent_set_cert_key((uint8_t *)client_crt, strlen(client_crt), (uint8_t *)client_key, strlen(client_key), NULL, 0)) {
    Serial.println("Failed to set WPA2 Client Certificate and Key");
    return;
  }
  if (esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EXAMPLE_EAP_ID, strlen(EXAMPLE_EAP_ID))) {
    Serial.println("Failed to set WPA2 Identity");
    return;
  }
  if (EXAMPLE_EAP_METHOD == EAP_PEAP || EXAMPLE_EAP_METHOD == EAP_TTLS) {
    if (esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EXAMPLE_EAP_USERNAME, strlen(EXAMPLE_EAP_USERNAME))) {
      Serial.println("Failed to set WPA2 Username");
      return;
    }
    if (esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EXAMPLE_EAP_PASSWORD, strlen(EXAMPLE_EAP_PASSWORD))) {
      Serial.println("Failed to set WPA2 Password");
      return;
    }
  }
  if (esp_wifi_sta_wpa2_ent_enable()) {
    Serial.println("Failed to enable WPA2");
    return;
  }
  esp_wifi_connect();
}

void setup() {
  Serial.begin(115200);
  initialise_wifi();
}

void loop() {}