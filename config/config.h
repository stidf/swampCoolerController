#ifndef ARDUINO_H
    #include <Arduino.h>
#endif

#ifndef config_h
    #define config_h

    double const ambientTemperatureColdLimit = 18.0; // 20 C = 68 F
    double const ambientTemperatureHotLimit = 20; // 25 C = 77 F
    double const ambientHumidityUpperLimit = 75;
    double const ambientHumidityLowerLimit = 0;

    double const dryWaterLevel = 40; // level of inlet pipe in mm
    const int maxRange =  4000; // limit of sensor; in mm

#endif