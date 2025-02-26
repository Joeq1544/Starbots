#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHLCD.h>
#include <FEHServo.h>

FEHMotor left_motor(FEHMotor::Motor0,9.0); 
FEHMotor right_motor(FEHMotor::Motor2,9.0); 

DigitalInputPin fr_micro(FEHIO::P0_7);
DigitalInputPin br_micro(FEHIO::P0_5);
DigitalInputPin fl_micro(FEHIO::P3_7);
DigitalInputPin bl_micro(FEHIO::P3_6);

#define MOTORPOWER 40

int main(void)
{
    float x,y;
    while(!LCD.Touch(&x,&y)){};

}

//Code for Peformance Test 1
void peformanceTest1(void){
    /*  FIRST PART: 
        Starting with robots trailing edge AGAINST the wall to the right of the starting area. 
        Then, drive until you hit the other wall on the left side WITHOUT touching compost bin or tree stump
    */
   setMotorSpeed(MOTORPOWER, MOTORPOWER);


    /*  SECOND PART: 
        Place robot arbitrarily on the lower level at the bottom of the ramp with NO driving surfaces
        touching any part of the ramp.
        Then, the robot drives up the ramp to the upper level and ends with no drive surfaces touching any part of the ramp.
    */

    /*  EXTRA CREDIT
        Robot is able to drive back down the ramp to the lower level with no drive surfaces touching any part of the course/ramp. 
        Must be done WITHOUT interruption AFTER having driven up to the upper level.
    */

}

//Sets the left and right motor speed
void setMotorSpeed(int leftSpeed, int rightSpeed){
    left_motor.SetPercent(leftSpeed);
    right_motor.SetPercent(rightSpeed);
}


    //Exploration 02
    /*

    left_motor.SetPercent(40);
    right_motor.SetPercent(40);
    while(fr_micro.Value() || fl_micro.Value()){};
    left_motor.SetPercent(-25);
    right_motor.SetPercent(-25);
    Sleep(250);
    left_motor.SetPercent(-40);
    right_motor.SetPercent(40);
    Sleep(500);
    left_motor.SetPercent(-25);
    right_motor.SetPercent(-25);
    while(br_micro.Value()){};
    left_motor.SetPercent(40);
    right_motor.SetPercent(40);
    while(fr_micro.Value() || fl_micro.Value()){};
    left_motor.SetPercent(-25);
    right_motor.SetPercent(-25);
    Sleep(250);
    left_motor.SetPercent(40);
    right_motor.SetPercent(-40);
    Sleep(500); 
    left_motor.SetPercent(-25);
    right_motor.SetPercent(-25);
    while(br_micro.Value()||bl_micro.Value()){};
    left_motor.SetPercent(40);
    right_motor.SetPercent(40);
    while(fr_micro.Value() || fl_micro.Value()){};
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);

    */