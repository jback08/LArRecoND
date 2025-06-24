/**
 *  @file   LArRecoND/include/MainNDPandora.h
 *
 *  @brief  Header file for the main Pandora ND instance
 *
 *  $Log: $
 */
#ifndef PANDORA_MAIN_ND_PANDORA_H
#define PANDORA_MAIN_ND_PANDORA_H 1

#include "Pandora/Pandora.h"

#include "NDParameters.h"
#include "NDSimpleTPCGeom.h"

#include <map>
#include <string>
#include <vector>

namespace lar_nd_reco
{

class MainNDPandora
{
public:
    typedef std::map<const std::string, const NDParameters> NDParametersMap;
    typedef std::map<const std::string, NDSimpleTPCGeom> NDSimpleTPCGeomMap;
    typedef std::vector<std::string> nameVector;

    /**
     *  @brief  Constructor
     *
     *  @param  mainName The name of the main Pandora instance
     *  @param  parameters The main steering parameters
     */
    MainNDPandora(const std::string &mainName, const NDParameters &parameters);

    /**
     *  @brief  Destructor
     */
    virtual ~MainNDPandora();

    /**
     *  @brief  Add Pandora instance
     *
     *  @param  name The name of the Pandora instance
     *  @param  parameters The parameters
     */
    void AddPandoraInstance(const std::string &name, const NDParameters &parameters);

    /**
     * @brief Create the TPC geometry for all Pandora instances
     */
    void CreatePandoraTPCs();

    /**
     * @brief Create the gaps across the TPCs
     */
    void CreatePandoraDetectorGaps();

    /**
     *  @brief  Get the Pandora instance
     *
     *  @return The main Pandora instance pointer
     */
    const pandora::Pandora *GetMainPandoraInstance() const
    {
        return m_pMainPandora;
    }

    /**
     *  @brief  Get the parameters for the main Pandora instance
     *
     *  @return the main Pandora instance parameters
     */
    const NDParameters GetMainPandoraParameters() const
    {
        return m_mainParameters;
    }

    /**
     *  @brief  Get the parameters of the given Pandora instance
     *
     *  @param  name The name of the Pandora instance
     *  @return The parameters of the given Pandora instance
     */
    const NDParameters GetPandoraParameters(const std::string &name) const;

private:
    /**
     *  @brief  Create the Pandora instance
     *
     *  @param  name The name of the Pandora instance
     *  @param  parameters The Pandora parameter settings
     *  @return The address of the Pandora instance
     */
    const pandora::Pandora *CreatePandoraInstance(const std::string &name, const NDParameters &parameters) const;

    NDParameters m_mainParameters;           ///< The steering parameters
    const pandora::Pandora *m_pMainPandora;  ///< The main Pandora instance pointer
    NDParametersMap m_NDParametersMap;       ///< Map of NDParameters
    NDSimpleTPCGeomMap m_NDSimpleTPCGeomMap; ///< Map of the TPC geometries
    nameVector m_instanceOrder;              ///< Pandora named instance insertion order
};

} // namespace lar_nd_reco

#endif
