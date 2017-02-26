#include "my9291.h"

#define MY9291_DI_PIN       13
#define MY9291_DCKI_PIN     15
#define RAINBOW_DELAY       10

my9291 _my9291 = my9291(MY9291_DI_PIN, MY9291_DCKI_PIN, MY9291_COMMAND_DEFAULT);

void rainbow(unsigned char index) {

    if (index < 85) {
        _my9291.setColor((my9291_color_t) { (unsigned int) index * 3, (unsigned int) 255 - index * 3, 0, 0 });
    } else if (index < 170) {
        index -= 85;
        _my9291.setColor((my9291_color_t) { (unsigned int) 255 - index * 3, 0, (unsigned int) index * 3, 0 });
    } else {
        index -= 170;
        _my9291.setColor((my9291_color_t) { 0, (unsigned int) index * 3, (unsigned int) 255 - index * 3, 0 });
    }

}

void setup() {

    // Init serial port and clean garbage
    Serial.begin(115200);
    Serial.println();
    Serial.println();

    // MY9291
    _my9291.setState(true);

}

void loop() {

    static unsigned char count = 0;
    static unsigned long last = millis();
    if (millis() - last > RAINBOW_DELAY) {
        last = millis();
        rainbow(count++);
    }

}
