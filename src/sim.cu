#include <utility>
#include <math.h>
#include <numbers>
#include <ctime>
#include <cstdlib>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "math_functions.h"


using std::pair;
using std::numbers::sqrt2;
using std::numbers::pi;
using std::rand;





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

pair<double, double> convert_to_cartesian(pair<double, double> p) {
    double x = p.first * cos(p.second);
    double y = p.first * sin(p.second);
    return std::make_pair(x, y);
}



__global__ void simChunk(double* x_min, double* x_max, double* y_min, double* y_max, long int* r1_wins, long int* r2_wins, double delta) {
    int i = threadIdx.x;
    for (double x = x_min[i]; x <= x_max[i]; x += delta) {
        for (double y = y_min[i]; y_max[i] <= 1.0; y += delta) {
            pair<double, double> target = std::make_pair(x,y);
            double r;
            double theta;
            pair<double, double> target_polar = sim::convert_to_polar(target);            
            if (target_polar.first > 1.0) continue;
            trials ++;
            pair<double, double> robot_1_p = sim::robot1_move(target_polar.second);
            pair<double, double> robot_2_p = sim::robot2_move_const(target_polar.first);
            double robot_1_distance = sim::distance_squared(robot_1_p, target);
            double robot_2_distance = sim::distance_squared(robot_2_p, target);

            if (robot_1_distance < robot_2_distance) {
                robot_1_wins++;
            } else {
                robot_2_wins++;
            }
        }   
    }
}

int main(int argc, char* argv[]) {
    int accuracy = 10;
    long int trials = 0;
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-a") == 0) {
            if (i + 1 == argc) {
                cout << "Accuracy not given: defaulting to 10" << endl;
            } else {
                try {
                    accuracy = std::stoi(argv[i+1]);
                    if (accuracy < 1) {
                        cout << "Invalid accuracy: defaulting to 10" << endl;
                        accuracy = 10;
                    }
                } catch (std::exception &err) {
                    cout << "Invalid accuracy: defaulting to 10" << endl;
                    accuracy = 10;
                }
            }
        }
    }
    
    double delta = pow(0.5, accuracy);

    long int robot_1_wins = 0;
    long int robot_2_wins = 0;
    for (double x = 0.0; x <= 1.0; x += delta) {
        for (double y = -1.0; y <= 1.0; y += delta) {
            pair<double, double> target = std::make_pair(x,y);
            double r;
            double theta;
            pair<double, double> target_polar = sim::convert_to_polar(target);            
            if (target_polar.first > 1.0) continue;
            trials ++;
            pair<double, double> robot_1_p = sim::robot1_move(target_polar.second);
            pair<double, double> robot_2_p = sim::robot2_move_const(target_polar.first);
            double robot_1_distance = sim::distance_squared(robot_1_p, target);
            double robot_2_distance = sim::distance_squared(robot_2_p, target);

            if (robot_1_distance < robot_2_distance) {
                robot_1_wins++;
            } else {
                robot_2_wins++;
            }
        }   
    }

    double temp = sim::R_1*sim::R_1/8;

    double robot1_winrate = robot_1_wins/(double)(trials*2) + 0.5 - temp;
    double robot2_winrate = robot_2_wins/(double)(trials*2) + temp;
    
    cout << endl << std::setprecision(10) << "Robot 1 winrate: "<< robot1_winrate << endl << "Robot 2 winrate: " << robot2_winrate << endl;

    return 0;
}