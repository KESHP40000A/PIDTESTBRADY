#include "main.h"
#include "pros/misc.h"
#include <iostream>
#include <string>
// Example program
#include "setup/config.h"

extern double Lmax;

extern double Wmax;

extern bool holdMode;

extern double stackPos;


void ScoreToZero();

void holdArm();

void percentToRotation(double percent, int volts);

void percentToRotationWithMin(double percent, int volts, double min);

void moveWristPose(int pos);

void reset();

void readStack();

void score();
