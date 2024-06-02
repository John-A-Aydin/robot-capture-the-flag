#include <iostream>
#include <utility>
#include <math.h>
#include <iomanip>
#include <cstring>
#include <chrono>
#include "sim.h"

using std::cout;
using std::endl;
using std::pair;


#define const_r 0.501306994212753

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
    auto start = std::chrono::high_resolution_clock::now();
    double delta = pow(0.5, accuracy);

    long int robot_1_wins = 0;
    long int robot_2_wins = 0;
    for (double x = 0.0; x <= 1.0; x += delta) {
        for (double y = 0.0; y <= 1.0; y += delta) {
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
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    cout << endl << std::setprecision(10) << "Robot 1 winrate: "<< robot1_winrate << endl << "Robot 2 winrate: " << robot2_winrate << endl;

    cout << "Trials: " << trials << endl;
    cout << "Took:   " << duration.count() << "ms" << endl;

    return 0;
}