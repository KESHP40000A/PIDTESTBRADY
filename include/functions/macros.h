#pragma once

void keep();
void flip();

// Sets the active lookup column, re-enables colorSort, and (re)starts the
// intake/color roller. column: 0 for R1 (column 1), 1 for R2 (column 2).
// Safe to call again with the same column to re-trigger the spin.
void startColumn(int column);

// Disables colorSort's automatic pause/classify behavior, handing full manual
// control of Intake/Color_Roller/Claw back to opcontrol. Call this before
// commanding those motors directly (e.g. from the L2 handler), so colorSort
// doesn't fight the manual command on its next loop tick.
void cancelColorSort();