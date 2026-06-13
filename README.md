# 🥚 IncuBot

**IncuBot** is an ESP32-based smart egg incubator system with automated environmental control, monitoring, and remote access.

---

## 📌 Overview

IncuBot manages egg incubation conditions by automatically controlling temperature, humidity, ventilation, and egg rotation. It also provides real-time monitoring through a web interface.

Built for reliability, modularity, and expandability in DIY and research incubation systems.

---

## ⚙️ Features

- 🌡️ Automatic temperature control (heater regulation)
- 💧 Humidity monitoring and control support
- 🌀 Fan control for air circulation
- 🥚 Automated egg turning system (stepper motor)
- 📊 Real-time sensor monitoring (DHT22 or similar)
- 🌐 ESP32 web dashboard for remote access
- 🔌 Manual override for all actuators
- 📡 Expandable IoT-ready architecture

---

## 🧠 Hardware

- ESP32 microcontroller
- DHT22 temperature & humidity sensor
- Relay module (heater control)
- TB6612FNG / motor driver (fan control)
- Stepper motor (egg turning)
- I2C LCD/OLED display (optional)

---

## 📡 Web Interface

- Live temperature & humidity
- Heater / fan status
- Egg turning control
- System state monitoring
- Manual override toggles

---

## 🛠️ Firmware

Written in Arduino C++ for ESP32 with a modular architecture:

- Sensors module
- Actuators module
- Control logic
- Networking layer
- Web server interface

---

## 🚀 Future Improvements

- MQTT / cloud integration
- Data logging (SD / server)
- Mobile dashboard
- AI-based incubation optimization
- Power failure recovery system

---

## 📜 License

MIT License
