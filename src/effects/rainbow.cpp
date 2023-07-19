#include "effects/rainbow.h"

void Rainbow::execute(CRGB leds[], uint8_t hue)
{
    fill_rainbow(leds, LED_NUM, hue, 5);
}