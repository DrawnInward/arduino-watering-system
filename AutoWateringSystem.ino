#include "WateringModule.h"

// Format: controlPin, sensorPin, dryValue, optimalValue, triggerPoint, duration
WateringModule modules[] = {
  WateringModule(2, A0, 800, 400, 0.3, 3000),  // Zone 1 - water at 30% between dry and optimal
  WateringModule(3, A1, 780, 380, 0.3, 2500),  // Zone 2 - calibration values may differ per sensor
  WateringModule(4, A2, 820, 420, 0.3, 4000)   // Zone 3 - calibration values may differ per soil type
};

int numModules = sizeof(modules) / sizeof(modules[0]);

// Mode for calibration - set to true to enter calibration mode
bool calibrationMode = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Automated Watering System Starting...");
  
  if (calibrationMode) {
    Serial.println("CALIBRATION MODE ACTIVE");
    Serial.println("Follow the instructions to calibrate your soil moisture sensors");
  }
}

void loop() {
  if (calibrationMode) {
    runCalibration();
  } else {
    // Normal operation mode
    // Check and update each module
    for (int i = 0; i < numModules; i++) {
      Serial.print("Module ");
      Serial.print(i);
      Serial.print(" - Moisture: ");
      Serial.print(modules[i].getMoisturePercentage());
      Serial.println("%");
      modules[i].update();
    }
    
    delay(3600000);  // Check hourly, adjust as necessary
  }
}

// Calibration procedure
void runCalibration() {
  Serial.println("\n--- SOIL MOISTURE CALIBRATION ---");
  Serial.println("For each sensor, we'll take readings in dry and optimally moist soil");
  
  for (int i = 0; i < numModules; i++) {
    Serial.print("\nCalibrating Sensor ");
    Serial.println(i);
    
    // Dry soil reading
    Serial.println("Ensure sensor is in completely DRY soil, then press any key...");
    waitForSerialInput();
    int dryReading = 0;
    for (int j = 0; j < 10; j++) {
      dryReading += analogRead(i + A0);
      delay(100);
    }
    dryReading /= 10;
    Serial.print("Dry soil reading: ");
    Serial.println(dryReading);
    
    // Optimal soil reading
    Serial.println("Now place sensor in OPTIMALLY MOIST soil, then press any key...");
    waitForSerialInput();
    int optimalReading = 0;
    for (int j = 0; j < 10; j++) {
      optimalReading += analogRead(i + A0);
      delay(100);
    }
    optimalReading /= 10;
    Serial.print("Optimal soil reading: ");
    Serial.println(optimalReading);
    
    // Output results
    Serial.println("\nCALIBRATION RESULTS:");
    Serial.print("For sensor ");
    Serial.print(i);
    Serial.println(", use these values in your code:");
    Serial.print("Dry Value: ");
    Serial.println(dryReading);
    Serial.print("Optimal Value: ");
    Serial.println(optimalReading);
    Serial.println("----------------------------");
    delay(1000);
  }
  
  Serial.println("\nCalibration complete! Update your code with these values.");
  Serial.println("Set calibrationMode = false to return to normal operation.");
  while(1) { delay(1000); } // Stop execution
}

// Wait for serial input
void waitForSerialInput() {
  while (!Serial.available()) {
    delay(100);
  }
  while (Serial.available()) {
    Serial.read(); // Clear the input buffer
  }
}