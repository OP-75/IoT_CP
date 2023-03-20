/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  NOTE: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPLtTI9x7Va"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "pvrVwdfx0kDxYw-vcF2THxUXL5aSibVy"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "laptop";
char pass[] = "12345678";


// Mac address should be different for each device in your LAN
// byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
// IPAddress device_ip  (192, 168,   0,  80);
// IPAddress dns_ip     (  8,   8,   8,   8);
// IPAddress gateway_ip (192, 168,   0,   1);
// IPAddress subnet_mask(255, 255, 255,   0);





bool turnOff=false, isUnlocked=false;

int distance,count=0, ringCount=0, duration=0;


int buzzer=D3, solenoid_lock=D4;
int trigPin=D1, echoPin=D2;

void setup()
{
  // Debug console
  Serial.begin(9600);

  // Setup WiFi network
  // WiFi.config(device_ip, gateway_ip, subnet_mask);
  WiFi.begin(ssid, pass);

  pinMode(trigPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(solenoid_lock, OUTPUT);
  pinMode(echoPin, INPUT);

  // Setup Blynk
  Blynk.config(BLYNK_AUTH_TOKEN);
  while (Blynk.connect() == false) {
   Serial.print(".");
  }

}






BLYNK_WRITE(V0) 
{
   if(param.asInt()==0){
    digitalWrite(solenoid_lock, LOW);
    isUnlocked=false;
   }
    


   else if(param.asInt()==1){
    digitalWrite(solenoid_lock, HIGH);
    isUnlocked = true;
   }
}




BLYNK_WRITE(V1) 
{
   if(param.asInt()==0){
    turnOff=false;
   }
    


   else if(param.asInt()==1) turnOff=true;;

}





void loop()
{
  Blynk.run();


   // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);



  if(distance<50){ 
    count++;
    delay(100);
  }
  else count=0;


  if(count>=30 && !turnOff && ringCount<3 && !isUnlocked){
    tone(buzzer, 3000, 3000);
    ringCount++;
    delay(2000);
  }

  else if(count>=30 && !turnOff && ringCount>=3 && !isUnlocked){
    Blynk.logEvent("someone_at_door","Someone is at the door -- nodemcu");
    ringCount++;
    noTone(buzzer);
  }

  else{
    noTone(buzzer);
    ringCount=0;
  }




}

