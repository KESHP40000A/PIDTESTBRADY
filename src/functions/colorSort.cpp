// #include "main.h"
// #include "pros/misc.h"
// #include <string>
// #include <cmath>
// #include <utility>
// #include <atomic>
// #include <cstdio>

// #include "setup/config.h"
// #include "functions/colorSort.h"
// #include "functions/lookup.h"
// #include "functions/macros.h"

// using namespace std;

// int hue1, hue2, hue3;
// int prox1, prox2, prox3;
// double sat1, sat2, sat3;
// // Object, Color1, Color2, Color3, and mode are NOT defined here - they're
// // already defined in setup/config.cpp. colorSort.h's extern declarations are
// // enough for this file to read/write them.

// std::atomic<int> currentColumn{0};
// // The task is started during initialize(), but it deliberately does nothing
// // until the driver selects a color-sort column with X or Y.
// std::atomic<bool> colorSortEnabled{false};
// std::string colorSortStage = "Disabled - select X/Y";
// std::string lastLookupAction = "--";
// int cupNearSensor = 0;
// double cupSatBefore = 0.0;
// double cupSatAfter = 0.0;
// double cupSatDelta = 0.0;

// namespace {

// constexpr int PROX_THRESHOLD = 40;
// constexpr int PRE_STOP_DELAY_MS = 50;   // Keep this short: sensor 3 has already detected the
//                                          // object, and delaying here lets a pin travel past its
//                                          // intended classification position.
// constexpr int SETTLE_DELAY_MS = 40;     // Short fresh-read delay after stopping. 1000 ms let
//                                          // pins coast out of the classifier before lookup.
// constexpr int LED_SETTLE_MS = 120;      // enough for the LED setting/readings to settle without
//                                          // making the stopped cup wait unnecessarily long
// constexpr int CUP_SAMPLE_COUNT = 3;
// constexpr int CUP_SAMPLE_GAP_MS = 10;
// constexpr double CUP_SAT_DELTA = 0.015; // 1.5% saturation change; Brain screen shows actual delta
// constexpr double MIN_COLOR_SATURATION = 0.08; // Gray/clear hue is noise, not a pin color
// constexpr int SORT_SLOW_INTAKE_POWER = 90;
// constexpr uint32_t DECISION_LOCK_MS = 3000;

// bool colorSortStarted = false;

// void setStage(const char* stage) {
//     dataMutex.take();
//     colorSortStage = stage;
//     dataMutex.give();
// }

// void setLookupAction(const char* action) {
//     dataMutex.take();
//     lastLookupAction = action;
//     dataMutex.give();
// }

// string classifyHue(double hue, double saturation) {
//     // Hue is arbitrary when saturation is near zero. Without this guard, a
//     // gray/clear cup frequently reports hue 0 and gets mistaken for Red.
//     if (saturation < MIN_COLOR_SATURATION) return "NONE";
//     if (hue > 15 && hue < 80) return "Yellow";
//     if (hue > 150 && hue < 260) return "Blue";
//     if ((hue > 320 && hue < 360) || hue < 15) return "Red";
//     return "NONE";
// }

// double averageSaturation(pros::Optical& optical) {
//     double total = 0.0;
//     for (int sample = 0; sample < CUP_SAMPLE_COUNT; ++sample) {
//         total += optical.get_saturation();
//         if (sample + 1 < CUP_SAMPLE_COUNT) pros::delay(CUP_SAMPLE_GAP_MS);
//     }
//     return total / CUP_SAMPLE_COUNT;
// }

// pair<string, string> classifyGrayClear(pros::Optical& nearOpt, pros::Optical& farOpt,
//                                         int nearSensorNumber) {
//     // The far sensor's LED is turned down. Per the requested procedure, a
//     // saturation change at the *other* (near) sensor means that near side is
//     // Clear and the dimmed/far side is Gray.
//     double before = averageSaturation(nearOpt);

