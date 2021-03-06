/* 
Bart Theys, with inspiration from:
http://barraganstudio.com>
http://www.arduino.cc/en/Tutorial/Sweep
used on an arduino nano
*/

#include <Servo.h>
#include <ServoInput.h>
 
ServoInputPin<2> servo1; // steering wheel
ServoInputPin<3> servo2; // throttle stick
Servo myservo1;  // create servo object to control a servo
Servo myservo2;


// twelve servo objects can be created on most boards

// configuration
float wheelbase= 0.4; //virtual length between main differential wheels and virtual steering wheels
float trackwidth=0.4; //real width between the wheels
float betamax=0.6; //0.1; //0.785; //45/180*Pi;  //virtual max steering angle = atan(wheelbase/min_turn_radius)
float throttle_max=1;   //factor that scales the throttle input, 1 allows for full throttle, decrease to decrease the maximum throttle


int pos = 0;    // variable to store the servo position
float angle_in1 = 90;
float angle_in2 = 90;
float Pi = 3.14159;
float steering = 0;
float throttle = 0;
float beta = 0;
float throttle_lim = 0;
float PWM1 = 0;
float PWM2 = 0;
float angle_out1 = 90;
float angle_out2 = 90;



void setup() {
      Serial.begin(9600);
  myservo1.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(10);
  
}

void loop() {

     angle_in1 = servo1.getAngle();  // get angle of throttle wheel (0 - 180)
     angle_in2 = servo2.getAngle();  // get angle of throttle stick
throttle = map(angle_in2,0,180,-1000,1000);  //throttle input from -1000 to 1000  
steering = map(angle_in1,0,180,-1000,1000);  //steering angle input from -1000 to 1000

 // apply deadzone
 if (abs(throttle) < 100)
 {
  throttle = 0;
 }
 if (abs(steering) <100)
 {
  steering = 0;
 }
 
 if (throttle >=100)
 {
 throttle = map(throttle,100,1000,0,1000);
 }
  if (throttle <=-100)
 {
 throttle = map(throttle,-100,-1000,0,-1000);
 }
  if (steering >=100)
 {
 steering = map(steering,100,1000,0,1000);
 }
  if (steering <=-100)
 {
 steering = map(steering,-100,-1000,0,-1000);
 }

beta = steering*betamax/1000;  // virtual steering angle
throttle_lim=1;


if (abs(beta)>0) // further scale the throttle with the current steering input (perhaps we can calculate the scaling factor below , and then use the smallest of the two)
{
throttle_lim = 1/(1+trackwidth/(2*wheelbase)*abs(tan(beta)));   //scale the throttle so that any steering angle is always possible at full throttle input without exceeding the max speed/input of the outboard motor
}
 
throttle=throttle*min(throttle_max,throttle_lim); //scale the throttle input with the smallest scaling factor (limit max speed, or make sure full turns are possible without exceeding the max speed to one channel)

PWM1 = throttle_lim*(1+trackwidth*tan(beta)/(2*wheelbase));
PWM2 = throttle_lim*(1-trackwidth*tan(beta)/(2*wheelbase));
angle_out1 = map(PWM1,-1000,1000,0,180);  //scale to the max pwm input
angle_out2=  map(PWM2,-1000,1000,0,180);  //scale to the max pwm input
Serial.println(throttle_lim);

    myservo1.write(angle_out1);              // tell servo to go to position in variable 'pos'
    myservo2.write(angle_out2);
               
  }
