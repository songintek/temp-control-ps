#include <max6675.h>
#include <LiquidCrystal.h>

// MAX6675K thermocouple pins
const int thermoDO = 11;  // SO
const int thermoCS = 12;
const int thermoCLK = 13; // SCK

// LCD pins (adjust if your wiring is different)
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Relay pin
const int relayPin = 0;  // D0

// Button pin for input
const int buttonPin = A0;  // Analog pin for button input

// Initialize the thermocouple
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

// Variables for timing and temperature control
unsigned long previousMillis = 0;
const long interval = 1000;  // Update interval (milliseconds)
float targetTemp = 25.0;  // Initial target temperature
float currentTemp = 0.0;
const float hysteresis = 1.0;  // Temperature range for relay control
bool relayState = false;

// Button debounce variables
unsigned long lastButtonPress = 0;
const long debounceDelay = 500;  // Debounce time in milliseconds

void setup() {
  lcd.begin(16, 2);
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(relayPin, LOW);  // Start with relay off
  delay(500);  // Wait for MAX6675 chip to stabilize
}

void loop() {
  unsigned long currentMillis = millis();

  // Check for button press to adjust target temperature
  int buttonState = analogRead(buttonPin);
  if (currentMillis - lastButtonPress > debounceDelay) {
    if (buttonState < 100) {  // Right button
      targetTemp += 10;
      lastButtonPress = currentMillis;
    } else if (buttonState < 250) {  // Up button
      targetTemp += 1;
      lastButtonPress = currentMillis;
    } else if (buttonState < 500) {  // Down button
      targetTemp -= 1;
      lastButtonPress = currentMillis;
    } else if (buttonState < 750) {  // Left button
      targetTemp -= 10;
      lastButtonPress = currentMillis;
    }
  }

  // Update temperature and control relay every second
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Read current temperature
    currentTemp = thermocouple.readCelsius();

    // Control relay based on temperature
    if (currentTemp < targetTemp - hysteresis && !relayState) {
      digitalWrite(relayPin, HIGH);  // Turn on heating
      relayState = true;
    } else if (currentTemp > targetTemp + hysteresis && relayState) {
      digitalWrite(relayPin, LOW);  // Turn off heating
      relayState = false;
    }

    // Update LCD display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tgt:");
    lcd.print(targetTemp, 1);
    lcd.print("C ");
    lcd.print(relayState ? "ON" : "OFF");
    lcd.setCursor(0, 1);
    lcd.print("Cur:");
    lcd.print(currentTemp, 1);
    lcd.print("C");
  }
}