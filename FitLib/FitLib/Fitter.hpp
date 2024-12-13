#ifndef FITTER_H
#define FITTER_H

#include "Settings.hpp"
#include "ObservedYields.hpp"
#include "Inputs.hpp"
#include "Log.hpp"

#include "Minuit2/Minuit2Minimizer.h"

#include <map>
#include <vector>

class Fitter {

public:

    // Initialisation
    Fitter(Settings s, ObservedYields* yields);
    ~Fitter();
    void RunFit();
    Int_t SetupParameters();

    // Other
    double GetKappa(std::string prod){ return m_minuit->X()[ m_constants.PROD_INDEX[prod] ]; }
    double GetKappaHesseError(std::string prod){ return m_minuit->Errors()[ m_constants.PROD_INDEX[prod] ]; }
    double GetMinChiSquared(){ return m_minuit->MinValue(); }

    void SetDebug(bool debug){ m_debug = debug; }

private:
    Log m_log;
    Settings m_settings;
    Constants m_constants;
    ObservedYields* m_yields;
    ROOT::Minuit2::Minuit2Minimizer* m_minuit = new ROOT::Minuit2::Minuit2Minimizer();
    bool m_debug = false;
};

#endif //  Fitter_H
