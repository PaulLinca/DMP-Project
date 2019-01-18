#include <Wire.h>
#include <Servo.h> 

// Pins of motor 1
#define mpin00 5
#define mpin01 6

// Pins of motor 2
#define mpin10 3
#define mpin11 11

//value received
int x;

//speed of motors
int motorSpeed = 100;

//servo motor
Servo srv; 

void setup()
{
  digitalWrite(mpin00, 0);
  digitalWrite(mpin01, 0);
  digitalWrite(mpin10, 0);
  digitalWrite(mpin11, 0);

  //set pins as outputs
  pinMode (mpin00, OUTPUT);
  pinMode (mpin01, OUTPUT);
  pinMode (mpin10, OUTPUT);
  pinMode (mpin11, OUTPUT); 
  
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  
  Serial.begin(9600);
}

//start the motor with a given direction and speed
void StartMotor (int m1, int m2, int forward, int speed)
{
  if (speed == 0) // stop
  {
    digitalWrite(m1, 0); 
    digitalWrite(m2, 0);
  }
  else
  {
    if (forward)
    {
      digitalWrite(m2, 0);
      analogWrite (m1, speed); // use PWM
    }
    else
    {
      digitalWrite(m1, 0);
      analogWrite(m2, speed);
    }
  }
} 

//read the WIFI value
void receiveEvent(int bytes)
{
  x = Wire.read();
  Serial.println(x);
}

//servo motor control
void playWithServo(int pin)
{
 srv.attach(pin);
 srv.write(0);
 delay(1000);
 srv.write(180);
 delay(1000);
 srv.write(90);
 delay(1000);
 srv.detach();
} 

void loop() 
{
  //how the motors should work based on the received signals
  if(x == 0) //forward
  {
    StartMotor (mpin00, mpin01, 0, motorSpeed);
    StartMotor (mpin10, mpin11, 0, motorSpeed);
  }
  else if(x == 1) //backwards
  {
    StartMotor (mpin00, mpin01, 1, motorSpeed);
    StartMotor (mpin10, mpin11, 1, motorSpeed);
  }
  else if(x == 2) //stop
  {
    StartMotor (mpin00, mpin01, 0, 0);
    StartMotor (mpin10, mpin11, 0, 0);
  }
  else if(x == 3) //right
  {
    StartMotor (mpin00, mpin01, 1, motorSpeed);
    StartMotor (mpin10, mpin11, 0, motorSpeed);
  }
  else if(x == 4) //left
  {
    StartMotor (mpin00, mpin01, 0, motorSpeed);
    StartMotor (mpin10, mpin11, 1, motorSpeed);
  }
  else if(x == 5) //accelerate
  {
    motorSpeed += 20;
  }
  else if(x == 6) //decelerate
  {
    motorSpeed -= 20;
  }
  else if(x == 7) //use servo
  {
    playWithServo(9);
  }
}
