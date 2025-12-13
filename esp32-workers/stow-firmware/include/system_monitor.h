#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <Arduino.h>

class SystemMonitor {
    private:
        unsigned long lastReport;
        
        static const unsigned long REPORT_INTERVAL = 60000; // Report every 60 seconds
    public:
        SystemMonitor();
        void begin();
        void checkSystem();
        void printSystemStatus();
        void updateOLEDDisplay();
};

extern SystemMonitor systemMonitor;

#endif // SYSTEM_MONITOR_H