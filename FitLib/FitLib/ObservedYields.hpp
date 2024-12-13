#ifndef OBSERVEDYIELDS_H
#define OBSERVEDYIELDS_H

#include "Log.hpp"
#include "TVectorD.h"

#include <ostream>
#include <map>

class ObservedYields {

public:

    ObservedYields();
    ~ObservedYields();
    TVectorD* Load(std::string double_tag, std::string hist_to_load);
    void LoadAll();

    TVectorD* GetYields(std::string double_tag){ return m_all_yields[double_tag]; }
    TVectorD* GetErrors(std::string double_tag){ return m_all_errors[double_tag]; }

    void SetDebug(bool debug){ m_debug = debug; }

private:
    bool m_debug;
    Log m_log;
    std::map<std::string, TVectorD*> m_all_yields;
    std::map<std::string, TVectorD*> m_all_errors;
};

#endif //  ObservedYields_H