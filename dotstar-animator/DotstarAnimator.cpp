#include <Arduino.h>
#include "DotstarAnimator.h"

#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>

DotstarAnimator::DotstarAnimator()
{

	neoPixel.begin();
	dotStar.begin();

}

void DotstarAnimator::fadeIn(uint32_t animTime, byte colors[][3], uint16_t dataLen)
{

	byte colorStops = dataLen / 3;

	Serial.println("FADEIN; Array length: "+ String(dataLen));
	Serial.println("FADEIN; Color stops: "+ String(colorStops));

	unsigned long stTime = millis();

	while(millis() < stTime + animTime)
	{
		unsigned long diff = millis() - stTime;
		float perc = (float)diff / (float)animTime;

		for (int i = 0; i < NUMPIXELS; i++)
		{

			float pos = (float)i / (float)NUMPIXELS;

			byte r = ((colors[0][0] * pos) + (colors[1][0] * (1 - pos))) * perc;
			byte g = ((colors[0][1] * pos) + (colors[1][1] * (1 - pos))) * perc;
			byte b = ((colors[0][2] * pos) + (colors[1][2] * (1 - pos))) * perc;

			// Serial.println("r: "+ String(r));
			// Serial.println("g: "+ String(g));
			// Serial.println("b: "+ String(b));
			// Serial.println();
			neoPixel.setPixelColor(i, neoPixel.Color(r, g, b));
		}

		neoPixel.show();
	}
}

void DotstarAnimator::wipeIn(uint32_t animTime, uint16_t wipeLen, byte colors[][3])
{

}

void DotstarAnimator::createGradient(byte colors[][3], uint16_t dataLen)
{
	byte colorStops = dataLen / 3;
	float colorStopLen = 1.0 / (colorStops - 1);
	float pixelsPerColorStop = colorStopLen * NUMPIXELS;

	for (uint16_t i = 0; i < NUMPIXELS; i++)
	{
		byte colorStop = floor((float)i / pixelsPerColorStop);
		float colorStopPerc = ((float)i / pixelsPerColorStop) - colorStop;

		gradient1[i][0] = ((colors[colorStop][0] * (1 - colorStopPerc)) + (colors[colorStop + 1][0] * (colorStopPerc)));
		gradient1[i][1] = ((colors[colorStop][1] * (1 - colorStopPerc)) + (colors[colorStop + 1][1] * (colorStopPerc)));
		gradient1[i][2] = ((colors[colorStop][2] * (1 - colorStopPerc)) + (colors[colorStop + 1][2] * (colorStopPerc)));

		setPixelColorGC(i, gradient1[i][0], gradient1[i][1], gradient1[i][2]);
	}
	neoPixel.show();
}

void DotstarAnimator::setPixelColorGC(uint16_t pixel, byte r, byte g, byte b)
{

	float gamma   = 2.2; // Correction factor
    int   max_in  = 255, // Top end of INPUT range
          max_out = 255; // Top end of OUTPUT range
          
    r = (int)(pow((float)r / (float)max_in, gamma) * max_out + 0.5);
    g = (int)(pow((float)g / (float)max_in, gamma) * max_out + 0.5);
    b = (int)(pow((float)b / (float)max_in, gamma) * max_out + 0.5);

	neoPixel.setPixelColor(pixel, neoPixel.Color(r, g, b));

}