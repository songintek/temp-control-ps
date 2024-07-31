#include <LiquidCrystal.h>

// Initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Analog pin for button input
const int buttonPin = A0;

// Define key value ranges
const int rightThreshold = 60;
const int upThreshold = 200;
const int downThreshold = 400;
const int leftThreshold = 600;
const int selectThreshold = 800;

void setup() {
  // Set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  
  // Set up the backlight control pin
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);  // Turn on backlight
}

void loop() {
  // Read the analog value from the button
  int buttonValue = analogRead(buttonPin);
  
  // Clear the LCD
  lcd.clear();
  
  // Display the analog value
  lcd.setCursor(0, 0);
  lcd.print("Analog Value:");
  lcd.setCursor(0, 1);
  lcd.print(buttonValue);
  
  // Identify the button pressed
  lcd.setCursor(8, 1);
  if (buttonValue < rightThreshold) {
    lcd.print("Right");
  } else if (buttonValue < upThreshold) {
    lcd.print("Up");
  } else if (buttonValue < downThreshold) {
    lcd.print("Down");
  } else if (buttonValue < leftThreshold) {
    lcd.print("Left");
  } else if (buttonValue < selectThreshold) {
    lcd.print("Select");
  } else {
    lcd.print("None");
  }
  
  // Wait a bit before next reading
  delay(200);
}