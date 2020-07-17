#include "cyTrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "SDManager.hh"
#include "G4ios.hh"

cyTrackerSD::cyTrackerSD(const G4String& name,
			 const G4String& hitsCollectionName)
    : G4VSensitiveDetector(name), fHitsCollection(NULL) {
    collectionName.insert(hitsCollectionName);
}

cyTrackerSD::~cyTrackerSD() {}

void cyTrackerSD::Initialize(G4HCofThisEvent* hce) {
    fHitsCollection
	= new cyTrackerHitsCollection(SensitiveDetectorName, collectionName[0]);

    G4int hcID
	= G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool cyTrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    // energy deposit
    G4double edep = aStep->GetTotalEnergyDeposit();

    if(edep == 0.) return false;
    
    cyTrackerHit* newHit = new cyTrackerHit();

    // energy spectrum of secondaries and tof
    G4double energy = aStep->GetTrack()->GetKineticEnergy();
    G4double tof    = aStep->GetTrack()->GetGlobalTime();
    G4bool  charged = (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.);

    newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
    newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()
			 ->GetCopyNumber());
    newHit->SetEdep(edep);
    G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
    newHit->SetPos ( pos );
    newHit->SetTime( tof );

    fHitsCollection->insert( newHit );

    return true;
}

void cyTrackerSD::EndOfEvent(G4HCofThisEvent*) {
    if(verboseLevel > 1) {
	G4int nofHits = fHitsCollection->entries();
    }
}
