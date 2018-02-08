#include "EventAction.hh"
#include "NeutrinoDetectorSD.hh"
#include "NeutrinoDetectorHit.hh"
#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"
#include "RunAction.hh"

#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>
#include "G4PhysicalConstants.hh"


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

  auto analysisManager = G4AnalysisManager::Instance();
  // fill histograms
  analysisManager->FillH1(0, tubeHit->GetEdep());
  analysisManager->FillH1(1, tubeHit->GetTrackLength());

  
  // fill ntuple
  analysisManager->FillNtupleDColumn(0, tubeHit->GetEdep());
  analysisManager->FillNtupleDColumn(1, tubeHit->GetTrackLength());
  analysisManager->AddNtupleRow();  
}  




