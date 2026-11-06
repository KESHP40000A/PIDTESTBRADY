#pragma once

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/adi.hpp"
#include "pros/device.hpp"
#include "pros/optical.hpp"
#include "pros/rotation.hpp"
#include "pros/apix.h"
#include "distance.hpp"


extern bool scoreMode;
extern bool mode;
extern int ind;
extern pros::Mutex dataMutex;
extern std::string flipd;
extern std::string Object;
extern std::string Color1;
extern std::string Color2;
extern std::string Color3;
extern std::string Color4;
extern pros::Controller controller;
extern pros::MotorGroup DL;
extern pros::MotorGroup DR;
extern pros::Optical optical1;
extern pros::Optical optical2;
extern pros::Optical optical3;
extern pros::Motor Color_Roller;
extern pros::Motor Intake;

extern pros::Motor Wrist;
extern pros::Motor Claw;

extern pros::Rotation LSensor;

extern pros::MotorGroup lift;

extern pros::Distance scoringDistance;
// lemlib constructors

extern lemlib::Drivetrain drivetrain;
extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;
extern lemlib::TrackingWheel vertical_tracking_wheel;
extern lemlib::TrackingWheel horizontal_tracking_wheel;
extern lemlib::Chassis chassis;

extern pros::Imu immu;

extern pros::Distance distanceRight;
extern pros::Distance distanceLeft;
extern pros::Distance distanceBack;

extern Lidar lidRight;
extern Lidar lidLeft;
extern Lidar lidCenter;
