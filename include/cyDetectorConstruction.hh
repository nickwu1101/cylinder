#ifndef cyDetectorConstruction_h
#define cyDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "tls.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class cyDetectorConstruction : public G4VUserDetectorConstruction {
public:
    cyDetectorConstruction();
    virtual ~cyDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    // Set methods
    void SetCheckOverlaps(G4bool);

private:
    // methods
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    // data members
    G4LogicalVolume* coreLV;
    G4LogicalVolume* middleLV;
    G4LogicalVolume* shellLV;

    G4Material*      vacuum;
    G4Material*      coreMaterial;
    G4Material*      midMaterial;
    G4Material*      shellMaterial;

    G4bool           fCheckOverlaps;
};

#endif
