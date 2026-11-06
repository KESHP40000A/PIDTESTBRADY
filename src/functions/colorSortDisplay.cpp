// #include "main.h"
// #include "pros/screen.h"

// #include <algorithm>
// #include <cstdint>
// #include <string>

// #include "functions/colorSort.h"
// #include "setup/config.h"

// namespace {

// bool displayStarted = false;

// uint32_t colorFor(const std::string& color) {
//     if (color == "Red") return pros::c::COLOR_RED;
//     if (color == "Blue") return pros::c::COLOR_BLUE;
//     if (color == "Yellow") return pros::c::COLOR_YELLOW;
//     if (color == "Gray") return pros::c::COLOR_GRAY;
//     if (color == "Clear") return pros::c::COLOR_WHITE;
//     return pros::c::COLOR_BLACK;
// }

// // Mix white with the sensed color. Zero saturation is white; full saturation
// // is the same color used by the object square above it.
// uint32_t saturationColor(uint32_t base, double saturation) {
//     saturation = std::max(0.0, std::min(1.0, saturation));
//     const auto mix = [saturation](uint32_t component) {
//         return static_cast<uint32_t>(255.0 + (static_cast<double>(component) - 255.0) * saturation);
//     };
//     return RGB2COLOR(mix(COLOR2R(base)), mix(COLOR2G(base)), mix(COLOR2B(base)));
// }

// void square(int x, int y, uint32_t fill) {
//     pros::c::screen_set_pen(fill);
//     pros::c::screen_fill_rect(x, y, x + 68, y + 28);
//     pros::c::screen_set_pen(pros::c::COLOR_WHITE);
//     pros::c::screen_draw_rect(x, y, x + 68, y + 28);
// }

// } // namespace

// void startColorSortDisplay() {
//     if (displayStarted) return;
//     displayStarted = true;

//     pros::Task displayTask([]() {
//         while (true) {
//             int p1, p2, p3;
//             int cupNear;
//             double s1, s2, s3, cupDelta;
//             std::string c1, c2, c3, stage, action, object;
//             bool isCup;

//             dataMutex.take();
//             p1 = prox1; p2 = prox2; p3 = prox3;
//             s1 = sat1; s2 = sat2; s3 = sat3;
//             c1 = Color1; c2 = Color2; c3 = Color3;
//             stage = colorSortStage; action = lastLookupAction;
//             object = Object; isCup = mode;
//             cupNear = cupNearSensor; cupDelta = cupSatDelta;
//             dataMutex.give();

//             const bool favorOne = p1 >= p2;
//             const std::string& objectSide = favorOne ? c1 : c2;
//             const double objectSat = favorOne ? s1 : s2;

//             pros::c::screen_set_eraser(pros::c::COLOR_BLACK);
//             pros::c::screen_erase();

//             // Object colors on top; below them, the same colors fade from
//             // white according to each optical sensor's reported saturation.
//             square(8, 5, colorFor(objectSide));
//             square(84, 5, colorFor(c3));
//             square(8, 38, saturationColor(colorFor(objectSide), objectSat));
//             square(84, 38, saturationColor(colorFor(c3), s3));

//             // Begin at screen line 3, leaving the four diagnostic squares clear.
//             pros::c::screen_set_pen(pros::c::COLOR_WHITE);
//             pros::c::screen_print(pros::E_TEXT_SMALL, 3, "Action: %s", action.c_str());
//             pros::c::screen_print(pros::E_TEXT_SMALL, 4, "S1:%s  S2:%s  S3:%s", c1.c_str(), c2.c_str(), c3.c_str());
//             pros::c::screen_print(pros::E_TEXT_SMALL, 5, "P1:%d P2:%d -> O%d  P3:%d", p1, p2, favorOne ? 1 : 2, p3);
//             if (object.empty()) {
//                 pros::c::screen_print(pros::E_TEXT_SMALL, 6, "Object: -- (none yet)");
//             } else if (isCup) {
//                 pros::c::screen_print(pros::E_TEXT_SMALL, 6, "Cup: %s N%d d%.3f", object.c_str(), cupNear, cupDelta);
//             } else {
//                 pros::c::screen_print(pros::E_TEXT_SMALL, 6, "Object: %s (%s)", object.c_str(), isCup ? "Cup" : "Pin");
//             }
//             pros::c::screen_print(pros::E_TEXT_SMALL, 7, "Stage: %s", stage.c_str());

//             pros::delay(100);
//         }
//     });
// }
