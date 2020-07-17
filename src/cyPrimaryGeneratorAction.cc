#include "cyPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"

cyPrimaryGeneratorAction::cyPrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(), fParticleGun(0) {
    fParticleGun = new G4GeneralParticleSource();
}

cyPrimaryGeneratorAction::~cyPrimaryGeneratorAction() {
    delete fParticleGun;
}

void cyPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
