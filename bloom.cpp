// Olivia Hsieh
// BLOOM: Bloom is a project exploring the integration of hardware with artwork. Using laser cut polyhedra 
// nets, truncated octahedron were assembled with Arduino Trinkets, breadboards, Neopixel diffused 8mm lights, and 
// motion sensors inside of them, which, when set off, triggers five rainbow light cycles. The following code pulls from
// the Neopixel API: https://github.com/adafruit/Adafruit_NeoPixel

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            3         // Input pin for Neopixel lights
#define NUMPIXELS      18        // total number of Pixels

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const int PIRpin = 2;           // PIR input pin
uint8_t pirState = LOW;

int delayval = 1000; // delay for half a second

void setup() {
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1); // This is for Trinket 5V 16MHz
#endif

  pinMode(PIRpin, INPUT);                                 // Initial state is low
  pixels.begin();                                         // This initializes the NeoPixel library.
  pixels.setBrightness(30);
  pixels.show();

  delay(30000);                                           // Delays for 30 seconds to allow motion sensor to calibrate
}

void loop() {

  int sense = digitalRead(PIRpin);                        // Read PIR Sensor
  if (sense == HIGH) {                                    // If high and it was low, sensor has been tripped
    if (pirState == LOW) {
      pirState = HIGH;
      rainbow(20);
      rainbowCycle(20);
    }
  } else {
    if (pirState == HIGH) { 
      pirState = LOW;
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        pixels.show();
        delay(200);
      }
    }
  }
}

void colorWipe(uint32_t color, uint8_t wait) {
  for (uint16_t i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, Wheel((i + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) {                           // 5 cycles of all colors on wheel
    for (i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / NUMPIXELS) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
