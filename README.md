🥚 IncuBot

An open-source ESP32-based smart egg incubator featuring automated environmental control, real-time monitoring, remote management, and telemetry integration.

Designed for hobbyists, makers, researchers, and small-scale poultry operations, IncuBot provides precise control of incubation conditions while offering local and cloud-based monitoring capabilities.

---

✨ Features

Environmental Control

- Automatic temperature regulation
- Humidity monitoring and control
- Fan-assisted air circulation
- Configurable hysteresis control logic
- Over-temperature protection

Egg Management

- Automated egg turning system
- Configurable turning intervals
- Manual turn override
- Support for chicken, guinea fowl, duck, and other poultry eggs

Monitoring & Connectivity

- Built-in ESP32 web dashboard
- MQTT telemetry publishing
- InfluxDB data logging
- Grafana visualization dashboards
- Telegram bot integration
- Real-time status monitoring
- Wi-Fi connectivity

Manual Control

- Heater override
- Fan override
- Humidifier override
- Egg turner override

User Interface

- I2C LCD display support
- Status LED indicators
- Local system status display

---

🧠 Hardware

- ESP32 Development Board
- DHT22 Temperature & Humidity Sensor
- Relay Module (Heater Control)
- TB6612FNG Motor Driver
- DC Fan
- Stepper Motor (Egg Turner)
- I2C LCD Display
- Heating Element

---

📊 System Architecture

ESP32
 ├── DHT22 Sensor
 ├── Heater Control
 ├── Fan Control
 ├── Humidifier Control
 ├── Stepper Egg Turner
 ├── LCD Interface
 ├── Web Dashboard
 ├── MQTT Client
 ├── Telegram Bot
 └── InfluxDB Telemetry

MQTT → InfluxDB → Grafana

---

📡 Telemetry

Sensor readings and actuator states are published via MQTT and stored in InfluxDB for historical analysis and visualization through Grafana dashboards.

Tracked metrics include:

- Temperature
- Humidity
- Heater State
- Fan State
- Humidifier State
- Egg Turner Events
- Wi-Fi Status
- System Uptime

---

📱 Telegram Integration

Supported commands include:

- "/status"
- "/temp"
- "/hum"
- "/heater_on"
- "/heater_off"
- "/hum_on"
- "/hum_off"
- "/fan_on"
- "/fan_off"

---

🚀 Firmware

Built using:

- Arduino Framework
- PlatformIO
- ESPAsyncWebServer
- PubSubClient
- UniversalTelegramBot

---

📦 Releases

Prebuilt firmware binaries are available under GitHub Releases.

Files included:

- "bootloader.bin"
- "partitions.bin"
- "firmware.bin"

Flash using:

esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 460800 \
  write_flash \
  0x1000 bootloader.bin \
  0x8000 partitions.bin \
  0x10000 firmware.bin

---

🛣️ Roadmap

- OTA firmware updates
- Multiple incubation profiles
- Hatch countdown timer
- Historical hatch analytics
- Multi-incubator management
- Mobile application

---

📜 License

MIT License