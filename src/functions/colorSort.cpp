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
// bool sortingPaused = false;

// namespace {

// constexpr int PROX_THRESHOLD = 80;
// //-.27 - .17
// //-.1 = 0 

// //.20

// constexpr double SENSOR_BASELINE[3] = {0.600, 0.500, 0.300};

// void computeResiduals(const double sat[3], double residualOut[3]) {
//     for (int i = 0; i < 3; i++) {
//         residualOut[i] = sat[i] - SENSOR_BASELINE[i];
//     }
// }

// // string classifyPin(double hue, int prox) {
// //     if (prox <= PROX_THRESHOLD) return "NONE";
// //     if (hue > 15 && hue < 80) return "Yellow";
// //     if (hue > 150 && hue < 260) return "Blue";
// //     if ((hue > 320 && hue < 360) || hue < 15) return "Red";
// //     return "NONE";
// // }

// // bool isRedOrBlue(const string& c) {
// //     return c == "Red" || c == "Blue";
// // }

// // void classifyCups(const double residual[3], const int prox[3], string colors[3]) {
// //     bool present = (prox[0] > PROX_THRESHOLD && prox[1] > PROX_THRESHOLD) ||
// //                    (prox[2] > PROX_THRESHOLD);
// //     if (!present) {
// //         colors[0] = colors[1] = colors[2] = "NONE";
// //         return;
// //     }

// //     double avgA = (residual[0] + residual[1]) / 2.0;
// //     double avgB = residual[2];

// //     if (avgA < avgB) {
// //         colors[0] = colors[1] = "Clear";
// //         colors[2] = "Gray";
// //     } else {
// //         colors[0] = colors[1] = "Gray";
// //         colors[2] = "Clear";
// //     }
// // }

// // string resolveSetColor(const string& a, const string& b) {
// //     static const char* priority[] = {"Yellow", "Blue", "Red", "Gray", "Clear"};
// //     for (const char* c : priority) {
// //         if (a == c || b == c) return c;
// //     }
// //     return "NONE";
// // }

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

//         while (true) {
            

//             double hues[3], sats[3];
//             int prox[3];

//             for (int i = 0; i < 3; i++) {
//                 hues[i] = opticals[i]->get_hue();
//                 sats[i] = opticals[i]->get_saturation();
//                 prox[i] = opticals[i]->get_proximity();
//             }

//             hue1 = hues[0]; hue2 = hues[1]; hue3 = hues[2];
//             sat1 = sats[0]; sat2 = sats[1]; sat3 = sats[2];
//             prox1 = prox[0]; prox2 = prox[1]; prox3 = prox[2];

//             string pinColors[3];
            
//             double residual[3];
//             computeResiduals(sats, residual);

//             string colors[3];
            

//             Color1 = colors[0]; Color2 = colors[1]; Color3 = colors[2];

//             string setColorA = "";

//             if(prox[0] > prox[1]){
//                 setColorA = colors[0];
//             } else if(prox[1] > prox[0]){
//                 setColorA = colors[1];
//             }

//             string setColorB = colors[2];

//             print(1, "Res:%.2f %.2f %.2f", residual[0], residual[1], residual[2]);
//             print(3, "Prox:%i %i %i", prox1, prox2, prox3);
//             print(4, "C1:%s C2:%s C3:%s", Color1.c_str(), Color2.c_str(), Color3.c_str());
//             print(5, "Set1:%s Set2:%s", setColorA.c_str(), setColorB.c_str());

//             if (setColorA != "NONE" && setColorB != "NONE") {
//                 dataMutex.take();
// Object = setColorA + setColorB;
// dataMutex.give();
//                 setX(Object);
//                 if (Object == "GrayClear" || Object == "ClearGray") {
//                     mode = true;
//                 } else {
//                     mode = false;
//                 }
//             }

//             print(6, "Obj:%s", Object.c_str());
//             print(7, "Sats:%.2f %.2f %.2f", sats[0], sats[1], sats[2]);

//             pros::delay(50);

//             yellowCount = 0; blueCount = 0; redCount = 0; grayCount = 0;
//         }
//     });
// }