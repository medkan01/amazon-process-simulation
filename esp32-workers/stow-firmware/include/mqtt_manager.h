#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include <WiFi.h>
#include "config.h"

class MQTTManager {
    private:
        WiFiClient espClient;
        PubSubClient client;

        String deviceMacAddress;
        unsigned long lastMqttReconnectAttempt;
        unsigned long lastConnectionCheck;
        int mqttAttemptCount;

        static const unsigned long MQTT_RECONNECT_INTERVAL = 30000; // 30 seconds
        static const int MAX_MQTT_ATTEMPTS = 5;
        static const unsigned long CONNECTION_CHECK_INTERVAL = 15000; // 15 seconds

        String getMqttErrorMessage(int errorCode);
        
        // Topic generation helpers
        String getBaseTopic();
        String getConnectionTopic();
        String getStatusTopic();
        String getMetricsTopic();
        String getCustomTopic(const String& suffix);

    public:
        MQTTManager();
        void begin(const String& macAddress);
        bool connect();
        bool isConnected();
        bool publish(const String& topic, const String& payload);
        void loop();
        void maintainConnection();
        void printStatus();
        void printSignal();

        String MQTTManager::getBaseTopic() {
            return "process/stow/" + deviceMacAddress;
        }

        String MQTTManager::getConnectionTopic() {
            return getBaseTopic() + "/state/connection";
        }

        String MQTTManager::getStatusTopic() {
            return getBaseTopic() + "/status";
        }

        String MQTTManager::getMetricsTopic() {
            return getBaseTopic() + "/metrics";
        }

        String MQTTManager::getCustomTopic(const String& suffix) {
            return getBaseTopic() + "/" + suffix;
        }
};

extern MQTTManager mqttManager;

#endif // MQTT_MANAGER_H