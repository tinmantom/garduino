/* Automatic watering system - Tom Dixon Dec. 2014
D.I.Y. moisture sensor made out of two pensils and a 10k resistor forming a voltage divider onto A1
10k pot onto A0 to allow variable sensitivity
TIP120 on D5 to low-side switch a servo on and off (to save power between operations
Servo connected to D9 and mechanically coupled to water valve

System takes moisture measurment, acts on result then goes to sleep for 8 seconds (to conserve battery power)

sleep tutorial from Jeelib
*/
#include <Servo.h> 
#include <JeeLib.h>
#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 1
#define PIN 11
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Servo myservo;  
int pos = 15;    // variable to store the servo position 
int sensitivity;
int moistureSensor;
boolean valveOpenState;
ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup the watchdog


void setup() {
myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
pinMode (5, OUTPUT); //control for servo (via TIP120)
pinMode (12, OUTPUT); //power for moisture sensor
pinMode (10, OUTPUT); //power for neopixel
digitalWrite(10, HIGH);
digitalWrite(12, HIGH);
pixels.begin();

} 
 
void loop() 
{

    sensitivity=analogRead(A1);
    digitalWrite(12, HIGH);
    delay(10);
    moistureSensor=analogRead(A3);
    digitalWrite(12, LOW);
if (moistureSensor<=sensitivity)
{
  if (valveOpenState==0)
  {
  openValve();
  valveOpenState=1;
  }
digitalWrite(10, HIGH); //give the neoPixel some power
delay(10); //wait for the neoPixel to wake up
pixels.setPixelColor(0, pixels.Color(50,0,0)); //set the color to red to indicate water is flowing 
pixels.show(); 
Sleepy::loseSomeTime(40); //just flash the neopixel to keep power down
digitalWrite(10, LOW); //turn the neoPixel off!
}
else
{
  if (valveOpenState==1)
  {
  closeValve();
  valveOpenState=0;
  }
     digitalWrite(10, HIGH);
     delay(10);
    pixels.setPixelColor(0, pixels.Color(0,50,0)); 
    pixels.show(); 
Sleepy::loseSomeTime(40);
digitalWrite(10, LOW);
  
} 
Sleepy::loseSomeTime(5000);

}
void openValve()
  
  {
pos=12;
digitalWrite(5, HIGH);
delay(200);
myservo.write(pos);
delay(500);
digitalWrite(5, LOW);

  }
 
void closeValve()
 {
pos=110; 
digitalWrite(5, HIGH);
delay(200);
myservo.write(pos);
delay(500);
digitalWrite(5,LOW);
 }
 
