#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiClientSecure.h>
#include <esp_task_wdt.h>
#include <esp_system.h>

#include "config.h"

#include "sensors/dht_sensor.h"
#include "sensors/humidifier_sensor.h"

#include "actuators/heater.h"
#include "actuators/fan.h"
#include "actuators/humidifier.h"
#include "actuators/stepper_turner.h"

#include "connectivity/telegram_client.h"

// ================= OBJECTS =================
WiFiClient        espClient;
PubSubClient      mqtt(espClient);
TelegramClient    tg;
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

DHTSensor        dhtSensor(DHT_PIN, DHT_TYPE);
HumidifierSensor humSensor(HUM_SENSE_PIN);

Heater heater;
Fan          fan(FAN_EN, FAN_IN1, FAN_IN2);
Humidifier   humidifier(HUMIDIFIER_PIN, HUM_SENSE_PIN);
StepperTurner turner(IN1, IN3, IN2, IN4);

// ================= STATE =================
float temp     = 0;
float hum      = 0;
bool  heaterOn = false;   // ← added
bool  humOn    = false;   // ← added

unsigned long lastRead      = 0;
unsigned long lastPublish   = 0;
unsigned long lastHumToggle = 0;

// ================= I2C MUTEX =================
SemaphoreHandle_t i2cMutex = nullptr;

bool i2cLock()   { return xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(50)) == pdTRUE; }
void i2cUnlock() { xSemaphoreGive(i2cMutex); }

void updateDisplay(float t, float h) {
    if (!i2cLock()) return;

    // ---------- Line 1: Temp + Humidity ----------
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(t, 1);
    lcd.print(" H:");
    lcd.print(h, 1);
    lcd.print("    "); // clears leftovers

    // ---------- Line 2: IP + basic status ----------
    lcd.setCursor(0, 1);

    if (WiFi.status() == WL_CONNECTED) {
        lcd.print(WiFi.localIP());
    } else {
        lcd.print("WiFi: DISCONNECTED");
    }

    // pad to clear old characters
    lcd.print("                ");

    i2cUnlock();
}

// ================= MQTT =================
void callback(char* topic, byte* payload, unsigned int length) {

    // Convert topic once (cheap)
    const char* t = topic;

    // Build payload safely without String concatenation loop
    char msg[16];  // adjust if needed
    unsigned int len = (length < sizeof(msg) - 1) ? length : sizeof(msg) - 1;

    memcpy(msg, payload, len);
    msg[len] = '\0';

    bool on = (msg[0] == '1');

    if (strcmp(t, "incubator/control/heater") == 0) {
    }
    else if (strcmp(t, "incubator/control/humidifier") == 0) {
        humidifier.forcePulse(on);
    }
    else if (strcmp(t, "incubator/control/turn") == 0) {
        turner.start();
    }
}

void reconnect() {
    static unsigned long lastAttempt = 0;

    // Already connected
    if (mqtt.connected()) return;

    // Wait between attempts (non-blocking)
    if (millis() - lastAttempt < 5000) return;
    lastAttempt = millis();

    // IMPORTANT: ensure WiFi is up first
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[MQTT] WiFi not ready, skipping connect");
        return;
    }

    Serial.println("[MQTT] attempting connect...");

    // Optional: short timeout behavior depends on library, but this helps watchdog safety
    bool ok = mqtt.connect("ESP32_INCUBATOR");

    if (ok) {
        mqtt.subscribe("incubator/control/#");
        Serial.println("[MQTT] connected");
    } else {
        Serial.printf("[MQTT] failed rc=%d\n", mqtt.state());
    }
}

