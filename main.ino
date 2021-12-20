#include <SoftwareSerial.h>
#include <FastLED.h>

// If true, light modes are changed automatically
#define AUTO_MODE false
// If true, uses a random effect on every auto mode change
#define AUTO_RANDOM true
// Only effective if the above is true; delay between modes in seconds
#define AUTO_MODE_DELAY 15

// Length of array divided by length of data type
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Button properties
#define BUTTON_PIN  2
#define BOUNCE_DELAY  200

// LED strip properties
#define LED_PIN   5
#define CLOCK_PIN 6
#define NUM_LEDS  30
#define CHIPSET   DOTSTAR
#define FRAMES_PER_SECOND 60
#define BRIGHTNESS  200
CRGB leds[NUM_LEDS];

// Light modes
typedef void (*LightModeList[])();
uint8_t currentLightMode = 0;
uint8_t hue = 0;

// Saves the last state of the button after each loop iteration
boolean lastButtonState = true;
// Saves the relative timestamp of when the button was last pressed (rising edge)
long buttonPressTimestamp = 0;

void rainbow() {
  // last = delta hue, standard rainbow
  fill_rainbow(leds, NUM_LEDS, hue, 5);
}

void sineDot() {
  fadeToBlackBy(leds, NUM_LEDS, 15); // dim moved lights
  int pos = beatsin16(20, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(hue, 255, 192); // set color with hue
}

void confetti() {
  fadeToBlackBy(leds, NUM_LEDS, 8);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(qadd8(hue, random8(64)), 200, 255);
}

void doubleSine() {
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos1 = beatsin16(20, 0, NUM_LEDS / 2);
  int pos2 = (NUM_LEDS - 1) - (beatsin16(20, 0, NUM_LEDS / 2)); // reverse! dots meet in the center
  leds[pos1] += CHSV(qadd8(hue, random8(32)), 255, 192);
  leds[pos2] += CHSV(qadd8(hue, random8(32)), 255, 192);
}

void sparkle() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(0, 0, 130);
  }
  if (random8() < 50) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

// lists light mode functions
LightModeList lightModes = { rainbow, sineDot, doubleSine, confetti, sparkle };

void setup() {
  delay(3000); // startup delay

  // Button setup (negative logic)
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // FastLED setup
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  // Setup console communcation
  Serial.begin(9600);
}

void loop() {
  // Check button state
  checkButton();

  // Play light mode
  lightModes[currentLightMode]();

  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND); // framerate

  // adapt hue
  EVERY_N_MILLISECONDS(15) {
    hue++;
  }

  // auto mode
  if (AUTO_MODE) {
    EVERY_N_SECONDS(AUTO_MODE_DELAY) {
      if (AUTO_RANDOM) {
        currentLightMode = random8(ARRAY_SIZE(lightModes));
      } else {
        nextMode();
      }
    }
  }
}

/**
   Applies the next light mode
*/
void nextMode() {
  currentLightMode = (currentLightMode + 1) % ARRAY_SIZE(lightModes);
}

/**
   Checks the button's current state and moves to the next light mode if
   the button was pressed. (Anti-bounce: Button needs to be pressed at
   least 200ms for this function to move to the next light mode.
*/
void checkButton() {
  // Detect button state (anti-bounce)
  if (!digitalRead(BUTTON_PIN)) {
    // Button pressed
    if (lastButtonState) { // button wasn't pressed before
      buttonPressTimestamp = millis();
    }
  } else {
    // Button not pressed
    if (!lastButtonState) { // button was pressed before
      if (millis() - buttonPressTimestamp >= BOUNCE_DELAY) {
        nextMode();
      }
    }
  }
  lastButtonState = digitalRead(BUTTON_PIN);
}
