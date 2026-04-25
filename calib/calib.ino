// Smart Exhaust & Safety System - PES University Lab
// Gas Sensor: MQ-2 (Analog A1) | Temp Sensor: LM235Z (Analog A0)

void setup() {
  Serial.begin(9600);
  Serial.println("--- System Initializing ---");
  Serial.println("Stabilizing sensors... Please wait.");
}

void loop() {
  // 1. High-Precision Temperature Sampling (Averaging 100 readings)
  long tempSum = 0;
  for(int i = 0; i < 100; i++) {
    tempSum += analogRead(A0);
    delay(2); // Small delay for ADC capacitor to settle
  }
  float avgRawTemp = tempSum / 100.0;

  // 2. Gas Sensor Sampling (Averaging 20 readings)
  long gasSum = 0;
  for(int i = 0; i < 20; i++) {
    gasSum += analogRead(A1);
    delay(1);
  }
  float avgRawGas = gasSum / 20.0;

  // 3. Precision Conversion Math
  // Formula: (Raw * ReferenceVoltage) / Resolution
  float mVolts = (avgRawTemp * 5000.0) / 1023.0; 
  
  // Convert mV to Kelvin (10mV/K) and then to Celsius
  float tempC = (mVolts / 10.0) - 273.15;

  // 4. Offset Correction
  // If your lab is 27C but code says 52C, the offset is 25.
  // Uncomment the line below and adjust the number to match your room thermometer
  // tempC = tempC - 25.0; 

  // 5. Final Output to Serial Monitor
  Serial.print("GAS [Level: ");
  Serial.print(avgRawGas, 1);
  Serial.print("] | TEMP [mV: ");
  Serial.print(mVolts, 0);
  Serial.print(" | C: ");
  Serial.print(tempC, 2); // Two decimal places for precision
  Serial.println("]");
  
  delay(1000); // Update every second
}