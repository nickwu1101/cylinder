#ifndef cyActionInitialization_h
#define cyActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class cyActionInitialization : public G4VUserActionInitialization {
public:
    cyActionInitialization();
    virtual ~cyActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif
