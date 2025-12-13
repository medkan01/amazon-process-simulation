#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID       "OrneTHD-7530"
#define WIFI_PASSWORD   "B6OUR7UJ6ZHN37"

// NTP Configuration
#define NTP_SERVER              "pool.ntp.org"
#define GMT_OFFSET_SEC          0                   // GMT+0 to normalize to UTC (others systems will add their own offset)
#define DAYLIGHT_OFFSET_SEC     0                   // No daylight saving time

#endif // CONFIG_H