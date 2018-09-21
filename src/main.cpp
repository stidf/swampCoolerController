#include <Arduino.h>
#include <DHT.h>
#include <SPI.h>
#include "../config/pinout.h"
#include "../config/config.h"


void swampController();
void fanControl();
void pumpControl();
void lightControl();

void pumpSwitchRead();
void fanSwitchRead();
void lightSwitchRead();
void controllerSwitchRead();

void ambientSensorRead();
boolean ambientConditionsTest();

void thermostat();
void waterLevelTest();
int sonicRangeFinder();


// setting up ambient temperature and humidity sensor

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);



volatile double currentAmbientHumidity = 0;
volatile double currentAmbientTemperature = 0;
volatile double currentOutletTemperature = 0;
volatile boolean pumpToggle = true;
volatile boolean fanToggle = true;
volatile boolean waterLevelToggle = true;
volatile boolean lightToggle = true;





void setup(){


    dht.begin();
    pinMode(pumpSwitchPin, INPUT_PULLUP);
    pinMode(fanSwitchPin, INPUT_PULLUP);
    pinMode(lightSwitchPin, INPUT_PULLUP);

    pinMode(pumpRelayPin,OUTPUT);
    pinMode(fanRelayPin, OUTPUT);
    pinMode(lightRelayPin, OUTPUT);

    // pinMode(echoPin, INPUT);
    // pinMode(sensorTriggerPin, OUTPUT);

    // digitalWrite(sensorTriggerPin, LOW);

    ambientSensorRead();
    controllerSwitchRead();
    Serial.begin(115200);
}

void loop(){

  ambientSensorRead();
  controllerSwitchRead();
  swampController();
  lightControl();
  delay(1000);
}

void swampController(){
  pumpControl();
  fanControl();
}

void controllerSwitchRead(){
    pumpSwitchRead();
    fanSwitchRead();
    lightSwitchRead();
}

void fanControl(){
  if(fanToggle){
      digitalWrite(fanRelayPin, HIGH);
      Serial.println("Fan On");
  }
  else{
    digitalWrite(fanRelayPin, LOW);
    Serial.println("Fan Off");
  }
}

void pumpControl(){
//   waterLevelTest();
  if(pumpToggle && waterLevelToggle){
    digitalWrite(pumpRelayPin, HIGH);
    Serial.println("Pump On");
  }
  else{
    digitalWrite(pumpRelayPin, LOW);
    Serial.println("Pump Off");
  }
}

void lightControl(){
    if(lightToggle){
        digitalWrite(lightRelayPin, HIGH);
        Serial.println("Lights On");
    }
    else{
        digitalWrite(lightRelayPin, LOW);
        Serial.println("Lights Off");
    }
}

void pumpSwitchRead(){
    bool temp = pumpToggle;
    pumpToggle = digitalRead(pumpRelayPin);
    if (pumpToggle != temp){
      Serial.println("pump switch toggled");
    }
}

void fanSwitchRead(){
  bool temp = fanToggle;
    fanToggle = digitalRead(fanRelayPin);
    if(fanToggle != temp){
      Serial.println("fan switch toggled");
    }
}

void lightSwitchRead(){
    bool temp = lightToggle;
    lightToggle = digitalRead(lightRelayPin);
    if(lightToggle != temp){
      Serial.println("light switch toggled");

    }
}

void ambientSensorRead(){
    currentAmbientTemperature= dht.readTemperature();
    currentAmbientHumidity = dht.readHumidity();
    Serial.print("Current Temperature: ");
    Serial.println(currentAmbientTemperature);
    Serial.print("Current Relative Humidity: ");
    Serial.print(currentAmbientHumidity);
    Serial.println(" %");
}

void thermostat(){
    if(ambientConditionsTest){
        fanToggle = false;
        pumpToggle = true;
    }
    else{
        fanToggle = true;
        pumpToggle = true;
    }
}

void waterLevelTest(){
  double waterLevel = 1000;
  waterLevel = sonicRangeFinder();
  if(waterLevel>=dryWaterLevel){
    waterLevelToggle = true;
  }
  else{
    waterLevelToggle = false;
  }
}


boolean ambientConditionsTest(){
  boolean ambientConditionsResult = false;
  boolean ambientTemperatureResult = false;
  boolean ambientHumidityResult = false;
  currentAmbientTemperature = dht.readTemperature();
  currentAmbientHumidity = dht.readHumidity();
  if(currentAmbientTemperature >= ambientTemperatureColdLimit && currentAmbientTemperature <= ambientTemperatureHotLimit){
    ambientTemperatureResult = true;
  }
  else{ambientTemperatureResult = false;}

  if(currentAmbientHumidity >= ambientHumidityLowerLimit && currentAmbientHumidity <= ambientHumidityUpperLimit){
    ambientHumidityResult = true;
  }
  else {ambientHumidityResult = false;}

  if(ambientHumidityResult && ambientTemperatureResult){
    ambientConditionsResult = true;
  }
  /*if(ambientConditionsResult == false){
    Serial.println("!!!!!!!!!!!!!!!!!!!! Ambient conditions out of bounds !!!!!!!!!!!!!!!!!!!!");
  }*/
  return(ambientConditionsResult);
}

// int sonicRangeFinder() {
//   // fires the ultrasonic range finder and then returns the distance in mm.
//   unsigned long pulseWidth = 0;
//   unsigned long pulseT1 = 0;
//   unsigned long pulseT2 = 0;
//   int sampleCount = 5;
//
//   int distance = 0; // in mm
//   digitalWrite(sensorTriggerPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(sensorTriggerPin, LOW);
//   for(int i=0; i<sampleCount; i++){
//     while (digitalRead(echoPin) == 0) {
//       pulseT1 = micros();
//       while (digitalRead(echoPin) == 1) {
//         pulseT2 = micros();
//         pulseWidth=pulseT2-pulseT1;
//       }
//     }
//     distance = distance + pulseWidth / 5.80;
//   }
//   distance = distance / sampleCount;
//   Serial.print("water level: ");
//   Serial.println(distance);
//   return distance;
// }
