#include <SoftwareSerial.h>
//Install the following Libraries
#include <TinyGPS++.h> //https://github.com/mikalhart/TinyGPSPlus
//GPS Module RX pin to NodeMCU D1
//GPS Module TX pin to NodeMCU D2
const int RXPin = 4, TXPin = 5;
SoftwareSerial neo6m(RXPin, TXPin);
TinyGPSPlus gps;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  neo6m.begin(9600);  
}

void loop()
{
  smartdelay_gps(1000); 
  if (gps.location.isValid()) 
  {
    //Storing the Latitude. and Longitude
    String latitude = String(gps.location.lat(), 6);
    String longitude = String(gps.location.lng(), 6);
  
    } 
  else Serial.println("Gps not valid");
}

static void smartdelay_gps(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (neo6m.available())
      gps.encode(neo6m.read());
  } while (millis() - start < ms);
}
