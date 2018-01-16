#include "PrimaryGeneratorAction.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

// Specify constructed detector in argument
PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC)
	:Detector(DC)
{
	// Define particle gun object
	G4int n_particle = 1;
	particleGun = new G4ParticleGun(n_particle);

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	// Get simulation current event number
	G4int numEvent;
	numEvent = anEvent->GetEventID() + 1;

	G4double x0, y0, z0, theta, phi, xMom0, yMom0, zMom0, e0;

	// Specify kinetic energy
	e0 = 3 * MeV;
	particleGun->SetParticleEnergy(e0);

	// Specify emission direction
	theta = 0;
	phi = 0;
	xMom0 = std::sin(theta);
	yMom0 = std::sin(phi);
	zMom0 = std::sqrt(1. - xMom0*xMom0 - yMom0*yMom0);
	particleGun->SetParticleMomentumDirection(G4ThreeVector(xMom0, yMom0, zMom0));

	// Specify emission point
	x0 = 0;
	y0 = 0;
	z0 = -20 * micrometer;
	particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

	// Select electron
	G4ParticleDefinition* particle =
		G4ParticleTable::GetParticleTable()->FindParticle("electron");

	particleGun->SetParticleDefinition(particle);

	// Example of output display
	G4cout
		<< "-> Event= " << numEvent
		<< " : Theta (mrad)= " << theta / mrad
		<< " - Phi (mrad)= " << phi / mrad
		<< " - x0 (um)= " << x0 / um
		<< " - y0 (um)= " << y0 / um
		<< " - z0 (um)= " << z0 / um
		<< " - e0 (MeV)= " << e0 / MeV
		<< G4endl;

	// Shoot
	particleGun->GeneratePrimaryVertex(anEvent);

}

