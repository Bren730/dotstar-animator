#include <Arduino.h>
#include "DotstarAnimator.h"

#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>

#define R 1
#define G 0
#define B 2

DotstarAnimator::DotstarAnimator()
{

  dotStar.begin();

}

void DotstarAnimator::updateDotstar()
{

  if (!_didWipeIn)
  {
    float perc = (float)(millis() - wipeInStartTime) / (float)wipeInAnimTime;


    if (perc <= 1)
    {
      invalidateWipeMask(perc);
    }
    else
    {
      _didWipeIn = true;
    }
  }

  if (!_didFadeIn)
  {
    float perc = (float)(millis() - fadeInStartTime) / (float)fadeInAnimTime;


    if (perc <= 1)
    {
      setStripBrightness(perc);
    }
    else
    {
      _didFadeIn = true;
    }
  }

  invalidate();
}

void DotstarAnimator::invalidate()
{
  for (uint16_t i = 0; i < NUMPIXELS; i++)
  {
    //    Serial.println(String(i) + ", " + String(outputGradient[i][0]));
    setPixelColorGC(i, outputGradient[i][R], outputGradient[i][G], outputGradient[i][B]);
  }
  dotStar.show();
}

void DotstarAnimator::setStripBrightness(float perc)
{

  for (uint16_t i = 0; i < NUMPIXELS; i++)
  {
    outputGradient[i][R] = (float)gradient1[i][R] * perc;
    outputGradient[i][G] = (float)gradient1[i][G] * perc;
    outputGradient[i][B] = (float)gradient1[i][B] * perc;
  }

}

void DotstarAnimator::fadeIn(uint32_t animTime, byte colors[][3], uint16_t dataLen)
{
  if (_didFadeIn)
  {
    fadeInAnimTime = animTime;
    createGradient(colors, dataLen);
    fadeInStartTime = millis();
    _didFadeIn = false;
  }

}

void DotstarAnimator::wipeIn(uint32_t animTime, float wipeLen, byte colors[][3], uint16_t dataLen)
{
  if (_didWipeIn)
  {
    wipeInAnimTime = animTime;
    wipeLength = wipeLen;
    createGradient(colors, dataLen);
    wipeInStartTime = millis();
    _didWipeIn = false;
  }

}

void DotstarAnimator::invalidateWipeMask(float perc)
{

  float gradientPerc = perc * wipeLength;
  float gradientPercHead = gradientPerc + perc;

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
      outputGradient[i][R] = (float)gradient1[i][R] * factor;
      outputGradient[i][G] = (float)gradient1[i][G] * factor;
      outputGradient[i][B] = (float)gradient1[i][B] * factor;
    }
    else if (pixelPos > gradientPercHead)
    {
      outputGradient[i][R] = 0;
      outputGradient[i][G] = 0;
      outputGradient[i][B] = 0;
    }
    else
    {
      outputGradient[i][R] = (float)gradient1[i][R];
      outputGradient[i][G] = (float)gradient1[i][G];
      outputGradient[i][B] = (float)gradient1[i][B];
    }
  }
}

void DotstarAnimator::createGradient(byte colors[][3], uint16_t dataLen)
{
  byte colorStops = dataLen / 3;
  float colorStopLen = 1.0 / (colorStops - 1);
  float pixelsPerColorStop = colorStopLen * NUMPIXELS;

  Serial.println("CREATE_GRADIENT: colorStops: " + String(colorStops));
  Serial.println("CREATE_GRADIENT: colorStopLen: " + String(colorStopLen));
  Serial.println("CREATE_GRADIENT: pixelsPerColorStop: " + String(pixelsPerColorStop));

  for (uint16_t i = 0; i < NUMPIXELS; i++)
  {
    byte colorStop = floor((float)i / pixelsPerColorStop);
    float colorStopPerc = ((float)i / pixelsPerColorStop) - colorStop;

    gradient1[i][R] = ((colors[colorStop][R] * (1 - colorStopPerc)) + (colors[colorStop + 1][R] * (colorStopPerc)));
    gradient1[i][G] = ((colors[colorStop][G] * (1 - colorStopPerc)) + (colors[colorStop + 1][G] * (colorStopPerc)));
    gradient1[i][B] = ((colors[colorStop][B] * (1 - colorStopPerc)) + (colors[colorStop + 1][B] * (colorStopPerc)));

    //    Serial.println("Pixel " + String(i) + " r " + String(gradient1[i][0]) + " colorStopPerc " + String(colorStopPerc));
  }
}

void DotstarAnimator::setPixelColorGC(uint16_t pixel, byte r, byte g, byte b)
{
  dotStar.setPixelColor(pixel, dotStar.Color(gamma8[r], gamma8[g], gamma8[b]));
}

bool DotstarAnimator::didFadeIn()
{
  return _didFadeIn;
}
