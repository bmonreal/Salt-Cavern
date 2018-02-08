#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "NeutrinoDetectorHit.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UserTrackingAction.hh"
#include "G4ThreeVector.hh"

#include "globals.hh"

/// Event action class
///
/// In EndOfEventAction(), it prints the accumulated quantities of the energy 
/// deposit and track lengths of charged particles detector
/// stored in the hits collections.


class PrimaryGeneratorAction;

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();



  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);
    
private:
  // methods
  NeutrinoDetectorHitsCollection* GetHitsCollection(G4int hcID,
                                            const G4Event* event) const;
  void PrintEventStatistics(G4double tubeEdep, G4double tubeTrackLength) const;

  PrimaryGeneratorAction* fPrimary;
  
  // data members                   
  G4int  fTubeHCID;
  G4double fstartingpoint;
  
};


                     

#endif

    
