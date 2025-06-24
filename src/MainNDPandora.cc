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

using namespace pandora;

namespace lar_nd_reco
{

//------------------------------------------------------------------------------------------------------------------------------------------

MainNDPandora::MainNDPandora(const std::string &mainName, const NDParameters &mainParameters) :
    m_mainParameters(mainParameters),
    m_pMainPandora{nullptr},
    m_NDParametersMap{},
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

const NDParameters MainNDPandora::GetPandoraParameters(const std::string &name) const
{
    NDParametersMap::const_iterator iter = m_NDParametersMap.find(name);

    if (m_NDParametersMap.end() == iter)
        throw pandora::StatusCodeException(pandora::STATUS_CODE_NOT_FOUND);

    return iter->second;
}

} // namespace lar_nd_reco
