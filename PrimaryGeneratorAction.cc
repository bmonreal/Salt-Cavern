#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr)
   
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  //
  auto particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
  fParticleGun->SetParticleDefinition(particleDefinition);
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  fParticleGun->SetParticleEnergy(1.46*MeV);

  
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event
  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume 
  // from G4LogicalVolumeStore
  //
  G4double worldZHalfLength = 0.;
  auto logicCavern = G4LogicalVolumeStore::GetInstance()->GetVolume("Cavern");

  // Check that the world volume has box shape
  G4Box* worldBox = nullptr;
  if (  logicCavern) {
    worldBox = dynamic_cast<G4Box*>(logicCavern->GetSolid());
  }

  if ( worldBox ) {
    worldZHalfLength = worldBox->GetZHalfLength();  
  }
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("B4PrimaryGeneratorAction::GeneratePrimaries()",
      "MyCode0002", JustWarning, msg);
  } 
  // Set gun position: Uniformly random on (8m, 9m)
  G4double alpha = twopi * G4UniformRand();  //alpha uniform in (0, 2*pi)
  G4double uxx = std::cos(alpha);
  G4double uyy = std::sin(alpha);

  const G4double r = 8.*m;
  const G4double xmax = 8.01*m;
  G4double fstartingpoint = r + (xmax-r)*G4UniformRand();
  //G4double fstartingpoint = 8.0001*m;
  fParticleGun->SetParticlePosition(G4ThreeVector(-fstartingpoint*uxx, -fstartingpoint*uyy, 0.));
  G4double cosTheta = 2 * G4UniformRand() - 1., phi = twopi * G4UniformRand();
  G4double sinTheta = std::sqrt(1. - cosTheta * cosTheta);
  G4double ux = sinTheta * std::cos(phi),
	  uy = sinTheta * std::sin(phi),
	  uz = cosTheta;
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux, uy, uz));
  fParticleGun->GeneratePrimaryVertex(anEvent);

}


