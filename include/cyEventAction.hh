#ifndef cyEventAction_h
#define cyEventAction_h 1

#include "G4UserEventAction.hh"

#include "globals.hh"

#include <vector>

using std::vector;

class cyEventAction : public G4UserEventAction {
public:
    cyEventAction();
    virtual ~cyEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);

    vector<G4double>& getE_HitCore() { return E_HitCore; };
    vector<G4double>& getXCore    () { return XCore; };
    vector<G4double>& getYCore    () { return YCore; };
    vector<G4double>& getZCore    () { return ZCore; };
    vector<G4double>& gettimeCore () { return timeCore; };

private:
    vector<G4double> E_HitCore;
    vector<G4double> XCore;
    vector<G4double> YCore;
    vector<G4double> ZCore;
    vector<G4double> timeCore;
};

#endif
