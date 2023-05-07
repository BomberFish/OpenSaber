//bomberfish

// OpenSaber: open-source diy lightsaber

#include <FastLED.h>


//// CONFIG ////
#define LED_PIN 5 // what your strip is connected to
#define LED_COUNT 60 // how many leds your strip has
#define BRIGHTNESS 64
#define CONTROLLER_TYPE WS2811 // THE IMPORTANT PART: what controller chip your strip uses. note that `WS2811` should also work for the ws2812/ws2812b. See https://github.com/FastLED/FastLED/wiki/Overview#3-wire-chipsets for supported chipsets.
#define COLOR_FORMAT GRB // what color format your strip uses. can be something like rgb, rgbw, etc etc etc. the most common ws2812b-based strips usually use grb
#define REFRESH_RATE 120 // refresh rate in hz. (essentially updates per second)
#define COLOR 0x0F0FFF // color of your saber in hex format (0xrrggbb), not dependent on strip color format.

CRGB leds[LED_COUNT];



void setup() {
  // put your setup code here, to run once:
  FastLED.clear();
  Serial.begin(9600);
  Serial.println("OpenSaber 0.1");
  delay(3000); // wait three seconds before first start
  Serial.println("Initializing LEDs...");
  FastLED.addLeds<CONTROLLER_TYPE, LED_PIN, COLOR_FORMAT>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
  Serial.println("Done!");
  Serial.println("Playing startup animation...");
  Serial.println();
  for(int current = 0.00; current < LED_COUNT / 2; current++) {
    //serial prints commented out to normalize anim speed and save on them precious cpu cycles
    //Serial.print("Lighting LEDS ");
    //Serial.print(current);
    //Serial.print(", ");
    //Serial.print(LED_COUNT - current);
    //Serial.println();
    leds[current] = COLOR;
    leds[LED_COUNT - current] = COLOR;
    double current_double = current; // super janky fix
    double count_double = LED_COUNT;
    double brightness = current_double / count_double;
    //Serial.print("Setting Brightness to ");
    //Serial.print(brightness);
    //Serial.println();
    FastLED.setBrightness(255 * brightness);
    FastLED.show();
    FastLED.delay(10);
  } 
}

void loop() {
  // put your main code here, to run repeatedly:
  // rainbow loop for now until i get my hands on a strip
  //FastLED.showColor(CHSV(hue++, 255, 255)); 
  //Serial.println(hue);
  //delay(10);
}
