#include "effects/plasma.h"

void Plasma::execute(CRGB leds[], uint8_t hue)
{
    int thisPhase = beatsin8(6, -64, 64); // Setting phase change for a couple of waves.
    int thatPhase = beatsin8(7, -64, 64);

    for (int k = 0; k < LED_NUM; k++)
    { // For each of the LED's in the strand, set a brightness based on a wave as follows:

        int colorIndex = cubicwave8((k * 23) + thisPhase) / 2 + cos8((k * 15) + thatPhase) / 2; // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
        int thisBright = qsuba(colorIndex, beatsin8(7, 0, 96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

        leds[k] = ColorFromPalette(PartyColors_p, colorIndex, thisBright, LINEARBLEND); // Let's now add the foreground colour.
    }
}