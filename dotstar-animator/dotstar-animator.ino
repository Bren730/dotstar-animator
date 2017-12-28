#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>
#include "DotstarAnimator.h"

#define NUMPIXELS 60

// Dotstar strip pins
#define DATAPIN    4
#define CLOCKPIN   5

DotstarAnimator dotstarAnimator;

//byte exampleColors[][3] =
//{
//  {255, 255, 255},
//  {0, 0, 0},
//  {255, 255, 255},
//  {0, 0, 0},
//  {255, 255, 255},
//  {0, 0, 0},
//  {255, 255, 255},
//  {0, 0, 0},
//  {255, 255, 255},
//  {0, 0, 0},
//  {255, 255, 255},
//  {0, 0, 0},
//};

byte exampleColors[][3] =
{
  {255, 0, 0},
  {0, 127, 255},
};

void setup() {
  Serial.begin(115200);

  pinMode(A0, INPUT_PULLUP);

  // This function fades in the whole LED strip. It takes three arguments
  // The first one is the animation time in milliseconds, e.g.: 1000 = 1s
  // The second value is an array of colors. See the 'colors' array above.
  // The third value is used internally and should always be sizeof(theArrayYouAreUsing)
      dotstarAnimator.fadeIn(10000, exampleColors, sizeof(exampleColors));

  // This function wipes in a gradient from the start of the LED strip to the end
  // It takes four arguments
  // The first one is the animation time in milliseconds, e.g.: 1000 = 1s
  // The second one is the length of the wipe 'softness' if you will
  // It is defined in a factor that represents the total length of the LED strip
  // A value of .2 means it uses 20% of the length of the LED strip to fade in the wipe
  // The third value is an array of colors. See the 'colors' array above.
  // The fourth value is used internally and should always be sizeof(theArrayYouAreUsing)
//  dotstarAnimator.wipeIn(1000, .2, exampleColors, sizeof(exampleColors));
}

void loop() {

  // In this loop you can read the Distance sensor and, when someone is close/far enough, call
  // dotstarAnimator.fadeIn(animationTime, colors, sizeof(colors))
  // or dotstarAnimator.wipeIn(animationTime, fadeLength, colors, sizeof(colors))
  // As an example:
  if (analogRead(A0) < 200)
  {
    // Note that this reads the A0 analog pin. with an ultrasonic sensor you would do something like
    // ultrasonic.Ranging(CM) and check if the distance is lower/higher than you want.
    dotstarAnimator.wipeIn(5000, .0, exampleColors, sizeof(exampleColors));
  }


  // Leave this line in, it makes sure that the LED strip is updated properly
  dotstarAnimator.updateDotstar();

}
