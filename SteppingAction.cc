#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

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
