// #include "setup/config.h"
// #include "main.h"
// #include "pros/misc.h"
// #include <iostream>
// #include <string>
// #include <cstdio>
// #include "functions/macros.h"
// #include "functions/lookup.h"
// #include "functions/colorSort.h"

// namespace {
// // Open-space travel is full speed. colorSort.cpp slows only after optical 1
// // or 2 actually sees an object.
// constexpr int COLOR_SORT_INTAKE_POWER = 127;
// }

// void keep() {
//     printf("[macros] keep() called\n");
//     pros::lcd::print(0, "keep");
//     // A kept object goes through the opposite color-roller direction.
//     Color_Roller.move(-127);
//     Intake.move(-COLOR_SORT_INTAKE_POWER);
// }

// void flip() {
//     printf("[macros] flip() called\n");
//     pros::delay(10);
//     Claw.move(0);
//     Color_Roller.move(0);
//     // Intake is intentionally left untouched here - per spec, the intake
//     // "continues in the state it is in" after flip() runs. keep() re-asserts
//     // the normal running direction explicitly; flip() only cuts Claw/Color_Roller.
// }

// void startColumn(int column) {
//     printf("[macros] startColumn(%d) - colorSort re-enabled\n", column);
//     colorSortEnabled.store(true);
//     currentColumn.store(column);
//     dataMutex.take();
//     colorSortStage = "Waiting for sensor 3";
//     lastLookupAction = "--";
//     dataMutex.give();
//     Intake.move(-COLOR_SORT_INTAKE_POWER);
//     Color_Roller.move(127);
// }

// void cancelColorSort() {
//     printf("[macros] cancelColorSort() - colorSort disabled\n");
//     colorSortEnabled.store(false);
// }
