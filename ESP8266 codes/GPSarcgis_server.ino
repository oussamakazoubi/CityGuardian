#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const int RXPin = 4, TXPin = 5;
SoftwareSerial neo6m(RXPin, TXPin);
TinyGPSPlus gps;

const char* ssid = "WiFiSSID";
const char* password = "WiFiPassword";
const char* server = "https://www.arcgis.com/";
const String endpoint = "1c155c837971464cbdb6107f98ad4d82";  // Change this to your server's endpoint

void setup() {
  Serial.begin(115200);
  neo6m.begin(9600);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
}

void loop() {
  if (gps.location.isValid()) {
    WiFiClient client;

    float lat = gps.location.lat();
    float lon = gps.location.lng();
    if (client.connect(server, 80)) {
      String postData = "lat=" + String(lat, 6) + "&lon=" + String(lon, 6);
      client.println("POST " + endpoint + " HTTP/1.1");
      client.println("Host: " + String(server));
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.println("Content-Length: " + String(postData.length()));
      client.println();
      client.println(postData);

      // Wait for the server's response
      while (client.connected()) {
        if (client.available()) {
          Serial.write(client.read());
        }
      }
      client.stop();
    }
    delay(5000); // Change the interval based on how often you want to send data
  }
  smartdelay_gps(1000);
}

static void smartdelay_gps(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (neo6m.available())
      gps.encode(neo6m.read());
  } while (millis() - start < ms);
}
