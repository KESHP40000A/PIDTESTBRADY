// Example program
#include "main.h"
#include "pros/ai_vision.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include <iostream>
#include <string>
#include <sys/_intsup.h>

#include "main.h"
#include "lemlib/api.hpp" // IWYU 

#include "functions/lookup.h"
#include "pros/motors.h"
#include "pros/optical.hpp"
#include "pros/rtos.hpp"
#include "setup/config.h"
#include "setup/autons.h"
#include "functions/colorSort.h"
#include "functions/macros.h"
#include "armFunctions/armFunctions.h"

using namespace pros;
using namespace std;
using namespace lemlib;
using namespace lcd;


bool runSillyIntake = true;
bool reverseSillyIntake = true;

int liftZero = (1000 - LSensor.get_position()/100);
int liftDiff = (1000 - LSensor.get_position()/100) - liftZero;

void on_center_button() {
	
}


void initialize() {
	holdArm();
	// Run the task and diagnostics now; sorting remains disabled until X/Y
	// chooses a lookup-table column.
	// colorSort();
	// startColorSortDisplay();
	Wrist.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	Wrist.set_zero_position(Wrist.get_position());
	

	//lcd::initialize();
	chassis.calibrate();
	

	

	optical1.set_integration_time(7);
	optical2.set_integration_time(7);
	optical3.set_integration_time(7);

	optical1.set_led_pwm(100);
	optical2.set_led_pwm(100);
	optical3.set_led_pwm(100);
	   
	LSensor.reset();
	LSensor.reset_position();
	
	
    // Task brainTask([&]() {
    //     while(true){


	// //int reallPosition = LSensor.get_position() - lZERO;;

	// 			//double percent = (-LSensor.get_position()/100) / 966;	


    //             pros::lcd::print(0, "Ang: %i", (LSensor.get_angle()/100));
    // 			pros::lcd::print(1, "Pos: %i", (-LSensor.get_position()/100));
	// 			pros::lcd::print(3, "Dis: %i", scoringDistance.get_distance());
	// 			//pros::lcd::print(4, "Percent: %.2f", ((LSensor.get_angle()/100)/966));
	// 			pros::lcd::print(5, "Wrist: %d", Wrist.get_raw_position(0));
	// 			pros::lcd::print(6, (scoringDistance.get_distance() < 100) ? "True" : "False");
	// 			pros::lcd::print(7, "SPOS %d", stackPos);
			
	// 			//pros::lcd::print(4, "Real pos: %d", reallPosition);
	// 			// pros::lcd::print(5, "Pos: %i", LSensor.get_position());

	// 			//1562
				
	// 			delay(50);

				
    //     }
    
    
    // });
	
	// percentToRotation(.5, 127);
	// delay(500);
	// moveWristPose(-1000);
	// delay(500);
	// reset();
	// delay(500);
	// percentToRotation(.30, 127);
	// delay(500);
	// moveWristPose(-2000);
	// delay(500);
	// reset();
	


	//percentToRotation(.5, 127);
}


void disabled() {}


void competition_initialize() {

}


void autonomous() {
	//red();
	red();
}


void opcontrol() {

	lift.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
	

	pros::Controller master(pros::E_CONTROLLER_MASTER);


	while (true) {
		
		int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX);
        // delay to save resources


		// if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
		// 	ind = 1;
		// 	runColor();
		// } 
		// if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
		// 	ind = 0;
		// 	runColor();
		// }



		// if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
		// 	lift.move(-127);
		// } 
		// else if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
		// 	lift.move(127);
		// } else if(master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_R1) || master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_R2)){
		// 	lift.move(0);
		// }
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
	    	//cancelColorSort();
	    	if(runSillyIntake){
	        	Intake.move(-127);
	        	Color_Roller.move(127);
	        	Claw.move(127);
	    	} else{
	        	Intake.move(0);
	        	Color_Roller.move(0);
	       	Claw.move(0);
	    	}
	    	runSillyIntake = !runSillyIntake;
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
			if(reverseSillyIntake){
				Intake.move(127);
			} else {
				Intake.move(0);
				Color_Roller.move(0);
			}
			reverseSillyIntake = !reverseSillyIntake;
		}

		// if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){
		// 	startColumn(0);
		// }

		// if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
		// 	startColumn(1);
		// }

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
			//cancelColorSort();
			Intake.move(0);
			Color_Roller.move(0);
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
			holdMode = false;
			Claw.move(-127);
			double adjPercent = (-LSensor.get_position()/100)/Lmax * 1;
			percentToRotation(adjPercent + .1, 127);
			scoreMode = false; 
			holdMode = false;
			Claw.move(0);
			//scoreMode = false;
			
		}
// 		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
// 			chassis.setPose(64, 0, 270);
// chassis.moveToPoint(59, 0, 400);
// stackPos = 100;
// percentToRotation(.1 , 127);
// delay(50);
// reset();
// holdArm();
// chassis.moveToPoint(64,0,500, {.forwards = false, .minSpeed = 127});
// chassis.moveToPoint(59,0, 400, {.minSpeed = 127});
// chassis.moveToPoint(64,0, 400, {.forwards = false, .minSpeed = 127});
// 		}

		// if(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
		// 	holdMode = false;
		// 	lift.move(127);
		// } else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
		// 	holdMode = false;
		// 	lift.move(-127);
		// } else {
		// 	lift.move(0);
		// }

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
    if(scoreMode){
        pros::Task scoreTask(score,
			TASK_PRIORITY_DEFAULT + 1, TASK_STACK_DEPTH_DEFAULT, "scoreTask"
		);
		//pros::Task resetTask([]() { delay(100); reset(); delay(50); holdMode = true;  }, TASK_PRIORITY_DEFAULT + 2, TASK_STACK_DEPTH_DEFAULT, "resetTask");
    } else{
        //pros::Task resetTask(reset, TASK_PRIORITY_DEFAULT + 1, TASK_STACK_DEPTH_DEFAULT, "resetTask");
		pros::Task resetTask([]() {reset();
		Claw.move(127);}, TASK_PRIORITY_DEFAULT + 1, TASK_STACK_DEPTH_DEFAULT, "resetTask");
    }
    scoreMode = !scoreMode;
}
if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
    if(scoreMode){
        pros::Task scoreTaskZer0(ScoreToZero, TASK_PRIORITY_DEFAULT + 1, TASK_STACK_DEPTH_DEFAULT, "scoreToZeroTask");
    } else{
        //pros::Task resetTaskZer0(reset, TASK_PRIORITY_DEFAULT + 1, TASK_STACK_DEPTH_DEFAULT, "resetTask2");
		pros::Task resetTask([]() {reset();
		Claw.move(127);}, TASK_PRIORITY_DEFAULT + 1, TASK_STACK_DEPTH_DEFAULT, "resetTask");
    }
    scoreMode = !scoreMode;
}



		pros::delay(20);
	}
}
