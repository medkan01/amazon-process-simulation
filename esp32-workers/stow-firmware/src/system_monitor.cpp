#include "system_monitor.h"
#include "wifi_manager.h"
#include "display_manager.h"
#include "mqtt_manager.h"
#include "time_manager.h"

SystemMonitor systemMonitor;

SystemMonitor::SystemMonitor() {
    lastReport = 0;
}

void SystemMonitor::begin() {
    lastReport = millis();
}

void SystemMonitor::checkSystem() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - lastReport >= REPORT_INTERVAL) {
        lastReport = currentMillis;
        
        printSystemStatus();
        updateOLEDDisplay();
    }
}

void SystemMonitor::printSystemStatus() {
    Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
    Serial.println("║              SYSTEM STATUS REPORT                         ║");
    Serial.println("╠═══════════════════════════════════════════════════════════╣");
    
    // Device Information
    Serial.println("║   📱 DEVICE INFORMATION                                   ║");
    Serial.println("╟───────────────────────────────────────────────────────────╢");
    Serial.printf("║   MAC Address: %-43s║\n", WiFi.macAddress().c_str());
    
    unsigned long uptime = millis() / 1000;
    unsigned long days = uptime / 86400;
    unsigned long hours = (uptime % 86400) / 3600;
    unsigned long minutes = (uptime % 3600) / 60;
    unsigned long seconds = uptime % 60;
    
    char uptimeStr[32];
    snprintf(uptimeStr, sizeof(uptimeStr), "%lud %02lu:%02lu:%02lu", days, hours, minutes, seconds);
    Serial.printf("║   Uptime: %-48s║\n", uptimeStr);
    
    char heapStr[32];
    snprintf(heapStr, sizeof(heapStr), "%lu KB / %lu KB", ESP.getFreeHeap() / 1024, ESP.getHeapSize() / 1024);
    Serial.printf("║   Free Heap: %-45s║\n", heapStr);
    
    float heapUsage = 100.0 - ((float)ESP.getFreeHeap() / (float)ESP.getHeapSize() * 100.0);
    char heapUsageStr[32];
    snprintf(heapUsageStr, sizeof(heapUsageStr), "%.1f%%", heapUsage);
    Serial.printf("║   Heap Usage: %-44s║\n", heapUsageStr);
    
    // WiFi Status
    Serial.println("╟───────────────────────────────────────────────────────────╢");
    Serial.println("║   📡 WIFI STATUS                                          ║");
    Serial.println("╟───────────────────────────────────────────────────────────╢");
    
    if (wifiManager.isWifiConnected()) {
        Serial.println("║   Status: ✅ Connected                                    ║");
        Serial.printf("║   SSID: %-50s║\n", WiFi.SSID().c_str());
        Serial.printf("║   IP Address: %-44s║\n", WiFi.localIP().toString().c_str());
        
        int32_t rssi = WiFi.RSSI();
        const char* quality;
        if (rssi >= -50) quality = "Excellent";
        else if (rssi >= -60) quality = "Good";
        else if (rssi >= -70) quality = "Fair";
        else quality = "Weak";
        
        char signalStr[32];
        snprintf(signalStr, sizeof(signalStr), "%d dBm (%s)", rssi, quality);
        Serial.printf("║   Signal Strength: %-39s║\n", signalStr);
    } else {
        Serial.println("║   Status: ❌ Disconnected                                 ║");
    }
    
    // MQTT Status
    Serial.println("╟───────────────────────────────────────────────────────────╢");
    Serial.println("║   🔗 MQTT STATUS                                          ║");
    Serial.println("╟───────────────────────────────────────────────────────────╢");
    
    if (mqttManager.isConnected()) {
        Serial.println("║   Status: ✅ Connected                                    ║");
        char brokerStr[32];
        snprintf(brokerStr, sizeof(brokerStr), "%s:%d", MQTT_SERVER, MQTT_PORT);
        Serial.printf("║   Broker: %-48s║\n", brokerStr);
    } else {
        Serial.println("║   Status: ❌ Disconnected                                 ║");
    }
    
    // Time Status
    Serial.println("╟───────────────────────────────────────────────────────────╢");
    Serial.println("║   🕐 TIME SYNCHRONIZATION                                 ║");
    Serial.println("╟───────────────────────────────────────────────────────────╢");
    
    if (timeManager.isSynced()) {
        Serial.println("║   NTP Status: ✅ Synchronized                             ║");
        Serial.printf("║   Current Time: %-42s║\n", timeManager.getFormattedDateTime().c_str());
    } else {
        Serial.println("║   NTP Status: ❌ Not Synchronized                         ║");
    }
    
    Serial.println("╚═══════════════════════════════════════════════════════════╝\n");
}

void SystemMonitor::updateOLEDDisplay() {
    if (!displayManager.isInitialized()) {
        return;
    }

    String firstLineMessage = "Stow Worker\n" + WiFi.macAddress() + "\n";
    // Display status - Using CP437 compatible characters
    String wifiStatusOk = String("WiFi:") + (wifiManager.isWifiConnected() ? "OK" : "ERR");
    String ntpStatusOk = String("NTP:") + (timeManager.isSynced() ? "OK" : "ERR");
    String mqttStatusOk = String("MQTT:") + (mqttManager.isConnected() ? "OK" : "ERR");

    String secondLineMessage = wifiStatusOk + " - " + ntpStatusOk + " - " + mqttStatusOk;
    
    displayManager.displayTwoLines(
        firstLineMessage, 
        false, 
        secondLineMessage, 
        true,
        DISPLAY_SCROLL_SPEED,
        DISPLAY_LINE1_FONT_SIZE,
        DISPLAY_LINE2_FONT_SIZE
    );
}

