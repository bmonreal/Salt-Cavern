#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"

class PhysicsList : public G4VUserPhysicsList
{
public:
	PhysicsList();
	~PhysicsList();

	void SetGammaCut(G4double);
	void SetElectronCut(G4double);
	void SetPositronCut(G4double);
	void SetProtonCut(G4double);

protected:
	// these methods construct particles 
	void ConstructBosons();
	void ConstructLeptons();
	void ConstructBarions();

	// these methods construct physics processes and register them
	void ConstructGeneral();
	void ConstructEM();

	// Construct particle and physics
	void ConstructParticle();
	void ConstructProcess();

	// set cuts
	void SetCuts();

private:
	G4double cutForGamma;
	G4double cutForElectron;
	G4double cutForPositron;
	G4double cutForProton;

};
#endif

