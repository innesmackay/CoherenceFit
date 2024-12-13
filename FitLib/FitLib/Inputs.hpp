#ifndef INPUTS_H
#define INPUTS_H

#include <map>
#include <vector>
#include <string>

class Constants {

public:
    const double YMIX = 0.00636;
    const double FPLUS = 0.9406;
    const double XMIX = 0.00398;
    const double RKPI = 0.05865;
    const double DKPI_RAD = 3.34405;
    const double RKPI_SQUARED = 0.05865*0.05865;
    std::map<std::string, double> BF = {{"KPi", 0.03947}, {"KSPi0", 0.01240*0.692}, {"PiPiPi0", 0.0149}, {"KK", 0.00408}, {"PiPi", 0.001454}};
    std::map<std::string, int> C_PROD = {{"D0D0", -1}, {"DST0D0_g", 1}, {"DST0D0_pi", -1}, {"DST0DST0_even", 1}, {"DST0DST0_odd", -1}};
    std::vector<std::string> PRODS = {"D0D0", "DST0D0_g", "DST0D0_pi", "DST0DST0_even", "DST0DST0_odd"};
    std::map<std::string, int> PROD_INDEX = {{"D0D0", 0}, {"DST0D0_g", 1}, {"DST0D0_pi", 2}, {"DST0DST0_even", 3}, {"DST0DST0_odd", 4}};
    std::vector<std::vector<std::string>> DOUBLE_CP_MODES = {{"PiPiPi0", "KSPi0"}, {"PiPiPi0", "KK"}, {"KSPi0", "KSPi0"}, {"KSPi0", "KK"}, {"KSPi0", "PiPi"} };
    std::map<std::string, int> CP_VALS = {{"PiPiPi0", 1}, {"PiPi", 1}, {"KSPi0", -1}, {"KK", 1}};

};

#endif //  Inputs_H