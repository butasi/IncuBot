#include "telegram_client.h"
#include <WiFiClientSecure.h>

// ─────────────────────────────────────────────────────────────
//  Public
// ─────────────────────────────────────────────────────────────

void TelegramClient::begin(const char* botToken,
                           const char* chat,
                           SystemRefs  sysRefs) {
    token  = botToken;
    chatId = chat;
    _sys   = sysRefs;

    xTaskCreatePinnedToCore(
        taskEntry, "tg_task",
        9000,          // TLS needs ~6-7 KB stack
        this, 1,
        &_task, 0      // core 0 — loop() is on core 1
    );
}

void TelegramClient::update(const IncubatorState& state) {
    portENTER_CRITICAL(&_mux);
    _snap = state;
    portEXIT_CRITICAL(&_mux);
}

void TelegramClient::sendMessage(const String& msg) {
    String url = "/bot" + token +
                 "/sendMessage?chat_id=" + chatId +
                 "&text=" + urlEncode(msg);
    sendRequest(url);
}

// ─────────────────────────────────────────────────────────────
//  Background task (core 0)
// ─────────────────────────────────────────────────────────────

void TelegramClient::taskEntry(void* pv) {
    static_cast<TelegramClient*>(pv)->taskLoop();
}

void TelegramClient::taskLoop() {
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(3000));

        // snapshot shared state
        IncubatorState s;
        portENTER_CRITICAL(&_mux);
        s = _snap;
        portEXIT_CRITICAL(&_mux);

        // ── check humOverride expiry ──
        if (tgState.humOverride &&
            millis() > tgState.humOverrideUntil) {
            tgState.humOverride = false;
            tgState.humForceOn  = false;
            sendMessage("Hum override expired, back to AUTO");
            Serial.println("[TG] hum override expired");
        }

        // ── poll Telegram ──
        String url  = "/bot" + token +
                      "/getUpdates?offset=" + String(lastUpdateId + 1) +
                      "&timeout=0";
        String resp = sendRequest(url);

        Serial.print("[TG] response size=");
        Serial.println(resp.length());
        if (resp.length() == 0) continue;

        // ── parse updates ──
        int idx = 0;
        while ((idx = resp.indexOf("\"update_id\":", idx)) != -1) {
            int  us  = idx + 12;
            int  ue  = resp.indexOf(",", us);
            lastUpdateId = resp.substring(us, ue).toInt();

            int tp = resp.indexOf("\"text\":\"", idx);
            if (tp == -1) { idx++; continue; }
            tp += 8;
            String cmd = resp.substring(tp, resp.indexOf("\"", tp));
            Serial.println("[TG] cmd: " + cmd);

            // ── dispatch to command handler (state mutations) ──
            handleTelegramCommand(cmd, tgState, _sys);

            // ── send reply ──
if      (cmd == "/status")     sendMessage(buildStatus(tgState, _sys));
else if (cmd == "/temp")       sendMessage("Temp: " + String(s.temp, 1) + " C");
else if (cmd == "/hum")        sendMessage("Hum: "  + String(s.hum,  1) + " %");

else if (cmd == "/auto")       sendMessage("AUTO mode ON");

else if (cmd == "/hum_on")     sendMessage("Hum forced ON for 5 min");
else if (cmd == "/hum_off")    sendMessage("Hum forced OFF for 5 min");

else if (cmd == "/heater_on")  sendMessage("Heater ON");
else if (cmd == "/heater_off") sendMessage("Heater OFF");

// ================= FAN =================
else if (cmd == "/fan_on")     sendMessage("Fan FORCED ON");
else if (cmd == "/fan_off")    sendMessage("Fan FORCED OFF");
else if (cmd == "/fan_auto")   sendMessage("Fan AUTO mode");
else if (cmd == "/fan_burst_on")  sendMessage("Fan burst ENABLED");
else if (cmd == "/fan_burst_off") sendMessage("Fan burst DISABLED");

else if (cmd == "/help") sendMessage(
    "/status /temp /hum\n"
    "/auto\n"
    "/hum_on /hum_off\n"
    "/heater_on /heater_off\n"
    "/fan_on /fan_off /fan_auto\n"
    "/fan_burst_on /fan_burst_off"
);

else sendMessage("Unknown command. Try /help");

            idx++;
        }
    }
}

// ─────────────────────────────────────────────────────────────
//  Network
// ─────────────────────────────────────────────────────────────

String TelegramClient::sendRequest(const String& url) {
    WiFiClientSecure client;
    client.setInsecure();
    client.setTimeout(10);

     if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[TG] WiFi not connected, skipping");
        return "";
    }

    if (!client.connect("api.telegram.org", 443)) {
        Serial.println("[TG] connect failed");
        vTaskDelay(pdMS_TO_TICKS(5000)); 
        return "";
    }

    client.print(String("GET ") + url +
                 " HTTP/1.0\r\n"
                 "Host: api.telegram.org\r\n"
                 "Connection: close\r\n\r\n");

    String resp;
    resp.reserve(2048);
    unsigned long start = millis();

    while (millis() - start < 5000UL) {
        while (client.available())
            resp += (char)client.read();
        if (!client.connected()) break;
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    client.stop();
    return resp;
}

String TelegramClient::urlEncode(const String& s) {
    String out;
    out.reserve(s.length() * 3);
    for (unsigned i = 0; i < s.length(); i++) {
        char c = s[i];
        if (isAlphaNumeric(c) || c=='-' || c=='_' || c=='.' || c=='~')
            out += c;
        else {
            char buf[4];
            snprintf(buf, sizeof(buf), "%%%02X", (unsigned char)c);
            out += buf;
        }
    }
    return out;
}