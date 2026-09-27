
#include "main.h"
#include "lemlib/api.hpp" // IWYU 

#include "functions/lookup.h"
#include "setup/config.h"
#include "setup/autons.h"
#include "armFunctions/armFunctions.h"
#include "setup/distance.hpp"


double triangulateDist(Lidar distSensor, double wallAngle) {
    double angle = chassis.getPose(true).theta - (wallAngle - distSensor.angle)*std::numbers::pi/180;
    double angle1 = chassis.getPose(true).theta - (wallAngle - std::copysign(90, distSensor.yOffset))*std::numbers::pi/180;

    return std::abs(distSensor.get2()*cos(angle)) + std::abs(distSensor.dOffset*sin(distSensor.phi + angle1));
}

void red(){
chassis.setPose(64, 0, 270);
chassis.moveToPoint(59, 0, 400);
percentToRotation(150, 127);
delay(100);
reset();
holdArm();
chassis.moveToPoint(64,0,500, {.forwards = false, .minSpeed = 127});
chassis.moveToPoint(59,0, 400);
chassis.moveToPoint(64,0, 400, {.forwards = false, .minSpeed = 127});
Intake.move(-127);
Color_Roller.move(127);
Claw.move(127);
chassis.moveToPoint(28, 0, 1000, {}, false);
delay(700);
//chassis.moveToPoint(47, 0, 800, {.forwards = false});
//chassis.turnToHeading(0, 500);
chassis.moveToPoint(47, -21.5, 1400, {.forwards = false, .maxSpeed = 80}, false);
score();
//first pin has been scored
chassis.moveToPoint(36, -11, 1000);
Claw.move(127);
reset();
chassis.turnToHeading(250, 500);
chassis.moveToPoint(27, -14, 800);
Claw.move(127);

chassis.setPose(72 - triangulateDist(lidCenter, 90), chassis.getPose().y, chassis.getPose().theta);
chassis.turnToHeading(230, 500);
chassis.moveToPoint(46.5, 17.5, 1800, {.forwards = false, .maxSpeed = 90},false);
ScoreToZero();
chassis.moveToPoint(40, 12, 600, {}, false);
reset();
chassis.turnToHeading(135, 400);
percentToRotation(.30, 270);
moveWristPose(-670);
chassis.moveToPoint(31.5, 20.5, 1000, {.forwards = false, .maxSpeed = 100},false);
delay(1000);



Claw.move(127);


//percentToRotation(.1, 270);
delay(300);


}