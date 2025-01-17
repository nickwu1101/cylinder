#include "cyDetectorConstruction.hh"
#include "cyTrackerSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

cyDetectorConstruction::cyDetectorConstruction() :
    G4VUserDetectorConstruction(),
    airboxLV(NULL), chamLV(NULL),
    coreLV(NULL), middleLV(NULL), scintiLV(NULL), shellLV(NULL),
    vacuum(NULL), air(NULL), chamMaterial(NULL), coreMaterial(NULL),
    midMaterial(NULL), scintiMaterial(NULL), shellMaterial(NULL),
    fCheckOverlaps(true) {}

cyDetectorConstruction::~cyDetectorConstruction() {}

G4VPhysicalVolume* cyDetectorConstruction::Construct() {
    DefineMaterials();
    return DefineVolumes();
}

void cyDetectorConstruction::DefineMaterials() {
    G4NistManager* nistManager = G4NistManager::Instance();

    air = nistManager->FindOrBuildMaterial("G4_AIR");
    chamMaterial = nistManager->FindOrBuildMaterial("G4_PLEXIGLASS");
    coreMaterial = nistManager->FindOrBuildMaterial("G4_SODIUM_IODIDE");
    midMaterial = nistManager->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
    scintiMaterial = nistManager->FindOrBuildMaterial("G4_POLYSTYRENE");
    shellMaterial = nistManager->FindOrBuildMaterial("G4_Al");

    G4double vDens = 1.e-25*g/cm3;
    G4double vPres = 1.e-19*pascal;
    G4double vTemp = 0.1*kelvin;

    vacuum = new G4Material("Vacuum", 1, 1.008*g/mole, vDens, kStateGas, vTemp, vPres);
}

