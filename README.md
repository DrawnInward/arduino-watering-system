# Arduino Automated Watering System

A modular, solar-powered automated watering system using Arduino. This system can control multiple watering zones independently based on soil moisture sensors.

## Features

- Modular design: Each watering zone is controlled independently
- Soil moisture threshold-based watering with proper calibration
- Overwatering prevention with time constraints
- Compatible with 12V solar power systems
- Easy to add or remove zones

## Hardware Requirements

- Arduino board (Uno, Nano, etc.)
- Soil moisture sensors (one per zone)
- Relays or transistors for controlling solenoids/pumps
- Solenoid valves or water pumps
- 12V power source (solar panel + charge controller)
- Appropriate wiring

## Wiring

Connect components as follows:
- Digital pins 2, 3, 4, etc. → Relay inputs for solenoids/pumps
- Analog pins A0, A1, A2, etc. → Soil moisture sensors
- See wiring diagram in docs folder for details

## Installation

1. Clone this repository
2. Open AutoWateringSystem.ino in the Arduino IDE
3. Calibrate your soil moisture sensors (see calibration section)
4. Update the modules array with your calibration values
5. Upload to your Arduino

## Understanding the Calibration System

### How Calibration Works

The calibration process creates a customized moisture scale for each sensor and soil combination:

1. **Two Calibration Points**:
   - **Dry Value**: The sensor reading when soil is completely dry (typically 800-1000)
   - **Optimal Value**: The reading when soil has ideal moisture (typically 350-500)

2. **Moisture Threshold Calculation**:
   ```
   moistureThreshold = dryValue - ((dryValue - optimalValue) * triggerPoint)
   ```
   
   This creates a specific point between dry and optimal where watering should begin.

3. **Example**:
   - Dry Value: 850
   - Optimal Value: 400
   - Trigger Point: 0.3 (water when 30% between dry and optimal)
   - Calculation: 850 - ((850 - 400) * 0.3) = 715
   - Result: System waters when reading goes above 715

4. **Percentage Conversion**:
   The system converts raw readings to a user-friendly 0-100% scale where:
   - 0% = Completely dry (dryValue)
   - 100% = Optimally moist (optimalValue)

### Why Calibration Is Important

- Sensors vary between units
- Different soil types have different electrical properties
- Environmental factors affect readings
- Without calibration, a fixed threshold is unreliable

## Calibration Procedure

### Automatic Calibration (Recommended)

1. Set `calibrationMode = true` in AutoWateringSystem.ino
2. Upload the code to your Arduino
3. Open the Serial Monitor (set to 9600 baud)
4. Follow the on-screen instructions to calibrate each sensor
5. Record the values for each sensor
6. Set `calibrationMode = false`
7. Update the `modules` array with your calibration values
8. Re-upload the code

### Manual Calibration

If you prefer to calibrate manually:

1. Insert each sensor into completely dry soil and record the analog reading
2. Insert each sensor into optimally moist soil and record the analog reading 
3. Update the `modules` array with these values

## Setting Up Your Modules

After calibration, configure your watering zones in the `modules` array:

```cpp
WateringModule modules[] = {
  // controlPin, sensorPin, dryValue, optimalValue, triggerPoint, duration
  WateringModule(2, A0, 850, 400, 0.3, 3000),  // Zone 1
  WateringModule(3, A1, 830, 420, 0.3, 2500),  // Zone 2
  WateringModule(4, A2, 860, 380, 0.3, 4000)   // Zone 3
};
```

### Parameters Explained:

1. **controlPin**: Digital pin connected to the relay/valve (e.g., 2)
2. **sensorPin**: Analog pin connected to the moisture sensor (e.g., A0)
3. **dryValue**: Calibrated value for dry soil (e.g., 850)
4. **optimalValue**: Calibrated value for ideally moist soil (e.g., 400)
5. **triggerPoint**: When to start watering (0.0-1.0):
   - 0.0: Water only when completely dry
   - 0.3: Water when 30% between dry and optimal (recommended)
   - 1.0: Water as soon as moisture falls below optimal
6. **duration**: How long to water in milliseconds (e.g., 3000 = 3 seconds)

### Determining Watering Duration

The appropriate watering duration depends on:
- **Soil type**: Sandy soils drain faster than clay
- **Container size**: Larger pots need longer watering
- **Plant type**: Succulents need less water than tropical plants
- **Water flow rate**: Depends on your pump/valve setup

Start conservatively (3-5 seconds) and adjust based on:
- How deeply the water penetrates the soil
- How quickly the soil dries out after watering
- Plant response and health

## Usage

The system will:
1. Check soil moisture levels at regular intervals
2. Activate watering for zones where moisture is below threshold
3. Prevent overwatering by limiting watering frequency

To add more zones, simply add new WateringModule objects to the modules array.

## Monitoring and Maintenance

- Connect to the Arduino via USB to view moisture levels and system status
- Use the Serial Monitor (9600 baud) to see real-time readings
- Recalibrate seasonally or when changing soil or location
- Check sensor placement - consistent depth is important

## Troubleshooting

### Common Issues

1. **Inconsistent Readings**
   - Ensure sensor is making good contact with soil
   - Check for corrosion on sensor probes
   - Try recalibrating in controlled conditions

2. **No Watering Activation**
   - Check relay/solenoid connections
   - Verify power supply is sufficient
   - Ensure threshold values are appropriate

3. **System Waters Too Frequently**
   - Adjust triggerPoint to a lower value
   - Increase minTimeBetweenWaterings
   - Check for sensor placement issues

4. **Overwatering Issues**
   - Reduce watering duration
   - Adjust triggerPoint to water only when drier
   - Check for sensor placement in unusually dry pocket

### Debugging Tips

- Use `Serial.print()` statements to track variable values
- Monitor multiple readings over time to spot trends
- Check battery voltage if using solar power

## License

[MIT License](LICENSE)