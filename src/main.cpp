/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// Define drivetrain motors (6 motors, 3 left, 3 right)
motor leftMotor1(PORT1, gearSetting::ratio18_1, false);
motor leftMotor2(PORT2, gearSetting::ratio18_1, false);
motor leftMotor3(PORT3, gearSetting::ratio18_1, false);

motor rightMotor1(PORT4, gearSetting::ratio18_1, true);
motor rightMotor2(PORT5, gearSetting::ratio18_1, true);
motor rightMotor3(PORT6, gearSetting::ratio18_1, true);

// Define intake motors
motor intakeMotor1(PORT7, gearSetting::ratio18_1, false);
motor intakeMotor2(PORT8, gearSetting::ratio18_1, true);

// Define group motors: Lift (2 motors), Clamps (2 motors), Hook (1 motor)
motor liftMotor1(PORT9, gearSetting::ratio36_1, false);
motor liftMotor2(PORT10, gearSetting::ratio36_1, true);
motor clampMotor1(PORT11, gearSetting::ratio18_1, false);
motor clampMotor2(PORT12, gearSetting::ratio18_1, true);
motor hookMotor(PORT13, gearSetting::ratio6_1, false);

// Group motors into motor groups for easier control
motor_group leftMotors(leftMotor1, leftMotor2, leftMotor3);
motor_group rightMotors(rightMotor1, rightMotor2, rightMotor3);
motor_group intakeMotors(intakeMotor1, intakeMotor2);
motor_group liftMotors(liftMotor1, liftMotor2);
motor_group clampMotors(clampMotor1, clampMotor2);

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

   // Set all motors to brake mode for better control
  leftMotors.setStopping(brakeType::brake);
  rightMotors.setStopping(brakeType::brake);
  intakeMotors.setStopping(brakeType::hold);
  liftMotors.setStopping(brakeType::hold);
  clampMotors.setStopping(brakeType::hold);
  hookMotor.setStopping(brakeType::hold);

  // Set max torque and velocity
  leftMotors.setMaxTorque(100, percent);
  rightMotors.setMaxTorque(100, percent);
  intakeMotors.setMaxTorque(100, percent);
  liftMotors.setMaxTorque(100, percent);
  clampMotors.setMaxTorque(100, percent);
  hookMotor.setMaxTorque(100, percent);

  leftMotors.setVelocity(100, percent);
  rightMotors.setVelocity(100, percent);
  intakeMotors.setVelocity(100, percent);
  liftMotors.setVelocity(75, percent); // Slower for precision
  clampMotors.setVelocity(75, percent);
  hookMotor.setVelocity(50, percent); // Slowest for fine control
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

void autonomous(void) {
// Example autonomous routine
  // Move forward for 3 seconds
  leftMotors.spin(forward, 100, percent);
  rightMotors.spin(forward, 100, percent);
  wait(3, seconds);

  // Intake operation for 2 seconds
  intakeMotors.spin(forward, 100, percent);
  wait(2, seconds);
  intakeMotors.stop();

  // Lift operation to lift an object
  liftMotors.spin(forward, 75, percent);
  wait(2, seconds);
  liftMotors.stop();

  // Clamp operation to grab an object
  clampMotors.spin(forward, 50, percent);
  wait(1, seconds);
  clampMotors.stop();

  // Hook operation
  hookMotor.spin(forward, 25, percent);
  wait(1, seconds);
  hookMotor.stop();

  // Stop drivetrain
  leftMotors.stop();
  rightMotors.stop();
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
   controller Controller1;

  while (1) {
    // Drivetrain control: Tank drive
    int leftSpeed = Controller1.Axis3.position(); // Left joystick
    int rightSpeed = Controller1.Axis2.position(); // Right joystick
    leftMotors.spin(forward, leftSpeed, percent);
    rightMotors.spin(forward, rightSpeed, percent);

    // Intake control
    if (Controller1.ButtonR1.pressing()) {
      intakeMotors.spin(forward, 100, percent);
    } else if (Controller1.ButtonR2.pressing()) {
      intakeMotors.spin(reverse, 100, percent);
    } else {
      intakeMotors.stop();
    }

    // Lift control
    if (Controller1.ButtonL1.pressing()) {
      liftMotors.spin(forward, 75, percent);
    } else if (Controller1.ButtonL2.pressing()) {
      liftMotors.spin(reverse, 75, percent);
    } else {
      liftMotors.stop();
    }

    // Clamp control
    if (Controller1.ButtonX.pressing()) {
      clampMotors.spin(forward, 50, percent);
    } else if (Controller1.ButtonB.pressing()) {
      clampMotors.spin(reverse, 50, percent);
    } else {
      clampMotors.stop();
    }

    // Hook control
    if (Controller1.ButtonUp.pressing()) {
      hookMotor.spin(forward, 25, percent);
    } else if (Controller1.ButtonDown.pressing()) {
      hookMotor.spin(reverse, 25, percent);
    } else {
      hookMotor.stop();
    }

    wait(20, msec); // Prevent wasted resources
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



