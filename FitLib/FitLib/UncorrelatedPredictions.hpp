#ifndef UNCORRELATEDPREDICTIONS_H
#define UNCORRELATEDPREDICTIONS_H

#include "Log.hpp"
#include "TMatrixD.h"

#include <ostream>
#include <map>

class UncorrelatedPredictions {

public:

    UncorrelatedPredictions(std::string D1_string, std::string D2_string);
    ~UncorrelatedPredictions();
    TMatrixD CalculatePredictions();

    void SetDebug(bool debug){ m_debug = debug; }

private:
    std::string m_double_tag;
    std::string m_D1;
    std::string m_D2;
    bool m_debug = false;
    Log m_log;

};

#endif //  UncorrelatedPredictions_H
