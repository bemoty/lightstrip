#pragma once
#include "FastLED.h"
#include "Arduino.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define EFFECTS_AMOUNT 6
#define BUTTON_PIN 2
#define BUTTON_BOUNCE_DELAY 200
#define LED2_DELAY 1500
#define LED_PIN 5
#define LED_NUM 150
#define LED_CHIPSET WS2812
#define LED_FPS 92
#define LED_BRIGHTNESS 80

#define qsubd(x, b) ((x > b) ? b : 0)
#define qsuba(x, b) ((x > b) ? x - b : 0)

class Effect
{
public:
    virtual void execute(CRGB leds[], uint8_t hue) = 0;
};
