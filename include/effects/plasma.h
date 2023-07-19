#pragma once
#include "effects.h"

class Plasma : public Effect
{
public:
    virtual void execute(CRGB leds[], uint8_t hue);
};