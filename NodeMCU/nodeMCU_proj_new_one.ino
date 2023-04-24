
// the button in the blynk app tell the current state of the lock/mute


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


int buzzer=D0, solenoid_lock=D4;
int trigPin=D1, echoPin=D2;

void setup()
{
  // Debug console
  Serial.begin(9600);
  Serial.println("Serial.begin done");
  // Setup WiFi network
  // WiFi.config(device_ip, gateway_ip, subnet_mask);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
          }
  Serial.println("Connected to wifi");
  



  pinMode(trigPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(solenoid_lock, OUTPUT);
  pinMode(echoPin, INPUT);

  // Setup Blynk
  Blynk.config(BLYNK_AUTH_TOKEN);
  while (Blynk.connect() == false) {
   Serial.print(".");
  }
  Serial.println("Done");

}






BLYNK_WRITE(V0) 
{
   if(param.asInt()==0){
    digitalWrite(solenoid_lock, LOW);
    Serial.println("Door unocked!");
    isUnlocked=true;
    count=0;
    ringCount=0;
    delay(2000);
   }
    


   else if(param.asInt()==1){
    digitalWrite(solenoid_lock, HIGH);
    isUnlocked = false;

    Serial.println("Door locked!");
    delay(2000);
   }
}




BLYNK_WRITE(V1) 
{
   if(param.asInt()==0){
    turnOff=false;
   }
    

   else if(param.asInt()==1){ 
    turnOff=true;
    count=0;
    ringCount=0;
   }

}





void loop()
{
  Blynk.run();


   // Clears the trigPin
  delay(1);

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

  // Serial.print("count: ");
  // Serial.println(count);
  // Serial.print("muted: ");
  // Serial.println(turnOff); 
  
  // Serial.print("ringcount: "); 
  // Serial.println(ringCount);

  // Serial.print("isunlocked: "); 
  // Serial.println(ringCount);



  if(distance<50){ 
    count++;
    delay(100);
  }
  else{
  count=0;
  ringCount=0;
  }



  if(count>=30 && !turnOff && ringCount<3 && !isUnlocked){
    tone(buzzer, 2000);
    Serial.print("buzzing ");
    ringCount++;
    delay(2000);
    noTone(buzzer);
    delay(2000);
  }

  else if(count>=30 && !turnOff && ringCount>=3 && !isUnlocked){
    
   if(ringCount==3) Blynk.logEvent("someone_at_door","Someone is at the door -- nodemcu");
    ringCount++;
    noTone(buzzer);
  }

  else{
    noTone(buzzer);

  }
 




}

