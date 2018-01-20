//Geant4 and user header files to include
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UIcsh.hh"
#include "G4UImanager.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
//#include "RunAction.hh"
//#include "EventAction.hh"
//#include "SteppingAction.hh"


// Main called with optional arguments
int main(int argc, char** argv) {

        G4UIExecutive* ui = 0;
 	if ( argc == 1 ) {
    	  ui = new G4UIExecutive(argc, argv);
  	}

	// Construct the default run manager
	G4RunManager * runManager = new G4RunManager;

	// Set mandatory user initialization classes
	DetectorConstruction* detector = new DetectorConstruction;
	runManager->SetUserInitialization(detector);
	runManager->SetUserInitialization(new PhysicsList);

	// Set mandatory user action classes
	runManager->SetUserAction(new PrimaryGeneratorAction(detector));
	PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(detector);

	// Set optional user action classes
	//RunAction* RunAct = new RunAction(detector);
	//runManager->SetUserAction(RunAct);

	//runManager->SetUserAction(new EventAction(RunAct));

	//runManager->SetUserAction(new SteppingAction(RunAct, detector, primary));

	// Visualization manager
	
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();

	// Initialize Geant4 kernel
	runManager->Initialize();

  	// Get the pointer to the User Interface manager
  	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	// Process macro or start UI session
  	//
  	if ( ! ui ) { 
    	  // batch mode
    	  G4String command = "/control/execute ";
    	  G4String fileName = argv[1];
    	  UImanager->ApplyCommand(command+fileName);
  }
  	else { 
    	  // interactive mode
          UImanager->ApplyCommand("/control/execute init_vis.mac");
          ui->SessionStart();
        delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete visManager;
  delete runManager;
}
