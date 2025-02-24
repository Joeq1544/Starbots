#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHLCD.h>

int main(void)
{
    float x,y;
    while(!LCD.Touch(&x,&y)){};

    FEHMotor left_motor(FEHMotor::Motor0,9.0); 
    FEHMotor right_motor(FEHMotor::Motor2,9.0); 
    DigitalInputPin fr_micro(FEHIO::P0_7);
    DigitalInputPin br_micro(FEHIO::P0_5);
    DigitalInputPin fl_micro(FEHIO::P3_7);
    DigitalInputPin bl_micro(FEHIO::P3_6);

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
} 