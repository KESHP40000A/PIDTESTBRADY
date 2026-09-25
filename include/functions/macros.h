// Example program
#include "main.h"
#include "pros/misc.h"
#include <iostream>
#include <string>
// Example program
#include "setup/config.h"

void flip();

void keep();

void runColor();

extern std::atomic<bool> colorSortActive;
extern pros::Task* colorTaskHandle;