//     farOpt.set_led_pwm(0);
//     pros::delay(LED_SETTLE_MS);
//     double after = averageSaturation(nearOpt);
//     farOpt.set_led_pwm(100); // restore immediately so later reads aren't affected

//     const double delta = fabs(after - before);
//     dataMutex.take();
//     cupNearSensor = nearSensorNumber;
//     cupSatBefore = before;
//     cupSatAfter = after;
//     cupSatDelta = delta;
//     dataMutex.give();

//     printf("[colorSort] cup test: before_sat=%.3f after_sat=%.3f delta=%.3f threshold=%.3f\n",
//            before, after, delta, CUP_SAT_DELTA);

//     if (delta < CUP_SAT_DELTA) {
//         return {"Gray", "Clear"};
//     }
//     return {"Clear", "Gray"};
// }

// } // namespace

// void colorSort() {
//     if (colorSortStarted) return;
//     colorSortStarted = true;
//     printf("[colorSort] task starting\n");

//     pros::Task colorSortTask([]() {
//         bool objectBeingProcessed = false;
//         bool decisionLocked = false;
//         uint32_t decisionLockStarted = 0;

//         while (true) {
//             double h1 = optical1.get_hue(),        h2 = optical2.get_hue(),        h3 = optical3.get_hue();
//             double s1 = optical1.get_saturation(), s2 = optical2.get_saturation(), s3 = optical3.get_saturation();
//             int    p1 = optical1.get_proximity(),  p2 = optical2.get_proximity(), p3 = optical3.get_proximity();

//             string c1 = classifyHue(h1, s1), c2 = classifyHue(h2, s2), c3 = classifyHue(h3, s3);

//             // Optical 3 is the gate sensor. Optical 1/2 can have very
//             // different proximity values because one is intentionally farther
//             // from the object, so neither belongs in the gate condition.
//             bool gateSeesObject = p3 > PROX_THRESHOLD;
//             bool enabled = colorSortEnabled.load();

//             // Live per-tick readout, THROTTLED to ~3x/sec instead of every 20ms tick.
//             // At full tick rate this was printing ~50x/sec, which is almost certainly
//             // why GATE TRIGGERED / PARTIAL detection / after-settle lines were getting
//             // buried (or dropped outright - the V5 serial link can't keep up with that
//             // much printf traffic). Raise the modulo to print less often, or comment
//             // the whole block out once you've confirmed things are working.
//             static int debugTickCounter = 0;
//             if (++debugTickCounter % 15 == 0) {
//                 printf("[colorSort] p1=%d p2=%d p3=%d | h1=%.1f h2=%.1f h3=%.1f | c1=%s c2=%s c3=%s | enabled=%d gate=%d processing=%d\n",
//                        p1, p2, p3, h1, h2, h3, c1.c_str(), c2.c_str(), c3.c_str(),
//                        (int)enabled, (int)gateSeesObject, (int)objectBeingProcessed);
//             }

//             // Diagnostic for objects in the classifier area that have not yet
//             // reached optical 3, the actual gate sensor.
//             if (!gateSeesObject && (p1 > PROX_THRESHOLD || p2 > PROX_THRESHOLD || p3 > PROX_THRESHOLD)) {
//                 printf("[colorSort] PARTIAL detection (threshold=%d): p1=%d%s p2=%d%s p3=%d%s\n",
//                        PROX_THRESHOLD,
//                        p1, p1 > PROX_THRESHOLD ? "(ok)" : "(LOW)",
//                        p2, p2 > PROX_THRESHOLD ? "(ok)" : "(LOW)",
//                        p3, p3 > PROX_THRESHOLD ? "(ok)" : "(LOW)");
//             }

//             if (!enabled) {
//                 // Manual override is in effect (e.g. L2). Don't touch the motors at
//                 // all here, and don't carry stale detection state into whenever
//                 // colorSort gets re-enabled.
//                 objectBeingProcessed = false;
//                 decisionLocked = false;
//                 setStage("Disabled - select X/Y");
//             }

