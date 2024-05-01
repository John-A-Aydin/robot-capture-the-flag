#include <iostream>
#include <utility>
#include <math.h>
#include <numbers>

using std::cout;
using std::endl;
using std::pair;
using std::numbers::sqrt2;


double distance_squared(pair<double, double>, pair<double, double>);
double radius_squared(pair<double, double>);

pair<double, double> generate_point();
pair<double, double> robot_1(double theta);
pair<double, double> robot_2(double r);

// struct uniform_point_generator {
//     double delta;

// }

int main() {
    int trials = 1000000000;
    int robot_1_wins = 0;
    for (int i = 0; i < trials; i++) {
        pair<double, double> target = generate_point();
        double r = sqrt(radius_squared(target));
        double theta = atan(target.second/target.first);
        pair<double, double> robot_a = robot_1(theta);
        pair<double, double> robot_b = robot_2(r);
        double robot_1_distance = distance_squared(robot_a, target);
        double robot_2_distance = distance_squared(robot_b, target);

        // cout << " Target - (" << target.first << "," << target.second << ")" << endl;
        // cout << " Robo_1 - (" << robot_a.first << "," << robot_a.second << ")" << endl;
        // cout << " Robo_2 - (" << robot_b.first << "," << robot_b.second << ")" << endl;
        // cout << endl;

        if (robot_1_distance < robot_2_distance) {
            robot_1_wins++;
            //cout << "1 wins!" << endl << "------------------------------------------------------" << endl << endl;
        }
        
    }
    double percent = robot_1_wins/(double)trials;
    cout << endl << percent << endl;

    return 0;
}

double distance_squared(pair<double, double> p1, pair<double, double> p2) {
    return (p1.first - p2.first)*(p1.first - p2.first)
        + (p1.second - p2.second)*(p1.second - p2.second);
}


double radius_squared(pair<double, double> p) {
    return p.first*p.first + p.second*p.second;
}

// Generates a random point in the unit circle.
pair<double, double> generate_point() {
    double n;
    double radius = 2;
    pair<double, double> point;
    while (radius > 1.0) {
        n = std::rand();
        point.first = n/RAND_MAX;
        n = std::rand();
        point.second = n/RAND_MAX;
        radius = radius_squared(point);
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
pair<double, double> robot_1(double theta) {
    double r = sqrt2/2;
    pair<double, double> p = std::make_pair(r*cos(theta), r*sin(theta));
    return p;
}

pair<double, double> robot_2(double r) {
    double theta = 0.0;
    double robot_r;
    if (r <= sqrt2/4) {
        return std::make_pair(0.0, 0.0);
    } else {
        robot_r = sqrt(sqrt2*r-0.5);
    }
    return std::make_pair(robot_r, 0.0);
}

