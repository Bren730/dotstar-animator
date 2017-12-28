#include <Arduino.h>
#include "DotstarAnimator.h"

#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>

DotstarAnimator::DotstarAnimator()
{

  dotStar.begin();

}

void DotstarAnimator::updateDotstar()
{

  switch (_gradientType)
  {
    case GRADIENT_ANIMATED:
      drawAnimatedGradients();
      break;

    case GRADIENT_STATIC:
      //
      drawStaticGradient();
      break;

  }

  for (uint16_t i = 0; i < NUMPIXELS; i++)
  {
    outputGradient[i][0] = gradient1[i][0];
    outputGradient[i][1] = gradient1[i][1];
    outputGradient[i][2] = gradient1[i][2];
  }

  if (!_didWipeIn || !_didFadeIn)
  {
    invalidateMask();
  }

  invalidate();
}

float DotstarAnimator::getFadeInProgress()
{
  float progress = (float)(millis() - fadeInStartTime) / (float)fadeInAnimTime;

  if (progress >= 1.0 && !_didFadeIn)
  {
    progress = 1.0;
    _didFadeIn = true;
    Serial.println("fadeIn completed");
  }

  if (progress <= 0.0)
  {
    progress = 0.0;
  }

  if (_didFadeIn)
  {
    progress = 1.0;
  }

  return progress;
}

float DotstarAnimator::getWipeInProgress()
{
  float progress = (float)(millis() - wipeInStartTime) / (float)wipeInAnimTime;

  if (progress >= 1.0 && !_didWipeIn)
  {
    progress = 1.0;
    _didWipeIn = true;

    Serial.println("wipeIn completed");
  }

  if (progress <= 0.0)
  {
    progress = 0.0;
  }

  if (_didWipeIn)
  {
    progress = 1.0;
  }

  return progress;
}

void DotstarAnimator::invalidate()
{
  //  Serial.println("invalidate");
  for (uint16_t i = 0; i < NUMPIXELS; i++)
  {
    //    Serial.println(String(i) + ", " + String(outputGradient[i][0]));
    setPixelColorGC(i, outputGradient[i][0], outputGradient[i][1], outputGradient[i][2]);
  }
  dotStar.show();
}

void DotstarAnimator::setStripBrightness(float perc)
{

  for (uint16_t i = 0; i < NUMPIXELS; i++)
  {
    outputGradient[i][0] *= perc;
    outputGradient[i][1] *= perc;
    outputGradient[i][2] *= perc;
  }

}

void DotstarAnimator::fadeIn(uint32_t animTime)
{
  if (_didFadeIn)
  {
    fadeInAnimTime = animTime;
    //    createGradient(colors, dataLen);
    fadeInStartTime = millis();
    _didFadeIn = false;
  }

}

void DotstarAnimator::wipeIn(uint32_t animTime, float wipeLen)
{
  if (_didWipeIn)
  {
    wipeInAnimTime = animTime;
    wipeLength = wipeLen;
    //    createGradient(colors, dataLen);
    wipeInStartTime = millis();
    _didWipeIn = false;
  }

}

void DotstarAnimator::invalidateMask()
{
  float wipeProgress = getWipeInProgress();
  float brightness = getFadeInProgress();
  float gradientPerc = wipeProgress * wipeLength;
  float gradientPercHead = gradientPerc + wipeProgress;

  for (uint16_t i = 0; i < NUMPIXELS; i++)
  {
    float pixelPos = (float)i / NUMPIXELS;
    float factor = -(1 / wipeLength) * (pixelPos - gradientPercHead);

//    Serial.println(gradientPerc);
//    Serial.println(gradientPercHead);
//    Serial.println(pixelPos);
//    Serial.println(factor);
//    Serial.println();

    if (factor <= 1 && factor >= 0)
    {
      outputGradient[i][0] = gradient1[i][0] * factor * brightness;
      outputGradient[i][1] = gradient1[i][1] * factor * brightness;
      outputGradient[i][2] = gradient1[i][2] * factor * brightness;
    }
    else if (pixelPos > gradientPercHead)
    {
      outputGradient[i][0] = 0;
      outputGradient[i][1] = 0;
      outputGradient[i][2] = 0;
    }
    else
    {
      outputGradient[i][0] = gradient1[i][0] * brightness;
      outputGradient[i][1] = gradient1[i][1] * brightness;
      outputGradient[i][2] = gradient1[i][2] * brightness;
    }
  }
}

void DotstarAnimator::setGradientType(GradientType gradientType)
{
  _gradientType = gradientType;
}

