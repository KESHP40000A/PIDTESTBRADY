// #include "main.h"
// #include "pros/misc.h"
// #include <string>
// #include <cmath>

// #include "setup/config.h"
// #include "functions/colorSort.h"
// #include "functions/lookup.h"

// using namespace std;
// using namespace pros;
// using namespace lemlib;
// using namespace lcd;
// using namespace screen;

// int hue1, hue2, hue3;
// int prox1, prox2, prox3;
// int yellowCount, blueCount, redCount, grayCount;
// double sat1, sat2, sat3;
// int screenC1, screenC2;
// int screenH1, screenH2;

// namespace {

// constexpr int PROX_THRESHOLD = 140;
// constexpr double SENSOR_BASELINE[3] = {.600, .400, .300};
// constexpr uint32_t PIN_SAMPLE_DELAY_MS = 500;
// constexpr uint32_t GRAYCLEAR_SETTLE_MS = 1000;

// void computeResiduals(const double brightness[3], double residualOut[3]) {
//     for (int i = 0; i < 3; i++) {
//         residualOut[i] = (brightness[i] * 100.0) - SENSOR_BASELINE[i];
//     }
// }

// string classifyPin(double hue, int prox) {
//     if (prox <= PROX_THRESHOLD) return "NONE";
//     if (hue > 15 && hue < 85) return "Yellow";
//     if (hue > 145 && hue < 265) return "Blue";
//     if (hue > 315 || hue < 15) return "Red";
//     return "NONE";
// }

// void classifyCups(const double residual[3], const int prox[3], string colors[3]) {
//     bool present = (prox[0] > PROX_THRESHOLD && prox[1] > PROX_THRESHOLD) ||
//                    (prox[2] > PROX_THRESHOLD);
//     if (!present) {
//         colors[0] = colors[1] = colors[2] = "NONE";
//         return;
//     }

//     double avgA = (residual[0] + residual[1]) / 2.0;
//     double avgB = residual[2];

//     if (avgA < avgB) {
//         colors[0] = colors[1] = "Clear";
//         colors[2] = "Gray";
//     } else {
//         colors[0] = colors[1] = "Gray";
//         colors[2] = "Clear";
//     }
// }

// string resolveSetColor(const string& a, const string& b) {
//     static const char* priority[] = {"Yellow", "Blue", "Red", "Gray", "Clear"};
//     for (const char* c : priority) {
//         if (a == c || b == c) return c;
//     }
//     return "NONE";
// }

// bool isValidPinCombo(const string& a, const string& b) {
//     return a != "NONE" && b != "NONE" && a != b;
// }

// uint32_t colorToRGB(const string& c) {
//     if (c == "Yellow") return (uint32_t)pros::Color::yellow;
//     if (c == "Red")    return (uint32_t)pros::Color::red;
//     if (c == "Blue")   return (uint32_t)pros::Color::blue;
//     if (c == "Gray")   return (uint32_t)pros::Color::gray;
//     if (c == "Clear")  return (uint32_t)pros::Color::white;
//     return (uint32_t)pros::Color::black;
// }

// uint32_t hueToRGB(double hue) {
//     double c = 1.0;
//     double x = c * (1 - std::fabs(fmod(hue / 60.0, 2) - 1));
//     double r, g, b;
//     if (hue < 60)       { r = c; g = x; b = 0; }
//     else if (hue < 120) { r = x; g = c; b = 0; }
//     else if (hue < 180) { r = 0; g = c; b = x; }
//     else if (hue < 240) { r = 0; g = x; b = c; }
//     else if (hue < 300) { r = x; g = 0; b = c; }
//     else                { r = c; g = 0; b = x; }
//     return ((uint8_t)(r * 255) << 16) | ((uint8_t)(g * 255) << 8) | (uint8_t)(b * 255);
// }

// }  // namespace

// void colorSort() {
//     pros::Task colorSortTask([&]() {
//         pros::Optical* opticals[3] = {&optical1, &optical2, &optical3};

//         uint32_t cycleStartMillis = pros::millis();
//         bool locked = false;
//         uint32_t lockedElapsedSeconds = 0;

//         bool awaitingPinSample = false;
//         uint32_t pinSampleStartMillis = 0;
//         bool pinAttemptFailed = false;

//         bool grayClearPending = false;
//         uint32_t firstGrayClearMillis = 0;

//         while (true) {
//             double hues[3], sats[3], brightness[3];
//             int prox[3];

//             for (int i = 0; i < 3; i++) {
//                 hues[i]       = opticals[i]->get_hue();
//                 sats[i]       = opticals[i]->get_saturation();
//                 brightness[i] = opticals[i]->get_brightness();
//                 prox[i]       = opticals[i]->get_proximity();
//             }

//             hue1 = hues[0]; hue2 = hues[1]; hue3 = hues[2];
//             sat1 = sats[0]; sat2 = sats[1]; sat3 = sats[2];
//             prox1 = prox[0]; prox2 = prox[1]; prox3 = prox[2];

