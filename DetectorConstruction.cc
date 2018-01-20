#include "DetectorConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

DetectorConstruction::~DetectorConstruction()
{}

// Mandatory implementation of Construct method
G4VPhysicalVolume* DetectorConstruction::Construct()

{
	DefineMaterials();
	return ConstructDetector();
}

void DetectorConstruction::DefineMaterials()
{
	G4String name, symbol;
	G4double density;

	G4int ncomponents, natoms;
	G4double z, a;
	G4double fractionmass;
	G4double temperature, pressure;

	// Define Elements  
	// Example: G4Element* Notation  = new G4Element ("Element", "Notation", z, a);
	// Elements for making steel
	G4Element* Fe = new G4Element("Iron", "Fe", 26., 55.845*g / mole);
	G4Element* Ni = new G4Element("Nickel", "Ni", 28, 58.6934* g / mole);
	G4Element* Cr = new G4Element("Chromium", "Cr", 24, 51.996 * g / mole);

	//Elements for fill
	G4Element* Na = new G4Element("Sodium", "Na", 11, 22.9898 * g / mole);
	G4Element* Cl = new G4Element("Chlorine", "Cl", 17, 35.45 * g / mole);
	G4Element* Ar = new G4Element("Argon", "Ar", 18, 39.948 * g / mole);
	G4Element* Ne = new G4Element("Neon", "Ne", 10, 20.18 * g / mole);
	G4Element* C  = new G4Element("Carbon", "C", 6, 12.01070 * g / mole);
	G4Element* H  = new G4Element("Hydrogen", "H", 1, 2.016 * g / mole);  
	G4Element* O  = new G4Element("Oxygen", "O", 8, 15.99903 * g / mole);

	//Element for anode/ cathode
	G4Element* Cu = new G4Element("Copper", "Cu", 29, 63.546 * g / mole);

	// Define Material
	// Case 1: Stainless Steel
	density = 7.7 * g / cm3;
	G4Material* FeNiCr = new G4Material(name = "Steel", density, ncomponents = 3);
	FeNiCr->AddElement(Fe, natoms = 7);
	FeNiCr->AddElement(Cr, natoms = 2);
	FeNiCr->AddElement(Ni, natoms = 1);
	// Case 2: Salt
	density = 2.16 * g / cm3;
	G4Material* NaCl = new G4Material(name = "Salt", density, ncomponents = 2);
	NaCl->AddElement(Na, natoms = 1);
	NaCl->AddElement(Cl, natoms = 1);
	// Case 3: Polyer: Polyethylene
	density = 0.96 * g / cm3;
	G4Material* PE = new G4Material(name = "Polyethylene", density, ncomponents = 2);
	PE->AddElement(C, natoms = 2);
	PE->AddElement(H, natoms = 4);
	// Case 4: Pressurized Ne Gas
	density = 0.070369 * g / cm3;
	pressure = 100 * bar;
	temperature = 330 * kelvin;
	G4Material* NeGas = new G4Material(name = "Neon Gas", density, ncomponents = 1, kStateGas, 						   temperature, pressure);
	NeGas->AddElement(Ne, natoms = 1);
	//Case 5: Pressurized Water
	pressure = 100 * bar;
	temperature = 375 * kelvin;
	density = 1.000*g/cm3;
	G4Material* H2O = new G4Material(name="Water", density, ncomponents=2, kStateLiquid, 						 temperature, pressure);
	H2O->AddElement(H, natoms=2);
	H2O->AddElement(O, natoms=1);


	// Display list of defined materials
	G4cout << G4endl << *(G4Material::GetMaterialTable()) << G4endl;

	// Default materials in setup
	defaultMaterial = H2O;
	detectorMaterial = NeGas;
}

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{

	// MANDATORY MOTHER "WORLD" VOLUME

	WorldSizeX = 40 * meter;
	WorldSizeY = 40 * meter;
	WorldSizeZ = 140 * meter;


	// CAVERN VOLUME
	//---Solid
	solidCavern = new G4Box("Cavern",				        //its name
				WorldSizeX / 2, 
				WorldSizeY / 2, 
				WorldSizeZ / 2);				//its size
	//---Logic
	logicCavern = new G4LogicalVolume(solidCavern,     //its solid
					  defaultMaterial, // its material
					  "Cavern");	   // its name
								
	//---Physical
	physiCavern = new G4PVPlacement(0,				//no rotation
					G4ThreeVector(),		//at (0,0,0)
					"Cavern",			//its name
					logicCavern,			//its logical volume
					NULL,				//its mother  volume
					false,				//no boolean operation
					0);				//copy number

	// TUBE VOLUME 
	G4double innerRadiusOfTheTube = 0. * meter;
	G4double outerRadiusOfTheTube = 8.000 * meter;
	G4double heightOfTheTube = 65 * meter;
	G4double startAngleOfTheTube = 0.*deg;
	G4double spanningAngleOfTheTube = 360.*deg;

	solidTube = new G4Tubs("Tube",		//its name
			       innerRadiusOfTheTube, 
			       outerRadiusOfTheTube,		//its minRadius and maxRadius
			       heightOfTheTube,
			       startAngleOfTheTube,
			       spanningAngleOfTheTube);

	logicTube = new G4LogicalVolume(solidTube,	//its solid
					detectorMaterial,	//its material
					"Tube");		//its name

	physiTube = new G4PVPlacement(0, 			//rotation
				      G4ThreeVector(0.*m, 0.*m, 0.*m), // translation
				      "Tube",				//its name
				      logicTube,			//its logical volume
				      physiCavern,			//its mother  volume
				      false,				//no boolean operation
			              0);				//copy number
	fScoringVolume = logicTube;


	// Visualization attributes
	G4VisAttributes* cavernVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0)); //Blue Water
	cavernVisAtt->SetVisibility(true);
	logicCavern->SetVisAttributes(cavernVisAtt);

	G4VisAttributes* tubeVisAtt = new G4VisAttributes(G4Colour(.5, .5, .5)); //White
	//tubeVisAtt->SetForceSolid(true);
	tubeVisAtt->SetVisibility(true);

	logicTube->SetVisAttributes(tubeVisAtt);

	// User Limits on step size

	logicCavern->SetUserLimits(new G4UserLimits(1 * meter));
	logicTube->SetUserLimits(new G4UserLimits(1 * meter));

	return physiCavern;
}

