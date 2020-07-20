#include "cyDetectorConstruction.hh"
#include "cyActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include <vector>

using namespace std;

int main(int argc, char** argv) {
    // Detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = 0;
    if(argc == 1) ui = new G4UIExecutive(argc, argv);

    // Choose the random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Construct the default run manager
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
#else
    G4RunManager* runManager = new G4RunManager;
#endif

    // Set mandatory initialization classes
    runManager->SetUserInitialization(new cyDetectorConstruction());

    G4VModularPhysicsList* physicsList = new FTFP_BERT;
    //physicsList->RegisterPhysics(new G4StepLimiterPhysics());
    physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics);
    runManager->SetUserInitialization(physicsList);

    // Set user action classes
    runManager->SetUserInitialization(new cyActionInitialization());

    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    if(!ui) {
	string arg_string;
	vector<string> arg_list;

	for(int i = 0; i < argc; ++i) {
	    arg_string = argv[i];
	    arg_list.push_back(arg_string);
	    G4cout << argv[i] << G4endl;
	}

	// batch mode
	G4String command = "/control/execute ";
	G4String fileName = argv[1];
	UImanager->ApplyCommand(command + fileName);
    } else {
	// interactive mode
	UImanager->ApplyCommand("/control/execute init_vis.mac");
	if(ui->IsGUI()) {
	    UImanager->ApplyCommand("/control/execute gui.mac");
	}
	ui->SessionStart();
	delete ui;
    }
    // Job termination
    delete visManager;
    delete runManager;
}
