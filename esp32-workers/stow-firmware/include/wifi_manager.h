#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "config.h"

class WiFiManager {
    private:
        bool isConnected;
        unsigned long lastWifiSignalCheck;
        int consecutiveWeakSignals;
        static const unsigned long WIFI_SIGNAL_CHECK_INTERVAL = 60000; // 60 seconds
        static const int WEAK_SIGNAL_THRESHOLD = -80; // dBm
        static const int MAX_CONSECUTIVE_WEAK_SIGNALS = 3;

    public:
        WiFiManager();
        void begin();
        bool connect();
        bool isWifiConnected();
        bool isSignalStable();
        void checkConnection();
        void printStatus();
        void printSignal();
};

extern WiFiManager wifiManager;

#endif // WIFI_MANAGER_H