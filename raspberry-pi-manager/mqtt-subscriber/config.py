"""MQTT Subscriber Configuration for Warehouse Management System"""

# MQTT Broker Configuration
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_USERNAME = "manager"
MQTT_PASSWORD = "manager123"
MQTT_CLIENT_ID = "warehouse_manager"

# MQTT Topics
MQTT_TOPICS = [
    "warehouse/status/#",      # All status messages
    "warehouse/process/#",     # All process messages
    "warehouse/alert/#",       # All alert messages
]