//             double residual[3];
//             computeResiduals(brightness, residual);

//             string rawPin[3];
//             for (int i = 0; i < 3; i++) rawPin[i] = classifyPin(hues[i], prox[i]);

//             string cupColors[3];
//             classifyCups(residual, prox, cupColors);

//             bool anyPresent = prox[0] > PROX_THRESHOLD || prox[1] > PROX_THRESHOLD || prox[2] > PROX_THRESHOLD;
//             uint32_t elapsedSeconds = (pros::millis() - cycleStartMillis) / 1000;

//             if (!locked) {
//                 if (anyPresent) {
//                     if (!awaitingPinSample && !pinAttemptFailed) {
//                         awaitingPinSample = true;
//                         pinSampleStartMillis = pros::millis();
//                     }
//                     if (!grayClearPending) {
//                         grayClearPending = true;
//                         firstGrayClearMillis = pros::millis();
//                     }

//                     if (awaitingPinSample && pros::millis() - pinSampleStartMillis >= PIN_SAMPLE_DELAY_MS) {
//                         awaitingPinSample = false;
//                         string pinSideA = resolveSetColor(rawPin[0], rawPin[1]);
//                         string pinSideB = rawPin[2];
//                         if (isValidPinCombo(pinSideA, pinSideB)) {
//                             locked = true;
//                             lockedElapsedSeconds = elapsedSeconds;
//                             Object = pinSideA + pinSideB;
//                             setX(Object);
//                         } else {
//                             pinAttemptFailed = true;
//                         }
//                     }

//                     if (!locked && pinAttemptFailed) {
//                         if (pros::millis() - firstGrayClearMillis >= GRAYCLEAR_SETTLE_MS &&
//                             cupColors[0] != "NONE") {
//                             locked = true;
//                             lockedElapsedSeconds = elapsedSeconds;
//                             Object = cupColors[0] + cupColors[2];
//                             setX(Object);
//                         }
//                     }
//                 } else {
//                     awaitingPinSample = false;
//                     pinAttemptFailed = false;
//                     grayClearPending = false;
//                 }
//             }

//             if (pinAttemptFailed) {
//                 Color1 = cupColors[0]; Color2 = cupColors[1]; Color3 = cupColors[2];
//             } else {
//                 Color1 = rawPin[0]; Color2 = rawPin[1]; Color3 = rawPin[2];
//             }

//             string countColors[3] = {Color1, Color2, Color3};
//             for (int i = 0; i < 3; i++) {
//                 if (countColors[i] == "Yellow") yellowCount++;
//                 else if (countColors[i] == "Blue") blueCount++;
//                 else if (countColors[i] == "Red") redCount++;
//                 else if (countColors[i] == "Gray") grayCount++;
//             }

//             string setColorA = resolveSetColor(Color1, Color2);
//             string setColorB = Color3;

//             screenH1 = (hue1 + hue2) / 2;
//             screenH2 = hue3;
//             set_pen(colorToRGB(setColorA));
//             fill_rect(0, 0, 240, 100);
//             set_pen(colorToRGB(setColorB));
//             fill_rect(240, 0, 480, 100);
//             set_pen(hueToRGB(screenH1));
//             fill_rect(0, 100, 240, 200);
//             set_pen(hueToRGB(screenH2));
//             fill_rect(240, 100, 480, 200);

//             string liveObject = "";
//             if (setColorA != "NONE") liveObject += setColorA;
//             if (setColorB != "NONE") liveObject += setColorB;

//             if (locked && !anyPresent) {
//                 locked = false;
//                 awaitingPinSample = false;
//                 pinAttemptFailed = false;
//                 grayClearPending = false;
//                 cycleStartMillis = pros::millis();
//                 Object = "";
//             }

//             print(0, "Res:%.2f %.2f %.2f", residual[0], residual[1], residual[2]);
//             print(1, "Brt:%.2f %.2f %.2f", brightness[0], brightness[1], brightness[2]);
//             print(2, "Prox:%i %i %i", prox1, prox2, prox3);
//             print(3, "C1:%s C2:%s C3:%s", Color1.c_str(), Color2.c_str(), Color3.c_str());
//             print(4, "Set1:%s Set2:%s", setColorA.c_str(), setColorB.c_str());
//             print(5, "Obj:%s", liveObject.c_str());
//             print(6, "Sats:%.1f %.1f %.1f", sats[0], sats[1], sats[2]);

//             uint32_t displaySeconds = locked ? lockedElapsedSeconds : elapsedSeconds;
//             print(7, "T:%lus Obj:%s", (unsigned long)displaySeconds, locked ? Object.c_str() : "...");

//             pros::delay(50);

//             yellowCount = 0; blueCount = 0; redCount = 0; grayCount = 0;
//         }
//     });
// }