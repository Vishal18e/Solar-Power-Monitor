// this is the code for NodeMCU
// There has to be quite a few libraries installed for establishisng a connection
// with Arduino, Blynk and to Wifi.
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>// using Wifi.
#include <SimpleTimer.h>//The goal is to be able to execute a particular 
//piece of code every n milliseconds, without using interrupts.
#include <BlynkSimpleEsp8266.h>// library to introduce blynk and node Mcu.
#include <SoftwareSerial.h>// for the serial communication with the Arduino


// Blynk token you received via email
char auth[] = "Adw4GH4v2ilMyt0yvHvD7EaavEIatMun";// the code that was sent in my email id.
// Your WiFi credentials
char ssid[] = "viks";// mobile wifi.
char pass[] = "note5pro";//password



SimpleTimer timer;// object creation for simple Timer.

SoftwareSerial S(2,3);// object creation for communicating with Arduino.

//SimpleTimer timer;
//void ICACHE_RAM_ATTR ISRoutine (); 
String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors

float firstVal, secondVal; // sensors 
// This function sends Arduino's up time every second to Virtual Pin (1).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, millis() / 1000);
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  S.begin(9600); 
  Blynk.begin(auth, ssid, pass);
 
    timer.setInterval(1000L,sensorvalue1); // timer call for each sensor.
     timer.setInterval(1000L,sensorvalue2); 
     timer.setInterval(1000L,sensorvalue3);   
}

void loop() {

   if (S.available() == 0 ) // if there is no info from arduino to mcu
   //so write in blynk app using virtual pins.
   {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
   }
   
  if (S.available() > 0 ) // if available then decode it.
  {
  // put your main code here, to run repeatedly:
  rdata = S.read(); 
    myString = myString+ rdata; 
   Serial.print(rdata);
    if( rdata == '\n')
    {
    Serial.println(myString); 
   Serial.println("check");
// new code
String l = getValue(myString, ',', 0);
String m = getValue(myString, ',', 1);
//String n = getValue(myString, ',', 2);

firstVal = l.toInt();// converting it to Int
secondVal = m.toInt();// converting it to Int
 
myString = "";
// end new code
    }
  }
}
void sensorvalue1()//voltage
{
float sdata = firstVal;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V3, sdata);
 
}
void sensorvalue2()// current
{
float sdata = secondVal;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, sdata);
 
}

void sensorvalue3()// power
{
float sdata = (secondVal*firstVal)/1000;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V4, sdata);
 
}

//extraction of data from string in form of string.
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
