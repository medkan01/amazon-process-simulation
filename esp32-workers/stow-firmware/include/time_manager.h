#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>
#include "config.h"

class TimeManager {
    private:
        WiFiUDP ntpUDP;
        NTPClient timeClient;
        unsigned long lastSyncTime;
        bool isNtpSynced;
        
        static const unsigned long NTP_SYNC_INTERVAL = 3600000; // 1 hour in milliseconds

    public:
        TimeManager();
        void begin();
        void maintainTimeSync();
        String getFormattedDateTime();
        bool isSynced();
        void printTimeStatus();
};

extern TimeManager timeManager;

#endif // TIME_MANAGER_H