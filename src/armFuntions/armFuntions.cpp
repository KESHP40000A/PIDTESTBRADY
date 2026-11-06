#include "main.h"
#include "pros/misc.h"
#include <cstdlib>
#include <string>
#include <cmath>

#include "setup/config.h"
#include "functions/colorSort.h"
#include "functions/lookup.h"
#include "armFunctions/armFunctions.h"

double Lmax = 966;
double Wmax = 360;

double min;

double stackPos;

bool holdMode = true;
bool over500 = false;
bool under40 = false;

int wristRegression(double stackPosition){
    return static_cast<int>(std::lround(-0.2185 * stackPosition - 640.0));
}

void holdArm(){
    pros::Task brainTask([&]() {
        while(true){
            while(holdMode){
                lift.move(-15);
                Wrist.move(10);
                pros::delay(5);
            }
            pros::delay(5);
        }
    });
}

void percentToZero(){
    while((-LSensor.get_position()/100) > 0){
        lift.move(-127);
        pros::delay(10);
    }
}

void ScoreToZero(){
    readStack();
    if(stackPos == 0){
        holdMode = false;
        Claw.move(127);
        stackPos = 0;
        percentToRotation(.05, 120);
        pros::delay(100);
        moveWristPose(-645);
        pros::delay(50);
        stackPos = 0;
        percentToZero();
        pros::delay(30);
        Claw.move(-127);
        pros::delay(100);
        percentToRotation(.1 + .028, 100);
        Claw.move(0);
        pros::delay(30);
        moveWristPose(-1650);
    } else {
        double adjPercent = stackPos/Lmax * 1;
        if(under40){
            percentToRotation(adjPercent + .23, 120);
        } else {
            percentToRotation(adjPercent + .03, 120);
        }

        if(over500){
            moveWristPose(-820);
        }
        else if(stackPos > 620){
            moveWristPose(-790);
        }
        else if(stackPos > 550){
            moveWristPose(-780);
        } else if(stackPos > 475){
            moveWristPose(-740);
        }
        else if(stackPos > 370){
            moveWristPose(-720);
        } else if(stackPos > 300){
            moveWristPose(-700);
        } else if(under40){
            moveWristPose(-640);
        }
        else {
            moveWristPose(-670);
        }
        //moveWristPose(wristRegression(stackPos));

        stackPos = -LSensor.get_position()/100;

        adjPercent = stackPos/Lmax * 1;
        pros::delay(50);

        if(over500){
            while((-LSensor.get_position()/100) > (.931 * Lmax)){
                lift.move(-50);
                pros::delay(5);
            }
        } else{
            percentToRotation(adjPercent - .028, 120);
        }
        pros::delay(30);
        Claw.move(-127);
        pros::delay(100);
        percentToRotation(adjPercent + .028, 120);
        Claw.move(0);
        pros::delay(30);
        moveWristPose(-1650);
    }
}

void percentToRotation(double percent, int volts){
    holdMode = false;

    double move = Lmax * percent;
    if(stackPos == 0){
        percent = .20;
        move = Lmax * percent;

        while((-LSensor.get_position()/100) < move){
            lift.move(volts);
            pros::delay(5);
        }
    } else if(over500) {
        while((-LSensor.get_position()/100) < move){
            lift.move(volts);
            pros::delay(10);
        }
        pros::delay(200);
    } else {
        if((-LSensor.get_position()/100) < move){
            while((-LSensor.get_position()/100) < move){
                lift.move(volts);
                pros::delay(10);
            }
        } else if((-LSensor.get_position()/100) > move){
            while((-LSensor.get_position()/100) > move){
                lift.move(-volts);
                pros::delay(10);
            }
        }
    }

    lift.move(0);
    pros::delay(10);
}

void moveWristPose(int pos){
    holdMode = false;
    if(Wrist.get_raw_position(0) > pos){
    while(Wrist.get_raw_position(0) > pos){
        Wrist.move(-127);
        pros::delay(10);
    }
} else {
    while(Wrist.get_raw_position(0) < pos){
        Wrist.move(127);
        pros::delay(10);
    }
}
    Wrist.move(0);
}

void reset(){
    while((-LSensor.get_position()/100) > 0){
        lift.move(-127);
        pros::delay(10);
    }
    lift.move(0);
    while(Wrist.get_raw_position(0) < -20){
        Wrist.move(127);
        pros::delay(10);
    }
    Wrist.move(0);
    LSensor.reset_position();
    Wrist.set_zero_position(Wrist.get_raw_position(0));
    holdMode = true;
    pros::delay(10);
}

void score(){
    holdMode = false;
    Claw.move(127);
    readStack();
    pros::delay(50);
    if(stackPos != 0){
        double adjPercent = stackPos/Lmax * 1;
        if(stackPos > 620){
            percentToRotation(adjPercent + .22, 120);
        } else if(under40){
            percentToRotation(adjPercent + .23, 120);
        }
        else {
            percentToRotation(adjPercent + .15, 120);
        }

        if(over500){
            moveWristPose(-810);
        }
        else if(stackPos > 620){
            moveWristPose(-800);
        }
        else if(stackPos > 550){
            moveWristPose(-780);
        } else if(stackPos > 475){
            moveWristPose(-740);
        }
        else if(stackPos > 370){
            moveWristPose(-720);
        } else if(stackPos > 300){
            moveWristPose(-700);
        }
        else {
            moveWristPose(-670);
        }
        //moveWristPose(wristRegression(stackPos));

        stackPos = -LSensor.get_position()/100;

        adjPercent = stackPos/Lmax * 1;
        pros::delay(50);

        if(over500){
            while((-LSensor.get_position()/100) > (.931 * Lmax)){
                lift.move(-50);
                pros::delay(5);
            }
        } else if(under40){
            percentToRotation(adjPercent - .05, 120);
        }
        else{
            percentToRotation(adjPercent - .028, 120);
        }
        pros::delay(30);
        Claw.move(-127);
        pros::delay(100);
        percentToRotation(adjPercent + .028, 120);
        Claw.move(0);
        pros::delay(30);
        moveWristPose(-1650);
    } else {
        percentToRotation(.03, 120);
        moveWristPose(-650);
        pros::delay(50);
        stackPos = 100;
        while((-LSensor.get_position()/100) > (.27 * Lmax)){
            lift.move(-127);
            pros::delay(5);
        }
        lift.move(0);
        pros::delay(30);
        Claw.move(-127);
        pros::delay(150);
        while((-LSensor.get_position()/100) < (.30 * Lmax)){
            lift.move(127);
            pros::delay(5);
        }
        lift.move(0);
        Claw.move(0);
        pros::delay(30);
        moveWristPose(-1650);
    }
    pros::delay(10);
}

void readStack(){
    holdMode = false;
    if(scoringDistance.get_distance() > 140){
        stackPos = 0;
    }
    while(scoringDistance.get_distance() < 140 && scoringDistance.get_confidence() > 5){
        lift.move(127);
        pros::delay(5);
        stackPos = -LSensor.get_position()/100;
    }
    over500 = stackPos > 650;
    under40 = stackPos < 80;

    lift.move(0);
}