#include "effects/confetti.h"

void Confetti::execute(CRGB leds[], uint8_t hue)
{
    fadeToBlackBy(leds, LED_NUM, 15); // dim moved lights
    int pos = beatsin16(20, 0, LED_NUM - 1);
    leds[pos] += CHSV(hue, 255, 192); // set color with hue
}
