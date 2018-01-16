#include "DetectorConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

DetectorConstruction::DetectorConstruction()
{}

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
	G4Material* NaCl = new G4Material(name = "Salt", density, ncomponents =2);
	NaCl->AddElement(Na, natoms = 1);
	NaCl->AddElement(Cl, natoms = 1);

	// Display list of defined materials
	G4cout << G4endl << *(G4Material::GetMaterialTable()) << G4endl;

	// Default materials in setup
	defaultMaterial = NaCl;
	detectorMaterial = FeNiCr;
}

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{

	// MANDATORY MOTHER “WORLD” VOLUME

	WorldSizeX = 10 * meter;
	WorldSizeY = 10 * meter;
	WorldSizeZ = 30 * meter;

	//---Solid
	solidCavern = new G4Box("Cavern",				        //its name
							WorldSizeX / 2, 
							WorldSizeY / 2, 
							WorldSizeZ / 2);			//its size
	//---Logic
	logicCavern = new G4LogicalVolume(solidCavern,     //its solid
									  defaultMaterial, // its material
									  "Cavern");		   // its name
								
	//---Physical
	physiCavern = new G4PVPlacement(0,				//no rotation
								   G4ThreeVector(),	//at (0,0,0)
								   "Cavern",		//its name
							       logicCavern,	//its logical volume
								   NULL,			//its mother  volume
								   false,			//no boolean operation
								   0);				//copy number

	// TUBE VOLUME 
	G4double innerRadiusOfTheTube = 1. * meter;
	G4double outerRadiusOfTheTube = 1.1 * meter;
	G4double heightOfTheTube = 11 * meter;
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

	physiTube = new G4PVPlacement(0,			//rotation
								  G4ThreeVector(1.4*m, 1.4*m, 2.*m), // translation
								  "Tube",		//its name
								  logicTube,	//its logical volume
								  physiCavern,		//its mother  volume
								  false,		//no boolean operation
								  0);			//copy number

	// Visualization attributes
	G4VisAttributes* cavernVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0)); //White
	cavernVisAtt->SetVisibility(true);
	logicCavern->SetVisAttributes(cavernVisAtt);

	G4VisAttributes* tubeVisAtt = new G4VisAttributes(G4Colour(0, 1.0, 1.0)); //Blue
	tubeVisAtt->SetForceSolid(true);
	tubeVisAtt->SetVisibility(true);

	logicTube->SetVisAttributes(tubeVisAtt);

	// User Limits on step size

	logicCavern->SetUserLimits(new G4UserLimits(1 * meter));
	logicTube->SetUserLimits(new G4UserLimits(1 * meter));

	return physiCavern;
}

