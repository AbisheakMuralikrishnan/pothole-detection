#inc_lude <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

#define trigPin 10
#define echoPin 13

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial GPS(RXPin, TXPin);

#define FIREBASE_AUTH "Your secret" 
#define FIREBASE_HOST "your FIREBASE HOST"
#define WIFI_SSID " your WIFI SSID"
#define WIFI_PASSWORD " your WIFI PASSWORD"

String values,sensor_data;

void setup() {
  //Initializes the serial connection at 9600 get sensor data from arduino.
   Serial.begin(9600);
   GPS.begin(GPSBaud);

   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    
  }
  while(1){
  float duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) * 0.0344;
  if (distance >= 400 || distance <= 20){
   Serial.print("Distance = ");
   Serial.println("No Potholes Found");

   }
  else {
  Serial.println("Pothole found");
  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");
  while (GPS.available() > 0){
    gps.encode(GPS.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
    }
  }
  break;

  delay(500);
  }
  delay(500);
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}
void loop() {

 bool Sr =false;

  while(Serial.available()){
    
        //get sensor data from serial put in sensor_data
        sensor_data=Serial.readString(); 
        Sr=true;    
        
    }
  
delay(1000);

  if(Sr==true){  
    
  values=sensor_data;
  //store sensors data as string in firebase
  Firebase.setString("pathole founded",values);

  //store previous sensors data as string in firebase
  Firebase.pushString("previous values",values);
  
  delay(1000);
  
  if (Firebase.failed()) {  
      return;
  }
  
}   
}