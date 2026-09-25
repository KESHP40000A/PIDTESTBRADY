// #include "setup/config.h"
// #include "main.h"
// #include "pros/misc.h"
// #include <iostream>
// #include <string>
// #include <atomic>
// #include <memory>
// #include "functions/macros.h"
// #include "functions/lookup.h"
// #include "functions/colorSort.h"

// // Shared across tasks: guards Object/Color1/Color2/Color3/x.
// // Declared in config.cpp.
// extern pros::Mutex dataMutex;

// // Controls whether the background color-sort task keeps looping.
// // Declared here so main.cpp can flip it on button press.
// std::atomic<bool> colorSortActive{false};
// pros::Task* colorTaskHandle = nullptr;

// namespace {

// constexpr int SEEN_PROX = 80;
// constexpr int FAST_SPEED = 127;
// constexpr int SLOW_SPEED = 80;
// constexpr int STOP_DELAY_MS = 250;
// constexpr int SETTLE_MS = 50;
// constexpr int ACTION_TIMEOUT_MS = 5000;

// void driveIntake(int speed) {
//     Intake.move(-speed);
//     Color_Roller.move(-speed);
// }

// bool sensors12See() {
//     return optical1.get_proximity() > SEEN_PROX &&
//            optical2.get_proximity() > SEEN_PROX;
// }

// bool sensor3Sees() {
//     return optical3.get_proximity() > SEEN_PROX;
// }

// // Runs `action` on its own task and waits for it to finish or time out.
// // Uses an explicit atomic flag (set by the action itself right before it
// // returns) rather than polling pros::Task::get_state(), since checking
// // task state right around self-deletion is unreliable.
// template <typename Fn>
// void runActionWithTimeout(Fn action) {
//     auto done = std::make_shared<std::atomic<bool>>(false);

//     pros::Task task([action, done]() {
//         action();
//         *done = true;
//     });

//     uint32_t start = pros::millis();
//     while (!*done && pros::millis() - start < ACTION_TIMEOUT_MS) {
//         pros::delay(10);
//     }

//     if (!*done) {
//         task.remove();
//         Intake.move(0);
//         Color_Roller.move(0);
//         Claw.move(0);
//     }
// }

// }  // namespace

// void keep(){
//     //pros::lcd::print(0, "keep");
//     Color_Roller.move(127);
//     Intake.move(-127);
// }

// void flip(){
//     //pros::lcd::print(0, "flip");
//     // Color_Roller.move(-100);
//     // Intake.move(-90);
//     //Claw.move(100);
//     //Claw.move(0);
//     Intake.move(0);
//     Color_Roller.move(0);
//     // Color_Roller.move(0);
//     // Color_Roller.move(127);
//     // Intake.move(-127);
// }

// // Runs a single intake -> sort -> flip/keep cycle. Blocking, but only
// // blocks whatever task calls it (see runColor() below for the looped,
// // task-spawned version used from opcontrol).
// void runColorOnce(){
//     dataMutex.take();
//     Object = "";
//     dataMutex.give();

//     driveIntake(FAST_SPEED);
//     while (!sensors12See()) {
//         pros::delay(5);
//     }

//     driveIntake(SLOW_SPEED);
//     while (!sensor3Sees()) {
//         pros::delay(5);
//     }

//     //pros::delay(STOP_DELAY_MS);
//     Intake.move(0);
//     Color_Roller.move(0);
//     //pros::delay(SETTLE_MS);

//     dataMutex.take();
//     std::string obj = Object;
//     Object = "";
//     dataMutex.give();

//     sortingPaused = true;

//     if (obj.empty()) {
//         runActionWithTimeout(keep);
//     } else {
//         setX(obj);

//         if (obj == "GrayClear" || obj == "ClearGray") {
//             runActionWithTimeout([]{ lookupCups(ind); });
//         } else {
//             runActionWithTimeout([]{ lookupPins(ind); });
//         }
//     }

//     sortingPaused = false;
// }

// // Loops runColorOnce() for as long as colorSortActive is true. Meant to
// // be spawned as its own pros::Task (see main.cpp) so it never blocks
// // opcontrol()'s own polling loop.
// void runColor(){
//     while (colorSortActive) {
//         runColorOnce();
//         pros::delay(10);
//     }
// }