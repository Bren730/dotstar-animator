#ifndef DOTSTAR_ANIMATOR_H
#define DOTSTAR_ANIMATOR_H

#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>

#define NUMPIXELS 60

class DotstarAnimator
{

private:
	byte gradient1[NUMPIXELS][3];

public:

	// Constructor
	DotstarAnimator();

	Adafruit_NeoPixel neoPixel = Adafruit_NeoPixel(60, 6, NEO_RGBW + NEO_KHZ800);
	Adafruit_DotStar dotStar = Adafruit_DotStar(60, 4, 5, DOTSTAR_BRG);

	void fadeIn(uint32_t animTime, byte colors[][3], uint16_t dataLen);
	void wipeIn(uint32_t animTime, uint16_t wipeLen, byte colors[][3]);
	void createGradient(byte colors[][3], uint16_t dataLen);
	void setPixelColorGC(uint16_t pixel, byte r, byte g, byte b);
};

#endif