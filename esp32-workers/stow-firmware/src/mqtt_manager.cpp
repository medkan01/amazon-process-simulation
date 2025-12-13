#include "mqtt_manager.h"
#include "wifi_manager.h"
#include "time_manager.h"

MQTTManager mqttManager;

MQTTManager::MQTTManager() : client(espClient) {
    lastMqttReconnectAttempt = 0;
    lastConnectionCheck = 0;
    mqttAttemptCount = 0;
}

void MQTTManager::begin(const String& macAddress) {
    deviceMacAddress = macAddress;
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setKeepAlive(90);
    client.setSocketTimeout(45);
}

bool MQTTManager::connect() {
    // First, check WiFi signal stability
    if (!wifiManager.isWifiConnected() || !wifiManager.isSignalStable()) {
        Serial.println("❌ WiFi not connected or signal unstable. Cannot connect to MQTT broker.");
        return false;
    }

    if (mqttAttemptCount >= MAX_MQTT_ATTEMPTS) {
        unsigned long currentMillis = millis();
        
        if (currentMillis - lastMqttReconnectAttempt < MQTT_RECONNECT_INTERVAL) {
            return false; // Wait before next attempt
        }
        mqttAttemptCount = 0; // Reset attempt count after waiting period
        lastMqttReconnectAttempt = currentMillis;
    }

    if (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "StowWorker-" + deviceMacAddress;
        mqttAttemptCount++;
        lastMqttReconnectAttempt = millis();

        // Connect with Last Will and Testament (LWT) to detect unexpected disconnects
        String lwtTopic = "process/stow/" + deviceMacAddress + "/status";
        String lwtPayload = "{\"device_mac_address\":\"" + deviceMacAddress + "\",\"timestamp\":\"" + timeManager.getFormattedDateTime() + "\"}";

        if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD, lwtTopic.c_str(), 1, true, lwtPayload.c_str())) {
            Serial.println("✅ Connected to MQTT broker successfully.");

            // Publish initial online status
            String onlineTopic = "process/stow/" + deviceMacAddress + "/status";
            String onlinePayload = "{\"device_mac_address\":\"" + deviceMacAddress + "\",\"timestamp\":\"" + timeManager.getFormattedDateTime() + "\"}";

            client.publish(onlineTopic.c_str(), onlinePayload.c_str(), true);

            mqttAttemptCount = 0; // Reset attempt count on successful connection
            return true;
        } else {
            int errorCode = client.state();

            Serial.print("❌ MQTT connection failed, rc=");
            Serial.print(errorCode);
            Serial.print(" (");
            Serial.print(getMqttErrorMessage(errorCode));
            Serial.println(")");

            Serial.printf("(Attempt: %d/%d | Signal: %d dBm)\n", mqttAttemptCount, MAX_MQTT_ATTEMPTS, WiFi.RSSI());
            return false;
        }
    }

    return true;
}

String MQTTManager::getMqttErrorMessage(int errorCode) {
    switch (errorCode) {
        case -4: return "MQTT_CONNECTION_TIMEOUT - Network timeout";
        case -3: return "MQTT_CONNECTION_LOST - Network connection lost";
        case -2: return "MQTT_CONNECT_FAILED - Network connection failed";
        case -1: return "MQTT_DISCONNECTED - Disconnected";
        case 0: return "MQTT_CONNECTED - Connected";
        case 1: return "MQTT_CONNECT_BAD_PROTOCOL - MQTT protocol version not supported";
        case 2: return "MQTT_CONNECT_BAD_CLIENT_ID - Client identifier rejected";
        case 3: return "MQTT_CONNECT_UNAVAILABLE - Server unavailable";
        case 4: return "MQTT_CONNECT_BAD_CREDENTIALS - Bad username or password";
        case 5: return "MQTT_CONNECT_UNAUTHORIZED - Not authorized";
        default: return "Unknown error: " + String(errorCode);
    }
}

bool MQTTManager::isConnected() {
    return client.connected();
}

bool MQTTManager::publish(const String& topic, const String& payload) {
    if (client.connected()) {
        boolean result = client.publish(topic.c_str(), payload.c_str());
        if (!result) {
            Serial.println("❌ MQTT publish failed.");
        } else {
            Serial.println("✅ MQTT publish succeeded.");
        }
        return result;
    } else {
        Serial.println("❌ MQTT client not connected. Cannot publish message.");
        return false;
    }
}

void MQTTManager::loop() {
    if (client.connected()) {
        client.loop();
    }
}

void MQTTManager::maintainConnection() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastConnectionCheck >= CONNECTION_CHECK_INTERVAL) {
        lastConnectionCheck = currentMillis;

        if (isConnected()) {
            mqttAttemptCount = 0; // Reset attempt count on successful connection
        } else {
            if (!wifiManager.isSignalStable()) {
                if (currentMillis - lastMqttReconnectAttempt < MQTT_RECONNECT_INTERVAL) {
                    Serial.println("❌ WiFi signal unstable. Skipping MQTT reconnect attempt.");
                    lastMqttReconnectAttempt = currentMillis;
                    return; // Wait before next attempt
                }
            }
        }
    }
}

void MQTTManager::printStatus() {
    Serial.printf("MQTT Status: %s (code: %d)\n", client.connected() ? "Connected" : "Disconnected", client.state());
}