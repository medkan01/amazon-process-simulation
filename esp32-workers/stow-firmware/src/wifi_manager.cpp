#include "wifi_manager.h"

WiFiManager wifiManager;

WiFiManager::WiFiManager() {
    isConnected = false;
    lastWifiSignalCheck = 0;
    consecutiveWeakSignals = 0;
}

void WiFiManager::begin() {
    WiFi.mode(WIFI_STA);
}

bool WiFiManager::connect() {
    Serial.print("Connecting to " + String(WIFI_SSID) + "...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    Serial.println();
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("✅ WiFi connected successfully.");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("MAC address: ");
        Serial.println(WiFi.macAddress());

        isConnected = true;
        return true;
    } else {
        Serial.println("❌ Failed to connect to WiFi.");
        isConnected = false;
        return false;
    }
}

bool WiFiManager::isWifiConnected() {
    return isConnected && (WiFi.status() == WL_CONNECTED);
}

bool WiFiManager::isSignalStable() {
    int32_t rssi = WiFi.RSSI();
    if (rssi < WEAK_SIGNAL_THRESHOLD) {
        consecutiveWeakSignals++;
    } else {
        consecutiveWeakSignals = 0;
    }

    return rssi >= WEAK_SIGNAL_THRESHOLD || consecutiveWeakSignals < MAX_CONSECUTIVE_WEAK_SIGNALS;
}

void WiFiManager::checkConnection() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - lastWifiSignalCheck >= WIFI_SIGNAL_CHECK_INTERVAL) {
        lastWifiSignalCheck = currentMillis;
        isSignalStable(); // Update signal stability status
    }

    // Only reconnect if actually disconnected, not just weak signal
    if (!isWifiConnected()) {
        Serial.println("⚠️ WiFi disconnected, attempting to reconnect...");
        connect();
    }
}

void WiFiManager::printStatus() {
    Serial.print("WiFi Status: ");
    if (isWifiConnected()) {
        Serial.println("✅ Connected");
        Serial.print("Signal Strength (RSSI): ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
    } else {
        Serial.println("❌ Disconnected");
    }
}

void WiFiManager::printSignal() {
    static const unsigned int EXCELLENT_THRESHOLD = -50;
    static const unsigned int GOOD_THRESHOLD = -60;
    static const unsigned int FAIR_THRESHOLD = -70;
    int32_t rssi = WiFi.RSSI();
    String signalQuality;
    
    if (rssi >= EXCELLENT_THRESHOLD) signalQuality = "Excellent";
    else if (rssi >= GOOD_THRESHOLD) signalQuality = "Good";
    else if (rssi >= FAIR_THRESHOLD) signalQuality = "Fair";
    else signalQuality = "Weak";
    
    Serial.printf("WiFi RSSI: %d dBm (%s)\n", rssi, signalQuality.c_str());
}