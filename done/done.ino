#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// --- Pins ---
const int tempPin = A0;    // LM235Z on A0
const int gasPin = A1;     // MQ-2 on A1
const int fanPin = 9;
const int buzzerPin = 4;
const int ledPin = 2;
const int silencePin = 7;

// --- Thresholds (Based on your Calibration) ---
const float tempThreshold = 35.0; // Trigger if room gets over 35C
const int gasThreshold = 50;      // Trigger if gas is > 50% of range

// --- State Variables ---
bool alarmSilenced = false;
bool lastButtonState = HIGH;
unsigned long lastUpdateTime = 0; 
const int updateInterval = 500; 

LiquidCrystal_I2C lcd(0x27, 16, 2); // Common address is 0x27

void setup() {
  pinMode(fanPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(silencePin, INPUT_PULLUP);
  
  lcd.init();
  lcd.backlight();
  lcd.print("PES SMART SYSTEM");
  delay(2000);
}

void loop() {
  // 1. FAST BUTTON CHECK
  bool currentButtonState = digitalRead(silencePin);
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    alarmSilenced = !alarmSilenced; 
    if (alarmSilenced) noTone(buzzerPin); 
    delay(50); // Debounce
  }
  lastButtonState = currentButtonState;

  // 2. TIMED SENSOR & LCD UPDATE
  if (millis() - lastUpdateTime >= updateInterval) {
    lastUpdateTime = millis();

    // -- Temperature Math for LM235Z --
    int rawTemp = analogRead(tempPin);
    float mVolts = (rawTemp * 5000.0) / 1023.0;
    float tempC = (mVolts / 10.0) - 273.15;

    // -- Gas Math for MQ-2 (Mapping 175-800 to 0-100%) --
    int rawGas = analogRead(gasPin);
    int gasPercent = map(rawGas, 175, 1023, 0, 100); 
    gasPercent = constrain(gasPercent, 0, 100);

    // -- Fan Control (PWM based on higher of the two risks) --
    int fanPWM = max(map(tempC, 22, 40, 0, 255), map(gasPercent, 0, 100, 0, 255));
    analogWrite(fanPin, constrain(fanPWM, 0, 255));

    bool isUnsafe = (tempC > tempThreshold || gasPercent > gasThreshold);

    if (isUnsafe) {
      digitalWrite(ledPin, HIGH);
      if (!alarmSilenced) {
        tone(buzzerPin, 1000);
        updateLCD(tempC, gasPercent, "!! DANGER !!");
      } else {
        noTone(buzzerPin);
        updateLCD(tempC, gasPercent, "MUTED - ALERT");
      }
    } else {
      digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
      alarmSilenced = false; 
      updateLCD(tempC, gasPercent, "SYSTEM NORMAL");
    }
  }
}

void updateLCD(float t, int g, String msg) {
  lcd.setCursor(0, 0);
  lcd.print(msg);
  lcd.print("     "); 
  lcd.setCursor(0, 1);
  lcd.print("T:"); lcd.print((int)t); lcd.print("C G:"); 
  lcd.print(g); lcd.print("%    ");
}