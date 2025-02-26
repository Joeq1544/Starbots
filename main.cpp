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

//Peformance Test 1
void peformanceTest1(void){
    //first part

    //second part: ramp
    setMotorSpeed(MOTORPOWER, MOTORPOWER);
}

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