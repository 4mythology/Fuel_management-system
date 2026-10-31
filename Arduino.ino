#define BLYNK_TEMPLATE_ID "TMPL2xCPWGdxg"
#define BLYNK_TEMPLATE_NAME "Fuel Management"
#define BLYNK_AUTH_TOKEN "IvDzWJuAKV3nQiHU9SY3ekR8fnSTXe0G"
#define BLYNK_PRINT Serial

char ssid[] = "brainSTREAM";
char pass[] = "BrainWIFI";

int emptyTankDistance =  15;
int fullTankDistance =  3;

int triggerPer =   10 ;

#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <AceButton.h>
using namespace ace_button; 

#define TRIGPIN    27
#define ECHOPIN    26
//#define wifiLed    2
#define ButtonPin1 12
#define BuzzerPin  13
#define GreenLed   14
#define trig       25
#define echo       15

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3
#define VPIN_BUTTON_4    V4
#define VPIN_BUTTON_5    V5
#define VPIN_BUTTON_6    V6

float duration1;
float duration2;
float distance1;
float distance2;
int   waterLevelPer1;
int   waterLevelPer2;
bool  toggleBuzzer = HIGH;
int   volume1;
int   volume2;
bool  alert1;
bool  alert2;
int relaypin = 4;
int relaypin2 = 2;

char auth[] = BLYNK_AUTH_TOKEN;

ButtonConfig config1;
AceButton button1(&config1);

void handleEvent1(AceButton*, uint8_t, uint8_t);

BlynkTimer timer;

//void checkBlynkStatus() {
  //bool isconnected = Blynk.connected();
  //if (isconnected == false) {
    //Serial.println("Blynk Not Connected");
    //digitalWrite(wifiLed, LOW);
  //}
  //if (isconnected == true) {
    //digitalWrite(wifiLed, HIGH);
   // Serial.println("Blynk Connected");
  //}
//}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
  Blynk.syncVirtual(VPIN_BUTTON_5);
  Blynk.syncVirtual(VPIN_BUTTON_6);
}

void measureDistance(){
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIGPIN, LOW);
  duration1 = pulseIn(ECHOPIN, HIGH);
  distance1 = ((duration1 / 2) * 0.343)/10;

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(trig, LOW);
  duration2 = pulseIn(echo, HIGH);
  distance2 = ((duration2 / 2) * 0.343)/10;

   if ((distance1 > (fullTankDistance - 3)  && distance1 < emptyTankDistance ) || (distance2 > (fullTankDistance - 3)  && distance2 < emptyTankDistance )){
    waterLevelPer1 = map((int)distance1 ,emptyTankDistance, fullTankDistance, 0, 100);
    waterLevelPer2 = map((int)distance2, emptyTankDistance, fullTankDistance, 0, 100);
    volume1 = waterLevelPer1 * 450 / 100;
    volume2 = waterLevelPer2 * 450 / 100;
    Blynk.virtualWrite(VPIN_BUTTON_1, waterLevelPer1);
    Blynk.virtualWrite(VPIN_BUTTON_2, (String(volume1) + " ml"));
    Blynk.virtualWrite(VPIN_BUTTON_3, waterLevelPer2);
    Blynk.virtualWrite(VPIN_BUTTON_4, (String(volume2) + " ml"));
    Blynk.virtualWrite(VPIN_BUTTON_5, alert1);
    Blynk.virtualWrite(VPIN_BUTTON_6, alert2);

    Serial.print("First distance: ");
    Serial.print(distance1);
    Serial.println(" cm");
    Serial.print("Second distance: ");
    Serial.print(distance2);
    Serial.println(" cm");

    if ((waterLevelPer1 < triggerPer)) {
      digitalWrite(GreenLed, HIGH);
      alert1 = 1;
      if (toggleBuzzer == HIGH){
        digitalWrite(BuzzerPin, HIGH);
      }      
    } else{
      alert1 = 0;
    }
    if ((waterLevelPer2 < triggerPer)) {
      digitalWrite(GreenLed, HIGH);
      alert2 = 1;
      if (toggleBuzzer == HIGH){
        digitalWrite(BuzzerPin, HIGH);
      }      
    } else {
      alert2 = 0;
    }
    if ((distance1 < fullTankDistance)||(distance2 < fullTankDistance)){
      digitalWrite(GreenLed, LOW);
      if (toggleBuzzer == HIGH){
        digitalWrite(BuzzerPin, HIGH);
      } 
    }

    if ((distance1 > (fullTankDistance + 5) && waterLevelPer1 > (triggerPer + 5))||(distance2 > (fullTankDistance + 5) && waterLevelPer2 > (triggerPer + 5))){
      toggleBuzzer = HIGH;
      digitalWrite(BuzzerPin, LOW);
    }        
  }

  delay(100);
}

void setup() {
  Blynk.begin(auth, ssid, pass);

  Serial.begin(9600);

  pinMode(ECHOPIN, INPUT);
  pinMode(echo, INPUT);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(trig, OUTPUT);
 // pinMode(wifiLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(ButtonPin1, INPUT_PULLUP);
  pinMode(relaypin, OUTPUT);
  pinMode(relaypin2, OUTPUT);

  //digitalWrite(wifiLed, LOW);
  digitalWrite(GreenLed, LOW);
  digitalWrite(BuzzerPin, LOW);

  config1.setEventHandler(button1Handler);

  button1.init(ButtonPin1);

  WiFi.begin(ssid, pass);
  //timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(1000);
}

void loop() {
  measureDistance();
  Blynk.run();
  timer.run();
  button1.check();
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      digitalWrite(BuzzerPin, LOW);
      toggleBuzzer = LOW;
      break;
  }
}
