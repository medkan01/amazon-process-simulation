#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID       "OrneTHD-7530"
#define WIFI_PASSWORD   "B6OUR7UJ6ZHN37"

// NTP Configuration
#define NTP_SERVER              "pool.ntp.org"
#define GMT_OFFSET_SEC          0                   // GMT+0 to normalize to UTC (others systems will add their own offset)
#define DAYLIGHT_OFFSET_SEC     0                   // No daylight saving time

// MQTT Configuration
#define MQTT_SERVER     "192.168.0.205"
#define MQTT_PORT       1883
#define MQTT_USER       "stow_worker"
#define MQTT_PASSWORD   "reKroW-wotS-251213"

// Device Configuration
#define I2C_SDA_PIN     21
#define I2C_SCK_PIN     22
#define I2C_HEIGHT      32
#define I2C_WIDTH       128

// Display Configuration
#define DISPLAY_I2C_ADDRESS         0x3C
#define DISPLAY_SCROLL_SPEED        10                  // ms per pixel
#define DISPLAY_LINE1_Y             0                   // Y position for line 1 (contains 2 text lines with \n)
#define DISPLAY_LINE2_Y             16                  // Y position for line 2
#define DISPLAY_DEFAULT_FONT_SIZE   20                  // Default font size for single line
#define DISPLAY_LINE1_FONT_SIZE     4                   // Default font size for line 1 (two lines mode)
#define DISPLAY_LINE2_FONT_SIZE     12                  // Default font size for line 2 (two lines mode)

#endif // CONFIG_H