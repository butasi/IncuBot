#pragma once
#include <Arduino.h>
#include "telegram_commands.h"

struct IncubatorState {
    float temp;
    float hum;
    bool  heater;
    bool  humidifier;
    bool  fan;
};

class TelegramClient {
public:
    void begin(const char* botToken, const char* chatId,
               SystemRefs sysRefs);          // pass refs once at init

    // call from loop() — returns instantly, task does the work
    void update(const IncubatorState& state);

    void sendMessage(const String& msg);     // public for alerts from main

    // expose tgState so main.cpp can read humOverride etc.
    TelegramState tgState;

private:
    String token;
    String chatId;
    long   lastUpdateId = 0;

    SystemRefs     _sys   = {};
    IncubatorState _snap  = {};
    portMUX_TYPE   _mux   = portMUX_INITIALIZER_UNLOCKED;
    TaskHandle_t   _task  = nullptr;

    String sendRequest (const String& url);
    static String urlEncode(const String& s);
    static void   taskEntry(void* pv);
    void          taskLoop();
};