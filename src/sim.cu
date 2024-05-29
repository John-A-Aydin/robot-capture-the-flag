#include <utility>
#include <math.h>
#include <numbers>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <cstring>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

using std::endl;
using std::cout;

#define THREADS 512

inline constexpr double R_1  = 0.501306994212753;

__global__ void simChunk(double* y_min, double* y_max, long int* r1_wins, long int* r2_wins, double delta) {
    int i = threadIdx.x;
    for (double x = 0.0; x <= 1.0; x += delta) {
        for (double y = y_min[i]; y <= y_max[i]; y += delta) {
            double r_target = sqrt(x*x + y*y);
            if (r_target > 1.0) continue; // Outside of game area
            if (r_target <= R_1/2) { // Robot 2 wins by default
                r2_wins[i]++;
                continue;
            }
            double theta = atan(y/x);
            double r1_distance = r_target - R_1;
            // Calculating distance between robot 2 and target
            double x_r2 = sqrt((2*r_target*R_1) - (R_1*R_1));
            double r2_distance = sqrt((x - x_r2)*(x - x_r2) + y*y);
            if (r2_distance < r1_distance) {
                r2_wins[i]++;
            } else {
                r1_wins[i]++;
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

    double y_min[THREADS] = {0};
    double y_max[THREADS] = {0};
    long int r1_wins[THREADS] = {0};
    long int r2_wins[THREADS] = {0};

    for (int i = 0; i < THREADS; i++) {
        y_min[i] = (1.0/THREADS)*(double)i;
        y_max[i] = (1.0/THREADS)*(double)(i+1) - delta;
    }
    y_max[THREADS] = 1.0;
    
    double* cuda_y_min = 0;
    double* cuda_y_max = 0;
    long int* cuda_r1_wins = 0;
    long int* cuda_r2_wins = 0;

    cudaMalloc(&cuda_y_min, sizeof(y_min));
    cudaMalloc(&cuda_y_max, sizeof(y_max));
    cudaMalloc(&cuda_r1_wins, sizeof(r1_wins));
    cudaMalloc(&cuda_r2_wins, sizeof(r2_wins));

    cudaMemcpy(cuda_y_min, y_min, sizeof(y_min), cudaMemcpyHostToDevice);
    cudaMemcpy(cuda_y_max, y_max, sizeof(y_max), cudaMemcpyHostToDevice);

    simChunk <<< 1, THREADS >>> (cuda_y_min, cuda_y_max, cuda_r1_wins, cuda_r2_wins, delta);

    cudaMemcpy(r1_wins, cuda_r1_wins, sizeof(r1_wins), cudaMemcpyDeviceToHost);
    cudaMemcpy(r2_wins, cuda_r2_wins, sizeof(r2_wins), cudaMemcpyDeviceToHost);
    long int r1_total, r2_total = 0;

    for (int i = 0; i < THREADS; i++) {
        r1_total += r1_wins[i];
        r2_total += r2_wins[i];
        trials = trials + r1_wins[i] + r2_wins[i];
    }

    double temp = R_1*R_1/8;

    double robot1_winrate = r1_total/(double)(trials*2) + 0.5 - temp;
    double robot2_winrate = r2_total/(double)(trials*2) + temp;
    //cout << r1_wins[0] << endl;
    cout << endl << std::setprecision(10) << "Robot 1 winrate: "<< robot1_winrate << endl << "Robot 2 winrate: " << robot2_winrate << endl;

    return 0;
}