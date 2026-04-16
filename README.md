# 🥤 Vending Machine Jamu Otomatis

An automated herbal drink (Jamu) vending machine built with **Arduino Mega**. This machine allows users to select from 5 different types of Jamu and choose their preferred water temperature (Cold, Warm, or Hot). It features a coin-operated system, precise liquid dispensing using flow sensors, and safety detection using ultrasonic sensors.

## 🌟 Features

- **5 Varieties of Jamu:** Selectable via push buttons.
- **Temperature Control:** Options for Cold (Dingin), Warm (Hangat), and Hot (Panas) water.
- **Coin System:** Accepts coins via a coin acceptor (Pulse counting).
- **Precision Dispensing:** Uses a flow sensor to measure and dispense the exact volume (20ml Jamu + 180ml Water).
- **Safety Sensor:** Ultrasonic sensor ensures a cup is placed before dispensing liquid.
- **LCD Interface:** 20x4 I2C LCD to guide users through the transaction process.

## ⚙️ Hardware Components

- **Microcontroller:** Arduino Mega
- **Display:** LCD 20x4 with I2C module
- **Sensors:**
  - Ultrasonic Sensor (HC-SR04) - Cup detection
  - Flow Sensor (YF-S201) - Liquid volume measurement
  - Coin Acceptor - Payment input
- **Actuators:**
  - 5x Relay Modules (for Jamu pumps)
  - 2x Relay Modules (for Hot and Cold water solenoids/pumps)
- **Input:** 8x Push Buttons (Selection & Temp)

## 📂 Repository Structure

```
.
├── Vending_Machine_Jamu.ino   # Main Arduino Mega source code
├── dispenser/                 # Folder containing 3D design files (STL, etc.)
├── wiring1.sch                # Eagle schematic file (Wiring diagram)
└── README.md                  # Project documentation
```

## 🔌 Wiring

The hardware wiring connections are documented in the `wiring1.sch` file included in this repository.

**Pin Configuration Summary (based on code):**
- **Ultrasonic:** Trig `A1`, Echo `A0`
- **Relays (Jamu):** Pins `23, 24, 25, 26, 27`
- **Relays (Water):** Hot `28`, Cold `29`
- **Flow Sensor:** Pin `18` (Interrupt)
- **Coin Acceptor:** Pin `3` (Interrupt)
- **Buttons:** Pins `4, 5, 6, 7, 8` (Jamu), `9, 10, 11` (Temp)

## 🚀 How It Works

The system operates in the following sequence:

1.  **Payment:** User inserts coins. The price is set to **5 coins** per cup.
2.  **Selection:**
    -   Select Jamu type (Button 1-5).
    -   Select Water Temperature (Cold/Warm/Hot).
3.  **Detection:** User must place a cup in the dispensing area. The ultrasonic sensor detects the cup if the distance is < 9cm.
4.  **Dispensing:**
    -   The machine dispenses **20ml** of the selected Jamu concentrate.
    -   It then dispenses **180ml** of water at the selected temperature.
    -   Total volume per cup: **200ml**.
5.  **Completion:** The system resets credit and prepares for the next order.

## 🛠️ Development

This project was developed to modernize the way traditional Jamu is distributed, combining traditional heritage with modern IoT and automation technology.
