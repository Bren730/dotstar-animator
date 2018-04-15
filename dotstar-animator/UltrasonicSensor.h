#include <Arduino.h>

class UltrasonicSensor
{
  private:

  int trigPin;
  int echoPin;
  unsigned long lastMeasurement;

  // Maximum measuring distance in cm
  float maxDistance = 400.0;
  int lastMeasuredDistance;

  public:
  UltrasonicSensor(int _trigPin, int _echoPin);

  int getTriggerPin();
  int getEchoPin();

  int measureDistance();
  float getDistanceAsPercentage();
};
