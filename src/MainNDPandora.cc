/**
 *  @file   src/MainNDPandora.cc
 *
 *  @brief  Implementation of the main Pandora ND instance
 *
 *  $Log: $
 */

#include "MainNDPandora.h"

#include "Api/PandoraApi.h"
#include "PandoraMonitoringApi.h"

#include "larpandoracontent/LArContent.h"
#include "larpandoracontent/LArControlFlow/MasterAlgorithm.h"
#include "larpandoracontent/LArControlFlow/MultiPandoraApi.h"
#include "larpandoracontent/LArPlugins/LArPseudoLayerPlugin.h"
#include "larpandoracontent/LArPlugins/LArRotationalTransformationPlugin.h"

#ifdef LIBTORCH_DL
#include "larpandoradlcontent/LArDLContent.h"
#endif

#include "LArNDContent.h"
#include "LArNDEventInput.h"
#include "NDGeometryHelper.h"
#include "TMSEventInput.h"

using namespace pandora;

namespace lar_nd_reco
{

//------------------------------------------------------------------------------------------------------------------------------------------

MainNDPandora::MainNDPandora(const std::string &mainName, const NDParameters &mainParameters) :
    m_mainParameters(mainParameters),
    m_pMainPandora{nullptr},
    m_NDParametersMap{},
    m_NDSimpleTPCGeomMap{},
    m_instanceOrder{}
{
    m_pMainPandora = this->CreatePandoraInstance(mainName, mainParameters);
    MultiPandoraApi::AddPrimaryPandoraInstance(m_pMainPandora);
    MultiPandoraApi::SetVolumeId(m_pMainPandora, NDParameters::VolType::Main);
}

//------------------------------------------------------------------------------------------------------------------------------------------

MainNDPandora::~MainNDPandora()
{
    // Delete all Pandora instances, including the main one
    std::cout << "DeletePandoraInstances" << std::endl;
    MultiPandoraApi::DeletePandoraInstances(m_pMainPandora);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MainNDPandora::AddPandoraInstance(const std::string &name, const NDParameters &parameters)
{
    const pandora::Pandora *pPandora = this->CreatePandoraInstance(name, parameters);

    // Make this a daughter of the main instance
    MultiPandoraApi::AddDaughterPandoraInstance(m_pMainPandora, pPandora);

    // Also add this as a primary instance for the MultiPandoraApi,
    // which is used to add daughter instances inside MasterThreeDAlgorithm etc.
    MultiPandoraApi::AddPrimaryPandoraInstance(pPandora);
    // Also set the volume Id (VolType enum)
    MultiPandoraApi::SetVolumeId(pPandora, parameters.m_volType);

    // Add parameters to the internal map
    m_NDParametersMap.insert(std::make_pair(name, parameters));

    // Keep track of the instance insertion order
    m_instanceOrder.emplace_back(name);
}

//------------------------------------------------------------------------------------------------------------------------------------------

const pandora::Pandora *MainNDPandora::CreatePandoraInstance(const std::string &name, const NDParameters &parameters) const
{
    std::cout << "Creating Pandora instance " << name << std::endl;

    const pandora::Pandora *pPandora = new pandora::Pandora(name);

    PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, LArContent::RegisterAlgorithms(*pPandora));
#ifdef LIBTORCH_DL
    PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, LArDLContent::RegisterAlgorithms(*pPandora));
#endif
    PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, LArContent::RegisterBasicPlugins(*pPandora));

    if (parameters.m_use3D)
    {
        std::cout << "Registered 3D algorithms" << std::endl;
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, LArNDContent::RegisterAlgorithms(*pPandora));
    }

    return pPandora;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MainNDPandora::CreatePandoraTPCs()
{
    std::cout << "CreatePandoraTPCs" << std::endl;

    // Keep track of the total number of TPCs being made, since we need
    // to use this to set unique TPC IDs for all Pandora instances
    int nTotalTPCs{0};

    // Loop over the Pandora instances
    const PandoraInstanceList pandoraList = MultiPandoraApi::GetDaughterPandoraInstanceList(m_pMainPandora);
    for (const auto *pPandora : pandoraList)
    {
        const std::string pandoraName = pPandora->GetName();
        const NDParameters parameters = this->GetPandoraParameters(pandoraName);

        std::cout << "Creating TPCs for " << pandoraName << std::endl;

        const NDGeometryHelper geometry(parameters.m_geomFileName, parameters.m_geomManagerName);

        // TPC name from the parameters
        const std::string tpcName = parameters.m_tpcName;

        // Simple object storing the TPC boxes
        const NDSimpleTPCGeom tpcGeom = geometry.GetTPCGeometry(tpcName, nTotalTPCs);
        // Store this in the TPC geometry map (for processing the events)
        m_NDSimpleTPCGeomMap.insert(std::make_pair(pandoraName, tpcGeom));

        nTotalTPCs += tpcGeom.m_TPCs.size();
        std::cout << "nTotalTPCs = " << nTotalTPCs << std::endl;

        // Create the TPCs for the given & main Pandora instances
        std::map<unsigned int, NDSimpleTPC>::const_iterator tpcIter;
        for (tpcIter = tpcGeom.m_TPCs.begin(); tpcIter != tpcGeom.m_TPCs.end(); ++tpcIter)
        {
            const NDSimpleTPC tpc = tpcIter->second;
            PandoraApi::Geometry::LArTPC::Parameters geoParameters;

            geoParameters.m_centerX = 0.5 * (tpc.m_x_min + tpc.m_x_max);
            geoParameters.m_centerY = 0.5 * (tpc.m_y_min + tpc.m_y_max);
            geoParameters.m_centerZ = 0.5 * (tpc.m_z_min + tpc.m_z_max);
            geoParameters.m_widthX = (tpc.m_x_max - tpc.m_x_min);
            geoParameters.m_widthY = (tpc.m_y_max - tpc.m_y_min);
            geoParameters.m_widthZ = (tpc.m_z_max - tpc.m_z_min);

            // ATTN: parameters past here taken from uboone
            geoParameters.m_larTPCVolumeId = tpc.m_TPC_ID;
            geoParameters.m_wirePitchU = 0.300000011921;
            geoParameters.m_wirePitchV = 0.300000011921;
            geoParameters.m_wirePitchW = 0.300000011921;
            geoParameters.m_wireAngleU = 1.04719758034;
            geoParameters.m_wireAngleV = -1.04719758034;
            geoParameters.m_wireAngleW = 0.0;
            geoParameters.m_sigmaUVW = 1;
            geoParameters.m_isDriftInPositiveX = tpc.m_TPC_ID % 2;

            try
            {
                // Create TPC for both pPandora and the main Pandora instance
                PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::Geometry::LArTPC::Create(*pPandora, geoParameters));
                PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::Geometry::LArTPC::Create(*m_pMainPandora, geoParameters));
                std::cout << "Created TPC " << tpc.m_TPC_ID << " for " << pPandora->GetName() << " & " << m_pMainPandora->GetName() << std::endl;
            }
            catch (const pandora::StatusCodeException &)
            {
                std::cout << "CreatePandoraTPCs - unable to create TPC, insufficient or invalid information supplied" << std::endl;
            }
        } // TPCs

        // TPC transformation plugins for this Pandora instance
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::SetPseudoLayerPlugin(*pPandora, new lar_content::LArPseudoLayerPlugin));
        PANDORA_THROW_RESULT_IF(
            STATUS_CODE_SUCCESS, !=, PandoraApi::SetLArTransformationPlugin(*pPandora, new lar_content::LArRotationalTransformationPlugin));
    } // Pandora instances

    // Main instance TPC transformation plugins
    PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::SetPseudoLayerPlugin(*m_pMainPandora, new lar_content::LArPseudoLayerPlugin));
    PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=,
        PandoraApi::SetLArTransformationPlugin(*m_pMainPandora, new lar_content::LArRotationalTransformationPlugin));
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MainNDPandora::CreatePandoraDetectorGaps()
{
    // To be implemented
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MainNDPandora::ConfigurePandoraInstances()
{
    std::cout << "ConfigurePandoraInstances" << std::endl;

    this->ProcessExternalParameters(m_pMainPandora, m_mainParameters);
    PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::ReadSettings(*m_pMainPandora, m_mainParameters.m_settingsFile));

    const PandoraInstanceList pandoraList = MultiPandoraApi::GetDaughterPandoraInstanceList(m_pMainPandora);
    for (const auto *pPandora : pandoraList)
    {
        const NDParameters parameters = this->GetPandoraParameters(pPandora->GetName());
        this->ProcessExternalParameters(pPandora, parameters);
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::ReadSettings(*pPandora, parameters.m_settingsFile));
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MainNDPandora::ProcessExternalParameters(const pandora::Pandora *const pPandora, const NDParameters &parameters) const
{
    auto *const pEventSteeringParameters = new lar_content::MasterAlgorithm::ExternalSteeringParameters;
    pEventSteeringParameters->m_shouldRunAllHitsCosmicReco = parameters.m_shouldRunAllHitsCosmicReco;
    pEventSteeringParameters->m_shouldRunStitching = parameters.m_shouldRunStitching;
    pEventSteeringParameters->m_shouldRunCosmicHitRemoval = parameters.m_shouldRunCosmicHitRemoval;
    pEventSteeringParameters->m_shouldRunSlicing = parameters.m_shouldRunSlicing;
    pEventSteeringParameters->m_shouldRunNeutrinoRecoOption = parameters.m_shouldRunNeutrinoRecoOption;
    pEventSteeringParameters->m_shouldRunCosmicRecoOption = parameters.m_shouldRunCosmicRecoOption;
    pEventSteeringParameters->m_shouldPerformSliceId = parameters.m_shouldPerformSliceId;
    pEventSteeringParameters->m_printOverallRecoStatus = parameters.m_printOverallRecoStatus;

    // LArMasterThreeD or LArMaster algorithms
    if (parameters.m_use3D)
    {
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::SetExternalParameters(*pPandora, "LArMasterThreeD", pEventSteeringParameters));
    }
    else
    {
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::SetExternalParameters(*pPandora, "LArMaster", pEventSteeringParameters));
    }

