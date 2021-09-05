// this is the code involving instructions to the arduino borad.
// since this also contains some LCD code
//So we have to include "LiquidCrystal.h"
// for serial communication with Node Mcu we have to include #include <SoftwareSerial.h>


#include "LiquidCrystal.h"
#include <SoftwareSerial.h>
int contrast =75;
int Vsensor= A0; // voltage sensor 
int Csensor =A1; // Current sensor

String cdata;// Sending data to node mcu through string.


float current= 0.0;// initialled to 0
float vin= 0.0;// initialled to 0

SoftwareSerial S(15,14);//Rx =15 , Tx=14.

const float VCC   = 5.0;// supply voltage is from 4.5 to 5.5V. Normally 5V.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7

// for current sensor
float sensitivity =0.066;// In place of potentiometer.
const float QOV =   0.5 * VCC;// set quiescent Output voltage of 0.5V
float voltage;// internal variable for voltage

uint32_t delayMS = 1000;// delay specified.

// voltage sensor
float correctionfactor = 0; // adjust this for calibration
float vout = 0.0; 


// two resistors 30k and 7.5k ohms
float R1 = 30000;  // inbuilt internal resistances.   
float R2 = 7500; //  
int value = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);// baud rate is 9600 for communication with serial monitor.
  S.begin(9600);// baud rate for commnuication with node Mcu. 
  
  pinMode(Vsensor, INPUT);//Sensors to arduino 
  pinMode(Csensor, INPUT);//Sensors to arduino 

  analogWrite(6,contrast);// Potentiometer .
  lcd.begin(16,2);// LCD is of 2 rows and 16 columns.
  lcd.setCursor(0,0);// LCD starting address is 0,0
  lcd.print("IoT based Solar ");
  lcd.setCursor(0,1);// Moving to first row.
  lcd.print("Monitoring Sys.  ");
  delay(2000);
  lcd.setCursor(0,0);
  lcd.print("Vishal Kumar    ");
  lcd.setCursor(0,1);// first row.
  lcd.print("                ");
  delay(2000);//delay
  lcd.setCursor(0,0);
  lcd.print("Roll: 180102096  ");
  delay(2000);// delay
  Serial.println("Vishal Kumar");
  Serial.println("ACS712 Current Sensor");// printing on the serial monitor.
  
}

void loop() {
  // put your main code here, to run repeatedly:


  //Current sensor part. 
  float voltage_raw =   (5.0 / 1023.0)* analogRead(Csensor);// Read the voltage from sensor
  voltage =  voltage_raw - QOV + 0.012 ;// 0.000 is a value to make voltage zero when there is no current
  current = voltage / sensitivity;
  current = current*100;

    Serial.print("V: \n");
    Serial.print(voltage,3);// print voltage with 3 decimal places
    Serial.print("V, I: \n");
    Serial.print(current,2); // print the current with 2 decimal places
    Serial.println("mA");
//
  lcd.setCursor(0,0);
  lcd.print("Current =         ");
  lcd.setCursor(9,0);
  lcd.print(current);
  delay(3000);




// voltage sensor part.  
float vtot = 0.0;
  int loops = 10; // number of samples

  // loop multiple times and get average reading
  for (int i=0; i < loops; i++) {
    vtot = vtot + analogRead(Vsensor);
  }
  value = vtot/loops;

  // voltage calculation
  vout = (value * 5) / 1024.0; // 3.3V
  vin = vout / (R2/(R1+R2));
  
  vin = vin - correctionfactor; 
  vin= vin* 1000;//vin is in millivolts.
 Serial.print("Voltage in vsensor: "); 
  Serial.print(value);
  Serial.println("V\n");
  Serial.print("Voltage: \n"); 
  Serial.print(vin,4);
  Serial.println("V\n");
  lcd.setCursor(0,0);
  lcd.print("V_input=         ");
  lcd.setCursor(9,0);
  lcd.print(vin);
  delay(3000);


cdata = cdata+current+","+vin;// concatenating strings to get a transferable string.

Serial.println(cdata); 
   S.println(cdata);
   delay(2000); // 2000 milli seconds
   cdata = "";



}