G4VPhysicalVolume* cyDetectorConstruction::DefineVolumes() {
    G4double worldLength = 1000.*cm;
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);
    
    G4cout << "Computed tolerance = "
	   << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
	   << " mm" << G4endl;
    
    // world box
    G4Box* worldS = new G4Box("world", worldLength/2., worldLength/2., worldLength/2.);
    G4LogicalVolume* worldLV = new G4LogicalVolume(worldS, vacuum, "World");
    G4VPhysicalVolume* worldPV
	= new G4PVPlacement(
	    0,               // no rotation
	    G4ThreeVector(), // at (0, 0, 0)
	    worldLV,         // its logical volume
	    "World",         // its name
	    0,               // its mother volume
	    false,           // no boolean operations
	    0,               // copy number
	    fCheckOverlaps); // checking overlaps
    G4VisAttributes* worldVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.));
    worldVisAtt->SetVisibility(true);
    worldLV->SetVisAttributes(worldVisAtt);

    // core cylinder
    G4double rcore = 1.5*inch;
    G4double lcore = 3.*inch;
    G4double start_angle = 0.*deg;
    G4double end_angle = 360.*deg;

    G4Tubs* coreCylinder = new G4Tubs("Core Cylinder", 0.*inch, rcore, lcore/2., start_angle, end_angle);

    // middle cylinder
    G4double rmid = rcore + 0.185*cm;
    G4double lmid = lcore + 0.37*cm;

    G4Tubs* middleCylinder = new G4Tubs("Middle Cylinder", 0.*cm, rmid, lmid/2., start_angle, end_angle);

    // big cylinder
    G4double rbig = rmid + 0.05*cm;
    G4double lbig = lmid + 0.1*cm;

    G4Tubs* bigCylinder = new G4Tubs("Shell Cylinder", 0.*cm, rbig, lbig/2., start_angle, end_angle);

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

    G4ThreeVector centerOfCylinder = G4ThreeVector(0.*cm, 0.*cm, 0.*cm);
    if(true) {
	// placements
	new G4PVPlacement(0,
			  centerOfCylinder,
			  coreLV,
			  "core_PV",
			  worldLV,
			  false,
			  0,
			  fCheckOverlaps);
	G4VisAttributes* coreVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
	coreVisAtt->SetVisibility(true);
	coreLV->SetVisAttributes(coreVisAtt);

    }

    if(false) {
	new G4PVPlacement(0,
			  centerOfCylinder,
			  middleLV,
			  "middle_PV",
			  worldLV,
			  false,
			  0,
			  fCheckOverlaps);
	G4VisAttributes* midVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
	midVisAtt->SetVisibility(true);
	middleLV->SetVisAttributes(midVisAtt);

	new G4PVPlacement(0,
			  centerOfCylinder,
			  shellLV,
			  "shell_PV",
			  worldLV,
			  false,
			  0,
			  fCheckOverlaps);
	G4VisAttributes* shellVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
	shellVisAtt->SetVisibility(true);
	shellLV->SetVisAttributes(shellVisAtt);
    }
    
    if(false) { // ignore this block
	G4double xin = 8.*cm;
	G4double yin = 8.*cm;
	G4double zin = 4.*cm;

	G4Box* innerBox = new G4Box("innerBox", xin/2., yin/2., zin/2.);

	G4double xout = xin + 2.*cm;
	G4double yout = yin + 2.*cm;
	G4double zout = zin + 2.*cm;

	G4Box* outerBox = new G4Box("outerBox", xout/2., yout/2., zout/2.);

	G4SubtractionSolid* chamber = new G4SubtractionSolid("chamber",
							     outerBox,
							     innerBox);

	airboxLV = new G4LogicalVolume(innerBox, air, "airbox_LV", 0, 0, 0);
	chamLV = new G4LogicalVolume(chamber, chamMaterial, "cham_LV", 0, 0, 0);

	G4ThreeVector centerOfBox = G4ThreeVector(0.*cm, 0.*cm, -zout/2.);

	new G4PVPlacement(0,
			  centerOfBox,
			  airboxLV,
			  "airbox_PV",
			  worldLV,
			  false,
			  0,
			  fCheckOverlaps);
	new G4PVPlacement(0,
			  centerOfBox,
			  chamLV,
			  "cham_PV",
			  worldLV,
			  false,
			  0,
			  fCheckOverlaps);
	G4VisAttributes* airboxVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
	G4VisAttributes* chamVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
	airboxLV->SetVisAttributes(airboxVisAtt);
	chamLV->SetVisAttributes(chamVisAtt);
    }

    G4double scintiX = 8.*cm;
    G4double scintiY = 8.*cm;
    G4double scintiZ = 1.1*cm;
    
    G4Box* scintillator = new G4Box("scintillator", scintiX/2., scintiY/2., scintiZ/2.);
    
    scintiLV = new G4LogicalVolume(scintillator, scintiMaterial, "scintillator_LV", 0, 0, 0);

    G4RotationMatrix* rotm = new G4RotationMatrix();
    rotm->rotateX(90*deg);

    if(false)
	new G4PVPlacement(rotm,
			  G4ThreeVector(0.*cm, rbig + scintiZ/2. + 3*mm, lbig/2.),
			  scintiLV,
			  "scintillator_PV",
			  worldLV,
			  false,
			  0,
			  fCheckOverlaps);
    
    G4VisAttributes* scintiVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
    scintiLV->SetVisAttributes(scintiVisAtt);
    
    return worldPV;
}

void cyDetectorConstruction::ConstructSDandField() {
    // sensitive detectors
    G4String trackerCoreSDName = "cy/TrackerCoreSD";
    G4String trackerScintiSDName = "cy/TrackerScintiSD";

    cyTrackerSD* coreTrackerSD = new cyTrackerSD(trackerCoreSDName,
						 "TrackerHitsCollectionCore");
    cyTrackerSD* scintiTrackerSD = new cyTrackerSD(trackerScintiSDName,
						   "TrackerHitsCollectionScinti");
    
    G4SDManager::GetSDMpointer()->AddNewDetector(coreTrackerSD);
    G4SDManager::GetSDMpointer()->AddNewDetector(scintiTrackerSD);

    // Setting TrackerSD to all logical volume
    SetSensitiveDetector("core_LV", coreTrackerSD, true);
    SetSensitiveDetector("scintillator_LV", scintiTrackerSD, true);
}

void cyDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps) {
    fCheckOverlaps = checkOverlaps;
}
