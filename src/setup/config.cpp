
#include "lemlib/chassis/chassis.hpp"
#include "setup/config.h"
#include "lemlib/chassis/trackingWheel.hpp"
#include "lemlib/pid.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.hpp"
#include "pros/distance.hpp"
#include "pros/optical.hpp"
#include "pros/rotation.hpp"
#include "pros/misc.h"
#include <cstddef>
#include "setup/distance.hpp"


bool mode;
int ind;
pros::Mutex dataMutex;
std::string Object = "";
std::string Color1 = "";
std::string Color2 = "";
std::string Color3 = "";
std::string Color4 = "";

std::string flipd = "";

pros::Controller controller(pros::E_CONTROLLER_MASTER);
//Default drivetrain
pros::MotorGroup DL({2, 1}, pros::MotorGearset::blue); 
//done
pros::MotorGroup DR({-13, -12}, pros::MotorGearset::blue);

pros::MotorGroup lift({9, -8}, pros::MotorGearset::green);


pros::Optical optical1(3);
pros::Optical optical2(4);
pros::Optical optical3(19);

pros::Distance scoringDistance(5);

pros::Imu immu(11);


pros::Motor Intake(14);
pros::Motor Color_Roller(15);

pros::Motor Wrist(6);
pros::Motor Claw(7);

pros::Distance distanceRight(16);
pros::Distance distanceLeft(17);
pros::Distance distanceBack(18);

Lidar lidRight(16, 4.4, 0, 90);
Lidar lidLeft(17, -4.4, .5, -90);
Lidar lidCenter(18, -2.8, -1.6,180);

pros::Rotation LSensor(10);


//Gear ratio is Default 1 
// Default IMU


//Drivetrain - trackWidth still needs to be set + i think rpm is 480
lemlib::Drivetrain drivetrain(&DL, // left motor group
                              &DR, // right motor group
                              9.75, // 11.5 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 4" omnis
                              360, // drivetrain rpm is 450
                              2 // horizontal drift is 2 (for now)
);


//2 in

//2.5in

// lemlib::ControllerSettings lateral_controller(8, // proportional gain (kP)
//                                               0, // integral gain (kI)
//                                               22.35, // derivative gain (kD)
//                                               3, // anti windup
//                                               1, // small error range, in inches
//                                               100, // small error range timeout, in milliseconds
//                                               3, // large error range, in inches
//                                               1000, // large error range timeout, in milliseconds
//                                               15// maximum acceleration (slew)
// );

// lemlib::ControllerSettings angular_controller(3.9, // proportional gain (kP)
//                                               0, // integral gain (kI)
//                                               16.7, // derivative gain (kD)
//                                               3, // anti windup
//                                               1, // small error range, in degrees
//                                               100, // small error range timeout, in milliseconds
//                                               3, // large error range, in degrees
//                                               500, // large error range timeout, in milliseconds
//                                               0 // maximum acceleration (slew)
// );

// Need to tune 
lemlib::ControllerSettings lateral_controller(6, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              22, // derivative gain (kD)
                                              0, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              1000, // large error range timeout, in milliseconds
                                              20// maximum acceleration (slew)
);

lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              1, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);


//89.69
//89.16
//88.03

//93.7
//89.8
//94.18
//90.01
//92.98

//88.72
//89.81
//88.63
//92.13
//92.01
//89.73

//359.33
//2.87
//2.20

//3.3
//359.8
//4.21
//6.06

//2.31
//0.17

//1.95
//4.98
//4.1
// lemlib::ControllerSettings lateral_controller(8, // proportional gain (kP)
//                                               0, // integral gain (kI)
//                                               22.35, // derivative gain (kD)
//                                               3, // anti windup
//                                               1, // small error range, in inches
//                                               100, // small error range timeout, in milliseconds
//                                               3, // large error range, in inches
//                                               1000, // large error range timeout, in milliseconds
//                                               15// maximum acceleration (slew)
// );

// lemlib::ControllerSettings angular_controller(3.9, // proportional gain (kP)
//                                               0, // integral gain (kI)
//                                               16.7, // derivative gain (kD)
//                                               3, // anti windup
//                                               1, // small error range, in degrees
//                                               100, // small error range timeout, in milliseconds
//                                               3, // large error range, in degrees
//                                               500, // large error range timeout, in milliseconds
//                                               0 // maximum acceleration (slew)
// );

lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            nullptr, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &immu // inertial sensor
);

lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller,
						sensors // angular PID settings
                        
);

