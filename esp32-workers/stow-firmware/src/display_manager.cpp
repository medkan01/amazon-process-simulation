#include "display_manager.h"

DisplayManager displayManager;

DisplayManager::DisplayManager() : display(I2C_WIDTH, I2C_HEIGHT, &Wire, -1) {
    initialized = false;
    twoLinesMode = false;

    line1Scroll = {"", false, 0, 0, DISPLAY_SCROLL_SPEED, DISPLAY_LINE1_Y, DISPLAY_LINE1_FONT_SIZE};
    line2Scroll = {"", false, 0, 0, DISPLAY_SCROLL_SPEED, DISPLAY_LINE2_Y, DISPLAY_LINE2_FONT_SIZE};
}

void DisplayManager::begin() {
    Wire.begin(I2C_SDA_PIN, I2C_SCK_PIN);

    if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS)) {
        Serial.println("❌ Failed to initialize OLED display.");
        initialized = false;
        return;
    }

    Serial.println("✅ OLED display initialized successfully.");
    initialized = true;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

bool DisplayManager::isInitialized() {
    return initialized;
}

void DisplayManager::clear() {
    if (!initialized) return;

    display.clearDisplay();
    display.display();

    line1Scroll = {"", false, 0, 0, DISPLAY_SCROLL_SPEED, DISPLAY_LINE1_Y, DISPLAY_LINE1_FONT_SIZE};
    line2Scroll = {"", false, 0, 0, DISPLAY_SCROLL_SPEED, DISPLAY_LINE2_Y, DISPLAY_LINE2_FONT_SIZE};
    twoLinesMode = false;
}

void DisplayManager::displayLine(const String& text, bool scroll, int scrollSpeed, uint8_t fontSize) {
    if (!initialized) return;

    twoLinesMode = false;
    line1Scroll.text = text;
    line1Scroll.enabled = scroll;
    line1Scroll.speed = scrollSpeed;
    line1Scroll.position = 0;
    line1Scroll.lastUpdate = millis();
    line1Scroll.yPosition = 0;
    line1Scroll.fontSize = fontSize;

    display.clearDisplay();

    if (!scroll) {
        drawText(text, line1Scroll.yPosition, fontSize);
        display.display();
    }
}

void DisplayManager::displayTwoLines(const String& text1, bool scroll1, const String& text2, bool scroll2, int scrollSpeed, uint8_t fontSize1, uint8_t fontSize2) {
    if (!initialized) return;

    twoLinesMode = true;

    line1Scroll.text = text1;
    line1Scroll.enabled = scroll1;
    line1Scroll.speed = scrollSpeed;
    line1Scroll.position = 0;
    line1Scroll.lastUpdate = millis();
    line1Scroll.yPosition = DISPLAY_LINE1_Y;
    line1Scroll.fontSize = fontSize1;

    line2Scroll.text = text2;
    line2Scroll.enabled = scroll2;
    line2Scroll.speed = scrollSpeed;
    line2Scroll.position = 0;
    line2Scroll.lastUpdate = millis();
    line2Scroll.yPosition = DISPLAY_LINE2_Y;
    line2Scroll.fontSize = fontSize2;

    display.clearDisplay();

    if (!scroll1) {
        drawText(text1, line1Scroll.yPosition, fontSize1);
    }
    if (!scroll2) {
        drawText(text2, line2Scroll.yPosition, fontSize2);
    }

    display.display();
}

void DisplayManager::loop() {
    if (!initialized) return;

    bool needsUpdate = false;

    if (twoLinesMode) {
        if (line1Scroll.enabled) {
            scrollLine(line1Scroll);
            needsUpdate = true;
        }
        if (line2Scroll.enabled) {
            scrollLine(line2Scroll);
            needsUpdate = true;
        }
    } else {
        if (line1Scroll.enabled) {
            scrollLine(line1Scroll);
            needsUpdate = true;
        }
    }

    if (needsUpdate) {
        display.clearDisplay();

        if (twoLinesMode) {
            if (line1Scroll.enabled) {
                drawText(line1Scroll.text, line1Scroll.yPosition, line1Scroll.fontSize, -line1Scroll.position);
            } else {
                drawText(line1Scroll.text, line1Scroll.yPosition, line1Scroll.fontSize);
            }

            if (line2Scroll.enabled) {
                drawText(line2Scroll.text, line2Scroll.yPosition, line2Scroll.fontSize, -line2Scroll.position);
            } else {
                drawText(line2Scroll.text, line2Scroll.yPosition, line2Scroll.fontSize);
            }
        } else {
            if (line1Scroll.enabled) {
                drawText(line1Scroll.text, line1Scroll.yPosition, line1Scroll.fontSize, -line1Scroll.position);
            }
        }

        display.display();
    }
}

void DisplayManager::scrollLine(ScrollConfig &line) {
    unsigned long currentMillis = millis();

    if (currentMillis - line.lastUpdate >= line.speed) {
        line.lastUpdate = currentMillis;

        int textWidth = getTextWidth(line.text, line.fontSize);

        if (textWidth > I2C_WIDTH) {
            line.position++;
            if (line.position > textWidth) {
                line.position = 0;
            }
        }
    }
}

void DisplayManager::drawText(const String& text, uint8_t yPosition, uint8_t fontSize, int xOffset) {
    display.setTextSize(fontSize == 6 ? 1 : fontSize / 8);
    display.setCursor(xOffset, yPosition);
    display.print(text);
}

int DisplayManager::getTextWidth(const String& text, uint8_t fontSize) {
    int charWidth = (fontSize == 6) ? 6 : 6 * (fontSize / 8);
    return text.length() * charWidth;
}
