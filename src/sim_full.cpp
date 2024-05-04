#include <iostream>
#include <utility>
#include <math.h>
#include <numbers>
#include <iomanip>

using std::cout;
using std::endl;
using std::pair;
using std::numbers::sqrt2;
using std::numbers::pi;

#define const_r 0.501306994212753


double distance(pair<double, double>, pair<double, double>);
double radius(pair<double, double>);
void convert_to_polar(double x, double y, double &r, double &theta);

pair<double, double> generate_point();
pair<double, double> robot_1(double theta);
pair<double, double> robot_2(double r);


int main() {
    double delta = pow(0.5, 9);
    int trials = 0;
    
    int robot_1_wins = 0;
    int robot_2_wins = 0;
    for (double x = -1.0; x <= 1.0; x += delta) {
        for (double y = -1.0; y <= 1.0; y += delta) {
            pair<double, double> target = std::make_pair(x,y);
            double r;
            double theta;
            convert_to_polar(x, y, r, theta);
            
            if (r > 1.0) continue;
            trials ++;
            pair<double, double> robot_1_p = robot_1(theta);
            pair<double, double> robot_2_p = robot_2(r);
            double robot_1_distance = distance(robot_1_p, target);
            double robot_2_distance = distance(robot_2_p, target);

            if (robot_1_distance < robot_2_distance) {
                robot_1_wins++;
            } else {
                robot_2_wins++;
            }
        }
        //cout << "row done - x = " << x << endl;
        
    }

    double robot1_winrate = robot_1_wins/(double)trials;
    double robot2_winrate = robot_2_wins/(double)trials;
    
    cout << endl << std::setprecision(10) << "Robot 1: "<< robot1_winrate << endl << "Robot 2: " << robot2_winrate << endl;

    return 0;
}

double distance(pair<double, double> p1, pair<double, double> p2) {
    return sqrt((p1.first - p2.first)*(p1.first - p2.first) + (p1.second - p2.second)*(p1.second - p2.second));
}


double radius(pair<double, double> p) {
    double t1 = p.first*p.first;
    double t2 = p.second*p.second;
    return sqrt(t1 + t2);
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
    double r = const_r;
    pair<double, double> p = std::make_pair(r*cos(theta), r*sin(theta));
    return p;
}

// Robot 2 knows the radius of the target point on the unit circle.
pair<double, double> robot_2(double r) {
    double theta = 0.0;
    double robot_r;
    if (r <= const_r/2) {
        return std::make_pair(0.0, 0.0);
    } else {
        robot_r = sqrt((2*r*const_r) - (const_r*const_r));
    }
    return std::make_pair(robot_r, 0.0);
}

void convert_to_polar(double x, double y, double &r, double &theta) {
    r = radius(std::make_pair(x,y));
    theta = atan(y/x);
    if (x == 0 && y == 0) {  // origin
        theta = 0;
    } else if (x < 0) { // Q2 & Q3
        theta += pi;
    } else if (x > 0 && y < 0) { // Q4
        theta += 2*pi;
    }
    return;
}