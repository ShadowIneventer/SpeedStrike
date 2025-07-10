# ⚡ SpeedStrike – Reaction Time Tester

A real-time reflex game built on Arduino!  
Challenge yourself or a friend to see who reacts faster. Includes AI mode with difficulty levels, stats tracking, buzzer feedback, and LCD output.

![Made with Arduino](https://img.shields.io/badge/made%20with-Arduino-blue)
![License: MIT](https://img.shields.io/badge/license-MIT-green)

---

## 🎮 Features

- 🧠 **AI Mode** – Compete against random delay challenges  
- 🆚 **2-Player Mode** – First to press wins  
- ⏱️ **Reaction Time Display** – Real-time result on LCD  
- 🔔 **Buzzer Feedback** – For start signal and results  
- 📊 **Average of Last 5 Tries** – Track your consistency  
- 🎚️ **Difficulty Setting (AI Mode)** – Easy / Medium / Hard  
- 🔁 **Mode Switch Button** – Toggle between AI & 2P instantly

---

## 🔌 Components Used

- ✅ Arduino Uno / Nano / Mega  
- ✅ LCD Display (LiquidCrystal I2C)  
- ✅ 2 Push Buttons  
- ✅ 1 Passive Buzzer  
- ✅ Jumper Wires + Breadboard  

---

## 🧠 How It Works

- On startup, choose between **AI Mode** or **2-Player Mode**
- In AI mode, the Arduino triggers a start beep after a random delay — your reaction is measured in milliseconds
- In 2P mode, both players wait for the beep — the faster button press wins!
- Your average of the last 5 rounds is shown for practice and improvement

---

## 📟 Wiring Instructions

| **Component**        | **Arduino Pin**     | **Details**                              |
|----------------------|---------------------|-------------------------------------------|
| LCD I2C Display      | SDA → A4 (Uno/Nano) | Use I2C interface                         |
|                      | SCL → A5 (Uno/Nano) |                                           |
| Button 1 (Player 1)  | D2                  | Use pull-down resistor or INPUT_PULLUP    |
| Button 2 (Player 2)  | D3                  | Use pull-down resistor or INPUT_PULLUP    |
| Mode Select Button   | D4                  | Switch between AI and 2-Player modes      |
| OK Button            | D4                  | To select conform modes                   |
| Difficulty Button    | D5                  | Cycle through AI difficulty levels        |
| Passive Buzzer       | D6                  | Active HIGH to buzz                       |
| GND (All Components) | GND                 | Connect all GNDs to Arduino GND           |
| VCC (LCD + Buzzer)   | 5V                  | LCD & buzzer need 5V                      |

---

## 📁 Project Structure

<pre> SpeedStrike/ ├── <b>firmware/</b> │ └── reaction_time_tester.ino ← Arduino sketch ├── <b>libs/</b> │ └── LiquidCrystal_I2C/ ← Manual library (if needed) ├── <b>.vscode/</b> │ └── c_cpp_properties.json ← IntelliSense config ├── <b>docs/</b> │ └── wiring-diagram.png ← Optional: circuit diagram ├── <b>images/</b> │ └── demo.gif ← Optional: demo recording ├── <b>README.md</b> ← Project overview ├── <b>LICENSE</b> ← MIT License (open source!) ├── <b>.gitignore</b> ← Git exclusions (bin/, libs/, etc.) ├── <b>bin/</b> ← Optional: CLI tools or temp files </pre>
