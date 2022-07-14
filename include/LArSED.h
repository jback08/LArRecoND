/**
 *  @file   LArReco/include/LArSED.h
 *
 *  @brief  Header file defining the "SimEnergyDeposit" format.
 *          Based on autogenerated ROOT code with Pandora-style edits
 *
 *  $Log: $
 */
#ifndef PANDORA_LAR_SED_H
#define PANDORA_LAR_SED_H 1

#include "TChain.h"
#include "TFile.h"
#include "TROOT.h"

// Header file for the classes stored in the TTree if any.
#include <vector>

namespace lar_nd_reco
{

class LArSED
{
public:
    /**
     *  @brief  Constructor requiring TTree pointer
     *
     *  @param  tree The TTree pointer
     */
    LArSED(TTree *tree = nullptr);

    /**
     *  @brief  Destructor
     */
    virtual ~LArSED();

    /**
     *  @brief  Get the corresponding data entry
     *
     *  @param  entry The entry integer index
     *
     *  @return Total number of bytes read
     */
    virtual Int_t GetEntry(Long64_t entry);

    /**
     *  @brief  Initialise using the input TTree
     *
     *  @param  tree The input TTree
     */
    virtual void Init(TTree *tree);

    TTree *fChain;  ///< pointer to the analyzed TTree or TChain
    Int_t fCurrent; ///< current Tree number in a TChain

    // Declaration of leaf types
    Int_t run;
    Int_t subrun;
    Int_t event;
    std::vector<int> *nuPDG;
    std::vector<int> *ccnc;
    std::vector<int> *mode;
    std::vector<float> *enu;
    std::vector<float> *Q2;
    std::vector<float> *W;
    std::vector<float> *X;
    std::vector<float> *Y;
    std::vector<int> *hitnuc;
    std::vector<int> *target;
    std::vector<float> *nuvtxx;
    std::vector<float> *nuvtxy;
    std::vector<float> *nuvtxz;
    std::vector<float> *nu_dcosx;
    std::vector<float> *nu_dcosy;
    std::vector<float> *nu_dcosz;
    std::vector<float> *lep_mom;
    std::vector<float> *lep_dcosx;
    std::vector<float> *lep_dcosy;
    std::vector<float> *lep_dcosz;
    std::vector<float> *t0;
    std::vector<int> *mcp_id;
    std::vector<int> *mcp_mother;
    std::vector<int> *mcp_pdg;
    std::vector<int> *mcp_nuid;
    std::vector<float> *mcp_energy;
    std::vector<float> *mcp_px;
    std::vector<float> *mcp_py;
    std::vector<float> *mcp_pz;
    std::vector<float> *mcp_startx;
    std::vector<float> *mcp_starty;
    std::vector<float> *mcp_startz;
    std::vector<float> *mcp_endx;
    std::vector<float> *mcp_endy;
    std::vector<float> *mcp_endz;
    std::vector<float> *sed_startx;
    std::vector<float> *sed_starty;
    std::vector<float> *sed_startz;
    std::vector<float> *sed_endx;
    std::vector<float> *sed_endy;
    std::vector<float> *sed_endz;
    std::vector<float> *sed_energy;
    std::vector<int> *sed_id;
    std::vector<int> *sed_pdg;
    std::vector<std::string> *sed_det;

