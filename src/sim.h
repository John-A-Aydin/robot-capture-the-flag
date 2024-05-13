#include <utility>
#include <math.h>
#include <numbers>
#include <ctime>
#include <cstdlib>
namespace sim {

inline constexpr double R_1 = 0.501306994212753;

double radius(std::pair<double, double> p);

double distance(std::pair<double, double> p1, std::pair<double,double> p2);

double radius_squared(std::pair<double, double> p);

double distance_squared(std::pair<double, double> p1, std::pair<double, double> p2);

std::pair<double, double> generate_point();

std::pair<double, double> robot1_move(double theta);

std::pair<double, double> robot2_move(double r);

std::pair<double, double> robot2_move_const(double r);

std::pair<double, double> convert_to_polar(std::pair<double, double>);

std::pair<double, double> convert_to_cartesian(std::pair<double, double>);

}

