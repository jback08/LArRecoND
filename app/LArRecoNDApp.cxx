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

	MainNDPandora mainND("MainND", parameters);

	NDParameters LArNDPars(parameters);
	LArNDPars.m_volType = NDParameters::VolType::LArND;
	LArNDPars.m_tpcName = "volTPCActive";
	mainND.AddPandoraInstance("LArND", LArNDPars);

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
