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

DigitalEncoder left_encoder(FEHIO::P2_0);
DigitalEncoder right_encoder(FEHIO::P3_0);

#define MOTORPOWER 40
#define PI 3.14159265359

int main(void)
{
    float x,y;
    while(!LCD.Touch(&x,&y)){};

    performanceTest1();

    //for debugging
    while (true) {
        if (LCD.Touch(&x, &y)) {
            LCD.WriteLine(left_encoder.Counts());
            LCD.WriteLine(right_encoder.Counts());
            Sleep(1);
        }
    }

    return 0;

}

//Code for Peformance Test 1
void performanceTest1(void){
    //first part
    moveForward(37);//distance: 37 inches

    //second part: ramp
    moveForward(27);
    moveBackward(27);
}

//Sets the left and right motor speed
void setMotorSpeed(int leftSpeed, int rightSpeed){
    left_motor.SetPercent(leftSpeed);
    right_motor.SetPercent(rightSpeed);
}

void moveForward(double inches){
    //Convert inches to counts
    int counts=(inches * 360)/(2.0 * PI * 2.75);

    //Reset encodor counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    setMotorSpeed(MOTORPOWER, MOTORPOWER - 1);

    //Motors run while the average of the left and right encoder is less than counts
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts){}

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();

    //Sleep .5 seconds
    Sleep(.5);
}

void moveBackward(double inches){
    //Convert inches to counts
    int counts=(inches*360)/(2.0*PI*2.75);

    //Reset encodor counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    setMotorSpeed(-1*MOTORPOWER, -1*MOTORPOWER + 1);

    //Motors run while the average of the left and right encoder is less than counts
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts){}

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();

    //Sleep .5 seconds
    Sleep(.5);
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