#include "Likelihood.hpp"
#include "UncorrelatedPredictions.hpp"
#include "Unfolding.hpp"

#include "TMath.h"

Likelihood::Likelihood(ObservedYields* yields, double kspi_correction, bool include_ss, bool poisson_fit){
    m_kspi_correction = kspi_correction;
    m_yields = yields;
    m_poisson = poisson_fit;
    m_include_ss = include_ss;
    m_log = Log("Likelihood");
}


double Likelihood::operator()(const double* par){

    // Setup fit parameters
    double logL = 0;

    // Add contributions from double CP tags
    for (auto channel : m_constants.DOUBLE_CP_MODES){

        // Get observed yields
        std::string D1 = channel.at(0);
        std::string D2 = channel.at(1);
        std::string double_tag = D1 + "_vs_" + D2;
        if (m_debug){ m_log.debug("Adding the " + double_tag + " contribution to the likelihood"); }
        TVectorD* tmp_observed_yields = m_yields->GetYields( double_tag );
        TVectorD* tmp_observed_errors = m_yields->GetErrors( double_tag );

        // Calculate predictions
        double correction = 1;
        bool cp_even;
        if (D1 == "PiPiPi0" | D2 == "PiPiPi0"){
            correction = m_constants.FPLUS / (1 + m_kspi_correction);
            if (m_debug){ m_log.warning("Double tag contains PiPiPi0; correction kappa by a factor of " + std::to_string(correction)); }
        }
        TVectorD kappa_vec_to_use(5);
        if ( m_constants.CP_VALS[D1] * m_constants.CP_VALS[D2] > 0 ){ 
            kappa_vec_to_use(0) = 1 - (par[0]*correction); // D0D0
            kappa_vec_to_use(1) = (par[1]*correction);     // g D0D0
            kappa_vec_to_use(2) = 1 - (par[2]*correction); // pi D0D0
            kappa_vec_to_use(3) = (par[3]*correction);     // g pi D0D0
            kappa_vec_to_use(4) = 1 - (par[4]*correction); // gg/pipi D0D0
            cp_even = true;
        }
        else{
            kappa_vec_to_use(0) = (par[0]*correction);     // D0D0
            kappa_vec_to_use(1) = 1 - (par[1]*correction); // g D0D0
            kappa_vec_to_use(2) = (par[2]*correction);     // pi D0D0
            kappa_vec_to_use(3) = 1 - (par[3]*correction); // g pi D0D0
            kappa_vec_to_use(4) = (par[4]*correction);     // gg/pipi D0D0
            cp_even = false;
        }

        UncorrelatedPredictions up(D1, D2);
        up.SetDebug(m_debug);
        auto predicted_contributions = up.CalculatePredictions();
        for (int i=0; i<5; i++){

            double Nobs = (*tmp_observed_yields)[i];
            double e_Nobs = (*tmp_observed_errors)[i];

            // Get row i (contributions to prod i)
            TVectorD row(5);
            // row = TMatrixDRow(predicted_contributions, i);
            row = TMatrixDColumn(predicted_contributions, i);

            // Calculate predicted yields
            double N_pred = row * kappa_vec_to_use;
            N_pred *= 2;

            if (m_debug){
                m_log.debug("Value of kappa for process " + std::to_string(i) + ": " + std::to_string(par[i]));
                m_log.debug("Predicted yield for process " + std::to_string(i) + ": " + std::to_string(N_pred));
                m_log.debug("Observed yield for process " + std::to_string(i) + ": " + std::to_string(Nobs) + " +/- " + std::to_string(e_Nobs));
            }

            // Add conribution to likelihood
            if (m_poisson){
                m_log.error("POISSON FIT DOES NOT WORK YET");
                if (N_pred<0){ N_pred = 0; }
                logL += Nobs*TMath::Log(N_pred) - N_pred;
            }
            else{
                double pull = (Nobs - N_pred)/e_Nobs;
                logL -= TMath::Power(pull, 2);
            }
        }
    }

    // Add SS contribution
    Unfolding unfolding;
    unfolding.SetDebug(m_debug);
    if (m_include_ss){

        m_log.warning("Uncertainty on unfolded OS and LS yields given by relative errors from the observed yields");

        // Get unfolded OS yields
        TVectorD* tmp_observed_OS_yields = m_yields->GetYields( "KPi_vs_KPi" );
        TVectorD* tmp_observed_OS_errors = m_yields->GetErrors( "KPi_vs_KPi" );
        auto tmp_unfolded_OS_yields = unfolding.UnfoldYields(tmp_observed_OS_yields, "KPi_vs_KPi");
        
        // Get unfolded SS yields
        TVectorD* tmp_observed_LS_yields = m_yields->GetYields( "SSKPi_vs_KPi" );
        TVectorD* tmp_observed_LS_errors = m_yields->GetErrors( "SSKPi_vs_KPi" );
        auto tmp_unfolded_LS_yields = unfolding.UnfoldYields(tmp_observed_LS_yields, "KPi_vs_KPi");

        for (auto prod: m_constants.PRODS){

            int i = m_constants.PROD_INDEX[prod];

            double observed_ratio = tmp_unfolded_LS_yields(i) / tmp_unfolded_OS_yields(i);
            double unfolded_OS_rel_error = (*tmp_observed_OS_errors)[i] / (*tmp_observed_OS_yields)[i];
            double unfolded_LS_rel_error = (*tmp_observed_LS_errors)[i] / (*tmp_observed_LS_yields)[i];
            double observed_ratio_error = TMath::Sqrt( unfolded_LS_rel_error*unfolded_LS_rel_error + unfolded_OS_rel_error*unfolded_OS_rel_error );

            double predicted_ratio = 2*m_constants.RKPI_SQUARED*(1 + (2*par[i] - 1)*m_constants.C_PROD[prod])*(1 + (1/m_constants.RKPI)*( m_constants.XMIX*TMath::Cos(m_constants.DKPI_RAD) - m_constants.YMIX*TMath::Sin(m_constants.DKPI_RAD) ));
            if (m_debug){
                m_log.debug("Observed LS/OS ratio: " + std::to_string(observed_ratio) + " +/- " + std::to_string(observed_ratio_error));
                m_log.debug("Predicted LS/OS ratio: " + std::to_string(predicted_ratio));
            }
            double pull = (observed_ratio - predicted_ratio)/observed_ratio_error;
            logL -= TMath::Power(pull, 2);

        }

    }


    return -1*logL;
}
