/**
 *  @file   LArRecoND/app/LArRecoNDApp.cxx
 *
 *  @brief  Implementation of the LArRecoND application
 *
 *  $Log: $
 */

#include "LArRecoNDApp.h"

#include "MainNDPandora.h"
#include "NDParameters.h"

#ifdef MONITORING
#include "TApplication.h"
#endif

#include <iostream>

using namespace pandora;
using namespace lar_nd_reco;

int main(int argc, char *argv[])
{
    int errorNo(0);

    try
    {
        NDParameters parameters;

#ifdef MONITORING
        TApplication *pTApplication = new TApplication("LArRecoNDApp", &argc, argv);
        pTApplication->SetReturnFromRun(kTRUE);
#endif

	parameters.m_settingsFile = "settings/PandoraSettings_LArRecoND_Main.xml";
	MainNDPandora mainND("MainND", parameters);

	NDParameters LArNDPars(parameters);
	LArNDPars.m_volType = NDParameters::VolType::LArND;
	LArNDPars.m_tpcName = "volTPCActive";
	LArNDPars.m_inputFileName = "data/ND-LAr/MicroProdN3p1_NDLAr_2E18_FHC.flow.nu.0000001.FLOW.hdf5_hits_withMC.root";
	LArNDPars.m_inputTreeName = "events";
	LArNDPars.m_geomFileName = "data/ND-LAr/NDLArGeom.root";
	LArNDPars.m_geomManagerName = "Default";
	LArNDPars.m_settingsFile = "settings/PandoraSettings_LArRecoND_ThreeD_Evt.xml";
	mainND.AddPandoraInstance("LArND", LArNDPars);

	NDParameters TMSPars(parameters);
	TMSPars.m_volType = NDParameters::VolType::TMS;
	TMSPars.m_tpcName = "volTMS"; // scinBoxlvTMS
	TMSPars.m_dataFormat = NDParameters::DataFormat::TMSMC;
	TMSPars.m_inputFileName = "data/ND-LAr/MicroProdN3p1_NDLAr_2E18_FHC.tmsreco.nu.0000001.TMSRECOREADOUT.root";
	TMSPars.m_inputTreeName = "TMS";
	TMSPars.m_geomFileName = "data/ND-LAr/NDLArGeom.root";
	TMSPars.m_geomManagerName = "Default";
	TMSPars.m_lengthScale = 0.1; // mm to cm
	TMSPars.m_energyScale = 1e-3; // MeV to GeV
	TMSPars.m_settingsFile = "settings/PandoraSettings_LArRecoND_TMS.xml";
	mainND.AddPandoraInstance("TMS", TMSPars);

	// Create the TPC geometry using the tpcNames in the NDParameters geometry ROOT file.
	// This creates TPCs for both the main & added Pandora instances
	mainND.CreatePandoraTPCs();

	// Create detector gaps
	mainND.CreatePandoraDetectorGaps();

	// Setup external & algorithm parameters for all Pandora instances
	mainND.ConfigurePandoraInstances();

	// Setup the event inputs
	mainND.ConfigureEventInputs();

	// Process the events
	mainND.ProcessEvents();

	std::cout<<"Done"<<std::endl;
    }
    catch (const StatusCodeException &statusCodeException)
    {
        std::cerr << "Pandora StatusCodeException: " << statusCodeException.ToString() << statusCodeException.GetBackTrace() << std::endl;
        errorNo = 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception: " << std::endl;
        errorNo = 1;
    }

    // Cleanup
    return errorNo;
}