//             // After a keep/flip decision, do not read the same object again
//             // while it is still in the intake. Motor commands and lookup output
//             // remain fixed for three seconds, then a new object can be read.
//             if (enabled && decisionLocked && pros::millis() - decisionLockStarted >= DECISION_LOCK_MS) {
//                 decisionLocked = false;
//                 objectBeingProcessed = false;
//                 setStage("Decision reset - ready");
//             }

//             string localObject;
//             bool localMode = false;
//             bool didClassify = false;

//             if (enabled && !decisionLocked && !objectBeingProcessed &&
//                 (p1 > PROX_THRESHOLD || p2 > PROX_THRESHOLD)) {
//                 // Run at full speed through open space, then slow only once
//                 // either classifier sensor sees a pin/cup.
//                 Intake.move(-SORT_SLOW_INTAKE_POWER);
//                 setStage("S1/S2 tripped - slowing intake");
//             }

//             if (enabled && !decisionLocked && gateSeesObject && !objectBeingProcessed) {
//                 objectBeingProcessed = true;

//                 setStage("Gate tripped - waiting");
//                 printf("[colorSort] GATE TRIGGERED (sensor3) - waiting %dms before stopping intake/roller\n", PRE_STOP_DELAY_MS);
//                 pros::delay(PRE_STOP_DELAY_MS);

//                 // A driver may have cancelled sorting while the pre-stop delay
//                 // was running. Never take motor control back in that case.
//                 if (!colorSortEnabled.load()) {
//                     setStage("Cancelled during gate wait");
//                     objectBeingProcessed = false;
//                     continue;
//                 }

//                 setStage("Stopping intake");
//                 Intake.move(0);
//                 Color_Roller.move(0);
//                 printf("[colorSort] intake/roller stopped, settling %dms\n", SETTLE_DELAY_MS);
//                 setStage("Settling object");
//                 pros::delay(SETTLE_DELAY_MS);

//                 if (!colorSortEnabled.load()) {
//                     setStage("Cancelled while settling");
//                     objectBeingProcessed = false;
//                     continue;
//                 }

//                 // Re-read fresh - the pre-stop values above may be stale/mid-motion.
//                 h1 = optical1.get_hue();        h2 = optical2.get_hue();        h3 = optical3.get_hue();
//                 s1 = optical1.get_saturation(); s2 = optical2.get_saturation(); s3 = optical3.get_saturation();
//                 p1 = optical1.get_proximity();  p2 = optical2.get_proximity();  p3 = optical3.get_proximity();
//                 c1 = classifyHue(h1, s1); c2 = classifyHue(h2, s2); c3 = classifyHue(h3, s3);

//                 // Sensor 3 confirms the object remains at the gate. A valid
//                 // classification also needs optical 1 or 2 to see it; their
//                 // hue is only background noise when both proximity values are low.
//                 bool stillPresent = p3 > PROX_THRESHOLD;
//                 bool classifierSeesObject = p1 > PROX_THRESHOLD || p2 > PROX_THRESHOLD;

//                 bool oneIsNear = (p1 >= p2); // higher-proximity of optical1/optical2 is "near"
//                 pros::Optical& nearOpt = oneIsNear ? optical1 : optical2;
//                 pros::Optical& farOpt  = oneIsNear ? optical2 : optical1;
//                 string nearColor = oneIsNear ? c1 : c2;
//                 string farColor  = c3;

//                 printf("[colorSort] after settle: present=%d nearSensor=%s nearColor=%s farColor(opt3)=%s\n",
//                        (int)stillPresent, oneIsNear ? "optical1" : "optical2", nearColor.c_str(), farColor.c_str());

