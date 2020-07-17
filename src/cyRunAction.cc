#include "cyRunAction.hh"
#include "cyEventAction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "g4analysis.hh"

cyRunAction::cyRunAction(cyRunAction* eventAction)
    : G4UserRunAction(), fEventAction(eventAction) {
    auto analysisManager = G4Analysis::ManagerInstance("root");

    G4cout << "Using " << analysisManager->GetType() << G4endl;

    // Default settings
    // Note: merging ntuples is available only with root output
    analysisManager->SetNtupleMerging(true);
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFileName(fFileName);
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetActivation(true); // enable inactivation of histograms

    // Define histograms start values
    G4int   nbins = 100;
    G4double vmin = 0.;
    G4double vmax = 100.;
    const G4int kMaxHisto = 7;
    const G4String id[] = { "0", "1", "2", "3", "4", "5", "6" };
    const G4String title[] =
	{ "dummy",
	  "Energy of Hit",
	  "Position X",
	  "Position Y",
	  "Position Z",
	  "energy spectrum of charged particles",
	  "energy spectrum of neutral particles"
	};

    for(G4int k = 0; k < kMaxHisto; k++) {
	G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
	G4bool activ = false;
	if(k == 1) activ = true;
	analysisManager->SetH1Activation(ih, activ);
    }

    // Initialization of Ntuple column IDs:
    for(G4int k = 0; k < MaxNtCol; k++) { fNtColId[k] = 0; }

    if(fEventAction) {
	analysisManager->SetFirstNtupleId(1);
	analysisManager->CreateNtuple("ntuple", "ntuple");

	fNtColId[0] = analysisManager->CreateNtupleIColumn("Event");
	fNtColId[1] = analysisManager->CreateNtupleIColumn("NhitCore");
	fNtColId[2] = analysisManager->CreateNtupleDColumn("EtotalCore");

	vector<G4double>& myHitCore = fEventAction->getE_HitCore();
	G4out << "Number of Hits: " << myHitCore.size() << G4endl;

	fNtColId[3] = analysisManager->CreateNtupleDColumn("E_HitCore", fEventAction->getE_HitCore());
	fNtColId[4] = analysisManager->CreateNtupleDColumn("XCore"    , fEventAction->getXCore());
	fNtColId[5] = analysisManager->CreateNtupleDColumn("YCore"    , fEventAction->getYCore());
	fNtColId[6] = analysisManager->CreateNtupleDColumn("ZCore"    , fEventAction->getZCore());
	fNtColId[7] = analysisManager->CreateNtupleDColumn("timeCore" , fEventAction->gettimeCore());

	analysisManager->FinishNtuple();
    } else {
	std::cout << "ERROR ... fEventAction not defined !!!!!" << std::endl;
    }
}

cyRunAction::~cyRunAction() {
    delete G4AnalysisManager::Instance();
}

void cyRunAction::BeginOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();

    if(analysisManager->IsActive()) {
	analysisManager->OpenFile();
    }
}

void cyRunAction::EndOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();
    if(analysisManager->IsActive()) {
	analysisManager->Write();
	analysisManager->CloseFile();
    }
}
