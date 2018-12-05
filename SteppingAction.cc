#include "SteppingAction.hh"
#include "Analysis.hh"
#include "EventAction.hh"
#include "NeutrinoDetectorSD.hh"
#include "NeutrinoDetectorHit.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"

#include "G4Step.hh"
#include "G4String.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4EventManager.hh"
#include "G4UIcommand.hh"

#include "Randomize.hh"
#include <iomanip>
#include "G4PhysicalConstants.hh"
#include <math.h>

SteppingAction::SteppingAction()
:G4UserSteppingAction()
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    if (aStep == 0) return;
    G4StepPoint* spre = aStep->GetPreStepPoint();
    G4StepPoint* spost = aStep->GetPostStepPoint();
    auto prexyz = spre->GetPosition();
    auto postxyz = spost->GetPosition();
    G4cout<<"prepoint = "<<prexyz<<G4endl;
    G4cout<<"postpoint = "<<postxyz<<G4endl;
	const G4Track* track = aStep->GetTrack();
	G4ThreeVector m = track->GetMomentumDirection();
	auto isPrimary = aStep->GetTrack()->GetParentID();
	auto pcharge = aStep->GetTrack()->GetDefinition()->GetPDGCharge();
	//G4double pname = ConvertToDouble(name);
	auto mx = m.x();
	auto my = m.y();
	auto mz = m.z();



	G4int evtNb = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->FillNtupleDColumn(5, evtNb);
	analysisManager->FillNtupleDColumn(6, prexyz.x());
	analysisManager->FillNtupleDColumn(7, prexyz.y());
	analysisManager->FillNtupleDColumn(8, prexyz.z());
	analysisManager->FillNtupleDColumn(9, postxyz.x());
	analysisManager->FillNtupleDColumn(10, postxyz.y());
	analysisManager->FillNtupleDColumn(11, postxyz.z());
	analysisManager->FillNtupleDColumn(12, mx);
	analysisManager->FillNtupleDColumn(13, my);
	analysisManager->FillNtupleDColumn(14, mz);
	analysisManager->FillNtupleDColumn(15, isPrimary);
	analysisManager->FillNtupleDColumn(16, pcharge);
	analysisManager->AddNtupleRow();

    if (spre && spost && spre->GetPhysicalVolume()->GetName() ==
            spost->GetPhysicalVolume()->GetName()) return;
    G4cout << "post step volume=" << spost->GetPhysicalVolume()->GetName()
        << G4endl;



	if (spost->GetTouchable() == 0) return;
    G4cout << "touchable name, repl #, copy # = "
        << spost->GetTouchable()->GetVolume()->GetName() << ", "
        << spost->GetTouchable()->GetReplicaNumber() << ", "
        << spost->GetTouchable()->GetCopyNumber() << G4endl;
}

G4double SteppingAction::ConvertToDouble(const char* str)
{
	return G4double();
}