//                 if (!stillPresent) {
//                     // Object drifted out of the gate during the settle delay - the
//                     // reads above aren't trustworthy. Don't run the cup/pin logic on
//                     // noise; just resume via keep() and let the next real object
//                     // trigger a fresh cycle.
//                     localObject = nearColor + farColor;
//                     localMode = false;
//                     setStage("Object left gate - keeping");
//                     printf("[colorSort] object gone after settle (p1=%d p2=%d p3=%d) - skipping classification, keep()\n",
//                            p1, p2, p3);
//                     keep();
//                     didClassify = true;
//                 } else if (!classifierSeesObject) {
//                     // Do not let background color from optical 1/2 select a
//                     // lookup-table row when the pin never reached that plane.
//                     localObject = "No S1/S2 read";
//                     localMode = false;
//                     setStage("No S1/S2 read - no lookup");
//                     setLookupAction("NO LOOKUP (no S1/S2)");
//                     printf("[colorSort] classifier miss: p1=%d p2=%d p3=%d - no lookup, resuming intake\n",
//                            p1, p2, p3);
//                     keep();
//                     didClassify = true;
//                 } else if (nearColor == "Red" || nearColor == "Blue" ||
//                     farColor  == "Red" || farColor  == "Blue") {
//                     // PIN: two-letter code = (near pair sensor) + (optical3)
//                     localObject = nearColor + farColor;
//                     localMode = false;
//                     setStage("Pin - lookup");
//                     printf("[colorSort] -> PIN branch, code=\"%s\", column=%d, blue=%d\n",
//                            localObject.c_str(), currentColumn.load(), (int)blue);
//                     setX(localObject);
//                     lookupPins(currentColumn.load());
//                     didClassify = true;
//                 } else {
//                     // CUP: neither side read Red or Blue. We no longer require an
//                     // exact "Yellow" hue match on both sides to get here - a
//                     // gray/clear cup is inherently low-saturation, and hue is
//                     // unstable at low saturation, so classifyHue() was sometimes
//                     // landing outside the Yellow range (15-80) and returning NONE
//                     // instead, on physically identical passes. That's the
//                     // inconsistency you saw: the cup test firing once and then
//                     // missing on the next cup. classifyGrayClear() itself only
//                     // looks at a saturation delta, not hue, so it doesn't care
//                     // whether nearColor/farColor came out as Yellow or NONE -
//                     // "not Red, not Blue" is enough to treat it as a cup candidate.
//                     setStage("Cup test - dim far sensor");
//                     printf("[colorSort] no Red/Blue detected (near=%s far=%s) -> running cup check\n",
//                            nearColor.c_str(), farColor.c_str());
//                     auto labels = classifyGrayClear(nearOpt, farOpt, oneIsNear ? 1 : 2);
//                     localObject = labels.first + labels.second;
//                     localMode = true;
//                     setStage("Cup - lookup");
//                     printf("[colorSort] cup check result: near=%s far=%s code=\"%s\"\n",
//                            labels.first.c_str(), labels.second.c_str(), localObject.c_str());
//                     setX(localObject);
//                     lookupCups(currentColumn.load());
//                     didClassify = true;
//                 }
//             } else if (enabled && !decisionLocked && !gateSeesObject) {
//                 objectBeingProcessed = false; // gate is clear again; ready for the next object
//                 setStage("Waiting for sensor 3");
//             }

//             dataMutex.take();
//             hue1 = h1; hue2 = h2; hue3 = h3;
//             sat1 = s1; sat2 = s2; sat3 = s3;
//             prox1 = p1; prox2 = p2; prox3 = p3;
//             Color1 = c1; Color2 = c2; Color3 = c3;
//             if (didClassify) {
//                 Object = localObject;
//                 mode = localMode;
//                 decisionLocked = true;
//                 decisionLockStarted = pros::millis();
//                 colorSortStage = "Decision locked (3 sec)";
//             }
//             dataMutex.give();

//             pros::delay(20);
//         }
//     });
// }
