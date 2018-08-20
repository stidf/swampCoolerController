#ifndef ARDUINO_H
  #include <arduino.h>
#endif

#ifndef PINOUT_H
    #define pintout_h

    //pinout for arduino micro
    int const DHTPIN = 2;

    int const pumpRelayPin = 3;
    int const fanRelayPin = 4;
    int const lightRelayPin = 5;

    int const pumpSwitchPin = 10;
    int const fanSwitchPin = 11;
    int const lightSwitchPin = 12;

    const int echoPin = 5;
    const int sensorTriggerPin = 6;

    const int potPin = A1;

#endif
