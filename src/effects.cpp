#include "effects.h"
#define qsubd(x, b) ((x > b) ? b : 0)     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x > b) ? x - b : 0) // Analog Unsigned subtraction macro. if result <0, then => 0

void rainbow(CRGB leds[], uint8_t hue)
{
  fill_rainbow(leds, LED_NUM, hue, 5);
}

void sineDot(CRGB leds[], uint8_t hue)
{
  fadeToBlackBy(leds, LED_NUM, 15); // dim moved lights
  int pos = beatsin16(20, 0, LED_NUM - 1);
  leds[pos] += CHSV(hue, 255, 192); // set color with hue
}

void doubleSine(CRGB leds[], uint8_t hue)
{
  fadeToBlackBy(leds, LED_NUM, 10);
  int pos1 = beatsin16(20, 0, LED_NUM / 2);
  int pos2 = (LED_NUM - 1) - (beatsin16(20, 0, LED_NUM / 2)); // reverse! dots meet in the center
  leds[pos1] += CHSV(qadd8(hue, random8(32)), 255, 192);
  leds[pos2] += CHSV(qadd8(hue, random8(32)), 255, 192);
}

void confetti(CRGB leds[], uint8_t hue)
{
  fadeToBlackBy(leds, LED_NUM, 8);
  int pos = random16(LED_NUM);
  leds[pos] += CHSV(qadd8(hue, random8(64)), 200, 255);
}

void juggle(CRGB leds[], uint8_t hue)
{
  fadeToBlackBy(leds, LED_NUM, 20);
  for (int i = 0; i < 8; i++)
  {
    leds[beatsin16(i + 7, 0, LED_NUM - 1)] |= CHSV(hue, 200, 255);
  }
}

void bpm(CRGB leds[], uint8_t hue)
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < LED_NUM; i++)
  { // 9948
    leds[i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
  }
}

void plasma(CRGB leds[], uint8_t hue)
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
