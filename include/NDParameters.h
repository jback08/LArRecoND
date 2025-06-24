/**
 *  @file   LArRecoND/include/NDParameters.h
 *
 *  @brief  Header file for ND parameters
 *
 *  $Log: $
 */
#ifndef PANDORA_ND_PARAMETERS_H
#define PANDORA_ND_PARAMETERS_H 1

#include <string>

namespace lar_nd_reco
{

class NDParameters
{
public:
    // Near Detector volume type
    enum VolType
    {
        Main = 0,
        LArND = 1,
        TMS = 2
    };

    // Data format enumeration
    enum DataFormat
    {
        SP = 0,
        SPMC = 1,
        TMSMC = 2
    };

    /**
     *  @brief  Default constructor
     */
    NDParameters();

    std::string m_settingsFile;    ///< The path to the Pandora settings file
    std::string m_inputFileName;   ///< The path to the input file containing the events
    std::string m_inputTreeName;   ///< The name of the event TTree
    std::string m_geomFileName;    ///< The ROOT file name containing the TGeoManager info
    std::string m_geomManagerName; ///< The name of the TGeoManager
    std::string m_tpcName;         ///< The name of the TPC volume nodes

    VolType m_volType;       ///< The Near Detector volume Id
    DataFormat m_dataFormat; ///< The input data format
    bool m_use3D;            ///< Create 3D LArCaloHits
    bool m_useLArTPC;        ///< Create LArTPC-like LArCaloHits with U,V,W views
    float m_lengthScale;     ///< The scaling factor to set all lengths to cm
    float m_energyScale;     ///< The scaling factor to set all energies to GeV

    int m_nEventsToProcess; ///< The number of events to process (default all events)
    int m_nEventsToSkip;    ///< The number of events to skip
    int m_maxNHits;         ///< The max number of hits to process in a given event (default all)
    int m_minNHits;         ///< The minimum number of hits for processing an event (default = 2)

    bool m_shouldRunAllHitsCosmicReco;  ///< Whether to run all hits cosmic-ray reconstruction
    bool m_shouldRunStitching;          ///< Whether to stitch cosmic-ray muons crossing between volumes
    bool m_shouldRunCosmicHitRemoval;   ///< Whether to remove hits from tagged cosmic-rays
    bool m_shouldRunSlicing;            ///< Whether to slice events into separate regions for processing
    bool m_shouldRunNeutrinoRecoOption; ///< Whether to run neutrino reconstruction for each slice
    bool m_shouldRunCosmicRecoOption;   ///< Whether to run cosmic-ray reconstruction for each slice
    bool m_shouldPerformSliceId;        ///< Whether to identify slices and select most appropriate pfos
    bool m_printOverallRecoStatus;      ///< Whether to print current operation status messages
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline NDParameters::NDParameters() :
    m_settingsFile(""),
    m_inputFileName(""),
    m_inputTreeName(""),
    m_geomFileName(""),
    m_geomManagerName(""),
    m_tpcName(""),
    m_volType(NDParameters::VolType::Main),
    m_dataFormat(NDParameters::DataFormat::SP),
    m_use3D(true),
    m_useLArTPC(true),
    m_lengthScale(1.f),
    m_energyScale(1.f),
    m_nEventsToProcess(-1),
    m_nEventsToSkip(0),
    m_maxNHits(-1),
    m_minNHits(2),
    m_shouldRunAllHitsCosmicReco(true),
    m_shouldRunStitching(true),
    m_shouldRunCosmicHitRemoval(true),
    m_shouldRunSlicing(true),
    m_shouldRunNeutrinoRecoOption(true),
    m_shouldRunCosmicRecoOption(true),
    m_shouldPerformSliceId(true),
    m_printOverallRecoStatus(false)
{
}

} // namespace lar_nd_reco

#endif
