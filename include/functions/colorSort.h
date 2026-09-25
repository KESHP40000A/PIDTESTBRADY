// Example program
#include "main.h"
#include "pros/misc.h"
#include <iostream>
#include <string>
// Example program
#include "setup/config.h"


void colorSort();


extern int hue1;
extern int hue2;
extern int hue3;
extern int prox1;
extern int prox2;
extern int prox3;
extern double sat1;
extern double sat2;
extern double sat3;
extern int yellowCount;
extern int blueCount;
extern int grayCount;
extern int redCount;
extern bool mode; 
extern bool sortingPaused;

// true = taking in pins, false = taking in cups (set/read in main.cpp)