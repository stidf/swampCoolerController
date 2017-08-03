#include <DHT.h>



// setting up ambient temperature and humidity sensor
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

int const pumpRelayPin = 3;
int const fanRelayPin = 4;

volatile double ambientHumidity = 0;
volatile double ambientTemperature = 0;
volatile double outletTemperature = 0;
volatile boolean pumpToggle = false;
volatile boolean fanToggle = false;

double const ambientTemperatureColdLimit = 18.0; // 20 C = 68 F
double const ambientTemperatureHotLimit = 20; // 25 C = 77 F
double const ambientHumidityUpperLimit = 75;
double const ambientHumidityLowerLimit = 0;


void setup(){


  dht.begin();
  ambientTemperature= dht.readTemperature();
  ambientHumidity = dht.readHumidity();
  pumpToggle = true;
  pumpControl();
  pinMode(pumpRelayPin,OUTPUT);
  pinMode(fanRelayPin, OUTPUT);


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

void pumpControl(){
  if(fanToggle){
    digitalWrite(fanRelayPin, HIGH);
  }
  else{
    digitalWrite(fanRelayPin, LOW);
  }
}

void fanControl(){
  if(pumpToggle){
    digitalWrite(pumpRelayPin, HIGH);
  }
  else{
    digitalWrite(pumpRelayPin, LOW);
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
