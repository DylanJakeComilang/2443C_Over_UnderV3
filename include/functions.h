#include "vex.h"

double getArmEnc() { return Arm1.position(rotationUnits::raw); }
void resetArmEnc() { Arm1.setPosition(0, rotationUnits::raw); }

///////////////////////////
//   TELE-OP FUNCTIONS   //
///////////////////////////

///////////////////
// FLYWHEEL CODE //////////////////////////////////////////////////////////////////////
///////////////////

bool isFly = false;
int flySpeed = 50;

void fling(){
if(Controller1.ButtonA.pressing()){ //toggle Flywheel
      if(!isFly){
        isFly = true;
      }
      else if(isFly){
        isFly = false;
      }
    }

    if(isFly){
      if(Controller1.ButtonUp.pressing() && flySpeed < 80){ // increase Flywheel speed by 10 pct
        flySpeed += 10;
      }
      else if(Controller1.ButtonDown.pressing() && flySpeed > 50){ // decrease Flywheel speed by 10 pct
        flySpeed -= 10;
      }
      FlyWheel.spin(forward, flySpeed, pct);
    }
    else{
      FlyWheel.stop();
    }
}
/////////////////
// INTAKE CODE ///////////////////////////////////////////////////////////////////////
/////////////////

bool iTake = false;
bool oTake = false;

void toggleIntake(){
if(Controller1.ButtonR1.pressing()){ // toggle Outtake
      if(!oTake){
        oTake = true;
        iTake = false;
      }
      else if(oTake){
        oTake = false;
      }
    }

    if(Controller1.ButtonR2.pressing()){ // toggle Intake
      if(!iTake){
        iTake = true;
        oTake = false;
      }
      else if(iTake){
        iTake = false;
      }
    }

    if(iTake){
      Intake.spin(forward);
    }
    else if(oTake){
      Intake.spin(reverse);
    }
    else{
      Intake.stop();
    }
}
////////////////////
// PNEUMATIC CODE /////////////////////////////////////////////////////////////////////
////////////////////
void toggleFlaps(){
  if(!Flaps.value()){
    Flaps.set(true);
  }
  else if(Flaps.value()){
    Flaps.set(false);
  }
}

void toggleRatchet(){
  if(!Ratchet.value()){
    Flaps.set(true);
  }
  else if(Ratchet.value()){
    Flaps.set(false);
  }
}
//////////////
// ARM CODE ///////////////////////////////////////////////////////////////////////////
//////////////

void moveArm(){
  if(Controller1.ButtonL1.pressing() && getArmEnc() < 180){
    Arm1.spin(forward);
    Arm2.spin(forward);
  } else if (Controller1.ButtonL2.pressing() && getArmEnc() > 0){
    Arm1.spin(reverse);
    Arm2.spin(reverse);
  }else{
    Arm1.stop();
    Arm2.stop();
  }
}

//////////////////////
//   MEASUREMENTS   /////////////////////////////////////////////////////////////////////
//////////////////////

void stats(){
  Controller1.Screen.clearLine();
  Controller1.Screen.setCursor(0, 1);
  Controller1.Screen.print(flySpeed);
  Controller1.Screen.setCursor(0, 2);
  Controller1.Screen.print(Drivetrain.heading());
  Controller1.Screen.setCursor(0, 3);
  Controller1.Screen.print(Flaps.value());
}

//////////////////////////////
//   AUTONOMOUS FUNCTIONS   ///////////////////////////////////////////////////////////
//////////////////////////////

void setArm(double kp, double setpoint){
  resetArmEnc();
  double speed = 0;
  if(getArmEnc() < setpoint){
    while(getArmEnc() < setpoint){
      speed = setpoint - getArmEnc();
      speed *= kp;
      Arm1.spin(forward, speed, pct);
      Arm2.spin(forward, speed, pct);
  }
  } else{
    while(getArmEnc() > setpoint){
      speed = setpoint - getArmEnc();
      speed *= kp;
      Arm1.spin(forward, speed, pct);
      Arm2.spin(forward, speed, pct);
    }
  }
  Arm1.stop();
  Arm2.stop();
}

void autoFling(double speed ,double tim3){
  FlyWheel.spin(forward, speed, pct);
  wait(tim3-3,sec);
  Controller1.rumble(".--");
  wait(3, sec);
  FlyWheel.stop();
}

void driveForTime(double botSpeed, double tim3){  
  Drivetrain.setDriveVelocity(botSpeed, pct);  
  Drivetrain.drive(forward);  
  wait(tim3,sec);  
  Drivetrain.stop();
}

void intakingForward(double botSpeed, double dist){
  Intake.spin(forward, 80, pct);
  Drivetrain.setDriveVelocity(botSpeed, pct);
  Drivetrain.driveFor(forward, dist, inches);
  Intake.stop();
}

void intakingForTime(double botSpeed, double point){
  Intake.spin(forward, 80, pct);
  Drivetrain.setDriveVelocity(botSpeed, pct);
  Drivetrain.drive(forward);
  wait(point, sec);
  Intake.stop();
  Drivetrain.stop();
}

/////////////////////
//   AUTO SKILLS   //
/////////////////////
