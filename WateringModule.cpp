#include "WateringModule.h"
#include <Arduino.h>

// Constructor implementation
WateringModule::WateringModule(int _controlPin, int _sensorPin, int _dryValue, int _optimalValue, 
                              float _triggerPoint, unsigned long _duration) {
  controlPin = _controlPin;
  sensorPin = _sensorPin;
  dryValue = _dryValue;
  optimalValue = _optimalValue;
  triggerPoint = _triggerPoint;
  wateringDuration = _duration;
  lastWateringTime = 0;
  minTimeBetweenWaterings = 43200000; // 12 hours, set accordingly in ms 
  
  // Calculate the moisture threshold based on calibration values
  moistureThreshold = dryValue - ((dryValue - optimalValue) * triggerPoint);
  
  pinMode(controlPin, OUTPUT);
  pinMode(sensorPin, INPUT);
}

// Check if watering is needed
void WateringModule::update() {
  int moistureLevel = readMoisture();
  
  // Only water if moisture is below threshold AND enough time has passed since last watering
  if (moistureLevel > moistureThreshold && 
      (millis() - lastWateringTime > minTimeBetweenWaterings)) {
    water();
    lastWateringTime = millis();
  }
}

// Read moisture level
int WateringModule::readMoisture() {
  // Take multiple readings and average them for better accuracy
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    sum += analogRead(sensorPin);
    delay(10);
  }
  return sum / 3;
}

// Get moisture as percentage (0% = dry, 100% = optimal)
int WateringModule::getMoisturePercentage() {
  int rawValue = readMoisture();
  
  // Convert to percentage (ensure values stay in bounds)
  int percentage = map(constrain(rawValue, optimalValue, dryValue), 
                      dryValue, optimalValue, 0, 100);
  return percentage;
}

// Activate watering
void WateringModule::water() {
  digitalWrite(controlPin, HIGH);
  delay(wateringDuration);
  digitalWrite(controlPin, LOW);
}