#ifndef cyTrackerSD_h
#define cyTrackerSD_h 1

#include "G4SensitiveDetector.hh"

#include "cyTrackerHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class cyTrackerSD : public G4VSensitiveDetector {
public:
    cyTrackerSD(const G4String& name,
		const G4String& hitsCollectionName);
    virtual ~cyTrackerSD();

    // methods from base class
    virtual void   Initialize (G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent (G4HCofThisEvent* hitCollection);

private:
    cyTrackerHitsCollection* fHitsCollection;
};

#endif
