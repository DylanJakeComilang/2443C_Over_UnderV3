using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern smartdrive Drivetrain;
extern motor FlyWheel;
extern motor Intake;
extern motor Arm2;
extern motor Arm1;
extern digital_out Flaps;
extern digital_out Ratchet;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );