#include "time_manager.h"
#include "wifi_manager.h"

TimeManager timeManager;

TimeManager::TimeManager() : timeClient(ntpUDP, NTP_SERVER, GMT_OFFSET_SEC, NTP_SYNC_INTERVAL) {
    lastSyncTime = 0;
    isNtpSynced = false;
}

void TimeManager::begin() {
    timeClient.begin();
    
    Serial.println("Starting NTP client...");
    maintainTimeSync();

    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
}

void TimeManager::maintainTimeSync() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastSyncTime >= NTP_SYNC_INTERVAL || lastSyncTime == 0) {
        lastSyncTime = currentMillis;

        if (wifiManager.isWifiConnected() && wifiManager.isSignalStable()) {
            Serial.println("Synchronizing time with NTP server...");
            if (timeClient.forceUpdate()) {
                isNtpSynced = true;
                Serial.println("✅ NTP time synchronized successfully.");
                
                time_t epochTime = timeClient.getEpochTime();
                struct timeval tv = { epochTime, 0 };
                settimeofday(&tv, NULL);
            } else {
                Serial.println("❌ Failed to synchronize NTP time.");
                if (isNtpSynced) {
                    Serial.println("⚠️ Retaining previous NTP sync status.");
                }
            }
        } else {
            Serial.println("WiFi not connected or signal unstable. Skipping NTP sync.");
        }
    }
}

String TimeManager::getFormattedDateTime() {
    char buffer[32];

    if (isNtpSynced && timeClient.isTimeSet()) {
        // Prioritize NTPClient time
        time_t epochTime = timeClient.getEpochTime();
        struct tm * timeinfo = gmtime(&epochTime);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    } else {
        // Fallback to system time
        time_t rawtime;
        time(&rawtime);
        struct tm * timeinfo = gmtime(&rawtime);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    }

    return String(buffer);
}

bool TimeManager::isSynced() {
    return isNtpSynced;
}

void TimeManager::printTimeStatus() {
    if (isNtpSynced) {
        Serial.print("✅ NTP Synced Time: ");
        Serial.println(getFormattedDateTime());
    } else {
        Serial.println("❌ NTP Not Synced.");
    }
}