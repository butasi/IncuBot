#pragma once

#include <Arduino.h>
#define SENSOR_READ_INTERVAL 2000UL

// ================= WIFI =================
constexpr char WIFI_SSID[] = "xxxxxxxx-xxxxxx";
constexpr char WIFI_PASSWORD[] = "xxxxxxxx-xxxxxxxx";

// ================= MQTT =================
constexpr char MQTT_SERVER[] = "192.168.100.11";
constexpr uint16_t MQTT_PORT = 1883;

// ================= TELEGRAM =================
constexpr char BOT_TOKEN[] =
    "xxxxxxxx:xxxxxxxx-xxxxxxxx";

constexpr char CHAT_ID[] = "xxxxxxxx";

// ================= DHT =================
constexpr uint8_t DHT_PIN  = 13;
constexpr uint8_t DHT_TYPE = 11; // DHT11

// ================= LCD =================
constexpr uint8_t LCD_ADDR = 0x27;
constexpr uint8_t LCD_COLS = 16;
constexpr uint8_t LCD_ROWS = 2;

// ================= PINS =================
constexpr uint8_t HEATER_PIN = 23;

constexpr uint8_t HUMIDIFIER_PIN = 4;
constexpr uint8_t HUM_SENSE_PIN  = 33;

constexpr uint8_t FAN_EN  = 5;
constexpr uint8_t FAN_IN1 = 18;
constexpr uint8_t FAN_IN2 = 19;

constexpr uint8_t LED_PIN = 2;

// ================= STEPPER =================
constexpr uint8_t IN1 = 14;
constexpr uint8_t IN2 = 27;
constexpr uint8_t IN3 = 26;
constexpr uint8_t IN4 = 25;

constexpr uint8_t LIMIT_TOP    = 35;
constexpr uint8_t LIMIT_BOTTOM = 34;

constexpr int STEPS_PER_REV   = 2048;
constexpr int STEPS_PER_CHUNK = 120;

// ================= THRESHOLDS =================
constexpr float TEMP_LOW  = 36.9f;
constexpr float TEMP_HIGH = 37.6f;

constexpr float HUM_LOW  = 50.0f;
constexpr float HUM_HIGH = 75.0f;

#define HUM_ON_THRESHOLD  55
#define HUM_OFF_THRESHOLD 70
#define HUM_OVERRIDE_TIME   300000UL   // 5 min
#define HUM_TOGGLE_GUARD   5000UL      // 5 sec

// ================= HUMIDIFIER =================
constexpr uint16_t HUM_SAMPLE_COUNT = 50;
constexpr uint16_t HUM_SAMPLE_DELAY = 20;

constexpr unsigned long HUM_BURST_MS    = 6000UL;
constexpr unsigned long HUM_COOLDOWN_MS = 20000UL;
constexpr unsigned long HUM_CHECK_MS    = 3000UL;

// ================= FAN =================
constexpr uint8_t FAN_CIRC_PWM      = 120;
constexpr uint8_t FAN_MAX_PWM       = 120;
constexpr uint8_t FAN_SOFTSTART_PWM = 90;

constexpr unsigned long FAN_CIRC_RUN_MS = 30000UL;
constexpr unsigned long FAN_CIRC_GAP_MS = 1800000UL;

constexpr float HUM_EMERGENCY = 70.0f;

constexpr unsigned long FAN_EMERGENCY_RUN = 6000UL;
constexpr unsigned long FAN_EMERGENCY_GAP = 180000UL;
constexpr uint8_t FAN_PWM_CHANNEL = 0;

// ================= TURNER =================
constexpr unsigned long TURN_INTERVAL =
    3600000UL;

// ================= TELEGRAM =================
constexpr unsigned long TG_CHECK_NORMAL =
    5000UL;

constexpr unsigned long TG_CHECK_MOVING =
    15000UL;
