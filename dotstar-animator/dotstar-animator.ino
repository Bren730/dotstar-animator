#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>
#include "DotstarAnimator.h"
#include "UltrasonicSensor.h"

// Dotstar strip pins
#define DATAPIN    7
#define CLOCKPIN   6

DotstarAnimator dotstarAnimator;
// Create one of these objects for every UltrasonicSensor
UltrasonicSensor sensor1 = UltrasonicSensor(22, 23);

unsigned long lastMeasurement;

// How often the system measures distance
// Expressed in milliseconds
// e.g.: 20, means 20 milliseconds, means 50 times per second
uint16_t measurementInterval = 20;

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

  // Another way of defining an animatedGradientConfig is like this:
  // First, create an AnimatedGradientConfig object.
  AnimatedGradientConfig gradient1;

  // Then specify all the paramaters you want.
  gradient1.frequency = 1.0; // higher numbers create more complex gradients with more valleys and peaks
  gradient1.amplitude = 1.0; // higher numbers increase the influence this gradient has on the end-result
  gradient1.animationSpeed = 0; // The speed with which the animation moves. Negative numbers will reverse the direction. 0 will stop it
  gradient1.phaseShift = 0.0; // Shifts the whole gradient over the length of the strip
  gradient1.yOffset = 0.11; // Use this to brighten up the whole gradient
  gradient1.color[0] = 255; // Red channel
  gradient1.color[1] = 255; // Green channel
  gradient1.color[2] = 255; // Blue channel

  // Create another AnimatedGradientConfig object
  AnimatedGradientConfig gradient2;

  // Specify the parameters for this gradient
  gradient2.frequency = 3;
  gradient2.amplitude = 1.0;
  gradient2.animationSpeed = 1;
  gradient2.phaseShift = 0.0;
  gradient2.yOffset = 0.15;
  gradient2.color[0] = 0;
  gradient2.color[1] = 0;
  gradient2.color[2] = 50;

  // Add all your separate configs into an array.
  // You can add 10 gradients in total. There is one big BUT!
  // Every gradient you add slows down the Arduino, resulting in noticeably less smooth animations
  // More than 2 is already pretty choppy. You might want to switch to a faster Arduino if you want
  // Fancier animations. I can recommend one if you want.
  AnimatedGradientConfig longGradientNotationExample[] =
  {
    gradient1,
//    gradient2,
  };

  // Finally, tell the animator what configuration to use for animated gradients
  dotstarAnimator.setAnimatedGradients(longGradientNotationExample, sizeof(longGradientNotationExample));
  
  // Set the amount of smoothing you want.
  // This is set as the amount of measurements it uses to average the output
  // Can be up to 255
  dotstarAnimator.setWipeInSmoothing(80);

  // Set the feather length of the wipe in
  // This determines how 'blurry' the edge of the feather is
  // Expressed as a percentage of LED strip length
  // e.g.: 0.2 = 20% of the length of your LED strip
  dotstarAnimator.setWipeInFeatherLength(0.2);

  lastMeasurement = millis();
}

void loop() {
  
  if (millis() - lastMeasurement > measurementInterval){
    float perc = sensor1.getDistanceAsPercentage();
    dotstarAnimator.setWipeInProgress(perc);

    lastMeasurement = millis();
  }

  // Leave this line in, it makes sure that the LED strip is updated properly
  dotstarAnimator.updateDotstar();

}
