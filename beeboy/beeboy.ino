#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <Servo.h>

#define DIR_L 4
#define DIR_R 7
#define PWM_L 6
#define PWM_R 5

#define LED_GREEN 9
#define LED_BLUE 10
#define LED_RED 11
#define PIEZO_PIN 8
#define RX 12
#define TX 13
#define BUTTON 16

#define MOVING_SPEED 100
#define ROTATE_SPEED 50

int motorSeq = 0;
int angle = 20;

volatile unsigned char t2Flag=0;

SoftwareSerial mySerial(RX, TX);  
Servo myServo;

void setup() {
   Serial.begin(9600);
   mySerial.begin(9600);
   MsTimer2::set(50, flash); // 50ms period
   MsTimer2::start();
    
   pinMode(DIR_L, OUTPUT); pinMode(DIR_R, OUTPUT); pinMode(PWM_L, OUTPUT); pinMode(PWM_R, OUTPUT);
   pinMode(LED_GREEN, OUTPUT); pinMode(LED_BLUE, OUTPUT); pinMode(LED_RED, OUTPUT); 
   pinMode(PIEZO_PIN, OUTPUT); pinMode(BUTTON, INPUT_PULLUP);  

   myServo.attach(3);
   myServo.write(angle);
 }

char btbuf;

void flash()
{
  t2Flag = 1 ;
}
    
void loop() {    

   if (t2Flag /*mySerial.available()*/)
   {
    t2Flag = 0;
    btbuf = mySerial.read();
    
    if (btbuf >= 'A' && btbuf <= 'z') {
      Serial.print(btbuf + " ");
    }
    
    if      ('U' == btbuf) { Serial.println("forward");      moving(HIGH, HIGH, MOVING_SPEED, MOVING_SPEED); }
    else if ('u' == btbuf) { Serial.println("forward");      moving(LOW, LOW, 0, 0); }   
    else if ('D' == btbuf) { Serial.println("backward");     moving(LOW, LOW, MOVING_SPEED, MOVING_SPEED); }  
    else if ('d' == btbuf) { Serial.println("backward");     moving(HIGH, HIGH, 0, 0); }  
    else if ('L' == btbuf) { Serial.println("left rotate");  moving(LOW, HIGH, ROTATE_SPEED, ROTATE_SPEED); }  
    else if ('l' == btbuf) { Serial.println("left rotate");  moving(LOW, HIGH, 0, 0); }  
    else if ('R' == btbuf) { Serial.println("right rotate"); moving(HIGH, LOW, ROTATE_SPEED, ROTATE_SPEED); }  
    else if ('r' == btbuf) { Serial.println("right rotate"); moving(HIGH, LOW, 0, 0); }  
    else if ('B' == btbuf) { Serial.println("left axis");    moving(LOW, HIGH, 0, MOVING_SPEED); }  
    else if ('b' == btbuf) { Serial.println("left axis");    moving(LOW, LOW, 0, 0); }  
    else if ('E' == btbuf) { Serial.println("right axis");   moving(HIGH, LOW, MOVING_SPEED, 0); }  
    else if ('e' == btbuf) { Serial.println("right axis");   moving(LOW, LOW, 0, 0); }  
    else if ('F' == btbuf) { Serial.println("Accerlate");    moving(HIGH, HIGH, 255, 255); }  
    else if ('f' == btbuf) { Serial.println("Accerlate");    moving(LOW, LOW, 0, 0); }  
    else if ('A' == btbuf) { Serial.println("RGB LED");      rgb(LOW, LOW, HIGH); }  
    else if ('a' == btbuf) { Serial.println("RGB LED");      rgb(HIGH, HIGH, HIGH); }
    else if ('S' == btbuf) { Serial.println("Music");        digitalWrite(PIEZO_PIN, HIGH); }
    else if ('s' == btbuf) { Serial.println("Music");        digitalWrite(PIEZO_PIN, LOW); }
    
    // SERVO MOTOR MOVING
    else if ('M' == btbuf) { Serial.println("motor up");     motorSeq = 1; }
    else if ('N' == btbuf) { Serial.println("motor down");   motorSeq = -1; }
    else if ('O' == btbuf) { Serial.println("motor upup");   motorSeq = -2; }
    else if ('m' == btbuf || 'n' == btbuf || 'o' == btbuf) 
    {
     Serial.println("motor stop"); 
     motorSeq = 0;
    }
   }

 if(motorSeq == 1) {
  angle = min(180, angle);
  if(angle < 90) {
    angle += 1;
    delay(40);
    myServo.write(angle);
  }
 } else if(motorSeq == -1) {
  angle = max(0, angle);
  if(angle > 0) {
    angle -= 1;
    delay(40);
    myServo.write(angle);
  }
 } else if(motorSeq == -2) {
  myServo.write(angle+5);
  delay(40);
  myServo.write(angle);
  delay(40);
 }
}

void moving(int dirl, int dirr, int pwml, int pwmr)  
{
  digitalWrite(DIR_L, dirl);
  digitalWrite(DIR_R, dirr);
  analogWrite(PWM_L, pwml);
  analogWrite(PWM_R, pwmr);
}

void rgb(int ledgreen, int ledblue, int ledred)  
{
  digitalWrite(LED_GREEN, ledgreen);
  digitalWrite(LED_BLUE, ledblue);
  digitalWrite(LED_RED, ledred);
}
