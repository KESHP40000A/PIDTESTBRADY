#pragma once
#include <string>

// true: prioritize Blue (top column 1 / bottom column 2).
// false: prioritize Red the same way instead.
// Has NO effect on lookupCups() - cups have no blue/red distinction.
extern bool blue;

void setX(std::string x1);
void lookupPins(int indexy);
void lookupCups(int indexy);