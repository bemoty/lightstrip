#ifndef EFFECTS_H
#define EFFECTS_H
#include "FastLED.h"
#include "Arduino.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define BUTTON_PIN 2
#define BUTTON_BOUNCE_DELAY 200
#define LED2_TURNOFF_DELAY 1800000 // 30 mins
#define LED2_DELAY 2000
#define LED2_PIN 13
#define LED_PIN 5
#define LED_NUM 150
#define LED_CHIPSET WS2812
#define LED_FPS 60
#define LED_BRIGHTNESS 100

void rainbow(CRGB leds[], uint8_t hue);
void sineDot(CRGB leds[], uint8_t hue);
void doubleSine(CRGB leds[], uint8_t hue);
void confetti(CRGB leds[], uint8_t hue);
void juggle(CRGB leds[], uint8_t hue);
void bpm(CRGB leds[], uint8_t hue);

class Effect
{
public:
    virtual void execute(CRGB leds[], uint8_t hue);
};

#endif
