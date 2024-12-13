#include "UncorrelatedPredictions.hpp"
#include "Inputs.hpp"
#include "Unfolding.hpp"
#include "ObservedYields.hpp"


UncorrelatedPredictions::UncorrelatedPredictions(std::string D1_string, std::string D2_string){
    m_double_tag = D1_string + "_vs_" + D2_string;
    m_D1 = D1_string;
    m_D2 = D2_string;
    m_log = Log("UncorrelatedPredictions");
}


UncorrelatedPredictions::~UncorrelatedPredictions(){}


TMatrixD UncorrelatedPredictions::CalculatePredictions(){

    if (m_debug){ m_log.info("Calculating the uncorrelated predictions for the " + m_double_tag + " final state"); }

    // Load in the KPi vs KPi yields
    ObservedYields obs;
    TVectorD* KPi_vs_KPi_yields = obs.Load("KPi_vs_KPi", "DT_Yields");

    // Unfold the KPi vs KPi yields
    Unfolding unfolding;
    unfolding.SetDebug(m_debug);
    auto unfolded_KPi_vs_KPi_yields = unfolding.UnfoldYields(KPi_vs_KPi_yields, "KPi_vs_KPi");

    // Compute the contributions
    auto DT_effs = unfolding.GetUnfoldingMatrix(m_double_tag);
    TMatrixD contributions(5, 5);
    for (int i=0; i<5; i++){
        for (int j=0; j<5; j++){
            contributions(i,j) = DT_effs(i,j)*unfolded_KPi_vs_KPi_yields(j);
        }
    }

    // Multiply by BF etc.
    Constants c;
    double correction_factor = 2 * (c.BF[m_D1] * c.BF[m_D2]) / (c.BF["KPi"] * c.BF["KPi"]);
    if (m_D1 == m_D2){ correction_factor *= 0.5; }
    contributions *= correction_factor;
    if (m_debug){
        m_log.debug("Predicted uncorrelated yields\n====================");
        contributions.Print("v");
    }

    return contributions;

}
