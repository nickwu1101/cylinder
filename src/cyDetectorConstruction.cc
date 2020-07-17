#include "cyDetectorConstruction.hh"
#include "cyTrackerSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

cyDetectorConstruction::cyDetectorConstruction() :
    G4VUserDetectorConstruction(),
    coreLV(NULL), middleLV(NULL), shellLV(NULL),
    coreMaterial(NULL), midMaterial(NULL), shellMaterial(NULL),
    fCheckOverlaps(NULL) {}

cyDetectorConstruction::~cyDetectorConstruction() {}

G4VPhysicalVolume* cyDetectorConstruction::Construct() {
    DefineMaterials();
    return DefineVolumes();
}

void cyDetectorConstruction::DefineMaterials() {
    G4NistManager* nistManager = G4NistManager::Instance();

    coreMaterial = nistManager->FindOrBuildMaterial("G4_SODIUM_IODIDE");
    midMaterial = nistManager->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
    shellMaterial = nistManager->FindOrBuildMaterial("G4_Al");

    G4double vDens = 1.e-25*g/cm3;
    G4double vPres = 1.e-19*pascal;
    G4double vTemp = 0.1*kelvin;

    vacuum = new G4Material("Vacuum", 1, 1.008*g/mole, vDens, kStatteGas, vTemp, vPres);
}

G4VPhysicalVolume* cyDetectorConstruction::DefineVolumes() {
    G4double worldLength = 1000.*cm;
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);
    
    G4out << "Computed tolerance = "
	  << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
	  << " mm" << G4endl;
    
    // world box
    G4Box* worldS = new G4Box("world", worldLength/2., worldLength/2., worldLength/2.);
    G4LogicalVolume* worldLV = new G4LogicalVolume(worlds, vacuum, "World");
    G4PhysicalVolume* worldPV
	= new G4PhysicalVolume(
	    0,               // no rotation
	    G4ThreeVector(), // at (0, 0, 0)
	    worldLV,         // its logical volume
	    "World",         // its name
	    0,               // its mother volume
	    false,           // no boolean operations
	    0,               // copy number
	    fCheckOverlaps); // checking overlaps
    G4VisAttributes* worldVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    worldLV->SetVisAttributes(worldVisAtt);

    // core cylinder
    G4double rcore = 3*inch;
    G4double lcore = 3*inch;

    G4Tubs* coreCylinder = new G4Tubs("Core Cylinder", 0.*inch, rcore, lcore, 0.*deg, 360.*deg);

    // middle cylinder
    G4double rmid = rcore + 0.37*cm;
    G4double lmid = lcore + 0.37*cm;

    G4Tubs* middleCylinder = new G4Tubs("Middle Cylinder", 0.*cm, rmid, lmid, 0.*deg, 360.*deg);

    // big cylinder
    G4double rbig = rmid + 0.1*cm;
    G4double lbig = lmid + 0.1*cm;

    G4Tubs* bigCylinder = new G4Tubs("Shell Cylinder", 0.*cm, rbig, lbig, 0.*deg, 360.*deg);

    // middle layer
    G4SubtractionSolid* midLayer = new G4SubtractionSolid("Middle Layer",
							  middleCylinder,
							  coreCylinder);

    // shell layer
    G4SubtractionSolid* shellLayer = new G4SubtractionSolid("Shell Layer",
							    bigCylinder,
							    middleCylinder);

    // logical volumes
    coreLV = new G4LogicalVolume(coreCylinder, coreMaterial, "core_LV", 0, 0, 0);
    middleLV = new G4LogicalVolume(midLayer, midMaterial, "middle_LV", 0, 0, 0);
    shellLV = new G4LogicalVolume(shellLayer, shellMaterial, "shell_LV", 0, 0, 0);

    // placements
    new G4PVPlacement(0,
		      G4ThreeVector(),
		      coreLV,
		      "core_PV",
		      worldLV,
		      false,
		      0,
		      fCheckOverlaps);
    G4VisAttributes* coreVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
    coreLV->SetVisAttributes(coreVisAtt);

    new G4PVPlacement(0,
		      G4ThreeVector(),
		      middleLV,
		      "middle_PV",
		      worldLV,
		      false,
		      0,
		      fCheckOverlaps);
    G4VisAttributes* midVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
    middleLV->SetVisAttributes(midVisAtt);

    new G4PVPlacement(0,
		      G4ThreeVector(),
		      shellLV,
		      "shell_PV",
		      worldLV,
		      false,
		      0,
		      fCheckOverlaps);
    G4VisAttributes* shellVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));

    return worldPV;
}

void cyDetectorConstruction::ConstructSDandField() {
    // sensitive detectors
    G4String trackerCoreSDName = "cy/TrackerCoreSD";

    cyTrackerSD* coreTrackerSD = new cyTrackerSD(trackerCoreSDName,
						 "TrackerHitsCollectionCore");
    G4SDManager::GetSDMpointer()->AddNewDetector(coreTrackerSD);

    // Setting TrackerSD to all logical volume
    SetSensitiveDetector("core_LV", coreTrackerSD, true);
}

void cyDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps) {
    fCheckOverlaps = checkOverlaps;
}
