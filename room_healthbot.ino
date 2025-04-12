#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define DHTPIN 4         // DHT11 data pin connected to GPIO4
#define DHTTYPE DHT11    // DHT11 sensor type

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

// IR sensor pin (adjust as per your connection)
#define IR_SENSOR_PIN 5  // Pin connected to the IR sensor

// Eye position and size
int leftEyeX = 45;
int rightEyeX = 80;
int eyeY = 18;
int eyeWidth = 25;
int eyeHeight = 30;

// Target offsets for smooth movement
int targetOffsetX = 0;
int targetOffsetY = 0;
int moveSpeed = 5;

// Blinking and animation variables
int blinkState = 0;
int blinkDelay = 4000;
unsigned long lastBlinkTime = 0;
unsigned long moveTime = 0;
unsigned long displayTime = 0;
bool isDisplayingDHT = false;  // Renamed variable

float t; // temperature
float h; // humidity

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // OLED setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Stay in an infinite loop if the display fails
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  
  pinMode(IR_SENSOR_PIN, INPUT);  // Set IR sensor pin as input
}

void loop() {
  unsigned long currentTime = millis();
  bool objectDetected = digitalRead(IR_SENSOR_PIN) == LOW; // Check if IR sensor detects an object (adjust for your IR sensor's logic)

  if (objectDetected && (currentTime - displayTime) > 2000) {  // Display DHT11 reading for 2 seconds
    isDisplayingDHT = true;
    displayTime = currentTime;
  }
  
  if (isDisplayingDHT) {
    showDHTData();
  } else {
    showRoboEyeAnimation();
  }
  
  delay(30); // Small delay to prevent flickering
}

void showDHTData() {
  // Read DHT11 data
  t = dht.readTemperature();
  h = dht.readHumidity();

  // Print to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\tHumidity: ");
  Serial.print(h);
  Serial.println(" %");

  // Print to OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(t);
  display.println(" C");

  display.print("Hum: ");
  display.print(h);
  display.println(" %");

  display.display();

  // After 2 seconds, go back to Robo Eye animation
  if (millis() - displayTime > 2000) {
    isDisplayingDHT = false;
  }
}

void showRoboEyeAnimation() {
  unsigned long currentTime = millis();

  // Blinking logic
  if (currentTime - lastBlinkTime > blinkDelay && blinkState == 0) {
    blinkState = 1;
    lastBlinkTime = currentTime;
  } else if (currentTime - lastBlinkTime > 150 && blinkState == 1) {
    blinkState = 0;
    lastBlinkTime = currentTime;
  }

  // Random eye movement logic
  if (currentTime - moveTime > random(1500, 3000) && blinkState == 0) {
    int movementType = random(0, 8);  // Choose movement: 0 to 7 (diagonal + center)
    if (movementType == 0) {          // Look left
      targetOffsetX = -10;
      targetOffsetY = 0;
    } else if (movementType == 1) {   // Look right
      targetOffsetX = 10;
      targetOffsetY = 0;
    } else if (movementType == 2) {   // Look up-left
      targetOffsetX = -10;
      targetOffsetY = -8;
    } else if (movementType == 3) {   // Look up-right
      targetOffsetX = 10;
      targetOffsetY = -8;
    } else if (movementType == 4) {   // Look down-left
      targetOffsetX = -10;
      targetOffsetY = 8;
    } else if (movementType == 5) {   // Look down-right
      targetOffsetX = 10;
      targetOffsetY = 8;
    } else {                          // Default (center)
      targetOffsetX = 0;
      targetOffsetY = 0;
    }
    moveTime = currentTime;
  }

  // Smooth interpolation for movement
  static int offsetX = 0;
  static int offsetY = 0;
  offsetX += (targetOffsetX - offsetX) / moveSpeed;
  offsetY += (targetOffsetY - offsetY) / moveSpeed;

  // Clear display for redraw
  display.clearDisplay();

  // Draw the left eye
  if (blinkState == 0) {
    drawEye(leftEyeX + offsetX, eyeY + offsetY, eyeWidth, eyeHeight);
  } else {
    display.fillRect(leftEyeX + offsetX, eyeY + offsetY + eyeHeight / 2 - 2, eyeWidth, 4, WHITE); // Eye blink
  }

  // Draw the right eye
  if (blinkState == 0) {
    drawEye(rightEyeX + offsetX, eyeY + offsetY, eyeWidth, eyeHeight);
  } else {
    display.fillRect(rightEyeX + offsetX, eyeY + offsetY + eyeHeight / 2 - 2, eyeWidth, 4, WHITE); // Eye blink
  }

  // Refresh the display
  display.display();
}

void drawEye(int eyeX, int eyeY, int eyeWidth, int eyeHeight) {
  display.fillRoundRect(eyeX, eyeY, eyeWidth, eyeHeight, 5, WHITE);  // Draw rounded rectangle
}
