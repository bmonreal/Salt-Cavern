void plot()
{
    gROOT->Reset();
    TFile *f=new TFile("Run1Data.root"); // opens the root file
    TTree *tr=(TTree*)f->Get("ND"); // creates the TTree object
    TBranch *Edet = (TBranch*)tr->GetBranch("Edet");
    TBranch *StartingPt = (TBranch*)tr->GetBranch("StartingPt");
    tr->SetMarkerStyle(7);
    tr->Draw("Edet:StartingPt");
}
