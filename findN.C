void findN()
{
    gROOT->Reset();
    TFile *f=new TFile("Run1Data.root"); // opens the root file
    TTree *tree=(TTree*)f->Get("ND"); // creates the TTree object
    TTree *tree2=(TTree*)f->Get("ND"); // creates the TTree object
    double_t Edet;
    double_t Ldet;
    tree->SetBranchAddress("Edet", &Edet);
    tree2->SetBranchAddress("Ldet", &Ldet);

    Int_t entries = tree->GetEntries();
    Int_t pathlength = 0;
    Int_t count = 0;
    Int_t ETOT = 0;
    for (int i = 0; i<entries; i++)
    {
	tree->GetEntry(i); 
	if(Edet > 0)
		count = count + 1;
		ETOT = ETOT + Edet;
    }
    cout<<"number depositing energy = "<<count<<" particles"<<endl;
    cout<<"total energy deposited = "<<ETOT<<" MeV"<<endl;
    
   
    for (int i = 0; i<entries; i++)
    {
        tree->GetEntry(i);
	if(Edet == 0.8){
		tree2->GetEntry(i); 
                cout<<"run number = "<<i<<endl;
                cout<<"total track length= "<<Ldet<<" cm"<<endl;
        }
    }
}
