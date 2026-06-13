#include "telegram_commands.h"

void handleTelegramCommand(const String& cmd,
                           TelegramState& tgState,
                           SystemRefs& sys) {

    // ================= STATUS =================
    if (cmd == "/status") {
        return;
    }

    else if (cmd == "/temp") { }
    else if (cmd == "/hum")  { }

    // ================= SYSTEM MODE =================
    else if (cmd == "/auto") {
        tgState.autoMode = true;
        tgState.humOverride = false;
        Serial.println("[TG] AUTO MODE ON");
    }

    // ================= HUMIDIFIER =================
    else if (cmd == "/hum_on") {
        tgState.humOverride = true;
        tgState.humForceOn = true;
        tgState.humOverrideUntil = millis() + 300000UL;
        Serial.println("[TG] HUM forced ON (5 min)");
    }

    else if (cmd == "/hum_off") {
        tgState.humOverride = true;
        tgState.humForceOn = false;
        tgState.humOverrideUntil = millis() + 300000UL;
        Serial.println("[TG] HUM forced OFF (5 min)");
    }

    // ================= HEATER =================
    else if (cmd == "/heater_on") {
        if (sys.heaterState) *sys.heaterState = true;
        tgState.autoMode = false;
        Serial.println("[TG] HEATER ON");
    }

    else if (cmd == "/heater_off") {
        if (sys.heaterState) *sys.heaterState = false;
        tgState.autoMode = false;
        Serial.println("[TG] HEATER OFF");
    }

    // ================= FAN CONTROL =================
    else if (cmd == "/fan_on") {
        tgState.fanOverride = true;
        tgState.fanForceOn = true;
        Serial.println("[TG] FAN FORCED ON");
    }

    else if (cmd == "/fan_off") {
        tgState.fanOverride = true;
        tgState.fanForceOn = false;
        Serial.println("[TG] FAN FORCED OFF");
    }

    else if (cmd == "/fan_auto") {
        tgState.fanOverride = false;
        tgState.fanBurstEnabled = true;
        Serial.println("[TG] FAN AUTO MODE");
    }

    else if (cmd == "/fan_burst_on") {
        tgState.fanBurstEnabled = true;
        Serial.println("[TG] FAN BURST ENABLED");
    }

    else if (cmd == "/fan_burst_off") {
        tgState.fanBurstEnabled = false;
        Serial.println("[TG] FAN BURST DISABLED");
    }
}

// ================= STATUS BUILDER =================
String buildStatus(const TelegramState& tgState,
                   const SystemRefs& sys) {

    String s;
    s.reserve(150);

    s += "Temp: " + String(*sys.temp, 1) + " C\n";
    s += "Hum: "  + String(*sys.hum, 1)  + " %\n";

    s += "Heater: " + String(*sys.heaterState ? "ON" : "OFF") + "\n";
    s += "Hum: "    + String(*sys.humidifierState ? "ON" : "OFF") + "\n";

    s += "Auto: " + String(tgState.autoMode ? "YES" : "NO") + "\n";

    s += "HumOvr: " + String(tgState.humOverride ? "YES" : "NO") + "\n";

    s += "FanOvr: " + String(tgState.fanOverride ? "YES" : "NO") + "\n";
    s += "FanBurst: " + String(tgState.fanBurstEnabled ? "YES" : "NO");

    return s;
}