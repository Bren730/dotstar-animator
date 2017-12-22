#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>
#include "DotstarAnimator.h"

#define JING

#define NUMPIXELS 60

// NeoPixel strip pins
#define NPPIN 6

// Dotstar strip pins
#define DATAPIN    4
#define CLOCKPIN   5

// Uncomment this line Jing
//  Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

// Comment out this line Jing
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, NPPIN, NEO_RGBW + NEO_KHZ800);

DotstarAnimator dotstarAnimator;

void setup() {
  Serial.begin(115200);
  byte colors[][3] =
  {
    {255, 255, 0},
    {0, 0, 0},
    {0, 255, 255},
    {0, 0, 0},
    {255, 0, 255},
    {0, 0, 0},
    {255, 255, 0},
    {0, 0, 0},
    {0, 255, 255},
    {0, 0, 0},
    {255, 0, 255},
    {0, 0, 0},
    {255, 255, 0},
    {0, 0, 0},
    {0, 255, 255},
    {0, 0, 0},
    {255, 0, 255},
  };

  //  dotstarAnimator.fadeIn(10000, colors, sizeof(colors));
  dotstarAnimator.createGradient(colors, sizeof(colors));
}

void loop() {
  // put your main code here, to run repeatedly:

  //  for (byte i = 0; i < NUMPIXELS; i++)
  //  {
  //    strip.setPixelColor(i, strip.Color(255, 255, 255));
  //  }
  //
  //  strip.show();

  //  byte colors[][3] =
  //  {
  //    {255, 0, 0},
  //    {0, 255, 0},
  //    {0, 0, 255}
  //  };
  //
  ////  dotstarAnimator.fadeIn(10000, colors, sizeof(colors));
  //dotstarAnimator.createGradient(colors, sizeof(colors));

}
