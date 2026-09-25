
#include "main.h"
#include "lemlib/api.hpp" // IWYU 

#include "functions/lookup.h"
#include "setup/config.h"
#include "setup/autons.h"

void red(){
    chassis.turnToHeading(270, 10000);
}