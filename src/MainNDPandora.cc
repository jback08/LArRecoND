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
#include "NDGeometryHelper.h"

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

const NDParameters MainNDPandora::GetPandoraParameters(const std::string &name) const
{
    NDParametersMap::const_iterator iter = m_NDParametersMap.find(name);

    if (m_NDParametersMap.end() == iter)
        throw pandora::StatusCodeException(pandora::STATUS_CODE_NOT_FOUND);

    return iter->second;
}

} // namespace lar_nd_reco
