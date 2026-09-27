#include "main.h"
#include "pros/misc.h"
#include <iostream>
#include <string>
#include <cstdio>
#include "setup/config.h"
#include "functions/macros.h"
#include "functions/lookup.h"

using namespace std;

string x;
bool blue = true;

void setX(string x1) {
    dataMutex.take();
    x = x1;
    dataMutex.give();
}

void lookupPins(int indexy) {
    static const string XVALS[7] = {
        "YellowRed",   // 0
        "RedYellow",   // 1
        "YellowYellow",// 2
        "YellowBlue",  // 3
        "BlueYellow",  // 4
        "RedBlue",     // 5
        "BlueRed"      // 6
    };

    // results1[i][col]: true => flip(), false => keep(). Written assuming blue = true.
    static const bool results1[7][2] = {
        {true, false},
        {false, true},
        {false, false},
        {true, false},
        {false, true},
        {false, true},
        {true, false}
    };

    // When blue == false, we want Red to get the treatment Blue currently gets
    // (and vice versa), with YellowYellow unaffected. Rather than maintaining a
    // second hardcoded table, swap to the row of the Red<->Blue counterpart:
    //   0 YellowRed  <-> 3 YellowBlue
    //   1 RedYellow  <-> 4 BlueYellow
    //   2 YellowYellow <-> 2 (itself - no red or blue involved)
    //   5 RedBlue    <-> 6 BlueRed
    static const int SWAP_INDEX[7] = {3, 4, 2, 0, 1, 6, 5};

    string current;
    bool useBlue;
    dataMutex.take();
    current = x;
    useBlue = blue;
    dataMutex.give();

    int resultx = -1;
    for (int i = 0; i < 7; i++) {
        if (XVALS[i] == current) {
            resultx = i;
            break;
        }
    }

    printf("[lookupPins] current=\"%s\" indexy=%d useBlue=%d matchIndex=%d\n",
           current.c_str(), indexy, (int)useBlue, resultx);

    if (resultx < 0 || indexy < 0 || indexy > 1) {
        printf("[lookupPins] NO MATCH for \"%s\" (or bad indexy=%d) -> defaulting to keep()\n",
               current.c_str(), indexy);
        keep();
        return;
    }

    if (!useBlue) {
        int swapped = SWAP_INDEX[resultx];
        printf("[lookupPins] blue=false, swapping row %d -> %d\n", resultx, swapped);
        resultx = swapped;
    }

    bool doFlip = results1[resultx][indexy];
    printf("[lookupPins] row=%d col=%d -> %s\n", resultx, indexy, doFlip ? "FLIP" : "KEEP");

    if (doFlip) {
        flip();
    } else {
        keep();
    }
}

void lookupCups(int indexy) {
    static const string XVALS[2] = {
        "GrayClear",
        "ClearGray"
    };

    static const bool results1[2][2] = {
        {false, true},
        {true, false}
    };

    string current;
    dataMutex.take();
    current = x;
    dataMutex.give();

    int resultx = -1;
    for (int i = 0; i < 2; i++) {
        if (XVALS[i] == current) {
            resultx = i;
            break;
        }
    }

    printf("[lookupCups] current=\"%s\" indexy=%d matchIndex=%d\n",
           current.c_str(), indexy, resultx);

    if (resultx < 0 || indexy < 0 || indexy > 1) {
        printf("[lookupCups] NO MATCH for \"%s\" (or bad indexy=%d) -> defaulting to keep()\n",
               current.c_str(), indexy);
        keep();
        return;
    }

    // "blue" intentionally does not apply here - cups have no red/blue distinction.
    bool doFlip = results1[resultx][indexy];
    printf("[lookupCups] row=%d col=%d -> %s\n", resultx, indexy, doFlip ? "FLIP" : "KEEP");

    if (doFlip) {
        flip();
    } else {
        keep();
    }
}