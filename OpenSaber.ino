//bomberfish

// OpenSaber: open-source diy lightsaber
#include <FastLED.h>
#define LED_PIN 5 // what your strip is connected to
#define LED_COUNT 60 // how many leds your strip has
#define BRIGHTNESS 64
#define CONTROLLER_TYPE WS2811 // THE IMPORTANT PART: what controller chip your strip uses. note that `WS2811` should also work for the ws2812/ws2812b. See https://github.com/FastLED/FastLED/wiki/Overview#3-wire-chipsets for supported chipsets.
#define COLOR_FORMAT GRB // what color format your strip uses. can be something like rgb, rgbw, etc etc etc
#define REFRESH_RATE 120 // refresh rate in hz. (essentially updates per second)

CRGB leds[LED_COUNT];



void setup() {
  // put your setup code here, to run once:
  delay(3000); // wait three seconds
  FastLED.addLeds<CONTROLLER_TYPE, LED_PIN, COLOR_FORMAT>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
}

void loop() {
  // put your main code here, to run repeatedly:
  // rainbow loop for now until i get my hands on a strip
  static uint8_t hue = 0;
  FastLED.showColor(CHSV(hue++, 255, 255)); 
  delay(10);
}
