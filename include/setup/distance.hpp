// This file declares all objects defined in classes.cpp
#include "api.h" // IWYU pragma: export
#pragma once // tells the code to run this file only once
using namespace pros;

class Lidar : public pros::Distance {
public:
    double xOffset;
    double yOffset;
    double dOffset;
    double phi;
    double angle;

    Lidar(std::uint8_t port, double xOffset, double yOffset, double angle);

    double get2();

    double get_distance2();
};