#ifdef LIBTORCH_DL
    auto *const pEventSettingsParametersCopy = new lar_content::MasterAlgorithm::ExternalSteeringParameters(*pEventSteeringParameters);
    PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=,
        pandora::ExternallyConfiguredAlgorithm::SetExternalParameters(*pPandora, "LArDLMaster", pEventSettingsParametersCopy));
#endif
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MainNDPandora::ConfigureEventInputs()
{
    // Create NDEventInputs for each Pandora daughter instance
    const PandoraInstanceList pandoraList = MultiPandoraApi::GetDaughterPandoraInstanceList(m_pMainPandora);
    for (const auto *pPandora : pandoraList)
    {
        const std::string pandoraName = pPandora->GetName();
        const NDParameters parameters = this->GetPandoraParameters(pandoraName);

        // The TPC geometry
        const NDSimpleTPCGeom tpcGeom = this->GetNDSimpleTPCGeom(pandoraName);

        // Create the NDEventInput pointer
        const NDEventInput *pEventInput;
        const int volType = parameters.m_volType;

        if (volType == NDParameters::VolType::LArND)
            pEventInput = new LArNDEventInput(pPandora, parameters, tpcGeom);
        else if (volType == NDParameters::VolType::TMS)
            pEventInput = new TMSEventInput(pPandora, parameters, tpcGeom);

        // Insert into the event input map
        m_NDEventInputMap.insert(std::make_pair(pandoraName, pEventInput));
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MainNDPandora::ProcessEvents()
{
    std::cout << "ProcessEvents" << std::endl;

    // ntuple index number 0 to N-1, event number any other integer

    // Starting event
    //const int startEvt = m_mainParameters.m_nEventsToSkip > 0 ? m_mainParameters.m_nEventsToSkip : 0;
    // Number of events to process, up to nEntries
    //const int nProcess = m_mainParameters.m_nEventsToProcess > 0 ? m_mainParameters.m_nEventsToProcess : nEntries;
    // End event, up to nEntries
    //const int endEvt = (startEvt + nProcess) < nEntries ? startEvt + nProcess : nEntries;

    const int startEvt(0), endEvt(2);

    std::cout << "Start event is " << startEvt << " and end event is " << endEvt - 1 << std::endl;
    NDEventInputMap::const_iterator iter;
    int nTotalHits{0}, nTotalTracks{0};

    for (int iEvt = startEvt; iEvt < endEvt; iEvt++)
    {
        std::cout << std::endl << "   PROCESSING EVENT: " << iEvt << std::endl << std::endl;

        // Make MCParticles, CaloHits & Tracks for each Pandora event input.
        // Keep track of how many CaloHits & Tracks we are making in order to set unique
        // parent addresses for the main Pandora instance (copied to the specific instance).
        // MCParticles should use true IDs to set their parent addresses
        for (iter = m_NDEventInputMap.begin(); iter != m_NDEventInputMap.end(); ++iter)
        {
            const NDEventInput *pEventInput = iter->second;
            if (!pEventInput)
                continue;

            std::cout << "NDEventInput name = " << iter->first << std::endl;
            pEventInput->CreateMCParticles(iEvt);
            nTotalHits = pEventInput->CreateCaloHits(iEvt, nTotalHits);
            nTotalTracks = pEventInput->CreateTracks(iEvt, nTotalTracks);

            std::cout << "nTotalHits = " << nTotalHits << ", nTotalTracks = " << nTotalTracks << std::endl;

            // Run the algorithms for this Pandora event instance
            const pandora::Pandora *pPandora = pEventInput->GetPandoraInstance();
            PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::ProcessEvent(*pPandora));

            // Copy PFOs to the main instance, renamed with the Pandora instance name prefix
            this->CopyPFOs(pPandora);
        }

        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::ProcessEvent(*m_pMainPandora));

        // Reset for the next event
        for (iter = m_NDEventInputMap.begin(); iter != m_NDEventInputMap.end(); ++iter)
        {
            const NDEventInput *pEventInput = iter->second;
            if (!pEventInput)
                continue;

            const pandora::Pandora *pPandora = pEventInput->GetPandoraInstance();
            PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::Reset(*pPandora));
        }
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::Reset(*m_pMainPandora));
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MainNDPandora::CopyPFOs(const pandora::Pandora *const pPandora) const
{
    const PfoList *pPFOs{nullptr};
    PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::GetCurrentPfoList(*pPandora, pPFOs));

    const int nPFOs = (pPFOs != nullptr) ? pPFOs->size() : 0;
    std::cout << "Copying " << nPFOs << " PFOs from " << pPandora->GetName() << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

const NDParameters MainNDPandora::GetPandoraParameters(const std::string &name) const
{
    NDParametersMap::const_iterator iter = m_NDParametersMap.find(name);

    if (m_NDParametersMap.end() == iter)
        throw pandora::StatusCodeException(pandora::STATUS_CODE_NOT_FOUND);

    return iter->second;
}

//------------------------------------------------------------------------------------------------------------------------------------------

const NDSimpleTPCGeom MainNDPandora::GetNDSimpleTPCGeom(const std::string &name) const
{
    NDSimpleTPCGeomMap::const_iterator iter = m_NDSimpleTPCGeomMap.find(name);

    if (m_NDSimpleTPCGeomMap.end() == iter)
        throw pandora::StatusCodeException(pandora::STATUS_CODE_NOT_FOUND);

    return iter->second;
}

//------------------------------------------------------------------------------------------------------------------------------------------

const NDEventInput *MainNDPandora::GetNDEventInput(const std::string &name) const
{
    NDEventInputMap::const_iterator iter = m_NDEventInputMap.find(name);

    if (m_NDEventInputMap.end() == iter)
        throw pandora::StatusCodeException(pandora::STATUS_CODE_NOT_FOUND);

    return iter->second;
}

} // namespace lar_nd_reco
