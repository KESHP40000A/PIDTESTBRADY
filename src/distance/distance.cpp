#include "main.h" // IWYU pragma: export
#include "setup/config.h"
#include "setup/distance.hpp"


using namespace pros;

Lidar::Lidar(std::uint8_t port, double _xOffset, double _yOffset, double _angle) : 
    pros::Distance(port), 
    xOffset(_xOffset), 
    yOffset(_yOffset), 
    dOffset(std::hypot(xOffset, yOffset)),
    phi((_angle == 0 || _angle == 180)
        ? std::atan(yOffset / xOffset) 
        : std::atan(xOffset / yOffset)),
    angle(_angle) {}

double Lidar::get2() {
    return this->pros::Distance::get() / 25.4;
}

double Lidar::get_distance2() {
    return this->pros::Distance::get_distance() / 25.4;
}