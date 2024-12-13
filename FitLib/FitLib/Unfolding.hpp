#ifndef UNFOLDING_H
#define UNFOLDING_H

#include "Settings.hpp"
#include "Log.hpp"
#include "TMatrixD.h"
#include "TVectorD.h"
#include <map>

class Unfolding {

public:

    Unfolding(Settings settings){
        m_settings = settings;
        m_log = Log("Unfolding");
    }
    Unfolding(){
        m_log = Log("Unfolding");
    }
    ~Unfolding(){}

    TMatrixD GetUnfoldingMatrix(std::string double_tag);
    TVectorD GetRowOfUnfoldingMatrix(std::string double_tag, int rowIndex){
        TVectorD row(5);
        TMatrixD m = this->GetUnfoldingMatrix(double_tag);
        row = TMatrixDRow(m, rowIndex);
        return row;
    }

    TMatrixD GetInverseUnfoldingMatrix(std::string double_tag){ return this->GetUnfoldingMatrix(double_tag).Invert(); }
    TVectorD FoldYields(TVectorD* yields_by_prod, std::string double_tag);
    TVectorD UnfoldYields(TVectorD* yields_by_prod, std::string double_tag);

    void SetDebug(bool debug){ m_debug = debug; }

private:
    Settings m_settings;
    Log m_log;
    bool m_debug = false;

};

#endif //  Unfolding_H
