#include "TrackingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Track.hh"
#include "G4PhysicalConstants.hh"



TrackingAction::TrackingAction(PrimaryGeneratorAction* prim)
:G4UserTrackingAction(), fPrimary(prim)
{ 
  fstartingpoint = fPrimary->GetStartingPoint();
}



void TrackingAction::PreUserTrackingAction(const G4Track* track)
{ 
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4ThreeVector vertex   = track->GetVertexPosition();
  G4double r = vertex.mag();
  analysisManager->FillH1(2, r);
  analysisManager->FillNtupleDColumn(2, r);
  
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track*)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
