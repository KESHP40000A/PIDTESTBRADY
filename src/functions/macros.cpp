#include "setup/config.h"
#include "main.h"
#include "pros/misc.h"
#include <iostream>
#include <string>
#include <cstdio>
#include "functions/macros.h"
#include "functions/lookup.h"
#include "functions/colorSort.h"

void keep() {
    printf("[macros] keep() called\n");
    pros::lcd::print(0, "keep");
    Color_Roller.move(127);
    Intake.move(-127);
}

void flip() {
    printf("[macros] flip() called\n");
    pros::delay(10);
    Claw.move(0);
    Color_Roller.move(0);
    // Intake is intentionally left untouched here - per spec, the intake
    // "continues in the state it is in" after flip() runs. keep() re-asserts
    // the normal running direction explicitly; flip() only cuts Claw/Color_Roller.
}

void startColumn(int column) {
    printf("[macros] startColumn(%d) - colorSort re-enabled\n", column);
    colorSortEnabled.store(true);
    currentColumn.store(column);
    Intake.move(-127);
    Color_Roller.move(127);
}

void cancelColorSort() {
    printf("[macros] cancelColorSort() - colorSort disabled\n");
    colorSortEnabled.store(false);
}