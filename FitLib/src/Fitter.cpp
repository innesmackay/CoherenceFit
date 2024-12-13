#include "Fitter.hpp"
#include "Likelihood.hpp"

#include "Math/Functor.h"
#include "Math/Functions.h"
#include "Minuit2/FCNBase.h"

Fitter::Fitter(Settings s, ObservedYields* yields){
    m_settings = s;
    m_yields = yields;
    m_log = Log("Fitter");
}

Fitter::~Fitter(){}

void Fitter::RunFit(){

    m_log.info("Initialisng the likelihood fit");

    // Other details
    m_minuit->SetPrintLevel(3);
    m_minuit->SetStrategy(2);

    // Setup parameters
    Int_t nPars = this->SetupParameters();

    // Set the likelihood function
    Likelihood* L = new Likelihood(m_yields, m_settings.getD("kspi_correction"), m_settings.getB("include_ss"), m_settings.getB("poisson_likelihood"));
    L->SetDebug(m_debug);
    ROOT::Math::Functor fcn(*L, nPars);
    m_minuit->SetFunction(fcn);

    m_log.success("Minimising ...");
    m_minuit->Minimize();

    m_log.success("Running HESSE ...");
    m_minuit->Hesse();

    // std::cout << "Running MINOS" << std::endl;
    // m_minuit->GetMinosError(0, m_minos_errLo, m_minos_errHi);

    return;
}

Int_t Fitter::SetupParameters(){
    m_log.info("Initialisng the input parameters");
    Int_t parCounter = 0;
    for (auto prod : m_constants.PRODS){
        m_minuit->SetVariable(parCounter, "Kappa_" + prod, 0.95, 0.0002);
        m_minuit->SetVariableLimits(parCounter, 0.5, 1.5);
        if (m_debug){ m_log.debug("Defining parameter " + std::to_string(parCounter)); }
        parCounter += 1;
    }
    return parCounter;
}