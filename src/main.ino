#include <DHT.h>
#include "SPI.h"


// setting up ambient temperature and humidity sensor
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

int const pumpRelayPin = 3;
int const fanRelayPin = 4;
const int echoPin = 5;
const int sensorTriggerPin = 6;

volatile double ambientHumidity = 0;
volatile double ambientTemperature = 0;
volatile double outletTemperature = 0;
volatile boolean pumpToggle = false;
volatile boolean fanToggle = false;
volatile boolean waterLevelToggle = false;

double const ambientTemperatureColdLimit = 18.0; // 20 C = 68 F
double const ambientTemperatureHotLimit = 20; // 25 C = 77 F
double const ambientHumidityUpperLimit = 75;
double const ambientHumidityLowerLimit = 0;
double const dryWaterLevel = 40; // level of inlet pipe in mm

const int maxRange =  4000; // limit of sensor; in mm


void setup(){


  dht.begin();
  ambientTemperature= dht.readTemperature();
  ambientHumidity = dht.readHumidity();
  pumpToggle = true;
  pumpControl();
  pinMode(pumpRelayPin,OUTPUT);
  pinMode(fanRelayPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(sensorTriggerPin, OUTPUT);

  digitalWrite(sensorTriggerPin, LOW);
  Serial.begin(115200);



}

void loop(){

  ambientTemperature= dht.readTemperature();
  ambientHumidity = dht.readHumidity();
  if(ambientConditionsTest){
    fanToggle = false;
    pumpToggle = true;
  }
  else{
    fanToggle = true;
    pumpToggle = true;
  }
  swampController();
}

void swampController(){
  pumpControl();
  fanControl();
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
  waterLevelTest();
  if(pumpToggle && waterLevelToggle){
    digitalWrite(pumpRelayPin, HIGH);
    Serial.println("Pump On");
  }
  else{
    digitalWrite(pumpRelayPin, LOW);
    Serial.println("Pump Off");
  }
}

void waterLevelTest(){
  double waterLevel = 0;
  waterLevel = sonicRangeFinder();
  if(waterLevel>=dryWaterLevel){
    waterLevelToggle = true;
  }
  else{
    waterLevelToggle = false;
  }
}


boolean ambientConditionsTest (){
  boolean ambientConditionsResult = false;
  boolean ambientTemperatureResult = false;
  boolean ambientHumidityResult = false;
  ambientTemperature = dht.readTemperature();
  ambientHumidity = dht.readHumidity();
  if(ambientTemperature >= ambientTemperatureColdLimit && ambientTemperature <= ambientTemperatureHotLimit){
    ambientTemperatureResult = true;
  }
  if(ambientHumidity >= ambientHumidityLowerLimit && ambientHumidity <= ambientHumidityUpperLimit){
    ambientHumidityResult = true;
  }
  if(ambientHumidityResult && ambientTemperatureResult){
    ambientConditionsResult = true;
  }
  /*if(ambientConditionsResult == false){
    Serial.println("!!!!!!!!!!!!!!!!!!!! Ambient conditions out of bounds !!!!!!!!!!!!!!!!!!!!");
  }*/
  return(ambientConditionsResult);
}

int sonicRangeFinder() {
  // fires the ultrasonic range finder and then returns the distance in mm.
  unsigned long pulseWidth = 0;
  unsigned long pulseT1 = 0;
  unsigned long pulseT2 = 0;
  int sampleCount = 5;

  int distance = 0; // in mm
  digitalWrite(sensorTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensorTriggerPin, LOW);
  for(int i=0; i<sampleCount; i++){
    while (digitalRead(echoPin) == 0) {
      pulseT1 = micros();
      while (digitalRead(echoPin) == 1) {
        pulseT2 = micros();
        pulseWidth=pulseT2-pulseT1;
      }
    }
    distance = distance + pulseWidth / 5.80;
  }
  distance = distance / sampleCount;
  Serial.print("water level: ");
  Serial.println(distance);
  return distance;
}
