#ifndef DOTSTAR_ANIMATOR_H
#define DOTSTAR_ANIMATOR_H

#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>

#define NUMPIXELS 91

enum GradientType
{
  GRADIENT_STATIC,
  GRADIENT_ANIMATED
};

struct RGB
{
  byte r;
  byte g;
  byte b;
};

struct StaticGradientConfig
{
  byte activeStops = 0;
  RGB stops[10];
};

// peaks
// amp
// animation speed
// phase shift
// y-offset
struct AnimatedGradientConfig
{
  float frequency;
  float amplitude;
  float animationSpeed;
  float phaseShift;
  float yOffset;
  byte color[3];
};

struct AnimatedGradientsConfig
{
  byte activeGradients = 0;
  AnimatedGradientConfig gradients[10];
};

class DotstarAnimator
{

  private:

    float _masterBrightness;

    byte gradient1[NUMPIXELS][3];
    byte outputGradient[NUMPIXELS][3];
    GradientType _gradientType;

    // FadeIn variables
    float fadeInProgress;
    uint32_t fadeInAnimTime;
    uint32_t fadeInStartTime;
    bool _didFadeIn = true;

    // WipeIn variables
    float wipeInProgress;
    uint32_t wipeInAnimTime;
    uint32_t wipeInStartTime;
    float wipeInFeatherLength;
    bool _didWipeIn = true;
    byte wipeInSmoothingFrames = 1;
    float wipeInValues[255];
    byte wipeInSmoothingIndex = 0;

    // StaticGradient variables
    StaticGradientConfig staticGradientConfig;

    // AnimatedGradient variables
    uint32_t animatedGradientsStartTime;
    AnimatedGradientsConfig animatedGradientsConfig;

    const uint8_t PROGMEM gamma8[256] = {
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
      1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
      2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
      5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
      10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
      17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
      25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
      37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
      51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
      69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
      90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
      115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
      144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
      177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
      215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
    };

  public:

    // Constructor
    DotstarAnimator();

    //    Adafruit_NeoPixel neoPixel = Adafruit_NeoPixel(60, 6, NEO_RGBW + NEO_KHZ800);
    Adafruit_DotStar dotStar = Adafruit_DotStar(NUMPIXELS, 7, 6, DOTSTAR_BRG);

    void updateDotstar();
    void invalidate();

    void setStripBrightness(float perc);

    void fadeIn(uint32_t animTime);
    void wipeIn(uint32_t animTime, float wipeLen);

    void setGradientType(GradientType gradientType);
    void setAnimatedGradients(AnimatedGradientConfig conf[], uint16_t dataLen);
    void drawAnimatedGradients();
    void setStaticGradient(byte colors[][3], uint16_t dataLen);
    void drawStaticGradient();

    void setPixelColorGC(uint16_t pixel, byte r, byte g, byte b);
    void invalidateMask();

    //getters and setters
    bool didFadeIn();
    void setFadeInCompleted(bool fadeInCompleted);
    
    bool didWipeIn();
    void setWipeInCompleted(bool wipeInCompleted);
    void setWipeInFeatherLength(float _wipeInFeatherLength);
    void setWipeInSmoothing(uint16_t _smoothingFrames);
    
    GradientType getGradientType();
    float getFadeInProgress();
    float getWipeInProgress();
    void setWipeInProgress(float _wipeInProgress);

};

#endif
