#include "main.h"
#include "pros/misc.h"
#include <string>
#include <cmath>
#include <utility>
#include <atomic>
#include <cstdio>

#include "setup/config.h"
#include "functions/colorSort.h"
#include "functions/lookup.h"
#include "functions/macros.h"

using namespace std;

int hue1, hue2, hue3;
int prox1, prox2, prox3;
double sat1, sat2, sat3;
// Object, Color1, Color2, Color3, and mode are NOT defined here - they're
// already defined in setup/config.cpp. colorSort.h's extern declarations are
// enough for this file to read/write them.

std::atomic<int> currentColumn{0};
std::atomic<bool> colorSortEnabled{true};

namespace {

constexpr int PROX_THRESHOLD = 40;
constexpr int SETTLE_DELAY_MS = 1000;   // "for now" placeholder per spec - time to let the
                                         // object stop moving after the intake stops, before
                                         // re-reading sensors for classification
constexpr int LED_SETTLE_MS = 400;      // separate delay: time for a sensor to settle after
                                         // its OWN LED brightness changes, during the cup test
constexpr double CUP_SAT_DELTA = 0.05;  // TODO: calibrate against real gray/clear rings on your robot

bool colorSortStarted = false;

string classifyHue(double hue) {
    if (hue > 15 && hue < 80) return "Yellow";
    if (hue > 150 && hue < 260) return "Blue";
    if ((hue > 320 && hue < 360) || hue < 15) return "Red";
    return "NONE";
}

pair<string, string> classifyGrayClear(pros::Optical& nearOpt, pros::Optical& farOpt) {
    double before = nearOpt.get_saturation();

    farOpt.set_led_pwm(0);
    pros::delay(LED_SETTLE_MS);
    double after = nearOpt.get_saturation();
    farOpt.set_led_pwm(100); // restore immediately so later reads aren't affected

    printf("[colorSort] cup test: before_sat=%.3f after_sat=%.3f delta=%.3f threshold=%.3f\n",
           before, after, fabs(after - before), CUP_SAT_DELTA);

    if (fabs(after - before) < CUP_SAT_DELTA) {
        return {"Gray", "Clear"};
    }
    return {"Clear", "Gray"};
}

} // namespace

