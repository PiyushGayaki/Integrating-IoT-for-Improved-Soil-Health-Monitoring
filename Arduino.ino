#include <SoftwareSerial.h>
const int drySoilValue = 1023;  // Change this value to the minimum analog reading in dry soil
const int wetSoilValue = 0;     // Change this value to the maximum analog reading in wet soil
int soilMoistureValue;
SoftwareSerial nodemcu(0, 1);  // RX, TX pins of Arduino
const int lm35_pin = A0;
const int fc28_pin = A1;
const int ph_pin = A2;
float calibration_value = 16;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

void setup() {
  Serial.begin(9600);
  nodemcu.begin(9600);
}

void loop() {
  float lm35 = analogRead(lm35_pin) * 0.48828125;
  float fc28=moistureRead();
  float ph=phSensorRead();

  Serial.print((String)lm35+",");
  Serial.print((String)fc28+",");
  Serial.print((String)ph+"\n");
  delay(1000);
}

float moistureRead()
{
    soilMoistureValue = analogRead(fc28_pin);
    float soilMoisturePercentage = ((float)(soilMoistureValue - drySoilValue) / (float)(wetSoilValue - drySoilValue)) * 100.0;
    delay(1000); 
    return soilMoisturePercentage;
}
float phSensorRead() 
{
  for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(ph_pin);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
    if(buffer_arr[i]>buffer_arr[j])
    {
      temp=buffer_arr[i];
      buffer_arr[i]=buffer_arr[j];
      buffer_arr[j]=temp;
    }
  }
 }
 avgval=0;
 for(int i=2;i<8;i++)
  avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6;
 float ph_act = -5.70 * volt + calibration_value;
 delay(800);
 return ph_act;
}