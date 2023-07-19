#include "effects/juggle.h"

void Juggle::execute(CRGB leds[], uint8_t hue)
{
    fadeToBlackBy(leds, LED_NUM, 20);
    for (int i = 0; i < 8; i++)
    {
        leds[beatsin16(i + 7, 0, LED_NUM - 1)] |= CHSV(hue, 200, 255);
    }
}