void colorSort() {
    if (colorSortStarted) return;
    colorSortStarted = true;
    printf("[colorSort] task starting\n");

    pros::Task colorSortTask([]() {
        bool objectBeingProcessed = false;

        while (true) {
            double h1 = optical1.get_hue(),        h2 = optical2.get_hue(),        h3 = optical3.get_hue();
            double s1 = optical1.get_saturation(), s2 = optical2.get_saturation(), s3 = optical3.get_saturation();
            int    p1 = optical1.get_proximity(),  p2 = optical2.get_proximity(), p3 = optical3.get_proximity();

            string c1 = classifyHue(h1), c2 = classifyHue(h2), c3 = classifyHue(h3);

            bool gateSeesObject = (p1 > PROX_THRESHOLD && p2 > PROX_THRESHOLD && p3 > PROX_THRESHOLD);
            bool enabled = colorSortEnabled.load();

            // Live per-tick readout. Noisy on purpose - comment this one line
            // out once you've confirmed things are working.
            printf("[colorSort] p1=%d p2=%d p3=%d | h1=%.1f h2=%.1f h3=%.1f | c1=%s c2=%s c3=%s | enabled=%d gate=%d processing=%d\n",
                   p1, p2, p3, h1, h2, h3, c1.c_str(), c2.c_str(), c3.c_str(),
                   (int)enabled, (int)gateSeesObject, (int)objectBeingProcessed);

            if (!enabled) {
                // Manual override is in effect (e.g. L2). Don't touch the motors at
                // all here, and don't carry stale detection state into whenever
                // colorSort gets re-enabled.
                objectBeingProcessed = false;
            }

            string localObject;
            bool localMode = false;
            bool didClassify = false;

            if (enabled && gateSeesObject && !objectBeingProcessed) {
                objectBeingProcessed = true;

                Intake.move(0);
                Color_Roller.move(0);
                printf("[colorSort] GATE TRIGGERED (all 3 sensors) - stopping intake/roller, settling %dms\n", SETTLE_DELAY_MS);
                pros::delay(SETTLE_DELAY_MS);

                // Re-read fresh - the pre-stop values above may be stale/mid-motion.
                h1 = optical1.get_hue();        h2 = optical2.get_hue();        h3 = optical3.get_hue();
                s1 = optical1.get_saturation(); s2 = optical2.get_saturation(); s3 = optical3.get_saturation();
                p1 = optical1.get_proximity();  p2 = optical2.get_proximity();  p3 = optical3.get_proximity();
                c1 = classifyHue(h1); c2 = classifyHue(h2); c3 = classifyHue(h3);

                // Confirm the object is STILL actually in front of all three sensors
                // after the settle delay. A cup can keep coasting/rolling after the
                // intake stops (more than a pin typically does), and if it drifts out
                // of view during SETTLE_DELAY_MS, the "fresh" reads above are just
                // noise from an empty gap - which almost never happens to land on
                // Yellow/Yellow, so it was silently falling into the inconclusive
                // branch below and calling keep(). This makes that case explicit
                // instead of quietly misclassifying it.
                bool stillPresent = (p1 > PROX_THRESHOLD && p2 > PROX_THRESHOLD && p3 > PROX_THRESHOLD);

                bool oneIsNear = (p1 >= p2); // higher-proximity of optical1/optical2 is "near"
                pros::Optical& nearOpt = oneIsNear ? optical1 : optical2;
                pros::Optical& farOpt  = oneIsNear ? optical2 : optical1;
                string nearColor = oneIsNear ? c1 : c2;
                string farColor  = c3;

                printf("[colorSort] after settle: present=%d nearSensor=%s nearColor=%s farColor(opt3)=%s\n",
                       (int)stillPresent, oneIsNear ? "optical1" : "optical2", nearColor.c_str(), farColor.c_str());

                if (!stillPresent) {
                    // Object drifted out of the gate during the settle delay - the
                    // reads above aren't trustworthy. Don't run the cup/pin logic on
                    // noise; just resume via keep() and let the next real object
                    // trigger a fresh cycle.
                    localObject = nearColor + farColor;
                    localMode = false;
                    printf("[colorSort] object gone after settle (p1=%d p2=%d p3=%d) - skipping classification, keep()\n",
                           p1, p2, p3);
                    keep();
                    didClassify = true;
                } else if (nearColor == "Red" || nearColor == "Blue" ||
                    farColor  == "Red" || farColor  == "Blue") {
                    // PIN: two-letter code = (near pair sensor) + (optical3)
                    localObject = nearColor + farColor;
                    localMode = false;
                    printf("[colorSort] -> PIN branch, code=\"%s\", column=%d, blue=%d\n",
                           localObject.c_str(), currentColumn.load(), (int)blue);
                    setX(localObject);
                    lookupPins(currentColumn.load());
                    didClassify = true;
                } else if (nearColor == "Yellow" && farColor == "Yellow") {
                    // CUP: only trigger the cup check on a clean Yellow+Yellow read.
                    printf("[colorSort] both sides Yellow -> running cup check\n");
                    auto labels = classifyGrayClear(nearOpt, farOpt);
                    localObject = labels.first + labels.second;
                    localMode = true;
                    printf("[colorSort] cup check result: near=%s far=%s code=\"%s\"\n",
                           labels.first.c_str(), labels.second.c_str(), localObject.c_str());
                    setX(localObject);
                    lookupCups(currentColumn.load());
                    didClassify = true;
                } else {
                    // Inconclusive read (e.g. NONE on one or both sides) - not a clean
                    // enough signal to classify. Default to keep(), same philosophy as
                    // the lookup tables' own unmatched-code fallback.
                    localObject = nearColor + farColor;
                    localMode = false;
                    printf("[colorSort] inconclusive read (near=%s far=%s) - resuming via keep(), no lookup called\n",
                           nearColor.c_str(), farColor.c_str());
                    keep();
                    didClassify = true;
                }
            } else if (!gateSeesObject) {
                objectBeingProcessed = false; // gate is clear again; ready for the next object
            }

            dataMutex.take();
            hue1 = h1; hue2 = h2; hue3 = h3;
            sat1 = s1; sat2 = s2; sat3 = s3;
            prox1 = p1; prox2 = p2; prox3 = p3;
            Color1 = c1; Color2 = c2; Color3 = c3;
            if (didClassify) {
                Object = localObject;
                mode = localMode;
            }
            dataMutex.give();

            pros::delay(20);
        }
    });
}