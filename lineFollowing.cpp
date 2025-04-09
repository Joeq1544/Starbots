#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHSD.h>
#include <FEHMotor.h>

// Declarations for analog optosensors
AnalogInputPin right_opto(FEHIO::P1_0);
AnalogInputPin middle_opto(FEHIO::P1_1);
AnalogInputPin left_opto(FEHIO::P1_2);

FEHMotor right_motor(FEHMotor::Motor0,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);

#define left_threshold 1.9
#define middle_threshold 1.9
#define right_threshold 1.9

enum DriveState{
    GOMIDDLE,
    GOLEFT,
    GORIGHT
};

void updateState(int *state)
{
    if(middle_opto.Value() > middle_threshold){
        *state = GOMIDDLE;
        // LCD.WriteLine("UMID");
        // LCD.WriteLine(middle_opto.Value());
    }
    else if(left_opto.Value() > left_threshold)
    {
        *state = GOLEFT;
        // LCD.WriteLine("ULEFT");
        // LCD.WriteLine(left_opto.Value());
    }
    else if(right_opto.Value() > right_threshold)
    {
        *state = GORIGHT;
        // LCD.WriteLine("URIGHT");
        // LCD.WriteLine(right_opto.Value());
    }
    else
    {
        *state = GOMIDDLE;
        // LCD.WriteLine("URIGHT");
    }
}

void moveLF(double inches){
    //Convert inches to counts
    int counts=(abs(inches) * 318)/(3.0 * PI);

    //Reset encodor counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    float rightSpeed = (11.5 / Battery.Voltage()) * MOTORPOWER;
    float leftSpeed = rightSpeed + 1;


    if(inches < 0)
    {
        rightSpeed *= -1;
        leftSpeed *= -1;
    }
    
    int state = GOMIDDLE;
    updateState(&state);
    //Motors run while the average of the left and right encoder is less than counts
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts){
        LCD.WriteLine(state);
        switch(state){
            case GOMIDDLE:
                right_motor.SetPercent(rightSpeed);
                left_motor.SetPercent(leftSpeed);
                break;
            case GORIGHT:
                right_motor.SetPercent(0);
                left_motor.SetPercent(leftSpeed);
                break;
            case GOLEFT:
                right_motor.SetPercent(rightSpeed);
                left_motor.SetPercent(0);
                break;
            default:
                break;
        }
        updateState(&state);
        Sleep(.05);
        LCD.Clear();
    }


    //Turn off motors once desired drive length is achieved
    left_motor.Stop();
    right_motor.Stop();

    //Sleep .5 seconds as to not damage motors
    Sleep(.5);
}