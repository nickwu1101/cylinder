#ifndef cyRunAction_h
#define cyRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class cyEventAction;
class G4Run;

const G4int MaxNtCol = 10;

class cyRunAction : public G4UserRunAction {
public:
    cyRunAction(cyEventAction* eventAction);
    virtual ~cyRunAction();

    virtual void BeginOfRunAction(const G4Run* run);
    virtual void   EndOfRunAction(const G4Run* run);

private:
    cyEventAction* fEventAction;
    G4String       fFileName;
    G4int          fNtColId[MaxNtCol];
};

#endif
