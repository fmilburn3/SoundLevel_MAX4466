/*
Sound level sketch

Uses an Adafruit Electret Microphone Amplifier breakout (MAX4466 with
Adjustable Gain) to detect sound level and then displays it graphically
with LEDs as a bar.

Things to work on: 
  * very sensitive to knocks and claps - filter?
  * why does the delay at the end of the sketch help?
  * move to 430G2
  * write LCD graph?
  * how to calibrate?  

Tested on MSP430F5529

Circuit / Pin Connections

MAX4466       LaunchPad      LEDs        Resistor   
--------      ---------      ------      --------------
Out           6
GND           GND
VCC           3V3
              23             Green       330 ohm to GND
              24             Green       330 ohm to GND
              25             Green       330 ohm to GND
              26             Green       330 ohm to GND
              27             Yellow      330 ohm to GND
              28             Yellow      330 ohm to GND
              29             Yellow      330 ohm to GND
              30             Red         330 ohm to GND

Frank Milburn  24 June 2015
Released into the public domain
*/

const int soundPin = 6;             // analog pin on booster pack ref V2
const int soundWindow = 20;         // milliseconds spent in each window
// Pins that LEDs are attatched to...
const int ledPin[8] = {23, 24, 25, 26, 27, 28, 29, 30};
// Sound level that will activate associated LED on pins above...
const int soundLevel[8] = {30, 60, 120, 240, 480, 960, 1920, 3840};

void setup()
{
  Serial.begin(115200);
  Serial.println("Beginning sound test...");
  for (int i = 0; i < 8; i++)
  {
    // Set the pins to output mode and test the LEDs
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], HIGH);
    delay(20);
    digitalWrite(ledPin[i], LOW);
  }
}

void loop()
{
  // The main loop reads the sound sensor multiple times
  // in a time "window".  During that time it records the
  // number of readings, minimum reading, maximum reading,
  // and sum oreadings so that it can average them.
  long startWindow = millis();
  int level = 0;
  int minLevel = 4095;
  int maxLevel = 0;
  int readings = 0;
  long sumReadings = 0;
  int avgLevel = 0;
  while ((millis() - startWindow) <= soundWindow)
  {
    readings = readings++;
    int thisReading = analogRead(soundPin);
    sumReadings += abs(thisReading - 2047);
    if (thisReading > maxLevel)
    {
      maxLevel = thisReading;
    }
    if (thisReading < minLevel)
    {
      minLevel = thisReading;
    }
  }  
  level = maxLevel - minLevel; 
  avgLevel = sumReadings / readings; 
  

  // Numerical output to serial monitor
  Serial.print(millis());
  Serial.print(" , ");
  Serial.print(readings);
  Serial.print(" , ");
  Serial.print(minLevel);
  Serial.print(" , ");
  Serial.print(maxLevel);
  Serial.print(" , ");
  Serial.print(level);
  Serial.print(" , ");
  Serial.print(avgLevel);
  Serial.print("   ");

  
  // "Graphical" output to serial monitor
  for (int i = 0; i < level/40; i++)
  {
    Serial.print("=");
  }
  Serial.println("");  
  
  // Blink the LEDs
  for (int i = 0; i < 8; i++)
  {
    if (level > soundLevel[i])
    {     
      digitalWrite(ledPin[i], HIGH);
    }
    else
    {
      digitalWrite(ledPin[i], LOW);
    }
  }
  delay(5);  // this seems to make it more readable, less jitter at low end...
}
