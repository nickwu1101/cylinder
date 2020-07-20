#ifndef cyTrackerHit_h
#define cyTrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class cyTrackerHit : public G4VHit {
public:
    cyTrackerHit();
    cyTrackerHit(const cyTrackerHit&);
    virtual ~cyTrackerHit();

    // operators
    const cyTrackerHit& operator=(const cyTrackerHit&);
    G4int operator==(const cyTrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID   (G4int       track) { fTrackID = track; };
    void SetChamberNb (G4int       chamb) { fChamberNb = chamb; };
    void SetEdep      (G4double       de) { fEdep = de; };
    void SetPos       (G4ThreeVector xyz) { fPos = xyz; };
    void SetTime      (G4double      tof) { ftof = tof; };

    // Get methods
    G4int         GetTrackID()   const { return fTrackID; };
    G4int         GetChamberNb() const { return fChamberNb; };
    G4double      GetEdep()      const { return fEdep; };
    G4ThreeVector GetPos()       const { return fPos; };
    G4double      GetTime()      const { return ftof; };

private:
    G4int         fTrackID;
    G4int         fChamberNb;
    G4double      fEdep;
    G4ThreeVector fPos;
    G4double      ftof;
};

typedef G4THitsCollection<cyTrackerHit> cyTrackerHitsCollection;

extern G4ThreadLocal G4Allocator<cyTrackerHit>* cyTrackerHitAllocator;

inline void* cyTrackerHit::operator new(size_t) {
    if(!cyTrackerHitAllocator)
	cyTrackerHitAllocator = new G4Allocator<cyTrackerHit>;
    return (void*) cyTrackerHitAllocator->MallocSingle();
}

inline void cyTrackerHit::operator delete(void*hit) {
    cyTrackerHitAllocator->FreeSingle((cyTrackerHit*) hit);
}

#endif
