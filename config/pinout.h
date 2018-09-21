#ifndef ARDUINO_H
  #include <Arduino.h>
#endif

#ifndef PINOUT_H
    #define pintout_h

    //pinout for arduino micro
    #define DHTPIN 10

    int const pumpRelayPin = 3;
    int const fanRelayPin = 2;
    int const lightRelayPin = 6;

    int const pumpSwitchPin = 4;
    int const fanSwitchPin = 5;
    int const lightSwitchPin = 7;

    // const int echoPin = 5;
    // const int sensorTriggerPin = 6;

    const int potPin = A1;

#endif
