#include "NeutrinoDetectorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<NeutrinoDetectorHit>* NeutrinoDetectorHitAllocator = 0;


NeutrinoDetectorHit::NeutrinoDetectorHit()
 : G4VHit(),
   fEdep(0.),
   fTrackLength(0.)
{}

NeutrinoDetectorHit::~NeutrinoDetectorHit() {}

NeutrinoDetectorHit::NeutrinoDetectorHit(const NeutrinoDetectorHit& right)
  : G4VHit()
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
}


const NeutrinoDetectorHit& NeutrinoDetectorHit::operator=(const NeutrinoDetectorHit& right)
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;

  return *this;
}


G4int NeutrinoDetectorHit::operator==(const NeutrinoDetectorHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}


void NeutrinoDetectorHit::Print()
{
  G4cout
     << "Edep: " 
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " track length: " 
     << std::setw(7) << G4BestUnit( fTrackLength,"Length")
     << G4endl;
}

