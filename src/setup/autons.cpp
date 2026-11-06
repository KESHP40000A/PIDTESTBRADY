
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
stackPos = 100;
percentToRotation(.1 , 127);
delay(50);
reset();
holdArm();
chassis.moveToPoint(65,0,500, {.forwards = false, .minSpeed = 127});
chassis.moveToPoint(58,0, 400, {.minSpeed = 127});
chassis.moveToPoint(85,0, 1000, {.forwards = false,  .maxSpeed = 80, .minSpeed = 70,});
Intake.move(-127);
Color_Roller.move(127);
Claw.move(127);
chassis.moveToPoint(30, 0, 1000, {}, false);
delay(700);
//chassis.moveToPoint(47, 0, 800, {.forwards = false});
//chassis.turnToHeading(0, 500);
chassis.moveToPoint(47, -21.5, 1125, {.forwards = false, .maxSpeed = 80}, false);
score();
//first pin has been scored
chassis.moveToPoint(36, -11, 400); //just changed ts one unc
Claw.move(127);
pros::Task resetTask(reset, TASK_PRIORITY_DEFAULT + 1, TASK_STACK_DEPTH_DEFAULT, "resetTask");
chassis.turnToHeading(250, 400);//changed min Speed
chassis.moveToPoint(27, -15, 1400);//changed min speed !!!!!!!!!!
Claw.move(127);

chassis.setPose(72 - triangulateDist(lidCenter, 90), chassis.getPose().y, chassis.getPose().theta);
chassis.turnToHeading(230, 200);
chassis.moveToPoint(46, 17.5, 1900, {.forwards = false, .maxSpeed = 90},false);
delay(1000);
ScoreToZero();
pros::Task percentTask([]() { percentToRotation(.10, 127); moveWristPose(-850); }, TASK_PRIORITY_DEFAULT + 2, TASK_STACK_DEPTH_DEFAULT, "percentTask");

chassis.moveToPoint(40, 0, 1000, {}, false);
reset();
chassis.turnToHeading(295,500);
chassis.moveToPoint(22,23, 1500);


//reset();
// chassis.turnToHeading(135, 700, {.maxSpeed = 70});

// // percentToRotation(.30, 127);
// // moveWristPose(-850);
// //check here!!!!!!!!!!!1
// chassis.moveToPoint(30, 20, 900, {.forwards = false, .maxSpeed = 100},false);
// Claw.move(127);
// holdMode = false;
// while((-LSensor.get_position()/100) > 0){
//     lift.move(-127);
//     pros::delay(10);
// }
// lift.move(0);
// Intake.move(0);


// //percentToRotation(.1, 270);
// stackPos = 100;
// percentToRotation(.20,127);
// chassis.turnToHeading(245, 500);
// chassis.moveToPoint(42, 22, 900, {.forwards = false},false);
// //moveWristPose(-500);
// delay(100);
// holdMode = false;
// while((-LSensor.get_position()/100) > 0){
//     lift.move(-90);
//     pros::delay(10);
// }

// lift.move(0);
// Claw.move(-127);
// percentToRotation(.3,127);
//scoreMode = false;

}

