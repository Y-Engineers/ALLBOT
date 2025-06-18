#include <FastLED.h>

#define LED_PIN     9
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define WIDTH       8
#define HEIGHT      8
#define NUM_LEDS    WIDTH * HEIGHT
#define BRIGHTNESS  64

CRGB leds[NUM_LEDS];

uint16_t XY(uint8_t x, uint8_t y) {
  if (y % 2 == 0) {
    return y * WIDTH + x;
  } else {
    return y * WIDTH + (WIDTH - 1 - x);
  }
}

void setup() {
  //FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);

}

void loop() {
  static uint8_t hue = 0;

  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      leds[XY(x, y)] = CHSV(hue + x * 10 + y * 5, 255, 255);
    }
  }

  FastLED.show();
  hue++;
  delay(30);
}
