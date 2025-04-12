# Room-health-bot
#  ESP32 Robo Eye with DHT11 & IR Sensor Display

This project uses an ESP32 to create a smart animated OLED display that reacts to object detection using an IR sensor. It also displays real-time **temperature** and **humidity** using the **DHT11 sensor** when an object is detected.

##  Features

-  Animated robo eyes on SSD1306 OLED display
-  Real-time temperature and humidity display using DHT11
-  IR-based object detection to trigger display
-  Auto switch between animation and sensor data

---

##  Components Used

| Component            | Description                          |
|---------------------|--------------------------------------|
| ESP32 Dev Board     | Main microcontroller                 |
| DHT11               | Temperature and humidity sensor      |
| IR Sensor           | For object detection (like touch)    |
| SSD1306 OLED Display| 128x64 I2C display                   |
| Jumper Wires        | For connections                      |
| Breadboard          | (Optional) for prototyping           |

---

##  Hardware Connections

| Component               | ESP32 Pin  | Description                           |
|------------------------|------------|---------------------------------------|
| **DHT11 Sensor**       | GPIO 4     | Data pin                              |
| **IR Sensor**          | GPIO 5     | Digital object detection pin          |
| **OLED Display (SDA)** | GPIO 21    | I2C data                              |
| **OLED Display (SCL)** | GPIO 22    | I2C clock                             |
| **Power Supply**       | 3.3V & GND | Powers DHT11 and IR sensor            |

---

##  Libraries Required

Install these libraries from the Arduino Library Manager:

- `Adafruit_SSD1306`
- `Adafruit_GFX`
- `DHT sensor library`

---

##  How It Works

1. Robo eyes animate continuously.
2. When an object is detected via the IR sensor:
   - Eyes disappear
   - OLED shows temperature and humidity from the DHT11
3. After 2 seconds, it switches back to animation mode.



