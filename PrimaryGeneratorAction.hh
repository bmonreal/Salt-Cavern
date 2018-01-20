#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "DetectorConstruction.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	PrimaryGeneratorAction(DetectorConstruction*);
	~PrimaryGeneratorAction();

	void GeneratePrimaries(G4Event*);

    	// method to access particle gun
    	const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

private:
	G4ParticleGun*           fParticleGun;
	DetectorConstruction*    Detector;
};

#endif

