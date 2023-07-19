#include <Arduino.h>
#include <SoftwareSerial.h>
#include <FastLED.h>
#include <effects.h>
#include <effects/confetti.h>
#include <effects/doublesine.h>
#include <effects/juggle.h>
#include <effects/plasma.h>
#include <effects/rainbow.h>
#include <effects/sinedot.h>

CRGB leds[LED_NUM];
uint8_t currentLightMode = 0;
uint8_t hue = 0;
boolean lastButtonState = true; // Saves the last state of the button after each loop iteration
long buttonPressTimestamp = 0;  // Saves the relative timestamp of when the button was last pressed (rising edge)
long led2OnTimestamp = 0;       // Saves the relative timestamp of when sleep mode was activated
bool led2On = false;
bool sleeping = false;

void executeMode(uint8_t mode);
void nextMode();
void checkButton();

void setup()
{
  delay(3000);
  // pinMode(LED2_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  FastLED.addLeds<LED_CHIPSET, LED_PIN, RGB>(leds, LED_NUM).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(LED_BRIGHTNESS);
}

void loop()
{
  if (sleeping)
  {
    checkButton();
    return;
  }
  checkButton();
  executeMode(currentLightMode);
  FastLED.show();
  FastLED.delay(1000 / LED_FPS);
  EVERY_N_MILLISECONDS(7)
  {
    hue++;
  }
}

/**
   Applies the next light mode
*/
void nextMode()
{
  currentLightMode = (currentLightMode + 1) % EFFECTS_AMOUNT;
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
      fill_solid(leds, LED_NUM, CRGB::Black); // turn all leds off
      FastLED.show();
      sleeping = true;
    }
  }
  else
  {
    // Button not pressed
    if (!lastButtonState)
    { // button was pressed before
      if (millis() - buttonPressTimestamp >= BUTTON_BOUNCE_DELAY)
      {
        if (sleeping)
        {
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

void executeMode(uint8_t mode) {
  // todo: find better way to solve this
  switch (mode) {
    case 0:
      Confetti().execute(leds, hue);
      break;
    case 1:
      DoubleSine().execute(leds, hue);
      break;
    case 2:
      Juggle().execute(leds, hue);
      break;
    case 3:
      Plasma().execute(leds, hue);
      break;
    case 4:
      Rainbow().execute(leds, hue);
      break;
    case 5:
      SineDot().execute(leds, hue);
      break;
  }
}