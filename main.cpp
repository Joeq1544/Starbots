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

bool skipStart;

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
FEHMotor hor_servo(FEHMotor::Motor2,5.0);   //hacked servo


#define MOTORPOWER 35
#define PI 3.14159265359
#define VERTICAL_SERVO_MAX 2200
#define VERTICAL_SERVO_MIN 815
#define HORIZONTAL_SERVO_MAX 2200
#define HORIZONTAL_SERVO_MIN 815
#define left_threshold 1.9
#define middle_threshold 1.9
#define right_threshold 1.9

enum DriveState{
    GOMIDDLE,
    GOLEFT,
    GORIGHT
};


void milestone1(void);
void milestone2(void);
void milestone3(void);
void milestone4(void);
void milestone5(void);
void setMotorSpeed(int leftSpeed, int rightSpeed);
void move(double inches);
void turn(double angle);
void powerMove(void);
void verticalServo(double degrees);
void horizontalServo(double seconds);
void finalStart(void);
void compostBin(void);
void openWindow(void);
void apples(void);
void closeWindow(void);
void buttons(void);
void levers(void);


//isWindowOpen()
/* isLeverFlipped() fertilizer lever: 1 if any lever is pressed down, 0 otherwise */

int main(void)
{
    skipStart = false;
    //set the servos min and max values
    vertical_servo.SetMin(VERTICAL_SERVO_MIN);
    vertical_servo.SetMax(VERTICAL_SERVO_MAX);
    //sets the vertical servo to 180 which is all the way down. 
    verticalServo(180);

    //initialize RCS for the proteus for lever and kill switch
    RCS.InitializeTouchMenu("1020C6GIQ");
    float x,y;

    LCD.Clear(BLACK);
    LCD.WriteLine("Waiting for touch");
    while(!LCD.Touch(&x, &y)) {
        LCD.WriteLine(CdS_cell.Value());
        Sleep(0.1);
    }
    LCD.Clear(BLACK);
    LCD.WriteLine("TOUCHED");
    double tNow = TimeNow();
    
    while (CdS_cell.Value() > 1.00 && TimeNow() - tNow  < 30.0) {
        LCD.WriteLine(CdS_cell.Value());
        Sleep(0.1);
        if (TimeNow() - tNow > 29.0) {
            skipStart = true;
        }
    }
    finalStart();

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
        // LCD.WriteLine(left_encoder.Counts());
        // LCD.WriteLine(right_encoder.Counts());
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
This is a function that would take an input of degrees to turn

    @var degrees
            degrees that the servo will turn. Can be negative or positive depending on which way user wants it to turn.
            Degrees must be between the minimum and maximum degree.
*/
void horizontalServo(double seconds) {
    //Set arm servo to 0 degrees
    if (seconds < 0) {
        hor_servo.SetPercent(75);
    } else {
        hor_servo.SetPercent(-75);
    }
    Sleep(abs(seconds));
    hor_servo.SetPercent(0);
}

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
        switch (lever)
        {
        case 0:
            /* code */
            break;
        case 1:
            /* code */
        break;
        case 2:
            /* code */
        break;
        default:
            break;
        }
    }

    void milestone5(void) {   
        move(1.5); //move forward from start
        turn(-70); //turn closer to the wall
        move(6.5); //move and drive into wall
        turn(20); //turn a little bit to align with the compose bit turner thing
        move(1.5); //move into it
        horizontalServo(165); //turn the compost bin
        Sleep(0.5); //sleep
        move(-2); //move back to reset the servo
        turn(-1.2);
        Sleep(0.5); //sleep
        horizontalServo(30); //turn servo back
        Sleep(0.5); //sleep
        move(2); //move forward
        horizontalServo(180); //turn compose bin again

        Sleep(0.5); //sleep
        move(-2); //move backward
        turn(-1);
        Sleep(0.5); //sleep
        horizontalServo(50); //turn back
        Sleep(0.5); //sleep
        move(2); //move forward
        horizontalServo(180); //turn compost bin final time
        Sleep(0.5);

        horizontalServo(10); //turn compost bin 
        Sleep(0.6);
        move(-2); //move backward
        turn(-1);
        horizontalServo(160); //turn back
        Sleep(0.5); //sleep
        move(2); //move forward
        horizontalServo(0); //turn compost bin 
        Sleep(0.6);

        move(-2); //move backward
        Sleep(0.5); //sleep
        turn(-1);
        horizontalServo(163); //turn back
        Sleep(0.5); //sleep
        move(2); //move forward
        horizontalServo(0); //turn compost bin 
        Sleep(0.6);

        hor_servo.Stop();
        move(-2);
        turn(7);
        move(-15);
    }

    /* 
        OFFICIAL CODE FOR FINAL ROBOT
    */

    void finalStart(void) {
        //hit start button

        if (!skipStart) {
            move(1.5);
            move(-2.5);
            move(2);
        }
        else {
            move(1);
        }

        /* Start with tasks */
        compostBin();
        LCD.Clear(BLACK);

        move(-1.5); 
        turn(90);
        move(-4);
        move(16);
        turn(-30);
        openWindow();
        LCD.Clear(BLACK);

        turn(-90);
        move(5.25);
        turn(100);
        move(-1);

        //once opened window, write code that goes to apples
        apples();
        LCD.Clear(BLACK);
        turn(-105);
        move(-7);
        verticalServo(0);
        move(7);
        moveLF(8);

        //write code to move towards closing window
        /*TODO after rest of tasks are done*/
        // closeWindow();
        LCD.Clear(BLACK);

        //write code that moves towards the buttons
        buttons();
        LCD.Clear(BLACK);


        levers();
        LCD.Clear(BLACK);


        turn(-45);
        move(-10);
        turn(-25);
        move(-12);
        move(1.5);
        turn(98);
        move(-23);
        turn(-10);
        move(-50);
    }

    void compostBin(void){
        LCD.Clear(BLACK);
        LCD.WriteLine("Compost Bin");

        verticalServo(0);
        turn(-80); //turn closer to the wall
        move(6); //move and drive into wall
        turn(21.5); //turn a little bit to align with the compose bit turner thing
        move(2.75); //move into it
        horizontalServo(2.25); //turn the compost bin
        Sleep(0.5); //sleep
        horizontalServo(-2.25);
    }

    void openWindow(void) {
        LCD.Clear(BLACK);
        LCD.WriteLine("Open Window");

        move(12);
        turn(-25);
        move(5);
        turn(-15);
        powerMove(-3, MOTORPOWER + 5, MOTORPOWER);
        powerMove(-2, MOTORPOWER + 5, 50);
        int count = 0;
        while(RCS.isWindowOpen() == 0) {
            count++;
            powerMove(-1, 60 + 8, 60);
            if(RCS.isWindowOpen() == 1) {
                break;
            }
            if (count % 3 == 0) {
                move(0.5);
                turn(-1);
            }
            if (count % 9 == 0) {
                turn(4);
            }
        }
        turn(5);
    }

    void apples(void) {
        LCD.Clear(BLACK);
        LCD.WriteLine("apples");

        verticalServo(109); //vertical servo up
        Sleep(0.5);
        move(4); //move to pick up apples
        verticalServo(20); //lift apples up
        Sleep(0.5);
        powerMove(-5.5, MOTORPOWER+8, MOTORPOWER); //move backwards away from apples
        turn(86); //turn to face the compost bin wall
        powerMove(-7, MOTORPOWER+8, MOTORPOWER - 2); //move slightly backward
        turn(-110); //turn to face the ramp wall
        powerMove(-25, MOTORPOWER+6, MOTORPOWER - 2); //move backwards to hit wall
        move(2.5);
        turn(102);
        powerMove(29, MOTORPOWER + 5, MOTORPOWER);
        verticalServo(0);
        Sleep(0.5);
        turn(30);
        move(4);
        verticalServo(75);
        Sleep(0.4);

    }

    void closeWindow(void) {
        LCD.Clear(BLACK);
        LCD.WriteLine("Closing Window");

    //int checkWindow = RCS.isWindowOpen();
    // 0 – Window is closed
    // 1 – Window is open



    }

    void buttons(void) {
        LCD.Clear(BLACK);
        LCD.WriteLine("buttons");

        bool forward = false;
        int countHumidifierButton = 0;
        while (fabs(CdS_cell.Value() - 0.75) > 0.1 && fabs(CdS_cell.Value() - 1.4) > 0.3 && countHumidifierButton < 8) { 
            move(0.1);
            countHumidifierButton++;
        }

        verticalServo(180);
        LCD.WriteLine(CdS_cell.Value());
        if(CdS_cell.Value() < 1) {
            //red
            LCD.Clear(RED);
            turn(87); //turn 
            move(4);
            turn(87); //turn 
            move(-12.6); //press the humidifier button
            move(2);
            move(-2);
            move(2);
            turn(-120);
            move(3);
        } else {
            //blue
            LCD.Clear(BLUE);
            turn(95); //turn backwards
            move(-4);  
            turn(87); //turn  
            move(-12.5); //press the humidifier button
            move(2);
            move(-2);
            move(7);
            turn(-120);
        }
    }
    
    void levers(void) {
        LCD.Clear(BLACK);
        LCD.WriteLine("levers");

        // bool checkLever = RCS.isLeverFlipped();
        // 0 – Initial state OR some lever has been flipped back up
        // 1 – At least one lever has been flipped down AND no lever has ever been flipped back up

        verticalServo(0);
        Sleep(0.6);
        move(5);
        verticalServo(150);
        Sleep(0.55);
        move(-5);
        verticalServo(145);
        turn(3);
        Sleep(5.0);
        move(5.5);
        verticalServo(80);
        Sleep(0.6);
        move(-5);

        verticalServo(0);

        int lever = RCS.GetLever();

        //0 left; 1 middle; 2 right
        switch (lever) {
        case 0:
            /* code */
            break;
        case 1:
            /* code */
        break;
        case 2:
            /* code */
        break;
        default:
            break;
        }
    }
