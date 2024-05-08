#include <utility>
#include <math.h>
#include <numbers>
#include <ctime>
#include <cstdlib>
#include "sim.h"

using std::pair;
using std::numbers::sqrt2;
using std::numbers::pi;
using std::rand;



namespace sim {

//inline constexpr double R_1  = 0.501306994212753;

double distance_squared(pair<double, double> p1, pair<double, double> p2) {
    return (p1.first - p2.first)*(p1.first - p2.first) + (p1.second - p2.second)*(p1.second - p2.second);
}

double distance(pair<double, double> p1, pair<double, double> p2) {
    return sqrt((p1.first - p2.first)*(p1.first - p2.first) + (p1.second - p2.second)*(p1.second - p2.second));
}

double radius_squared(pair<double, double> p) {
    return p.first*p.first + p.second*p.second;
}

double radius(pair<double, double> p) {
    double t1 = p.first*p.first;
    double t2 = p.second*p.second;
    return sqrt(t1 + t2);
}




// Generates a random point in the unit circle.
pair<double, double> generate_point() {
    double n;
    double r = 2;
    pair<double, double> point;
    std::srand(std::time(0));
    while (r > 1.0) {
        n = rand();
        point.first = n/RAND_MAX;
        n = rand();
        point.second = n/RAND_MAX;
        r = radius(point);
    }
    // Adding random signs
    int sign = rand() % 4;
    if (sign == 1) {
        point.first = -point.first;
    } else if (sign == 2) {
        point.second = -point.second;
    } else if (sign == 3) {
        point.first = -point.first;
        point.second = -point.second;
    }
    return point;
}

// Robot 1 knows the angle of the target point on the unit circle.
// It will use this to find the point that is on average closest to the target.
pair<double, double> robot1_move(double theta) {
    double r = R_1;
    pair<double, double> p = std::make_pair(r*cos(theta), r*sin(theta));
    return p;
}

pair<double, double> robot2_move(double r) {
    double theta = (rand()/(double)RAND_MAX)*2*pi;
    double robot_r;
    if (r <= R_1/2) {
        return std::make_pair(0.0, 0.0);
    } else {
        robot_r = sqrt((2*r*R_1) - (R_1*R_1));
    }

    return std::make_pair(robot_r*cos(theta), robot_r*sin(theta));
}

pair<double, double> robot2_move_const(double r) {
    double robot_r;
    if (r <= R_1/2) {
        return std::make_pair(0.0, 0.0);
    } else {
        robot_r = sqrt((2*r*R_1) - (R_1*R_1));
    }

    return std::make_pair(robot_r, 0.0);
}

pair<double, double> convert_to_polar(pair<double, double> p) {
    double r = radius(p);
    double theta = atan(p.second/p.first);
    if (p.first == 0 && p.second == 0) {  // origin
        theta = 0;
    } else if (p.first < 0) { // Q2 & Q3
        theta += pi;
    } else if (p.first > 0 && p.second < 0) { // Q4
        theta += 2*pi;
    }
    return std::make_pair(r, theta);

}

}