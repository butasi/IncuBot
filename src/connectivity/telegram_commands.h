#pragma once

#include <Arduino.h>

// ================= FULL DEFINITIONS =================

struct SystemRefs {
    float* temp = nullptr;
    float* hum  = nullptr;

    bool* heaterState     = nullptr;
    bool* humidifierState = nullptr;
};

struct TelegramState {
    bool autoMode = true;

    bool humOverride = false;
    bool humForceOn = false;
    unsigned long humOverrideUntil = 0;

    bool fanOverride = false;
    bool fanForceOn = false;
    bool fanBurstEnabled = true;
};

// ================= API =================

void handleTelegramCommand(const String& cmd,
                           TelegramState& tgState,
                           SystemRefs& sys);

String buildStatus(const TelegramState& tgState,
                   const SystemRefs& sys);