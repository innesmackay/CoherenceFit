#ifndef LIKELIHOOD_H
#define LIKELIHOOD_H

#include "ObservedYields.hpp"
#include "Inputs.hpp"
#include "Log.hpp"

#include <map>

class Likelihood {

public:
    Likelihood(ObservedYields* yields, double kspi_correction, bool include_ss, bool poisson_fit);
    double operator()(const double* x);
    void SetDebug(bool debug){ m_debug = debug; }

private:
    Constants m_constants;
    ObservedYields* m_yields;
    double m_kspi_correction = 0;
    bool m_include_ss = true;
    bool m_poisson = false;
    bool m_debug = false;
    Log m_log;

};

#endif //  Likelihood_H
