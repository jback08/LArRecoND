/**
 *  @file   src/CreateMainNDListsAlgorithm.cc
 *
 *  @brief  Implementation of creating various lists for the MainND instance
 *
 *  $Log: $
 */

#include "Api/PandoraApi.h"
#include "Api/PandoraContentApi.h"
#include "Objects/ParticleFlowObject.h"
#include "Pandora/AlgorithmHeaders.h"

#include "CreateMainNDListsAlgorithm.h"

#include "larpandoracontent/LArControlFlow/MultiPandoraApi.h"
#include "larpandoracontent/LArHelpers/LArClusterHelper.h"
#include "larpandoracontent/LArObjects/LArCaloHit.h"

using namespace pandora;

namespace lar_content
{

CreateMainNDListsAlgorithm::CreateMainNDListsAlgorithm() :
    m_inputCaloHitListName("Input")
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateMainNDListsAlgorithm::Reset()
{
    //m_processedHits.clear();
    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateMainNDListsAlgorithm::Run()
{
    std::cout << "Running CreateMainNDListsAlgorithm for Pandora instance " << this->GetPandora().GetName() << std::endl;

    this->CreateLists();

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void CreateMainNDListsAlgorithm::CreateLists()
{
    const CaloHitList *pCaloHitList(nullptr);
    PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetList(*this, m_inputCaloHitListName, pCaloHitList));

    if (pCaloHitList->empty())
        return;

    CaloHitList caloHitListU, caloHitListV, caloHitListW, caloHitList3D;

    for (const CaloHit *const pCaloHit : *pCaloHitList)
    {
        if (TPC_VIEW_U == pCaloHit->GetHitType())
        {
            caloHitListU.push_back(pCaloHit);
        }
        else if (TPC_VIEW_V == pCaloHit->GetHitType())
        {
            caloHitListV.push_back(pCaloHit);
        }
        else if (TPC_VIEW_W == pCaloHit->GetHitType())
        {
            caloHitListW.push_back(pCaloHit);
        }
        else if (TPC_3D == pCaloHit->GetHitType())
        {
            caloHitList3D.push_back(pCaloHit);
        }
    }

    // Save these hit lists
    if (!caloHitListU.empty() && !m_outputCaloHitListNameU.empty())
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList(*this, caloHitListU, m_outputCaloHitListNameU));

    if (!caloHitListV.empty() && !m_outputCaloHitListNameV.empty())
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList(*this, caloHitListV, m_outputCaloHitListNameV));

    if (!caloHitListW.empty() && !m_outputCaloHitListNameW.empty())
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList(*this, caloHitListW, m_outputCaloHitListNameW));

    if (!caloHitList3D.empty() && !m_outputCaloHitListName3D.empty())
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList(*this, caloHitList3D, m_outputCaloHitListName3D));

    // Add all of the PFOs from the daughter Pandora instances to the main PFO list
    PfoList mainPFOList;
    const PandoraInstanceList pandoraList = MultiPandoraApi::GetDaughterPandoraInstanceList(&this->GetPandora());
    for (const auto *pPandora : pandoraList)
    {
        const std::string pandoraName = pPandora->GetName();

        const PfoList *pPFOList{nullptr};
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::GetCurrentPfoList(*pPandora, pPFOList));

        if (!pPFOList)
            continue;

        for (const ParticleFlowObject *pPFO : *pPFOList)
        {
            mainPFOList.emplace_back(pPFO);
        }
        std::cout << "Pandora instance " << pPandora->GetName() << " has PFOList size = " << pPFOList->size() << std::endl;
    }

    // Save the main PFO list
    std::cout << "Main Pandora PFO list size = " << mainPFOList.size() << std::endl;
    //if (!mainPFOList.empty())
    //{
    //const std::string pfoListName("CurrentPfos");
    //PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList(*this, pfoListName, pfoListName, mainPFOList));
    //}
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateMainNDListsAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF_AND_IF(
        STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle, "InputCaloHitListName", m_inputCaloHitListName));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle, "OutputCaloHitListNameU", m_outputCaloHitListNameU));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle, "OutputCaloHitListNameV", m_outputCaloHitListNameV));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle, "OutputCaloHitListNameW", m_outputCaloHitListNameW));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle, "OutputCaloHitListName3D", m_outputCaloHitListName3D));

    return STATUS_CODE_SUCCESS;
}

} // namespace lar_content