void DotstarAnimator::setAnimatedGradients(AnimatedGradientConfig conf[], uint16_t dataLen)
{
  setGradientType(GRADIENT_ANIMATED);

  byte gradientCount = dataLen / sizeof(AnimatedGradientConfig);

  Serial.println("setAnimatedGradients: gradientCount " + String(gradientCount));

  animatedGradientsConfig.activeGradients = gradientCount;

  for (byte i = 0; i < gradientCount; i++)
  {
    animatedGradientsConfig.gradients[i] = conf[i];
  }

  animatedGradientsStartTime = millis();
}

void DotstarAnimator::drawAnimatedGradients()
{

  for (byte i = 0; i < NUMPIXELS; i++)
  {

    float perc = (float)i / (float)NUMPIXELS;
    float animPerc = (millis() - animatedGradientsStartTime) / 1000.0;

    float r = 0;
    float g = 0;
    float b = 0;

    for (byte j = 0; j < animatedGradientsConfig.activeGradients; j++)
    {
      AnimatedGradientConfig c = animatedGradientsConfig.gradients[j];

      //      Serial.println(c.frequency);
      //      Serial.println(c.amplitude);
      //      Serial.println(c.phaseShift);
      //      Serial.println(c.yOffset);
      //      Serial.println(c.animationSpeed);
      //      Serial.println("Perc: " + String(perc));

      float halfAmp = 0.5 * c.amplitude;

      float factor = halfAmp * cos( (c.frequency * PI) * (perc + (animPerc * -c.animationSpeed )) + (c.phaseShift * PI) ) + c.yOffset + halfAmp;

      r += (float)c.color[0] * factor;
      g += (float)c.color[1] * factor;
      b += (float)c.color[2] * factor;
      //      Serial.println("r: " + String(r));
      //      Serial.println();
    }

    r = (r > 255) ? 255 : r;
    g = (g > 255) ? 255 : g;
    b = (b > 255) ? 255 : b;

    gradient1[i][0] = r;
    gradient1[i][1] = g;
    gradient1[i][2] = b;
  }

}

void DotstarAnimator::setStaticGradient(byte colors[][3], uint16_t dataLen)
{
  setGradientType(GRADIENT_STATIC);
  uint16_t stops = dataLen / sizeof(byte) / 3;

  staticGradientConfig.activeStops = stops;

  Serial.println("setStaticGradient: colorstops " + String(staticGradientConfig.activeStops));

  for (byte i = 0; i < stops; i++)
  {
    staticGradientConfig.stops[i].r = colors[i][0];
    staticGradientConfig.stops[i].g = colors[i][1];
    staticGradientConfig.stops[i].b = colors[i][2];

    Serial.print("setStaticGradient: stop " + String(i) + ": ");
    Serial.print(String(staticGradientConfig.stops[i].r) + ", ");
    Serial.print(String(staticGradientConfig.stops[i].g) + ", ");
    Serial.print(String(staticGradientConfig.stops[i].b) + ", ");
    Serial.println();
  }

  drawStaticGradient();

}

void DotstarAnimator::drawStaticGradient()
{
  float colorStopLen = 1.0 / (staticGradientConfig.activeStops - 1);
  float pixelsPerColorStop = colorStopLen * NUMPIXELS;

  //    Serial.println("CREATE_GRADIENT: colorStops: " + String(staticGradientConfig.activeStops));
  //    Serial.println("CREATE_GRADIENT: colorStopLen: " + String(colorStopLen));
  //    Serial.println("CREATE_GRADIENT: pixelsPerColorStop: " + String(pixelsPerColorStop));

  for (uint16_t i = 0; i < NUMPIXELS; i++)
  {
    byte colorStop = floor((float)i / pixelsPerColorStop);
    float colorStopPerc = ((float)i / pixelsPerColorStop) - colorStop;

    gradient1[i][0] = ((staticGradientConfig.stops[colorStop].r * (1 - colorStopPerc)) + (staticGradientConfig.stops[colorStop + 1].r * (colorStopPerc)));
    gradient1[i][1] = ((staticGradientConfig.stops[colorStop].g * (1 - colorStopPerc)) + (staticGradientConfig.stops[colorStop + 1].g * (colorStopPerc)));
    gradient1[i][2] = ((staticGradientConfig.stops[colorStop].b * (1 - colorStopPerc)) + (staticGradientConfig.stops[colorStop + 1].b * (colorStopPerc)));

    //    Serial.println("Pixel " + String(i) + " r " + String(gradient1[i][0]) + " colorStopPerc " + String(colorStopPerc));
  }
}

void DotstarAnimator::setPixelColorGC(uint16_t pixel, byte r, byte g, byte b)
{
  dotStar.setPixelColor(pixel, dotStar.Color(gamma8[g], gamma8[r], gamma8[b]));
}

bool DotstarAnimator::didFadeIn()
{
  return _didFadeIn;
}

GradientType DotstarAnimator::getGradientType()
{

  return _gradientType;

}

