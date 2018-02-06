#ifndef NeutrinoDetectorHit_h
#define NeutrinoDetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/// Calorimeter hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class NeutrinoDetectorHit : public G4VHit
{
  public:
    NeutrinoDetectorHit();
    NeutrinoDetectorHit(const NeutrinoDetectorHit&);
    virtual ~NeutrinoDetectorHit();

    // operators
    const NeutrinoDetectorHit& operator=(const NeutrinoDetectorHit&);
    G4int operator==(const NeutrinoDetectorHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void Add(G4double de, G4double dl);

    // get methods
    G4double GetEdep() const;
    G4double GetTrackLength() const;
      
  private:
    G4double fEdep;        ///< Energy deposit in the sensitive volume
    G4double fTrackLength; ///< Track length in the  sensitive volume
};


using NeutrinoDetectorHitsCollection = G4THitsCollection<NeutrinoDetectorHit>;

extern G4ThreadLocal G4Allocator<NeutrinoDetectorHit>* NeutrinoDetectorHitAllocator;


inline void* NeutrinoDetectorHit::operator new(size_t)
{
  if (!NeutrinoDetectorHitAllocator) {
    NeutrinoDetectorHitAllocator = new G4Allocator<NeutrinoDetectorHit>;
  }
  void *hit;
  hit = (void *) NeutrinoDetectorHitAllocator->MallocSingle();
  return hit;
}

inline void NeutrinoDetectorHit::operator delete(void *hit)
{
  if (!NeutrinoDetectorHitAllocator) {
    NeutrinoDetectorHitAllocator = new G4Allocator<NeutrinoDetectorHit>;
  }
  NeutrinoDetectorHitAllocator->FreeSingle((NeutrinoDetectorHit*) hit);
}

inline void NeutrinoDetectorHit::Add(G4double de, G4double dl) {
  fEdep += de; 
  fTrackLength += dl;
}

inline G4double NeutrinoDetectorHit::GetEdep() const { 
  return fEdep; 
}

inline G4double NeutrinoDetectorHit::GetTrackLength() const { 
  return fTrackLength; 
}

#endif
