#pragma once
#include <atomic>
#include <string>

// Diagnostic snapshot of the last sensor read + classification.
// Always read/write these while holding dataMutex (declared in setup/config.h) -
// they're written every ~20ms by the colorSort task and may be read by a
// display task at any time.
extern int hue1, hue2, hue3;
extern int prox1, prox2, prox3;
extern double sat1, sat2, sat3;
extern std::string Color1, Color2, Color3; // per-sensor hue classification, for display
extern std::string Object;                  // last two-letter code sent to the lookup table
extern bool mode;                           // false = last object handled as a pin, true = handled as a cup

// Which lookup-table column is active (0 = column 1 / L1, 1 = column 2 / L2).
// Plain std::atomic<int> - no mutex needed for a single scalar. Written from
// opcontrol's task, read from the colorSort task.
extern std::atomic<int> currentColumn;

// Arbitration flag between colorSort's automatic pause/classify behavior and
// manual driver control of Intake/Color_Roller (e.g. the L2 "silly intake"
// override). true = colorSort is allowed to pause/classify objects at the
// gate. false = colorSort leaves Intake/Color_Roller/Claw alone entirely, so
// whatever last commanded them (manual input) stays in effect. startColumn()
// sets this true; cancelColorSort() (called from L2) sets it false.
extern std::atomic<bool> colorSortEnabled;

// Starts the background color-sort task. Safe to call more than once - only
// the first call actually spawns the task; later calls are a no-op.
void colorSort();