#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHLCD.h>
#include <FEHServo.h>
#include <FEHRCS.h>
#include <FEHBattery.h>
#include <math.h>
#include <time.h>
#include <cassert>


FEHMotor right_motor(FEHMotor::Motor1,9.0); 
FEHMotor left_motor(FEHMotor::Motor0,9.0); 

DigitalEncoder left_encoder(FEHIO::P0_0);
DigitalEncoder right_encoder(FEHIO::P3_5);

// Declarations for analog optosensors
AnalogInputPin right_opto(FEHIO::P2_0);
AnalogInputPin middle_opto(FEHIO::P2_1);
AnalogInputPin left_opto(FEHIO::P2_2);

AnalogInputPin CdS_cell(FEHIO::P0_1);

FEHServo vertical_servo(FEHServo::Servo0); 

#define MOTORPOWER 35
#define PI 3.14159265359
#define SERVO_MAX 2200
#define SERVO_MIN 800


void milestone1(void);
void milestone2(void);
void milestone3(void);
void milestone4(void);
void setMotorSpeed(int leftSpeed, int rightSpeed);
void move(double inches);
void turn(double angle);
void powerMove(void);
void verticalServo(double degrees);

int main(void)
{

    //set the servos min and max values
    vertical_servo.SetMin(SERVO_MIN);
    vertical_servo.SetMax(SERVO_MAX);
    
    //initialize RCS for the proteus for lever and kill switch
    // RCS.InitializeTouchMenu("1020C6GIQ");
    float x,y;

    verticalServo(180);

    //Waits for the light to turn red and then calls milestone4();
    //0.570 or below 1 is red
    //1.28 or between 1 and 2 is blue
    //3.2 or above 3 is nothing
    while (CdS_cell.Value() > 0.75) {
    }    
    milestone4();


    return 0;
}



//Sets the left and right motor speed
void setMotorSpeed(int leftSpeed, int rightSpeed){
    right_motor.SetPercent(rightSpeed);
    left_motor.SetPercent(leftSpeed);
}

void powerMove(double inches, double speedOfLeftMotor, double speedOfRightMotor) {
    //Convert inches to counts
    int counts=(abs(inches) * 318)/(3.0 * PI);

    //Reset encodor counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    float rightSpeed = (11.5 / Battery.Voltage()) * speedOfRightMotor;
    float leftSpeed = (11.5 / Battery.Voltage()) * speedOfLeftMotor;


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
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts){
        Sleep(0.2);
    }


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
    int counts=(distance * 306)/(3.0 * PI);

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    float actualSpeed = (11.5 / Battery.Voltage()) * MOTORPOWER;

    if(angle > 0){
        setMotorSpeed(actualSpeed, -actualSpeed); //turn right
    }else{
        setMotorSpeed(-actualSpeed, actualSpeed); //turn left
    }

     //Motors run while the average of the left and right encoder is less than counts
    while((left_encoder.Counts() + right_encoder.Counts()) / 2.0 < counts){}

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
    Sleep(0.1);
}

/*
This is a function that would take an input of degrees to turn

    @var degrees
            degrees that the servo will turn. Can be negative or positive depending on which way user wants it to turn.
            Degrees must be between the minimum and maximum degree.
*/
void verticalServo(double degrees) {
    //Set arm servo to 0 degrees
    vertical_servo.SetDegree(degrees);
    Sleep(.1);
}

/*
    Function to allow robot to line follow with a given amount of inches.

        @var inches
            amount of inches that the robot will move for the line following
*/
void lineFollowMove(double inches) {

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


    turn(-90); 
    move(6.4); //move towards ramp
    turn(-34);
    move(31); //up the ramp
    turn(-87); //turn towards humidifier buttons

    move(9.5);

    bool forward = false;
    int countHumidifierButton = 0;
    while (fabs(CdS_cell.Value() - 2.1) > 0.1 && fabs(CdS_cell.Value() - 1.4) > 0.3 && countHumidifierButton < 5) { 
        move(0.1);
        countHumidifierButton++;
    }

    // red or blue
    LCD.WriteLine(CdS_cell.Value());
    if(CdS_cell.Value() < 1.6) {
        //red
        LCD.Clear(RED);
        turn(87); //turn 
        move(2);
        turn(87); //turn 
        move(-12.6); //press the humidifier button
    } else {
        //blue
        LCD.Clear(BLUE);
        turn(-87); //turn backwards
        move(3);  
        turn(-87); //turn  
        move(-12.5); //press the humidifier button
 
    }
    move(24);
    turn(90);
    move(45);


}

void milestone3(void){
    move(-24); //move backwards toward window
    turn(-45); //turn so that robot is parallel to window
    move(-6);
    turn(-7);
    powerMove(12, 50, 40); //opens the window
    
    move(-5); //move backward 
    turn(45); // turn back towards start
    move(20); //move closer to start
    turn(-40); //turn towards wall
    move(10); //move close to the wall but not touching it so there's room for turning
    turn(-90); //turn towards the ramp
    powerMove(25, 50, 50); //move up the ramp 
    turn(-85); //turn towards the handle on the window
    move(15); //move towards the handle, closing it
}

    /*
    * removes bucket of apples from stump
    * deposits apples on crate or table
    * flips any lever to the down position
    * returns that lever back to up position
    * BONUS 1: flips the correct lever to the down position
    * BONUS 2: robot returns the correct lever back to up position after 5 seconds
    */
    void milestone4(void) {
        int lever = RCS.GetLever();

        turn(9);
        move(19); //move towards apples
        turn(-49); //turn towards apples
        verticalServo(65); //move servo to align with apple holder
        Sleep(.5);
        move(4.5); //move to pick up apples
        move(-3.6); //move backwards away from apples
   
        turn(40); //turn to face window
        move(-4);
        turn(45);
        move(-4); //move slightly backward
        turn(-105); //turn to face wall
        move(-16); //move towards wall
        move(1.2);
        turn(92); //turn to face ramp

        powerMove(33, MOTORPOWER + 1, MOTORPOWER); //move up the ramp
        turn(-90); //turn to left to go around table
        move(3); //move towards window
        turn(55); //turn towards levers
        move(6); //move towards levers

        verticalServo(2);
        Sleep(.2);
        move(3);
        verticalServo(160);
        Sleep(5.5);
        move(-3);
        verticalServo(180);
        Sleep(.2);
        move(4);
        verticalServo(2);

        

        //0 left; 1 middle; 2 right
        if (lever == 0) {

        }
        else if(lever == 1) {

        }
        else {

        }
    }