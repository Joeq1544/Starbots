#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHLCD.h>
#include <FEHServo.h>

FEHMotor right_motor(FEHMotor::Motor1,9.0); 
FEHMotor left_motor(FEHMotor::Motor0,9.0); 

DigitalEncoder left_encoder(FEHIO::P0_0);
DigitalEncoder right_encoder(FEHIO::P3_5);

#define MOTORPOWER 35
#define PI 3.14159265359

void performanceTest1(void);
void setMotorSpeed(int leftSpeed, int rightSpeed);
void moveForward(double inches);
void moveBackward(double inches);

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
    float x,y;
    //first part
    moveForward(28);//distance: 32 inches
    while(!LCD.Touch(&x,&y)){};

    //second part: ramp
    moveForward(25);
    moveBackward(45);
}

//Sets the left and right motor speed
void setMotorSpeed(int leftSpeed, int rightSpeed){
    right_motor.SetPercent(rightSpeed);
    left_motor.SetPercent(leftSpeed);
}

void moveForward(double inches){
    //Convert inches to counts
    int counts=(inches * 318)/(3.0 * PI);

    //Reset encodor counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    setMotorSpeed(MOTORPOWER + 1, MOTORPOWER);

    //Motors run while the average of the left and right encoder is less than counts
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts){}

    //Turn off motors
    left_motor.Stop();
    right_motor.Stop();

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
    setMotorSpeed(-1*MOTORPOWER, -1*MOTORPOWER -1);

    //Motors run while the average of the left and right encoder is less than counts
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts){}

    //Turn off motors
    left_motor.Stop();
    right_motor.Stop();

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
    */