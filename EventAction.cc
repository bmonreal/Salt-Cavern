#include "EventAction.hh"
#include "NeutrinoDetectorSD.hh"
#include "NeutrinoDetectorHit.hh"
#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

#include "G4RunManager.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>
#include "G4PhysicalConstants.hh"
#include <math.h>


EventAction::EventAction()
 : G4UserEventAction(),	
   fTubeHCID(-1) 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutrinoDetectorHitsCollection* 
EventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection 
    = static_cast<NeutrinoDetectorHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("EventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    


void EventAction::PrintEventStatistics(
                              G4double tubeEdep, G4double tubeTrackLength) const
{
  // print event statistics
  G4cout
     << "   Detector: total energy: " 
     << std::setw(7) << G4BestUnit(tubeEdep, "Energy")
     << "       total track length: " 
     << std::setw(7) << G4BestUnit(tubeTrackLength, "Length")
     << G4endl;
}


void EventAction::BeginOfEventAction(const G4Event* event)
{
}




void EventAction::EndOfEventAction(const G4Event* event)
{  
  // Get hits collections IDs (only once)
  if ( fTubeHCID == -1 ) {
    fTubeHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("NeutrinoDetectorHitsCollection");
  }

  // Get hits collections
  auto tubeHC = GetHitsCollection(fTubeHCID, event);

  // Get hit with total values
  auto tubeHit = (*tubeHC)[tubeHC->entries()-1];

  // Print per event (modulo n)
  //
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     

    PrintEventStatistics(
    tubeHit->GetEdep(), tubeHit->GetTrackLength()); 
 
  }
  
  auto aTrack = G4RunManagerKernel::GetRunManagerKernel()
    -> GetTrackingManager() -> GetTrack();
  auto tlength = aTrack -> GetTrackLength();
  auto aStep= aTrack->GetStep();
  auto length = aStep->GetStepLength(); 
  auto isPrimary = aStep->GetTrack()->GetParentID(); // 0 means is parent particle
  auto isprim = aTrack->GetParentID();
  G4TouchableHandle touchable = aStep->GetPreStepPoint()->GetTouchableHandle();
  G4int copyNo = touchable->GetVolume(0)->GetCopyNo();
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4ThreeVector displacexyz = aStep->GetDeltaPosition();
  auto displace = displacexyz.mag();
  auto prept = aStep->GetPreStepPoint();
  auto postpt = aStep->GetPostStepPoint();
  auto prexyz = prept->GetPosition();
  auto postxyz = postpt->GetPosition();
  auto pre = prexyz.mag();
  auto post = postxyz.mag();
  auto partDef_tr = aTrack->GetParticleDefinition()->GetParticleName() ;
  G4ThreeVector primaryVertex = event->GetPrimaryVertex()->GetPosition(); 
  G4double r = primaryVertex.mag();
  auto touch1 = prept->GetTouchableHandle();
  auto touch2 = postpt->GetTouchableHandle();
  auto volume1 = touch1->GetVolume();
  auto volume2 = touch2->GetVolume();
  auto name1 = volume1->GetName();
  auto name2 = volume2->GetName();
  auto sec = aStep->GetSecondary();


  G4cout << "------------------------------------" << G4endl;
  G4cout << "Vertex Initial Position (radially): " << r << G4endl;  
  G4cout << "Track Length = " << tlength << G4endl;
  G4cout << "Step Length = " << length << G4endl;
  G4cout << "touchable is " << touchable << G4endl;  
  G4cout << "copy number is: " << copyNo << G4endl;
  G4cout << "total energy deposited in step =" << edep << G4endl;  
  G4cout << "displace = " << displace << G4endl;
  G4cout << "track parent: " << isprim << G4endl;
  G4cout << "step parent: " << isPrimary<< G4endl;
  G4cout << "pre-step: " << pre<< G4endl;
  G4cout << "post-step: " << post<< G4endl;
  G4cout << "track particle: " << partDef_tr<< G4endl;
  G4cout << "Volume name 1: " << name1 << G4endl;
  G4cout << "Volume name 2: " << name2 << G4endl;
  G4cout << "secondary is: " << sec << G4endl;
  G4cout << " " << G4endl;

  G4int nbSteps = aTrack->GetCurrentStepNumber();
  G4cout << "nbSteps: " << nbSteps << G4endl;

  if(aTrack->GetTrackID() !=1) {
    G4cout<<"Particle is a secondary"<<G4endl;
    if(aTrack->GetParentID()==1)
      G4cout<<"But parent was a primary"<<G4endl;
    auto creatorProcess = aTrack->GetCreatorProcess();
    auto process = creatorProcess->GetProcessName();
    G4cout<<"Particle was crated by "<<process<<G4endl;
  }

  G4cout << "------------------------------------" << G4endl;


  auto analysisManager = G4AnalysisManager::Instance();
  // fill histograms
  //analysisManager->FillH1(0, tubeHit->GetEdep());
  //analysisManager->FillH1(1, tubeHit->GetTrackLength());
  //analysisManager->FillH1(2, r);

  
  // fill ntuple
  analysisManager->FillNtupleDColumn(0, tubeHit->GetEdep());
  analysisManager->FillNtupleDColumn(1, tubeHit->GetTrackLength());
  analysisManager->FillNtupleDColumn(2, r);
  analysisManager->FillNtupleDColumn(3, eventID);
  analysisManager->AddNtupleRow();  
}  




