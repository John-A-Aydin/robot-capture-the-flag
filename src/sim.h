#include <utility>
#include <math.h>
#include <numbers>
namespace sim {

double radius(std::pair<double, double> p);

double distance(std::pair<double, double> p1, std::pair<double,double> p2);

std::pair<double, double> generate_point();

std::pair<double, double> robot1_move(double theta);

std::pair<double, double> robot2_move(double r);

std::pair<double, double> convert_to_polar(std::pair<double, double>);

}

