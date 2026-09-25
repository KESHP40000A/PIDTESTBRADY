// #include "setup/config.h"
// #include "main.h"
// #include "pros/misc.h"
// #include <iostream>
// #include <string>
// #include "functions/macros.h"
// #include "functions/lookup.h"
// #include "functions/colorSort.h"

// namespace {

// constexpr int SEEN_PROX = 80;
// constexpr int FAST_SPEED = 127;
// constexpr int SLOW_SPEED = 100;
// constexpr int STOP_DELAY_MS = 60;
// constexpr int SETTLE_MS = 150;

// // void driveIntake(int speed) {
// //     Intake.move(-speed);
// //     Color_Roller.move(-speed);
// // }



// // bool sensors12See() {
// //     return optical1.get_proximity() > SEEN_PROX &&
// //            optical2.get_proximity() > SEEN_PROX;
// // }

// // bool sensor3Sees() {
// //     return optical3.get_proximity() > SEEN_PROX;
// // }

// }

// void keep(){
//     pros::lcd::print(0, "keep");
//     Color_Roller.move(127);
//     Intake.move(-127);
// }

// void flip(){
//     // pros::lcd::print(0, "flip");
//     // Color_Roller.move(-100);
//     // Intake.move(-90);
//     // Claw.move(20);
//     pros::delay(10);
//     Claw.move(0);
//     Color_Roller.move(0);
//     // Color_Roller.move(127);
//     // Intake.move(-127);
// }

// void runColor(){
//     while (true) {


//         //Object = "";

//         // driveIntake(FAST_SPEED);
//         // while (!sensors12See()) {
//         //     pros::delay(5);
//         // }

//         // driveIntake(SLOW_SPEED);
//         // while (!sensor3Sees()) {
//         //     pros::delay(5);
//         // }

//         // pros::delay(STOP_DELAY_MS);
//         // Intake.move(0);
//         // Color_Roller.move(0);
//         // pros::delay(SETTLE_MS);

//         // std::string obj = Object;
//         // Object = "";

//         // if (obj.empty()) {
//         //     keep();
//         //     continue;
//         // }

//         // setX(obj);

//     }
// }