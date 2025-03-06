#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHLCD.h>
#include <FEHServo.h>
#include <FEHRCS.h>
#include <FEHBattery.h>
#include <math.h>


FEHMotor right_motor(FEHMotor::Motor1,9.0); 
FEHMotor left_motor(FEHMotor::Motor0,9.0); 

DigitalEncoder left_encoder(FEHIO::P0_0);
DigitalEncoder right_encoder(FEHIO::P3_5);

AnalogInputPin CdS_cell(FEHIO::P0_1);

#define MOTORPOWER 35
#define PI 3.14159265359

void milestone1(void);
void setMotorSpeed(int leftSpeed, int rightSpeed);
void move(double inches);
void milestone2(void);
void turn(double angle);

int main(void)
{
    float x,y;
    while(!LCD.Touch(&x,&y)){};

    //milestone2();
    milestone2();

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

//Sets the left and right motor speed
void setMotorSpeed(int leftSpeed, int rightSpeed){
    right_motor.SetPercent(rightSpeed);
    left_motor.SetPercent(leftSpeed);
}

//Positive inches moves forward, negative inches moves backwards.
void move(double inches){
    //Convert inches to counts
    int counts=(abs(inches) * 318)/(3.0 * PI);

    //Reset encodor counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    float rightSpeed = (11.5 / Battery.Voltage()) * MOTORPOWER;
    float leftSpeed = rightSpeed + 1;


    if(inches > 0)
    {
        setMotorSpeed(leftSpeed, rightSpeed);
    }
    else{
        setMotorSpeed(-leftSpeed, -rightSpeed);
    }
    
    //Motors run while the average of the left and right encoder is less than counts
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts){}

    //Turn off motors once desired drive length is achieved
    left_motor.Stop();
    right_motor.Stop();

    //Sleep .5 seconds as to not damage motors
    Sleep(.5);
}


//Positive angle turns clockwise, negative angle turns counterclockwise
void turn(double angle){
    double turningRadius = 8.1; //distance from center of robot to a wheel
    double distance = (turningRadius * PI) * (abs(angle)/360.0);

    //Convert inches to counts
    int counts=(distance * 318)/(3.0 * PI);

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    if(angle > 0){
        setMotorSpeed(MOTORPOWER, -1 * MOTORPOWER); //turn right
    }else{
        setMotorSpeed(-1 * MOTORPOWER, MOTORPOWER); //turn left
    }

     //Motors run while the average of the left and right encoder is less than counts
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts){}

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*Code for Milestone 1.
    In this checkpoint, the robot must go from one edge to the other. 
    Then, must go up the ramp. Bonus points for going back down.
*/
void milestone1(void){
    //x and y variables for the touch input while loop.
    float x,y;

    //Part 1: Going from one edge to the other
    move(28);//distance: 32 inches
    while(!LCD.Touch(&x,&y)){};

    //second part: going up and back down the ramp.
    move(25);
    move(-30);
}

/*
    Code for Milestone2
    In this checkpoint, the robot drives to upper level, reads and displays color of 
    humidifier light on Proteus screen, then pushes the correct button. 
    Bonus points for dirving back to the lower level and pushing the final button.

*/
void milestone2(void){

    //start light

    do{
        LCD.WriteLine(CdS_cell.Value());
        Sleep(0.1);    
    } while (CdS_cell.Value() > 2.0);

    turn(-90); 
    move(7); //move towards ramp
    turn(-30);
    move(31); //up the ramp
    turn(-83); //turn towards humidifier buttons
    move(9); //Move towards humidifier buttons

    //TODO: line up to read the light


    // red or blue
    LCD.WriteLine(CdS_cell.Value());
    if(CdS_cell.Value() < 1.65) {
        //red
        LCD.WriteLine("RED");
        turn(90); //turn 
        move(2);
        turn(90); //turn 


    } else{
        //blue
        LCD.WriteLine("BLUE");
        turn(-90); //turn backwards
        move(2);  
        turn(-90); //turn   
    }
    move(-7); //press the button
    move(10);

}