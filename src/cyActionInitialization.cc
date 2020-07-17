#include "cyActionInitialization.hh"
#include "cyPrimaryGeneratorAction.hh"
#include "cyRunAction.hh"
#include "cyEventAction.hh"

cyActionInitialization::cyActionInitialization()
    : G4VUserActionInitialization() {}

cyActionInitialization::~cyActionInitialization() {}

void cyActionInitialization::BuildForMaster() const {
    cyEventAction* eventAction = new cyEventAction;
    SetUserAction(new cyRunAction(eventAction));
}

void cyActionInitialization::Build() const {
    SetUserAction(new cyPrimaryGeneratorAction);
    auto eventAction = new cyEventAction;
    SetUserAction(eventAction);
    SetUserAction(new cyRunAction(eventAction));
}
