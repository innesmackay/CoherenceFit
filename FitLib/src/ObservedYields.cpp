#include "ObservedYields.hpp"
#include "Inputs.hpp"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"

#include <iostream>

ObservedYields::ObservedYields(){
    m_log = Log("ObservedYields");
}

ObservedYields::~ObservedYields(){}

TVectorD* ObservedYields::GetMaxErrs(TVectorD* loErrs, TVectorD* hiErrs){
    if (m_debug){ m_log.debug("Getting maximum of MINOS errors"); }
    TVectorD* errs = new TVectorD(5);
    for (int i=0; i<5; i++){
        (*errs)[i] = std::max( (*loErrs)[i], (*hiErrs)[i] );
    }
    if (m_debug){ m_log.debug("================"); errs->Print("v"); }
    return errs;
}

TVectorD* ObservedYields::Load(std::string double_tag, std::string hist_to_load){
    std::string root_filename = "/home/gilman/analysis/BESIII/StrongPhaseWork/fitYields/" + double_tag + "_combined_Yields.root";
    if (double_tag == "SSKPi_vs_KPi"){ root_filename = "/home/gilman/analysis/BESIII/StrongPhaseWork/SSKPi/fitYields/KPi_vs_KPi_combined_Yields.root"; }
    if (m_debug){ m_log.info("Reading the " + double_tag + " " + hist_to_load + " from " + root_filename); }
    TFile root_file(root_filename.c_str());
    TH1F* yields_hist = (TH1F*) root_file.Get(hist_to_load.c_str());
    TVectorD* yields = new TVectorD(5);
    for (int i=0; i<5; i++){
        if (hist_to_load.find("Errs") != std::string::npos) { (*yields)[i] = TMath::Abs(yields_hist->GetBinContent(i+1)); }
        else{ (*yields)[i] = yields_hist->GetBinContent(i+1); }
    }
    if (m_debug){ m_log.debug(double_tag + " vector\n================"); yields->Print("v"); }
    return yields;
}

void ObservedYields::LoadAll(){
    Constants c;
    // Double CP tags
    for (auto mode : c.DOUBLE_CP_MODES){
        std::string D1 = mode.at(0);
        std::string D2 = mode.at(1);
        std::string DT = D1 + "_vs_" + D2;
        TVectorD* yields = Load(DT, "DT_Yields");
        TVectorD* hiErrs = Load(DT, "DT_YieldHiErrs");
        TVectorD* loErrs = Load(DT, "DT_YieldLoErrs");
        m_all_yields[DT] = yields;
        m_all_errors[DT] = GetMaxErrs(loErrs, hiErrs);
    }

    // OS KPi vs KPi
    m_all_yields["KPi_vs_KPi"] = Load("KPi_vs_KPi", "DT_Yields");
    m_all_errors["KPi_vs_KPi"] = Load("KPi_vs_KPi", "DT_YieldErrs");

    // LS KPi vs KPi
    m_all_yields["SSKPi_vs_KPi"] = Load("SSKPi_vs_KPi", "DT_Yields");
    TVectorD* LS_hiErrs = Load("SSKPi_vs_KPi", "DT_YieldHiErrs");
    TVectorD* LS_loErrs = Load("SSKPi_vs_KPi", "DT_YieldLoErrs");
    m_all_errors["SSKPi_vs_KPi"] = GetMaxErrs(LS_loErrs, LS_hiErrs);
    
    return;
}