    // List of branches
    TBranch *b_run;
    TBranch *b_subrun;
    TBranch *b_event;
    TBranch *b_nuPDG;
    TBranch *b_ccnc;
    TBranch *b_mode;
    TBranch *b_enu;
    TBranch *b_Q2;
    TBranch *b_W;
    TBranch *b_X;
    TBranch *b_Y;
    TBranch *b_hitnuc;
    TBranch *b_target;
    TBranch *b_nuvtxx;
    TBranch *b_nuvtxy;
    TBranch *b_nuvtxz;
    TBranch *b_nu_dcosx;
    TBranch *b_nu_dcosy;
    TBranch *b_nu_dcosz;
    TBranch *b_lep_mom;
    TBranch *b_lep_dcosx;
    TBranch *b_lep_dcosy;
    TBranch *b_lep_dcosz;
    TBranch *b_t0;
    TBranch *b_mcp_id;
    TBranch *b_mcp_mother;
    TBranch *b_mcp_pdg;
    TBranch *b_mcp_nuid;
    TBranch *b_mcp_energy;
    TBranch *b_mcp_px;
    TBranch *b_mcp_py;
    TBranch *b_mcp_pz;
    TBranch *b_mcp_startx;
    TBranch *b_mcp_starty;
    TBranch *b_mcp_startz;
    TBranch *b_mcp_endx;
    TBranch *b_mcp_endy;
    TBranch *b_mcp_endz;
    TBranch *b_sed_startx;
    TBranch *b_sed_starty;
    TBranch *b_sed_startz;
    TBranch *b_sed_endx;
    TBranch *b_sed_endy;
    TBranch *b_sed_endz;
    TBranch *b_sed_energy;
    TBranch *b_sed_id;
    TBranch *b_sed_pdg;
    TBranch *b_sed_det;
};

//------------------------------------------------------------------------------------------------------------------------------------------

LArSED::LArSED(TTree *tree) : fChain(nullptr)
{
    // if parameter tree is not specified (or zero), connect the file
    // used to generate this class and read the Tree.
    if (tree == nullptr)
    {
        TFile *f = dynamic_cast<TFile *>(gROOT->GetListOfFiles()->FindObject("simdump.root"));
        if (!f || !f->IsOpen())
        {
            f = new TFile("simdump.root");
        }
        TDirectory *dir = dynamic_cast<TDirectory *>(f->Get("simdump.root:/simdump"));
        dir->GetObject("ndsim", tree);
    }
    Init(tree);
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSED::~LArSED()
{
    if (!fChain)
        return;
    delete fChain->GetCurrentFile();
}

//------------------------------------------------------------------------------------------------------------------------------------------

Int_t LArSED::GetEntry(Long64_t entry)
{
    // Read contents of entry.
    if (!fChain)
        return 0;
    return fChain->GetEntry(entry);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSED::Init(TTree *tree)
{
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set object pointer
    nuPDG = 0;
    ccnc = 0;
    mode = 0;
    enu = 0;
    Q2 = 0;
    W = 0;
    X = 0;
    Y = 0;
    hitnuc = 0;
    target = 0;
    nuvtxx = 0;
    nuvtxy = 0;
    nuvtxz = 0;
    nu_dcosx = 0;
    nu_dcosy = 0;
    nu_dcosz = 0;
    lep_mom = 0;
    lep_dcosx = 0;
    lep_dcosy = 0;
    lep_dcosz = 0;
    t0 = 0;
    mcp_id = 0;
    mcp_mother = 0;
    mcp_pdg = 0;
    mcp_nuid = 0;
    mcp_energy = 0;
    mcp_px = 0;
    mcp_py = 0;
    mcp_pz = 0;
    mcp_startx = 0;
    mcp_starty = 0;
    mcp_startz = 0;
    mcp_endx = 0;
    mcp_endy = 0;
    mcp_endz = 0;
    sed_startx = 0;
    sed_starty = 0;
    sed_startz = 0;
    sed_endx = 0;
    sed_endy = 0;
    sed_endz = 0;
    sed_energy = 0;
    sed_id = 0;
    sed_pdg = 0;
    sed_det = 0;
    // Set branch addresses and branch pointers
    if (!tree)
        return;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("run", &run, &b_run);
    fChain->SetBranchAddress("subrun", &subrun, &b_subrun);
    fChain->SetBranchAddress("event", &event, &b_event);
    fChain->SetBranchAddress("nuPDG", &nuPDG, &b_nuPDG);
    fChain->SetBranchAddress("ccnc", &ccnc, &b_ccnc);
    fChain->SetBranchAddress("mode", &mode, &b_mode);
    fChain->SetBranchAddress("enu", &enu, &b_enu);
    fChain->SetBranchAddress("Q2", &Q2, &b_Q2);
    fChain->SetBranchAddress("W", &W, &b_W);
    fChain->SetBranchAddress("X", &X, &b_X);
    fChain->SetBranchAddress("Y", &Y, &b_Y);
    fChain->SetBranchAddress("hitnuc", &hitnuc, &b_hitnuc);
    fChain->SetBranchAddress("target", &target, &b_target);
    fChain->SetBranchAddress("nuvtxx", &nuvtxx, &b_nuvtxx);
    fChain->SetBranchAddress("nuvtxy", &nuvtxy, &b_nuvtxy);
    fChain->SetBranchAddress("nuvtxz", &nuvtxz, &b_nuvtxz);
    fChain->SetBranchAddress("nu_dcosx", &nu_dcosx, &b_nu_dcosx);
    fChain->SetBranchAddress("nu_dcosy", &nu_dcosy, &b_nu_dcosy);
    fChain->SetBranchAddress("nu_dcosz", &nu_dcosz, &b_nu_dcosz);
    fChain->SetBranchAddress("lep_mom", &lep_mom, &b_lep_mom);
    fChain->SetBranchAddress("lep_dcosx", &lep_dcosx, &b_lep_dcosx);
    fChain->SetBranchAddress("lep_dcosy", &lep_dcosy, &b_lep_dcosy);
    fChain->SetBranchAddress("lep_dcosz", &lep_dcosz, &b_lep_dcosz);
    fChain->SetBranchAddress("t0", &t0, &b_t0);
    fChain->SetBranchAddress("mcp_id", &mcp_id, &b_mcp_id);
    fChain->SetBranchAddress("mcp_mother", &mcp_mother, &b_mcp_mother);
    fChain->SetBranchAddress("mcp_pdg", &mcp_pdg, &b_mcp_pdg);
    fChain->SetBranchAddress("mcp_nuid", &mcp_nuid, &b_mcp_nuid);
    fChain->SetBranchAddress("mcp_energy", &mcp_energy, &b_mcp_energy);
    fChain->SetBranchAddress("mcp_px", &mcp_px, &b_mcp_px);
    fChain->SetBranchAddress("mcp_py", &mcp_py, &b_mcp_py);
    fChain->SetBranchAddress("mcp_pz", &mcp_pz, &b_mcp_pz);
    fChain->SetBranchAddress("mcp_startx", &mcp_startx, &b_mcp_startx);
    fChain->SetBranchAddress("mcp_starty", &mcp_starty, &b_mcp_starty);
    fChain->SetBranchAddress("mcp_startz", &mcp_startz, &b_mcp_startz);
    fChain->SetBranchAddress("mcp_endx", &mcp_endx, &b_mcp_endx);
    fChain->SetBranchAddress("mcp_endy", &mcp_endy, &b_mcp_endy);
    fChain->SetBranchAddress("mcp_endz", &mcp_endz, &b_mcp_endz);
    fChain->SetBranchAddress("sed_startx", &sed_startx, &b_sed_startx);
    fChain->SetBranchAddress("sed_starty", &sed_starty, &b_sed_starty);
    fChain->SetBranchAddress("sed_startz", &sed_startz, &b_sed_startz);
    fChain->SetBranchAddress("sed_endx", &sed_endx, &b_sed_endx);
    fChain->SetBranchAddress("sed_endy", &sed_endy, &b_sed_endy);
    fChain->SetBranchAddress("sed_endz", &sed_endz, &b_sed_endz);
    fChain->SetBranchAddress("sed_energy", &sed_energy, &b_sed_energy);
    fChain->SetBranchAddress("sed_id", &sed_id, &b_sed_id);
    fChain->SetBranchAddress("sed_pdg", &sed_pdg, &b_sed_pdg);
    fChain->SetBranchAddress("sed_det", &sed_det, &b_sed_det);
}

} // namespace lar_nd_reco

#endif
