#include "Settings.hpp"
#include "ObservedYields.hpp"
#include "Fitter.hpp"

#include "UncorrelatedPredictions.hpp"

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

}
