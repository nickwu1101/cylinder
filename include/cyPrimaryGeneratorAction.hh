#ifndef cyPrimaryGeneratorAction_h
#define cyPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4Event;

class cyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    cyPrimaryGeneratorAction();
    ~cyPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);

private:
    G4GeneralParticleSource* fParticleGun;
};

#endif
