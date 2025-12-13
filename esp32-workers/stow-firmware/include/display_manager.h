#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

class DisplayManager {
private:
    Adafruit_SSD1306 display;
    bool initialized;

    struct ScrollConfig {
        String text;
        bool enabled;
        int position;
        unsigned long lastUpdate;
        int speed;
        uint8_t yPosition;
        uint8_t fontSize;
    };

    ScrollConfig line1Scroll;
    ScrollConfig line2Scroll;
    bool twoLinesMode;

    void scrollLine(ScrollConfig &line);
    void drawText(const String& text, uint8_t yPosition, uint8_t fontSize, int xOffset = 0);
    int getTextWidth(const String& text, uint8_t fontSize);

public:
    DisplayManager();
    void begin();
    bool isInitialized();
    void clear();
    void displayLine(const String& text, bool scroll = false, int scrollSpeed = DISPLAY_SCROLL_SPEED, uint8_t fontSize = DISPLAY_DEFAULT_FONT_SIZE);
    void displayTwoLines(const String& text1, bool scroll1, const String& text2, bool scroll2, int scrollSpeed = DISPLAY_SCROLL_SPEED, uint8_t fontSize1 = DISPLAY_LINE1_FONT_SIZE, uint8_t fontSize2 = DISPLAY_LINE2_FONT_SIZE);
    void loop();
};

extern DisplayManager displayManager;

#endif // DISPLAY_MANAGER_H
