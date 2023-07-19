#pragma once
#include "effects.h"

class Confetti : public Effect
{
public:
    virtual void execute(CRGB leds[], uint8_t hue);
};