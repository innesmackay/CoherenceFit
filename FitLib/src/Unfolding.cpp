#include "Unfolding.hpp"
#include "TFile.h"
#include "TH2F.h"

#include <fstream>
#include <string>
#include <sstream>


TMatrixD Unfolding::GetUnfoldingMatrix(std::string double_tag){
    std::string root_filename = "/home/gilman/analysis/BESIII/StrongPhaseWork/rates/" + double_tag + "_combined_Rates.root";
    TFile root_file(root_filename.c_str());
    if (m_debug){ m_log.info("Reading the efficiencies from " + root_filename); }
    TH2F* eff_h = (TH2F*) root_file.Get("DT_Rates");
    TMatrixD unfoldingMatrix(5, 5);
    for (int i=0; i<5; i++){ for (int j=0; j<5; j++){ unfoldingMatrix(j,i) = eff_h->GetBinContent(i+1,j+1); } }
    return unfoldingMatrix;
}


TVectorD Unfolding::FoldYields(TVectorD* yields_by_prod, std::string double_tag){
    TMatrixD unfoldingMatrix = this->GetUnfoldingMatrix(double_tag);
    return unfoldingMatrix * (*yields_by_prod);
}


TVectorD Unfolding::UnfoldYields(TVectorD* yields_by_prod, std::string double_tag){
    TMatrixD unfoldingMatrix = this->GetUnfoldingMatrix(double_tag);
    return unfoldingMatrix.Invert() * (*yields_by_prod);
}
