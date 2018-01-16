//Geant4 and user header files to include
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UIcsh.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
//#include "RunAction.hh"
//#include "EventAction.hh"
//#include "SteppingAction.hh"

// If one wishes to use vizualisation
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

// Main called with optional arguments
int main(int argc, char** argv) {

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
#ifdef G4VIS_USE
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif

	// Initialize Geant4 kernel
	runManager->Initialize();

	// Remove user output files
	//system("rm -rf dose.txt");

	// Get the pointer to the User Interface manager 
	G4UImanager* UI = G4UImanager::GetUIpointer();

	if (argc == 1)   // Define UI session for interactive mode.
	{
		G4UIsession * session = new G4UIterminal(new G4UIcsh);
		//Use of a command macro file
		UI->ApplyCommand("/control/execute run.mac");
		session->SessionStart();
		delete session;
	}

	else           // Batch mode
	{
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UI->ApplyCommand(command + fileName);
	}

#ifdef G4VIS_USE
	delete visManager;
#endif

	delete runManager;

	return 0;
}