void redTwo(){
    chassis.setPose(64, 0, 270);
chassis.moveToPoint(59, 0, 400);
stackPos = 100;
percentToRotation(.1 , 127);
delay(50);
reset();
holdArm();
chassis.moveToPoint(64,0,500, {.forwards = false, .minSpeed = 127});
chassis.moveToPoint(59,0, 400, {.minSpeed = 100});
chassis.moveToPoint(85,0, 600, {.forwards = false, .minSpeed = 100});
Intake.move(-127);
Color_Roller.move(127);
Claw.move(127);
chassis.moveToPoint(28, 0, 1000, {}, false);
delay(700);
//chassis.moveToPoint(47, 0, 800, {.forwards = false});
//chassis.turnToHeading(0, 500);
chassis.moveToPoint(47, -21.5, 1125, {.forwards = false, .maxSpeed = 80}, false);
score();
//first pin has been scored
chassis.moveToPoint(36, -11, 400); //just changed ts one unc
Claw.move(127);
pros::Task resetTask(reset, TASK_PRIORITY_DEFAULT + 1, TASK_STACK_DEPTH_DEFAULT, "resetTask");
chassis.turnToHeading(250, 400);//changed min Speed
chassis.moveToPoint(27, -15, 400);//changed min speed !!!!!!!!!!
Claw.move(127);

chassis.setPose(72 - triangulateDist(lidCenter, 90), chassis.getPose().y, chassis.getPose().theta);
chassis.turnToHeading(230, 200);
chassis.moveToPoint(46.5, 17.5, 1300, {.forwards = false, .maxSpeed = 90},false);
ScoreToZero();
pros::Task percentTask([]() { percentToRotation(.10, 127); moveWristPose(-850); }, TASK_PRIORITY_DEFAULT + 2, TASK_STACK_DEPTH_DEFAULT, "percentTask");

chassis.moveToPoint(40, 9, 600, {}, true);


//reset();

chassis.turnToHeading(135, 700, {.maxSpeed = 70});

// percentToRotation(.30, 127);
// moveWristPose(-850);
//check here!!!!!!!!!!!1
chassis.moveToPoint(30, 20, 900, {.forwards = false, .maxSpeed = 100},false);
Claw.move(127);
holdMode = false;
while((-LSensor.get_position()/100) > 0){
    lift.move(-127);
    pros::delay(10);
}
lift.move(0);
Intake.move(0);


//percentToRotation(.1, 270);
stackPos = 100;
percentToRotation(.20,127);
chassis.turnToHeading(245, 500);
chassis.moveToPoint(42, 22, 900, {.forwards = false},false);
//moveWristPose(-500);
delay(100);
holdMode = false;
while((-LSensor.get_position()/100) > 0){
    lift.move(-90);
    pros::delay(10);
}

lift.move(0);
Claw.move(-127);
percentToRotation(.3,127);
//scoreMode = false;
}

void skills(){
chassis.setPose(64, 0, 270);
chassis.moveToPoint(59, 0, 400);
percentToRotation(150, 127);
delay(100);
reset();
holdArm();
Intake.move(127);
chassis.moveToPoint(64,0,500, {.forwards = false, .minSpeed = 127});
chassis.moveToPoint(60,0, 400, {.minSpeed = 127});
chassis.moveToPoint(64,0, 400, {.forwards = false, .minSpeed = 127});

Color_Roller.move(127);
Claw.move(127);
chassis.moveToPoint(28, 0, 1000, {}, false);
delay(700);
//chassis.moveToPoint(47, 0, 800, {.forwards = false});
//chassis.turnToHeading(0, 500);
chassis.moveToPoint(47, -21.5, 1250, {.forwards = false, .maxSpeed = 80}, false);
ScoreToZero();
Intake.move(-127);
//first pin has been scored
chassis.moveToPoint(36, -7, 800); //just changed ts one unc
Claw.move(127);
reset();
holdArm();
chassis.turnToHeading(250, 400);//changed min Speed
Claw.move(127);
chassis.moveToPoint(25, -15, 3000);//changed min speed !!!!!!!!!!


chassis.setPose(72 - triangulateDist(lidCenter, 90), chassis.getPose().y, chassis.getPose().theta);
chassis.turnToHeading(230, 200);
chassis.moveToPoint(46.5, 17.5, 5000, {.forwards = false, .maxSpeed = 90},false);
ScoreToZero();
chassis.moveToPoint(39.5, 11, 600, {}, false);
//reset();
chassis.turnToHeading(155, 700, {.maxSpeed = 70});
percentToRotation(.30, 127);
moveWristPose(-850);
chassis.moveToPoint(32.2, 22, 900, {.forwards = false, .maxSpeed = 100},false);//original is 30.9 = x and 21.3 =y
Claw.move(127);
holdMode = false;
while((-LSensor.get_position()/100) > 0){
    lift.move(-127);
    pros::delay(10);
}
lift.move(0);



//percentToRotation(.1, 270);
delay(300);
percentToRotation(30,127);
chassis.turnToHeading(255, 500);
chassis.moveToPoint(46, 23, 1200, {.forwards = false,},false);
moveWristPose(-750);
delay(100);
holdMode = false;

while((-LSensor.get_position()/100) > (.11 * Lmax)){
    lift.move(-90);
    pros::delay(10);
}

lift.move(0);
Claw.move(-127);
chassis.setPose(72 - triangulateDist(lidCenter, 90), chassis.getPose().y, chassis.getPose().theta);

percentToRotation(.3,127);
delay(1000);
chassis.moveToPoint(22, 18, 1500);
chassis.turnToHeading(225, 700);
chassis.moveToPoint(43.7, 44.7, 2500, {.forwards = false, .maxSpeed = 70},false);
Claw.move(127);
moveWristPose(-800);
while((-LSensor.get_position()/100) > 0){
    lift.move(-127);
    pros::delay(10);
}
lift.move(0);
stackPos = 100;
percentToRotation(.4, 127);
chassis.turnToHeading(0, 1000, {}, false);
chassis.moveToPoint(48, 30, 1500, {.forwards = false}, false);
percentToRotation(.3, 127);
Claw.move(-127);
percentToRotation(.4, 127);

chassis.moveToPoint(48, 45, 1500,{},false);
reset();

// while((-LSensor.get_position()/100) > 0){
//     lift.move(-127);
//     pros::delay(10);
// }
chassis.moveToPoint(0,0,15000, {.forwards = false, .maxSpeed = 90});

}

