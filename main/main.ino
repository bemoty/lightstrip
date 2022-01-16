#include <SoftwareSerial.h>
#include <FastLED.h>
#include "effects.h"

CRGB leds[LED_NUM];
uint8_t currentLightMode = 0;
uint8_t hue = 0;
boolean lastButtonState = true; // Saves the last state of the button after each loop iteration
long buttonPressTimestamp = 0;  // Saves the relative timestamp of when the button was last pressed (rising edge)
long led2OnTimestamp = 0;       // Saves the relative timestamp of when sleep mode was activated
bool led2On = false;
bool sleeping = false;

typedef void (*LightMode[])(CRGB leds[], uint8_t hue);
LightMode lightModes = {rainbow, sineDot, doubleSine, confetti};

void setup()
{
  delay(3000);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  FastLED.addLeds<LED_CHIPSET, LED_PIN, RGB>(leds, LED_NUM).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(LED_BRIGHTNESS);
}

void loop()
{
  if (sleeping) {
    checkButton();
    return;
  } else {
    if (led2On && millis() - led2OnTimestamp >= LED2_TURNOFF_DELAY) {
      fill_solid(leds, LED_NUM, CRGB::Black); // turn all leds off
      FastLED.show();
      sleeping = true;
      return;
    }
  }
  checkButton();
  lightModes[currentLightMode](leds, hue);
  FastLED.setBrightness(led2On ? LED_BRIGHTNESS / 5 : LED_BRIGHTNESS);
  FastLED.show();
  FastLED.delay(1000 / LED_FPS);
  EVERY_N_MILLISECONDS(15)
  {
    hue++;
  }
}

/**
   Applies the next light mode
*/
void nextMode()
{
  currentLightMode = (currentLightMode + 1) % ARRAY_SIZE(lightModes);
}

/**
   Checks the button's current state and moves to the next light mode if
   the button was pressed. (Anti-bounce: Button needs to be pressed at
   least 200ms for this function to move to the next light mode.
*/
bool flag = false;
void checkButton()
{
  // Detect button state (anti-bounce)
  if (!digitalRead(BUTTON_PIN))
  {
    // Button pressed
    if (lastButtonState)
    { // button wasn't pressed before
      buttonPressTimestamp = millis();
    }
    if (millis() - buttonPressTimestamp >= LED2_DELAY && !flag)
    {
      digitalWrite(LED2_PIN, led2On ? LOW : HIGH);
      led2On = !led2On;
      led2OnTimestamp = millis();
      flag = true;
    }
  }
  else
  {
    // Button not pressed
    if (!lastButtonState)
    { // button was pressed before
      if (millis() - buttonPressTimestamp >= BUTTON_BOUNCE_DELAY)
      {
        if (sleeping) {
          sleeping = false;
          return;
        }
        if (!flag)
        {
          nextMode();
        }
        else
        {
          flag = false;
        }
      }
    }
  }
  lastButtonState = digitalRead(BUTTON_PIN);
}
