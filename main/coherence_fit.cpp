#include "Settings.hpp"
#include "ObservedYields.hpp"
#include "Fitter.hpp"

#include "TMath.h"

int main(int argc , char* argv[]){

    Log log("main");

    // Fit config
    std::string settings_file = argv[1];
    Settings set(settings_file);
    set.read();

    // Load observed yields
    ObservedYields* obs = new ObservedYields();
    obs->SetDebug(set.getB("debug"));
    obs->LoadAll();

    // Set up and run likelihood fit
    Fitter* fit = new Fitter(set, obs);
    fit->SetDebug(set.getB("debug"));
    fit->RunFit();
    log.success("Fit complete!");
    log.success("Chi squared: " + std::to_string(fit->GetMinChiSquared()));
    log.success("Number of observed yields: 30");
    log.success("Number of free parameters: 5");
    log.success("Reduced chi2: " + std::to_string( fit->GetMinChiSquared() / 25 ));
    log.success("p-value: " + std::to_string(TMath::Prob(fit->GetMinChiSquared(), 25)));

}
