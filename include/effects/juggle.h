#pragma once
#include "effects.h"

class Juggle : public Effect
{
public:
    virtual void execute(CRGB leds[], uint8_t hue);
};