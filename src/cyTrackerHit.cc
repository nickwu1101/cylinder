#include "cyTrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<cyTrackerHit>* cyTrackerHitAllocator = 0;

cyTrackerHit::cyTrackerHit() : G4VHit(),
			       fTrackID(-1),
			       fChamberNb(-1),
			       fEdep(0.),
			       fPos(G4ThreeVector()) {}

cyTrackerHit::~cyTrackerHit() {}

cyTrackerHit::cyTrackerHit(const cyTrackerHit& right) : G4VHit() {
    fTrackID   = right.fTrackID;
    fChamberNb = right.fChamberNb;
    fEdep      = right.fEdep;
    fPos       = right.fPos;
}

const cyTrackerHit& cyTrackerHit::operator=(const cyTrackerHit& right) {
    fTrackID   = right.fTrackID;
    fChamberNb = right.fChamberNb;
    fEdep      = right.fEdep;
    fPos       = right.fPos;

    return *this;
}

G4int cyTrackerHit::operator==(const cyTrackerHit& right) const {
    return ( this == &right) ? 1 : 0;
}

void cyTrackerHit::Draw() {
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(pVVisManager) {
	G4Circle circle(fPos);
	circle.SetScreenSize(4.);
	circle.SetFillStyle(G4Circle::filled);
	G4Colour colour(1., 0., 0.);
	G4VisAttributes attribs(colour);
	circle.SetVisAttributes(attribs);
	pVVisManager->Draw(circle);
    }
}

void cyTrackerHit::Print() {
    G4cout
	<< "  trackID: " << fTrackID << " chamberNb: " << fChamberNb
	<< " Edep: "
	<< std::setw(7) << G4BestUnit(fEdep, "Energy")
	<< " Position: "
	<< std::setw(7) << G4BestUnit( fPos, "Length")
	<< G4endl;
}
