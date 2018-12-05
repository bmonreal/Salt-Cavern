#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


RunAction::RunAction()
 : G4UserRunAction()
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in Analysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  //analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //
  
  // Creating histograms
  //analysisManager->CreateH1("Edet","Edep in detector", 100, 0., 1.*MeV);
  //analysisManager->CreateH1("Ldet","trackL in detector", 100, 0., 0.015*m);
  //analysisManager->CreateH1("StartingPt","StartingPt", 100, 0., 20*m);

  // Creating ntuple
  //
  analysisManager->CreateNtuple("Data", "Run1");
  analysisManager->CreateNtupleDColumn("E_dep Tot");
  analysisManager->CreateNtupleDColumn("L_det Tot");
  analysisManager->CreateNtupleDColumn("StartingPt");
  analysisManager->CreateNtupleDColumn("EndPt");
  analysisManager->CreateNtupleDColumn("Event Number");
  analysisManager->CreateNtupleDColumn("Event Number (Step)");
  analysisManager->CreateNtupleDColumn("Pre-Step X");
  analysisManager->CreateNtupleDColumn("Pre-Step Y");
  analysisManager->CreateNtupleDColumn("Pre-Step Z");
  analysisManager->CreateNtupleDColumn("Post-Step X");
  analysisManager->CreateNtupleDColumn("Post-Step Y");
  analysisManager->CreateNtupleDColumn("Post-Step Z");
  analysisManager->CreateNtupleDColumn("Momentum X");
  analysisManager->CreateNtupleDColumn("Momentum Y");
  analysisManager->CreateNtupleDColumn("Momentum Z");
  analysisManager->CreateNtupleDColumn("Is Primary (0 = yes)");
  analysisManager->CreateNtupleDColumn("Particle Name");
  analysisManager->FinishNtuple();


}


RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();

}


void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "Run1DataGamma.csv";
  analysisManager->OpenFile(fileName);

}


void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    
    G4cout << " Edet : mean = " 
       << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;
     
    G4cout << " Ldet : mean = " 
      << G4BestUnit(analysisManager->GetH1(1)->mean(), "Length") 
      << " rms = " 
      << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Length") << G4endl;
  }

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();

}
