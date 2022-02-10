/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       AlexplaysVR                                               */
/*    Created:      Thu Nov 18 2021                                           */
/*    Description: Program for Vex Team 9623D                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    20, 19, 11, 12, 16
// ArmMotors            motor_group   13, 18          
// Controller1          controller                    
// ClawPiston           digital_out   A               
// LiftMotors           motor_group   7, 8            
// ---- END VEXCODE CONFIGURED DEVICES ----
//leftMotorA = Front Left Motor
//leftMotorB = Rear Left Motor
//rightMotorA = Front Right Motor
//rightMotorB = Rear Right Motor
#include "vex.h"
#include "easer.h"

using namespace vex;

// A global instance of competition
competition Competition;

extern motor leftMotorA;
extern motor leftMotorB;
extern motor rightMotorA;
extern motor rightMotorB;


// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  LiftMotors.setStopping(hold);
  ArmMotors.resetPosition();
  ArmMotors.resetRotation();
  LiftMotors.resetRotation();
  LiftMotors.resetPosition();
  

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
void drivestraight(int distance, int speed, bool waitForCompletion=true){
  Drivetrain.setDriveVelocity(speed, percent);
  Drivetrain.driveFor(forward, distance, inches, waitForCompletion);
}
void Claw(char action){
  if(action == 'O'){
    ClawPiston.set(false);
  }
  else if(action == 'C'){
    ClawPiston.set(true);
  }
}
void Arm(char action){
  if(action == 'U'){
    ArmMotors.spinFor(forward, 5.0, sec);
  }
  else if(action == 'D'){
    ArmMotors.spinFor(reverse, 5.0, sec);
  }
}
void Lift(char action, int time){
  if(action == 'U'){
    LiftMotors.spinFor(forward, time, sec);
    }
  else if(action == 'D') {
    LiftMotors.spinFor(reverse, time, sec);
  }
}

void turn(char direction, int angle){
  if(direction == 'L'){
    int leftangle = angle * -1;
    Drivetrain.turnFor(leftangle, vex::degrees);
  }
  else if(direction == 'R'){
    Drivetrain.turnFor(angle, vex::degrees);
  }
}
void autonomous(void) {
Claw('O');
while(true) {
double motorTurns = leftMotorA.rotation(turns) * 50;
double completionFactor = (motorTurns/50);
double easingFactor = getEasingFactor(EASE_IN_OUT_CUBIC, completionFactor);
Drivetrain.drive(forward, (100 * easingFactor), velocityUnits::pct);
if(completionFactor >= 1){break;};
}
drivestraight(5, 50);
Claw('C');
Lift('D', 1);
drivestraight(-22, 75);
}
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
      int turn = Controller1.Axis1.position();
		  int power = Controller1.Axis3.position();
		  int strafe = Controller1.Axis4.position();
			//Maths
		  int fl = power + turn - strafe;
		  int rl = power + turn + strafe;
		  int fr = power - turn + strafe;
		  int rr = power - turn - strafe;
      //Motor Control
      leftMotorA.spin(forward, fl, vex::percent);
      leftMotorB.spin(forward, rl, vex::percent);
      rightMotorA.spin(forward, fr, vex::percent);
      rightMotorB.spin(forward, rr, vex::percent);

      //Claw Control
      if(Controller1.ButtonA.pressing()){
        ClawPiston.set(true);
      }
      else if(Controller1.ButtonB.pressing()){
        ClawPiston.set(false);
      }
      //Arm Control
      if(Controller1.ButtonL1.pressing()){
        ArmMotors.spin(forward, 60, percent);
      }
      else if(Controller1.ButtonL2.pressing()){
        ArmMotors.spin(reverse, 60, percent);
      }
      else {
        ArmMotors.stop();
      }
      //Lift Control
      if(Controller1.ButtonR1.pressing()){
        LiftMotors.spin(forward, 30, percent);
      }
      else if(Controller1.ButtonR2.pressing()){
        LiftMotors.spin(reverse, 30, percent);
      }
      else {
        LiftMotors.stop();
      }
      LiftMotors.setStopping(hold);
      ArmMotors.setStopping(hold);
     

    wait(10, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
