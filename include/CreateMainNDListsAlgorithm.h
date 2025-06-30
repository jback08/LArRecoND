/**
 *  @file   include/CreateMainNDListsAlgorithm.h
 *
 *  @brief  Header file for creating various lists for the MainND instance
 *
 *  $Log: $
 */
#ifndef LAR_CREATE_MAIN_ND_LISTS_ALGORITHM_H
#define LAR_CREATE_MAIN_ND_LISTS_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace lar_content
{

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  CreateMainNDListsAlgorithm class
 */
class CreateMainNDListsAlgorithm : public pandora::Algorithm
{
public:
    /**
     *  @brief  Default constructor
     */
    CreateMainNDListsAlgorithm();

private:
    pandora::StatusCode Reset();
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    /**
     *  @brief Create the various lists
     */
    void CreateLists();

    std::string m_inputCaloHitListName;    ///< The input calo hit list name
    std::string m_outputCaloHitListNameU;  ///< The output calo hit list name for TPC_VIEW_U hits
    std::string m_outputCaloHitListNameV;  ///< The output calo hit list name for TPC_VIEW_V hits
    std::string m_outputCaloHitListNameW;  ///< The output calo hit list name for TPC_VIEW_W hits
    std::string m_outputCaloHitListName3D; ///< The output calo hit list name for TPC_3D hits
};

} // namespace lar_content

#endif // #ifndef LAR_PRE_PROCESSING_THREE_D_ALGORITHM_H
