#include "PhysicsList.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

PhysicsList::PhysicsList() : G4VUserPhysicsList()
{
	// Specify production cut for EM processes
	defaultCutValue = 1. * nanometer;
	cutForGamma = defaultCutValue;
	cutForElectron = defaultCutValue;
	cutForPositron = defaultCutValue;
	cutForProton = defaultCutValue;

	// Specify verbosity level
	SetVerboseLevel(1);
}

PhysicsList::~PhysicsList()
{}

// ***** Construction of particles

void PhysicsList::ConstructParticle()
{
	ConstructBosons();
	ConstructLeptons();
	ConstructBarions();
}
void PhysicsList::ConstructBosons()
{

	// gamma
	G4Gamma::GammaDefinition();

	// optical photon
	G4OpticalPhoton::OpticalPhotonDefinition();
}

void PhysicsList::ConstructLeptons()
{
	// leptons
	G4Electron::ElectronDefinition();
	G4Positron::PositronDefinition();
}

void PhysicsList::ConstructBarions()
{
	//  baryons
	G4Proton::ProtonDefinition();
	G4AntiProton::AntiProtonDefinition();
}

// ***** Processses and particles

void PhysicsList::ConstructProcess()
{
	AddTransportation();
	ConstructEM();
	ConstructGeneral();
}

// gamma
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"
#include "G4RayleighScattering.hh" 
#include "G4LivermoreRayleighModel.hh"


// e-
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"


// e+
#include "G4eIonisation.hh" 
#include "G4eBremsstrahlung.hh" 
#include "G4eplusAnnihilation.hh"

// alpha and GenericIon and deuterons, triton, He3:
//muon:
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonMinusCapture.hh"

//OTHERS:
#include "G4hIonisation.hh" 
#include "G4hMultipleScattering.hh"
#include "G4hBremsstrahlung.hh"
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"

//em process options to allow msc step-limitation to be switched off
#include "G4EmParameters.hh"
#include "G4VAtomDeexcitation.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4LossTableManager.hh"

#include "G4StepLimiter.hh"

void PhysicsList::ConstructEM()
{
	auto theParticleIterator = GetParticleIterator();
	theParticleIterator->reset();

	while ((*theParticleIterator)()) {

		G4ParticleDefinition* particle = theParticleIterator->value();
		G4ProcessManager* pmanager = particle->GetProcessManager();
		G4String particleName = particle->GetParticleName();


		if (particleName == "gamma") {

			pmanager->AddDiscreteProcess(new G4ComptonScattering);

			G4PhotoElectricEffect * LePeprocess = new G4PhotoElectricEffect();
			pmanager->AddDiscreteProcess(LePeprocess);

			pmanager->AddDiscreteProcess(new G4GammaConversion());

			pmanager->AddDiscreteProcess(new G4RayleighScattering());

			// Allow use of step size limitation specified in DetectorConstruction
			pmanager->AddProcess(new G4StepLimiter(), -1, -1, 3);


		}
		else if (particleName == "e-") {

			pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);

			G4eIonisation * LeIoprocess = new G4eIonisation("IONI");
			pmanager->AddProcess(LeIoprocess, -1, 2, 2);

			G4eBremsstrahlung * LeBrprocess = new G4eBremsstrahlung();
			pmanager->AddProcess(LeBrprocess, -1, -1, 3);

			// Allow use of step size limitation specified in DetectorConstruction
			pmanager->AddProcess(new G4StepLimiter(), -1, -1, 3);

		}
		else if (particleName == "e+") {

			pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
			pmanager->AddProcess(new G4eIonisation, -1, 2, 2);
			pmanager->AddProcess(new G4eBremsstrahlung, -1, -1, 3);
			pmanager->AddProcess(new G4eplusAnnihilation, 0, -1, 4);

			// Allow use of step size limitation specified in DetectorConstruction
			pmanager->AddProcess(new G4StepLimiter(), -1, -1, 3);

		}
		else if ((!particle->IsShortLived()) &&
			(particle->GetPDGCharge() != 0.0) &&
			(particle->GetParticleName() != "chargedgeantino")) {

			pmanager->AddProcess(new G4hMultipleScattering(), -1, 1, 1);

			G4hIonisation* hLowEnergyIonisation = new G4hIonisation();
			pmanager->AddProcess(hLowEnergyIonisation, -1, 2, 2);

			//hLowEnergyIonisation->SetElectronicStoppingPowerModel(particle, "ICRU_R49p");

			// Allow use of step size limitation specified in DetectorConstruction
			pmanager->AddProcess(new G4StepLimiter(), -1, -1, 3);

		}
	}
}

void PhysicsList::ConstructGeneral()
{ }

// ***** Set production cuts

void PhysicsList::SetCuts()
{
	if (verboseLevel >0)
	{
		G4cout << "PhysicsList::SetCuts:";
		G4cout << "CutLength : " << G4BestUnit(defaultCutValue, "Length") << G4endl;
	}

	// set cut values for gamma at first and for e- second and next for e+,
	// because some processes for e+/e- need cut values for gamma 
	SetCutValue(cutForGamma, "gamma");
	SetCutValue(cutForElectron, "e-");
	SetCutValue(cutForPositron, "e+");

	// set cut values for proton and anti_proton before all other hadrons
	// because some processes for hadrons need cut values for proton/anti_proton 
	SetCutValue(cutForProton, "proton");
	SetCutValue(cutForProton, "anti_proton");

	if (verboseLevel>0) DumpCutValuesTable();
}
