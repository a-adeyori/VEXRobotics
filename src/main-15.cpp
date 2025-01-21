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

// Define lift motors (4 high-torque motors)
motor liftMotor1(PORT7, gearSetting::ratio36_1, false);
motor liftMotor2(PORT8, gearSetting::ratio36_1, true);
motor liftMotor3(PORT9, gearSetting::ratio36_1, false);
motor liftMotor4(PORT10, gearSetting::ratio36_1, true);

// Define claw motors (2 normal motors)
motor clawMotor1(PORT11, gearSetting::ratio18_1, false);
motor clawMotor2(PORT12, gearSetting::ratio18_1, true);

// Define clamp motor (1 high-speed motor)
motor clampMotor(PORT13, gearSetting::ratio6_1, false);

// Group motors into motor groups for easier control
motor_group leftMotors(leftMotor1, leftMotor2, leftMotor3);
motor_group rightMotors(rightMotor1, rightMotor2, rightMotor3);
motor_group liftMotors(liftMotor1, liftMotor2, liftMotor3, liftMotor4);
motor_group clawMotors(clawMotor1, clawMotor2);
motor_group clampMotorGroup(clampMotor);

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // Set all motors to brake mode for better control
  leftMotors.setStopping(brakeType::brake);
  rightMotors.setStopping(brakeType::brake);
  liftMotors.setStopping(brakeType::hold);
  clawMotors.setStopping(brakeType::hold);
  clampMotorGroup.setStopping(brakeType::hold);

  // Set max torque and velocity
  leftMotors.setMaxTorque(100, percent);
  rightMotors.setMaxTorque(100, percent);
  liftMotors.setMaxTorque(100, percent);
  clawMotors.setMaxTorque(100, percent);
  clampMotorGroup.setMaxTorque(100, percent);

  leftMotors.setVelocity(100, percent);
  rightMotors.setVelocity(100, percent);
  liftMotors.setVelocity(75, percent); // Slower for precision
  clawMotors.setVelocity(100, percent);
  clampMotorGroup.setVelocity(100, percent); // High-speed motor
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

  // Lift operation
  liftMotors.spin(forward, 75, percent);
  wait(2, seconds);
  liftMotors.stop();

  // Claw operation to grab an object
  clawMotors.spin(forward, 100, percent);
  wait(1, seconds);
  clawMotors.stop();

  // Clamp operation to grab an object
  clampMotorGroup.spin(forward, 100, percent);
  wait(1, seconds);
  clampMotorGroup.stop();

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

    // Lift control
    if (Controller1.ButtonL1.pressing()) {
      liftMotors.spin(forward, 75, percent);
    } else if (Controller1.ButtonL2.pressing()) {
      liftMotors.spin(reverse, 75, percent);
    } else {
      liftMotors.stop();
    }

    // Claw control
    if (Controller1.ButtonR1.pressing()) {
      clawMotors.spin(forward, 100, percent);
    } else if (Controller1.ButtonR2.pressing()) {
      clawMotors.spin(reverse, 100, percent);
    } else {
      clawMotors.stop();
    }

    // Clamp control
    if (Controller1.ButtonX.pressing()) {
      clampMotorGroup.spin(forward, 100, percent);
    } else if (Controller1.ButtonB.pressing()) {
      clampMotorGroup.spin(reverse, 100, percent);
    } else {
      clampMotorGroup.stop();
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
