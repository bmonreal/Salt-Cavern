#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class PrimaryGeneratorAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TrackingAction : public G4UserTrackingAction {

  public:  
    TrackingAction(PrimaryGeneratorAction*);
   ~TrackingAction() {};
   
    virtual void  PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);
    
  private:
    PrimaryGeneratorAction* fPrimary;

   // parameters for generator action 
    G4double fstartingpoint;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
