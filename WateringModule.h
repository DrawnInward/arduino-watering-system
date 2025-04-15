#ifndef WATERING_MODULE_H
#define WATERING_MODULE_H

class WateringModule {
  private:
    int controlPin;         // Pin controlling solenoid/pump
    int sensorPin;          // Pin for moisture sensor
    int dryValue;           // Sensor reading when soil is completely dry
    int optimalValue;       // Sensor reading when soil is optimally moist
    float triggerPoint;     // Point between dry and optimal to trigger watering (0.0-1.0)
    int moistureThreshold;  // Calculated threshold to trigger watering
    unsigned long wateringDuration; // How long to water in milliseconds
    unsigned long lastWateringTime; // When we last watered
    unsigned long minTimeBetweenWaterings; // Minimum time between waterings
    
  public:
    // Constructor
    WateringModule(int _controlPin, int _sensorPin, int _dryValue, int _optimalValue, 
                  float _triggerPoint, unsigned long _duration);
    
    // Methods
    void update();
    int readMoisture();
    void water();
    int getMoisturePercentage(); // Returns moisture as percentage from dry (0%) to optimal (100%)
};

#endif