#include <Arduino.h>
#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(int _trigPin, int _echoPin)
{
  Serial.print("Initialising UltrasonicSensor on pins ");
  Serial.println(String(_trigPin) + " (trigger), " + String(_echoPin) + " (echo)");
  trigPin = _trigPin;
  echoPin = _echoPin;

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

int UltrasonicSensor::getTriggerPin()
{
  return trigPin;
}

int UltrasonicSensor::getEchoPin()
{
  return echoPin;
}

int UltrasonicSensor::measureDistance()
{
  if (millis() - lastMeasurement < 20) 
  {
    return lastMeasuredDistance;
  }
  // Start with a delay to prevent excessive measurements
  delay(16);
  long duration;
  int distance;
  // Clears the TRIG_PIN
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the TRIG_PIN on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the ECHO_PIN, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH, 38000);
  lastMeasurement = millis();

  if (duration > 0)
  {
    // Calculating the distance
    distance = duration * 0.034 / 2.0;
    // Prints the distance on the Serial Monitor
    lastMeasuredDistance = distance;
    return distance;
  }
  else
  {
    //    Serial.println("duration 0");
    //    reset_hcsr04();
    return -1;
  }
}

float UltrasonicSensor::getDistanceAsPercentage()
{
  int dist = measureDistance();
  float perc = (float)dist / maxDistance;
  return perc;
}

