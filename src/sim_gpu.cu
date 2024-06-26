#include <math.h>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <chrono>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

using std::endl;
using std::cout;

#define THREADS 512

inline constexpr double R_1  = 0.501306994212753;

__global__ void simChunk(double* y_min, double* y_max, long int* r1_wins, long int* r2_wins, long int* trials, double delta) {
    int i = threadIdx.x;
    for (double x = 0.0; x <= 1.0; x += delta) {
        for (double y = y_min[i]; y <= y_max[i]; y += delta) {
            double r_target = sqrt(x*x + y*y);
            // Outside of game area
            if (r_target > 1.0) continue;
            trials[i]++;
            // Robot 2 wins by default
            if (r_target <= R_1/2) {
                r2_wins[i]++;
                continue;
            }
            double r1_distance = abs(r_target - R_1);
            // Robot 2's optimal strategy given the target's radius
            double r2_x = sqrt((2*r_target*R_1) - (R_1*R_1));
            double r2_distance = sqrt((x - r2_x)*(x - r2_x) + y*y);
            if (r2_distance <= r1_distance) {
                r2_wins[i]++;
            } else {
                r1_wins[i]++;
            }
        }   
    }
}

int main(int argc, char* argv[]) {
    int accuracy = 10;
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

    double y_min[THREADS] = {0};
    double y_max[THREADS] = {0};
    long int r1_wins[THREADS] = {0};
    long int r2_wins[THREADS] = {0};
    long int trials[THREADS] = {0};

    for (int i = 0; i < THREADS; i++) {
        y_min[i] = (1.0/THREADS)*(double)i;
        y_max[i] = (1.0/THREADS)*(double)(i+1) - delta;
    }
    y_max[THREADS] = 1.0;
    
    double* cuda_y_min = 0;
    double* cuda_y_max = 0;
    long int* cuda_r1_wins = 0;
    long int* cuda_r2_wins = 0;
    long int* cuda_trials = 0;

    cudaMalloc(&cuda_y_min, sizeof(y_min));
    cudaMalloc(&cuda_y_max, sizeof(y_max));
    cudaMalloc(&cuda_r1_wins, sizeof(r1_wins));
    cudaMalloc(&cuda_r2_wins, sizeof(r2_wins));
    cudaMalloc(&cuda_trials, sizeof(trials));

    cudaMemcpy(cuda_y_min, y_min, sizeof(y_min), cudaMemcpyHostToDevice);
    cudaMemcpy(cuda_y_max, y_max, sizeof(y_max), cudaMemcpyHostToDevice);

    simChunk <<< 1, THREADS >>> (cuda_y_min, cuda_y_max, cuda_r1_wins, cuda_r2_wins, cuda_trials, delta);

    cudaMemcpy(r1_wins, cuda_r1_wins, sizeof(r1_wins), cudaMemcpyDeviceToHost);
    cudaMemcpy(r2_wins, cuda_r2_wins, sizeof(r2_wins), cudaMemcpyDeviceToHost);
    cudaMemcpy(trials, cuda_trials, sizeof(trials), cudaMemcpyDeviceToHost);

    long int r1_total, r2_total, total_trials = 0;

    for (int i = 0; i < THREADS; i++) {
        r1_total += r1_wins[i];
        r2_total += r2_wins[i];
        total_trials += trials[i];
    }

    double temp = R_1*R_1/8;

    double robot1_winrate = r1_total/(double)(total_trials*2) + 0.5 - temp;
    double robot2_winrate = r2_total/(double)(total_trials*2) + temp;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    
    cout << endl << std::setprecision(10) << "Robot 1 winrate: "<< robot1_winrate << endl << "Robot 2 winrate: " << robot2_winrate << endl;
    cout << "Trials: " << total_trials << endl;
    cout << "Took:   " << duration.count() << "ms" << endl;
    return 0;
}