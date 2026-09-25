// #include "main.h"
// #include "pros/misc.h"
// #include <iostream>
// #include <string>
// #include "setup/config.h"
// #include "functions/macros.h"

// using namespace std;

// string x;

// void setX(string x1){
//     dataMutex.take();
//     x = x1;
//     dataMutex.give();
// }

// void lookupPins(int indexy){
//     static const string XVALS[7] = {
//         "YellowRed",
//         "RedYellow",
//         "YellowYellow",
//         "YellowBlue",
//         "BlueYellow",
//         "RedBlue",
//         "BlueRed"
//     };

//     static const bool results1[7][2] = {
//         {true, false},
//         {false, true},
//         {false, false},
//         {true, false},
//         {false, true},
//         {false, true},
//         {true, false}
//     };

//     string current = x;
//     int resultx = -1;

//     for(int i = 0; i < 7; i++){
//         if(XVALS[i] == current){
//             resultx = i;
//             break;
//         }
//     }

//     if(resultx < 0 || indexy < 0 || indexy > 1){
//         keep();
//         return;
//     }

//     if(results1[resultx][indexy]){
//         flip();
//     } else{
//         keep();
//     }
// }

// void lookupCups(int indexy){
//     static const string XVALS[2] = {
//         "GrayClear",
//         "ClearGray"
//     };

//     static const bool results1[2][2] = {
//         {false, true},
//         {true, false}
//     };

//     string current = x;
//     int resultx = -1;

//     for(int i = 0; i < 2; i++){
//         if(XVALS[i] == current){
//             resultx = i;
//             break;
//         }
//     }

//     if(resultx < 0 || indexy < 0 || indexy > 1){
//         keep();
//         return;
//     }

//     if(results1[resultx][indexy]){
//         flip();
//     } else{
//         keep();
//     }
// }