#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <Servo.h>
volatile unsigned char t2flag=0;
int RX = 12;
int TX = 13;
SoftwareSerial mySerial(RX, TX);  
int dir_l = 4;
int dir_r = 7;
int pwm_l = 6;
int pwm_r = 5;
int led_green = 9;
int led_blue = 10;
int led_red = 11;
int piezo_pin = 8;
int button = 16;

#define MOVING_SPEED 100

int motor_seq = 0;
int angle = 20;
Servo myservo;

void setup() {
   Serial.begin(9600);
   mySerial.begin(9600);
   MsTimer2::set(50, flash); // 50ms period
   MsTimer2::start();
    
   pinMode(dir_l, OUTPUT); pinMode(dir_r, OUTPUT); pinMode(pwm_l, OUTPUT); pinMode(pwm_r, OUTPUT);
   pinMode(led_green, OUTPUT); pinMode(led_blue, OUTPUT); pinMode(led_red, OUTPUT); 
   pinMode(piezo_pin, OUTPUT); pinMode(button, INPUT_PULLUP);  

   myservo.attach(3);
   myservo.write(angle);
 }

char btbuf;

void flash()
{
  t2flag = 1 ;
}
    
void loop() {    

   if (t2flag /*mySerial.available()*/)
   {
    t2flag = 0;
    btbuf = mySerial.read();
    
    if (btbuf >= 'A' && btbuf <= 'z') {
      Serial.print(btbuf + " ");
    }
    
    if      ('U' == btbuf) { Serial.println("forward");      moving(HIGH, HIGH, MOVING_SPEED, MOVING_SPEED); }
    else if ('u' == btbuf) { Serial.println("forward");      moving(LOW, LOW, 0, 0); }   
    else if ('D' == btbuf) { Serial.println("backward");     moving(LOW, LOW, MOVING_SPEED, MOVING_SPEED); }  
    else if ('d' == btbuf) { Serial.println("backward");     moving(HIGH, HIGH, 0, 0); }  
    else if ('L' == btbuf) { Serial.println("left rotate");  moving(LOW, HIGH, MOVING_SPEED, MOVING_SPEED); }  
    else if ('l' == btbuf) { Serial.println("left rotate");  moving(LOW, HIGH, 0, 0); }  
    else if ('R' == btbuf) { Serial.println("right rotate"); moving(HIGH, LOW, MOVING_SPEED, MOVING_SPEED); }  
    else if ('r' == btbuf) { Serial.println("right rotate"); moving(HIGH, LOW, 0, 0); }  
    else if ('B' == btbuf) { Serial.println("left axis");    moving(LOW, HIGH, 0, MOVING_SPEED); }  
    else if ('b' == btbuf) { Serial.println("left axis");    moving(LOW, LOW, 0, 0); }  
    else if ('E' == btbuf) { Serial.println("right axis");   moving(HIGH, LOW, MOVING_SPEED, 0); }  
    else if ('e' == btbuf) { Serial.println("right axis");   moving(LOW, LOW, 0, 0); }  
    else if ('F' == btbuf) { Serial.println("Accerlate");    moving(HIGH, HIGH, 255, 255); }  
    else if ('f' == btbuf) { Serial.println("Accerlate");    moving(LOW, LOW, 0, 0); }  
    else if ('A' == btbuf) { Serial.println("RGB LED");      rgb(LOW, LOW, HIGH); }  
    else if ('a' == btbuf) { Serial.println("RGB LED");      rgb(HIGH, HIGH, HIGH); }
    else if ('S' == btbuf) { Serial.println("Music");        digitalWrite(piezo_pin, HIGH); }
    else if ('s' == btbuf) { Serial.println("Music");        digitalWrite(piezo_pin, LOW); }
    
    // SERVO MOTOR MOVING
    else if ('M' == btbuf) { Serial.println("motor up");     motor_seq = 1; }
    else if ('N' == btbuf) { Serial.println("motor down");   motor_seq = -1; }
    else if ('O' == btbuf) { Serial.println("motor upup");   motor_seq = -2; }
    else if ('m' == btbuf || 'n' == btbuf || 'o' == btbuf) 
    {
     Serial.println("motor stop"); 
     motor_seq = 0;
    }
   }

 if(motor_seq == 1) {
  angle = min(180, angle);
  if(angle < 90) {
    angle += 1;
    delay(40);
    myservo.write(angle);
  }
 } else if(motor_seq == -1) {
  angle = max(0, angle);
  if(angle > 0) {
    angle -= 1;
    delay(40);
    myservo.write(angle);
  }
 } else if(motor_seq == -2) {
  myservo.write(angle+5);
  delay(40);
  myservo.write(angle);
  delay(40);
 }
}

void moving(int dirl, int dirr, int pwml, int pwmr)  
{
  digitalWrite(dir_l, dirl);
  digitalWrite(dir_r, dirr);
  analogWrite(pwm_l, pwml);
  analogWrite(pwm_r, pwmr);
}

void rgb(int ledgreen, int ledblue, int ledred)  
{
  digitalWrite(led_green, ledgreen);
  digitalWrite(led_blue, ledblue);
  digitalWrite(led_red, ledred);
}
