#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "time_manager.h"
#include "mqtt_manager.h"
#include "display_manager.h"
#include "system_monitor.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
    Serial.println("║         STOW WORKER INITIALIZATION                        ║");
    Serial.println("╚═══════════════════════════════════════════════════════════╝\n");
    
    // 1. Initialize Display
    Serial.println("🖥️  Initializing Display...");
    displayManager.begin();
    if (displayManager.isInitialized()) {
        displayManager.displayLine("Booting...", false, DISPLAY_SCROLL_SPEED, DISPLAY_DEFAULT_FONT_SIZE);
    }
    
    // 2. Initialize WiFi
    Serial.println("\n📡 Initializing WiFi...");
    wifiManager.begin();
    if (wifiManager.connect()) {
        Serial.println("✅ WiFi initialization successful");
    } else {
        Serial.println("❌ WiFi initialization failed");
    }
    
    // 3. Initialize Time Manager
    Serial.println("\n🕐 Initializing Time Manager...");
    timeManager.begin();
    if (timeManager.isSynced()) {
        Serial.println("✅ Time synchronization successful");
    } else {
        Serial.println("⚠️  Time synchronization pending");
    }
    
    // 4. Initialize MQTT
    Serial.println("\n🔗 Initializing MQTT...");
    String macAddress = WiFi.macAddress();
    mqttManager.begin(macAddress);
    if (mqttManager.connect()) {
        Serial.println("✅ MQTT initialization successful");
    } else {
        Serial.println("⚠️  MQTT connection pending");
    }
    
    // 5. Initialize System Monitor
    Serial.println("\n📊 Initializing System Monitor...");
    systemMonitor.begin();
    Serial.println("✅ System Monitor initialized");
    
    Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
    Serial.println("║         INITIALIZATION COMPLETE                           ║");
    Serial.println("╚═══════════════════════════════════════════════════════════╝\n");
    
    // Display initial status
    systemMonitor.updateOLEDDisplay();
    systemMonitor.printSystemStatus();
}

void loop() {
    // Maintain WiFi connection
    wifiManager.checkConnection();
    
    // Maintain time synchronization
    timeManager.maintainTimeSync();
    
    // Maintain MQTT connection
    mqttManager.maintainConnection();
    mqttManager.loop();
    
    // Update display
    displayManager.loop();
    
    // Check system status
    systemMonitor.checkSystem();
}