// ================= SETUP =================
void setup() {
    Serial.begin(115200);

    Wire.begin();
    i2cMutex = xSemaphoreCreateMutex();
    lcd.init();
    lcd.backlight();

    esp_task_wdt_init(10, true);
    esp_task_wdt_add(NULL);

    Serial.println();
    Serial.printf("Reset reason: %d\n", esp_reset_reason());

    heater.begin(HEATER_PIN);



    fan.begin();
    humidifier.begin();
    humSensor.begin();
    dhtSensor.begin();
    turner.begin();

    // WiFi before tg.begin() — TLS needs it
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("[WiFi] connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        esp_task_wdt_reset();
        Serial.print(".");
    }
    Serial.println(" connected");

    // wire live variable addresses — must be after globals are declared
    SystemRefs sysRefs;
    sysRefs.temp            = &temp;
    sysRefs.hum             = &hum;
    sysRefs.heaterState     = &heaterOn;
    sysRefs.humidifierState = &humOn;
    tg.begin(BOT_TOKEN, CHAT_ID, sysRefs);

    mqtt.setServer(MQTT_SERVER, MQTT_PORT);
    mqtt.setCallback(callback);

    turner.start();

    Serial.println("BOOT DONE");
    Serial.println("BUILD: " + String(__DATE__) + " " + String(__TIME__));
}

// ================= LOOP =================
void loop() {
    esp_task_wdt_reset();

    unsigned long now = millis();

    //================ FAN SCHEDULER =================
    static unsigned long bootTime = now;
    static unsigned long lastHourly = 0;
    static bool hourlyActive = false;

    bool bootBurst = (now - bootTime) < 9000;

    if (lastHourly == 0) lastHourly = now;

    if (now - lastHourly >= 3600000UL) {
        lastHourly = now;
        hourlyActive = true;
    }

    if (hourlyActive && (now - lastHourly < 9000)) {
        hourlyActive = true;
    } else {
        hourlyActive = false;
    }

    bool emergency = (temp > 40.0f) || (hum > 80.0f);

    bool fanEnabled = bootBurst || hourlyActive || emergency;

    fan.update(fanEnabled, 90, turner.isMoving(), emergency, true);

    //================ MQTT =================
    reconnect();
    if (mqtt.connected()) mqtt.loop();

    //================ SENSORS =================
    if (now - lastRead >= SENSOR_READ_INTERVAL) {
        dhtSensor.update();
        lastRead = now;
    }

    temp = dhtSensor.getTemperature();
    hum  = dhtSensor.getHumidity();

    //================ HEATER =================
    

    heater.update(temp);

    //================ HUMIDIFIER ADC =================
    HumState adcState = humSensor.readState();
    bool adcHumOn = (adcState == HUM_MODE1 || adcState == HUM_MODE2);
    humOn = adcHumOn;

    //================ HUMIDIFIER CONTROL =================
    bool wantOn;

    if (tg.tgState.humOverride) {
        wantOn = tg.tgState.humForceOn;
    } else {
        wantOn = (hum < HUM_ON_THRESHOLD);
    }

    if (now - lastHumToggle > HUM_TOGGLE_GUARD) {
        if (wantOn && !adcHumOn) {
            humidifier.forcePulse(true);
            lastHumToggle = now;
        } else if (!wantOn && adcHumOn) {
            humidifier.forcePulse(false);
            lastHumToggle = now;
        }
    }

    humidifier.update(false, hum);

    //================ STEPPER =================
    turner.update();

    //================ DISPLAY =================
    updateDisplay(temp, hum);

    //================ TELEGRAM =================
    IncubatorState state;
    state.temp       = temp;
    state.hum        = hum;
    state.heater     = heaterOn;
    state.humidifier = humOn;
    state.fan        = fanEnabled;
    tg.update(state);

    //================ MQTT PUBLISH =================
    if (now - lastPublish > 3000) {
        String payload = "{";
        payload += "\"temp\":" + String(temp, 1) + ",";
        payload += "\"hum\":"  + String(hum, 1);
        payload += "}";

        if (mqtt.connected())
            mqtt.publish("incubator/data", payload.c_str());

        lastPublish = now;
    }

    delay(5);
}