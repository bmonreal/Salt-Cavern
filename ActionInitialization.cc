#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
//#include "TrackingAction.hh"


ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}


ActionInitialization::~ActionInitialization()
{}


void ActionInitialization::BuildForMaster() const
{
  RunAction* runAction = new RunAction;
  SetUserAction(runAction);
}


void ActionInitialization::Build() const
{
  PrimaryGeneratorAction* primary = new PrimaryGeneratorAction();
  SetUserAction(primary);

  RunAction* runAction = new RunAction;
  SetUserAction(runAction);

  SetUserAction(new EventAction);
  
  //TrackingAction* trackingAction = new TrackingAction(primary);
  //SetUserAction(trackingAction);
}  

