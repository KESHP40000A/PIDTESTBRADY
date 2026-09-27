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
bool over500 = false;;
bool under40 = false;

void holdArm(){
    pros::Task brainTask([&]() {
        while(true){
                 while(holdMode){
                    lift.move(-15);
                    //Wrist.move(10);
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
    percentToRotation( .05, 120);
    pros::delay(100);
    moveWristPose(-670);
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
    percentToRotation(adjPercent + .03, 120);
    if(over500){
        moveWristPose(-810);
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
    }
    else {
        moveWristPose(-670);
    }
    
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
    } else if (under40){
        percent = .20;
        move = Lmax * percent;
        while((-LSensor.get_position()/100) < Lmax){
            lift.move(volts);
            pros::delay(5);
        }
    } else if(over500) {
        
            while((-LSensor.get_position()/100) < move){
            lift.move(volts);
            pros::delay(10);

        }
         pros::delay(200);
    }
    else {
    
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
    
    while(Wrist.get_raw_position(0) > pos){
        Wrist.move(-127);
        pros::delay(10);
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
    

    // lift.move(-127);
    // Wrist.move(-127);
    // pros::delay(1000);
    // lift.move(0);
    // Wrist.move(0);
    // Wrist.set_zero_position(Wrist.get_position());
    // LSensor.reset_position();
    // holdMode = true;

}


void score(){
    holdMode = false;
    Claw.move(127);
    readStack();
    pros::delay(50);
    if(stackPos != 0){
    double adjPercent = stackPos/Lmax * 1;
    percentToRotation(adjPercent + .15, 120);
    if(over500){
        moveWristPose(-810);
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
    }
    else {
        moveWristPose(-670);
    }
    
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
} else {
    percentToRotation( .03, 120);
    moveWristPose(-670);
    pros::delay(50);
    stackPos = 100;
    // percentToRotation( .01, 120);
    while((-LSensor.get_position()/100) > (.27 * Lmax)){
        lift.move(-127);
        pros::delay(5);
    }
    lift.move(0);
    pros::delay(30);
    Claw.move(-127);
    pros::delay(150);
    //percentToRotation(.35, 120);
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
    if(over500){
        //Intake.move(127);
    }
    under40 = stackPos < 45;

    //pros::delay(200);

    lift.move(0);
    //Intake.move(127);

    

    
}




