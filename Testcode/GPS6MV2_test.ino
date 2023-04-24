/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */
 
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop(){
  // This sketch displays information every time a new sentence is correctly encoded.
  bool a = ss.available() > 0;

  while (a){
    gps.encode(ss.read());
        Serial.print("Latitude= "); 
        Serial.print(gps.location.lat(), 6);
        Serial.print(" Longitude= "); 
        Serial.println(gps.location.lng(), 6);
        Serial.println(gps.time.second());
        Serial.println(gps.satellites.value());
  }

  if (!a) Serial.print(" Not Available "); 
}
