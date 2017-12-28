#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>
#include "DotstarAnimator.h"

#define NUMPIXELS 60

// Dotstar strip pins
#define DATAPIN    4
#define CLOCKPIN   5

DotstarAnimator dotstarAnimator;

// You can pass a total of 10 gradient configurations to the animator.
// They are additive, meaning all the colors will be combined
// If you specify three configurations that are the same, except for the color
// And those colors happen to be pure red, green and blue, then the resulting gradient will be white.
// You can set the animationSpeed to 0.0, which will keep the gradient in place
// The notation below is a quick way of defining an animatedGradientConfig
AnimatedGradientConfig shortGradientNotationExample[] =
{
  // frequency, amplitude, animationSpeed, phaseShift, yOffset, color
  {3, 1.0, .0, 0.0, .15, {50, 50, 50}}, // Configuration 1
  {3, 1.0, .1, 0.0, .15, {50, 50, 50}}, // Configuration 2
  {3, 1.0, .2, 0.0, .15, {50, 50, 50}}, // Configuration 3
  {3, 1.0, .3, 0.0, .15, {50, 50, 50}}, // Configuration 4
};

void setup() {
  Serial.begin(115200);

  pinMode(A0, INPUT_PULLUP);

  // Another way of defining an animatedGradientConfig is like this:
  // First, create an AnimatedGradientConfig object.
  AnimatedGradientConfig gradient1;

  // Then specify all the paramaters you want.
  gradient1.frequency = 1.0; // higher numbers create more complex gradients with more valleys and peaks
  gradient1.amplitude = 1.0; // higher numbers increase the influence this gradient has on the end-result
  gradient1.animationSpeed = 0; // The speed with which the animation moves. Negative numbers will reverse the direction. 0 will stop it
  gradient1.phaseShift = 0.0; // Shifts the whole gradient over the length of the strip
  gradient1.yOffset = 0.15; // Use this to brighten up the whole gradient
  gradient1.color[0] = 255; // Red channel
  gradient1.color[1] = 255; // Green channel
  gradient1.color[2] = 255; // Blue channel

  // Create another AnimatedGradientConfig object
  AnimatedGradientConfig gradient2;

  // Specify the parameters for this gradient
  gradient2.frequency = 3;
  gradient2.amplitude = 1.0;
  gradient2.animationSpeed = .1;
  gradient2.phaseShift = 0.0;
  gradient2.yOffset = 0.15;
  gradient2.color[0] = 50;
  gradient2.color[1] = 50;
  gradient2.color[2] = 50;

  // Add all your separate configs into an array.
  // You can add 10 gradients in total. There is one big BUT!
  // Every gradient you add slows down the Arduino, resulting in noticeably less smooth animations
  // More than 2 is already pretty choppy. You might want to switch to a faster Arduino if you want
  // Fancier animations. I can recommend one if you want.
  AnimatedGradientConfig longGradientNotationExample[] =
  {
    gradient1,
    gradient2
  };

  // Finally, tell the animator what configuration to use for animated gradients
  dotstarAnimator.setAnimatedGradients(longGradientNotationExample, sizeof(longGradientNotationExample));

  // You can also still declare a static gradient by specifying the colors (rgb)
  // Each colorstop will be spaced out equally over the length of the LED strip
  byte staticGradientExample[][3] =
  {
    {255, 255, 255},
    {0, 0, 0},
    {255, 255, 255},
    {0, 0, 0},
    {255, 255, 255},
    {0, 0, 0},
  };
  
  // After defining a gradient, tell the animator what staticGradientConfiguration to use
  // dotstarAnimator.setStaticGradient(staticGradientExample, sizeof(staticGradientExample));

  // Whichever type of gradient you defined last (animated or static) will be used

  // Since gradients are defined separately from the wipeIn or fadeIn effects,
  // these can now be used together. 
  //You can define a static or animated gradient, then set both a wipeIn and fadeIn.

  // wipeIn takes two arguments: animationDuration in ms and the length of the fade in a factor of the strip length
  // In this case, it wipes in in 4 seconds, and the fade is 20% of the strip length
  dotstarAnimator.wipeIn(4000, .2);

  // fadeIn is simpler and takes just 1 argument: animationDuration.
  // This is simply the time over which the fade in will happen.
  dotstarAnimator.fadeIn(4000);

  // In this example, it does both a wipeIn and fadeIn, meaning the lights will animate on from beginning to end
  // and all of them will also brighten up over the duration of the fadeIn length
  // At the same time, the animatedGradientConfiguration ensures the gradients also animate
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

  }


  // Leave this line in, it makes sure that the LED strip is updated properly
  dotstarAnimator.updateDotstar();

}