// void red(){
//     chassis.setPose(64, 0, 270);
// chassis.moveToPoint(59, 0, 400);
// percentToRotation(150, 127);
// delay(100);
// reset();
// holdArm();
// chassis.moveToPoint(64,0,500, {.forwards = false, .minSpeed = 127});
// chassis.moveToPoint(59,0, 400, {.minSpeed = 127});
// chassis.moveToPoint(64,0, 400, {.forwards = false, .minSpeed = 127});
// Intake.move(-127);
// Color_Roller.move(127);
// Claw.move(127);
// chassis.moveToPoint(28, 0, 1000, {}, false);
// delay(700);
// //chassis.moveToPoint(47, 0, 800, {.forwards = false});
// //chassis.turnToHeading(0, 500);
// chassis.moveToPoint(47, -21.5, 1250, {.forwards = false, .maxSpeed = 80}, false);
// score();
// //first pin has been scored
// chassis.moveToPoint(36, -11, 300); //just changed ts one unc
// Claw.move(127);
// reset();
// chassis.turnToHeading(250, 400);//changed min Speed
// chassis.moveToPoint(27, -14, 400);//changed min speed !!!!!!!!!!
// Claw.move(127);

// chassis.setPose(72 - triangulateDist(lidCenter, 90), chassis.getPose().y, chassis.getPose().theta);
// chassis.turnToHeading(230, 200);
// chassis.moveToPoint(46.5, 17.5, 1600, {.forwards = false, .maxSpeed = 90},false);
// ScoreToZero();
// chassis.moveToPoint(40, 12, 600, {}, false);
// //reset();
// chassis.turnToHeading(135, 700, {.maxSpeed = 70});
// percentToRotation(.30, 127);
// moveWristPose(-850);
// chassis.moveToPoint(30.9, 21.3, 900, {.forwards = false, .maxSpeed = 100},false);
// Claw.move(127);
// holdMode = false;
// while((-LSensor.get_position()/100) > 0){
//     lift.move(-127);
//     pros::delay(10);
// }
// lift.move(0);



// //percentToRotation(.1, 270);
// delay(300);
// percentToRotation(30,127);
// chassis.turnToHeading(245, 500);
// chassis.moveToPoint(42, 22, 1000, {.forwards = false, .minSpeed = 127},false);
// //moveWristPose(-500);
// delay(100);
// holdMode = false;
// while((-LSensor.get_position()/100) > 0){
//     lift.move(-90);
//     pros::delay(10);
// }

// lift.move(0);
// Claw.move(-127);
// percentToRotation(.3,127);
// }