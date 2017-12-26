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
    setPixelColorGC(i, outputGradient[i][0], outputGradient[i][1], outputGradient[i][2]);
  }
  dotStar.show();
}

void DotstarAnimator::setStripBrightness(float perc)
{

  for (uint16_t i = 0; i < NUMPIXELS; i++)
  {
    outputGradient[i][0] = (float)gradient1[i][0] * perc;
    outputGradient[i][1] = (float)gradient1[i][1] * perc;
    outputGradient[i][2] = (float)gradient1[i][2] * perc;
  }

}

void DotstarAnimator::fadeIn(uint32_t animTime, byte colors[][3], uint16_t dataLen)
{
  fadeInAnimTime = animTime;
  createGradient(colors, dataLen);
  fadeInStartTime = millis();
  _didFadeIn = false;

}

void DotstarAnimator::wipeIn(uint32_t animTime, float wipeLen, byte colors[][3], uint16_t dataLen)
{
  wipeInAnimTime = animTime;
  wipeLength = wipeLen;
  createGradient(colors, dataLen);
  wipeInStartTime = millis();
  _didWipeIn = false;
}

void DotstarAnimator::invalidateWipeMask(float perc)
{
  
  float gradientPerc = perc * wipeLength;
  float gradientPercHead = gradientPerc + perc;
  
  for (uint16_t i = 0; i < NUMPIXELS; i++)
  {
    float pixelPos = (float)i / NUMPIXELS;
    float factor = -(1/wipeLength) * (pixelPos - gradientPercHead);

//    Serial.println(gradientPerc);
//    Serial.println(gradientPercHead);
//    Serial.println(pixelPos);
//    Serial.println(factor);
//    Serial.println();
    
    if (factor <= 1 && factor >= 0)
    {
      outputGradient[i][0] = (float)gradient1[i][0] * factor;
      outputGradient[i][1] = (float)gradient1[i][1] * factor;
      outputGradient[i][2] = (float)gradient1[i][2] * factor;
    }
    else if (pixelPos > gradientPercHead)
    {
      outputGradient[i][0] = 0;
      outputGradient[i][1] = 0;
      outputGradient[i][2] = 0;
    }
    else
    {
      outputGradient[i][0] = (float)gradient1[i][0];
      outputGradient[i][1] = (float)gradient1[i][1];
      outputGradient[i][2] = (float)gradient1[i][2];
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

    gradient1[i][0] = ((colors[colorStop][0] * (1 - colorStopPerc)) + (colors[colorStop + 1][0] * (colorStopPerc)));
    gradient1[i][1] = ((colors[colorStop][1] * (1 - colorStopPerc)) + (colors[colorStop + 1][1] * (colorStopPerc)));
    gradient1[i][2] = ((colors[colorStop][2] * (1 - colorStopPerc)) + (colors[colorStop + 1][2] * (colorStopPerc)));

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
