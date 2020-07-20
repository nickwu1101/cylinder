#include "cyEventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"

#include "cyTrackerHit.hh"
#include "cyTrackerSD.hh"

#include "globals.hh"
#include "g4root.hh"
#include "g4analysis.hh"

#include <vector>

using std::vector;

cyEventAction::cyEventAction() : G4UserEventAction() {}

cyEventAction::~cyEventAction() {}

void cyEventAction::BeginOfEventAction(const G4Event*) {
    E_HitCore.clear();
    XCore.clear();
    YCore.clear();
    ZCore.clear();
    timeCore.clear();
}

void cyEventAction::EndOfEventAction(const G4Event* event) {
    G4int eventID = event->GetEventID();
    if(eventID % 100000 == 0)
	G4cout << "Now filled event " << eventID << G4endl;

    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    if(!hce) {
	G4ExceptionDescription msg;
	msg << "No hits collection of this event (" << eventID << ") found.\n";
	G4Exception("EventAction::EndOfEventAction()", "Code001", JustWarning, msg);
	return;
    }

    cyTrackerHitsCollection* hHCcore = static_cast<cyTrackerHitsCollection*>(hce->GetHC(0));

    if(!hHCcore) {
	G4ExceptionDescription msg;
	msg << "Some of hits collections of this event (" << eventID << ") not found.\n";
	G4Exception("EventAction::EndOfEventAction()", "Code001", JustWarning, msg);
	return;
    }

    auto analysisManager = G4Analysis::ManagerInstance("root");

    G4int n_hitCore = hHCcore->entries();

    G4double totalEcore = 0.;

    for(G4int i = 0; i < n_hitCore; i++) {
	cyTrackerHit* hit = (*hHCcore)[i];
	G4double eDep = hit->GetEdep();
	totalEcore += eDep;
    }

    if(n_hitCore > 0) {
	analysisManager->FillNtupleIColumn(0, eventID);
	
	analysisManager->FillNtupleIColumn(1, n_hitCore);
	analysisManager->FillNtupleDColumn(2, totalEcore);
	
	for(G4int i = 0; i < n_hitCore; i++) {
	    cyTrackerHit* hit = (*hHCcore)[i];
	    G4double     eDep = hit->GetEdep();
	    G4double     tDep = hit->GetTime();
	    G4ThreeVector pos = hit->GetPos();
	    
	    E_HitCore.push_back(eDep);
	    XCore.push_back    (pos.x());
	    YCore.push_back    (pos.y());
	    ZCore.push_back    (pos.z());
	    timeCore.push_back (tDep);
	}
	
	analysisManager->AddNtupleRow(1);
    }
}
