/**
 *  @file   LArReco/validation/Validation.h
 *
 *  @brief  Header file for validation functionality
 *
 *  $Log: $
 */
#ifndef LAR_VALIDATION_H
#define LAR_VALIDATION_H 1

#include "ValidationIO.h"

/**
 * @brief   Parameters class
 */
class Parameters
{
public:
    /**
     *  @brief  Default constructor
     */
    Parameters();

    bool                    m_displayEvents;            ///< Whether to display the reconstruction outcomes for individual events
    bool                    m_displayMatchedEvents;     ///< Whether to display matching results for individual events
    int                     m_skipEvents;               ///< The number of events to skip
    int                     m_nEventsToProcess;         ///< The number of events to process

    int                     m_minPrimaryGoodHits;       ///< The minimum number of good mc primary hits
    bool                    m_useSmallPrimaries;        ///< Whether to consider matches to mc primaries with fewer than m_minPrimaryGoodHits
    int                     m_minHitsForGoodView;       ///< The minimum number of good mc primary hits in given view to declare view to be good
    int                     m_minPrimaryGoodViews;      ///< The minimum number of good views for a mc primary
    int                     m_minSharedHits;            ///< The minimum number of shared hits used in matching scheme
    float                   m_minCompleteness;          ///< The minimum particle completeness to declare a match
    float                   m_minPurity;                ///< The minimum particle purity to declare a match
    bool                    m_applyFiducialCut;         ///< Whether to apply fiducial volume cut to true neutrino vertex position
    bool                    m_correctTrackShowerId;     ///< Whether to demand that pfos are correctly flagged as tracks or showers

    float                   m_minNeutrinoPurity;        ///< The minimum neutrino purity to consider event (note: can't handle presence of multiple true neutrinos)
    float                   m_minNeutrinoCompleteness;  ///< The minimum neutrino completeness to consider event (note: can't handle presence of multiple true neutrinos)

    float                   m_vertexXCorrection;        ///< The vertex x correction, added to reported mc neutrino endpoint x value, in cm

    bool                    m_histogramOutput;          ///< Whether to produce output histograms

    std::string             m_histPrefix;               ///< Histogram name prefix
    std::string             m_mapFileName;              ///< File name to which to write output ascii tables, etc.
    std::string             m_eventFileName;            ///< File name to which to write list of correct events

    float                   m_minFractionOfAllHits;     ///< Input details must represent at least a given fraction of all hit in all drift volumes (as recorded in file, below)
    std::string             m_hitCountingFileName;      ///< The name of the file containing information about all hits in all drift volumes
};

//------------------------------------------------------------------------------------------------------------------------------------------


/**
 * @brief   MatchingDetails class
 */
class MatchingDetails
{
public:
    /**
     *  @brief  Default constructor
     */
    MatchingDetails();

    int                     m_matchedPrimaryId;         ///< The total number of occurences
    int                     m_nMatchedHits;             ///< The number of times the primary has 0 pfo matches
};

typedef std::map<int, MatchingDetails> PfoMatchingMap;

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief   CountingDetails class
 */
class CountingDetails
{
public:
    /**
     *  @brief  Default constructor  
     */
    CountingDetails();

    unsigned int            m_nTotal;                   ///< The total number of occurences
    unsigned int            m_nMatch0;                  ///< The number of times the primary has 0 pfo matches
    unsigned int            m_nMatch1;                  ///< The number of times the primary has 1 pfo matches
    unsigned int            m_nMatch2;                  ///< The number of times the primary has 2 pfo matches
    unsigned int            m_nMatch3Plus;              ///< The number of times the primary has 3 or more pfo matches
};

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief   ExpectedPrimary enum
 */
enum ExpectedPrimary
{
    MUON,
    ELECTRON,
    PROTON1,
    PROTON2,
    PROTON3,
    PROTON4,
    PROTON5,
    PIPLUS,
    PIMINUS,
    NEUTRON,
    PHOTON1,
    PHOTON2,
    OTHER_PRIMARY
};

typedef std::map<ExpectedPrimary, CountingDetails> CountingMap;

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief   InteractionType enum
 */
enum InteractionType
{
    // TODO Move to dynamic interaction type identification and labelling
    CC_MU,
    CC_MU_P,
    CC_MU_P_P,
    CC_MU_P_P_P,
    CC_MU_P_P_P_P,
    CC_MU_P_P_P_P_P,
    CC_MU_PIPLUS,
    CC_MU_P_PIPLUS,
    CC_MU_P_P_PIPLUS,
    CC_MU_P_P_P_PIPLUS,
    CC_MU_P_P_P_P_PIPLUS,
    CC_MU_P_P_P_P_P_PIPLUS,
    CC_MU_PHOTON,
    CC_MU_P_PHOTON,
    CC_MU_P_P_PHOTON,
    CC_MU_P_P_P_PHOTON,
    CC_MU_P_P_P_P_PHOTON,
    CC_MU_P_P_P_P_P_PHOTON,
    CC_MU_PIZERO,
    CC_MU_P_PIZERO,
    CC_MU_P_P_PIZERO,
    CC_MU_P_P_P_PIZERO,
    CC_MU_P_P_P_P_PIZERO,
    CC_MU_P_P_P_P_P_PIZERO,
    CC_E,
    CC_E_P,
    CC_E_P_P,
    CC_E_P_P_P,
    CC_E_P_P_P_P,
    CC_E_P_P_P_P_P,
    CC_E_PIPLUS,
    CC_E_P_PIPLUS,
    CC_E_P_P_PIPLUS,
    CC_E_P_P_P_PIPLUS,
    CC_E_P_P_P_P_PIPLUS,
    CC_E_P_P_P_P_P_PIPLUS,
    CC_E_PHOTON,
    CC_E_P_PHOTON,
    CC_E_P_P_PHOTON,
    CC_E_P_P_P_PHOTON,
    CC_E_P_P_P_P_PHOTON,
    CC_E_P_P_P_P_P_PHOTON,
    CC_E_PIZERO,
    CC_E_P_PIZERO,
    CC_E_P_P_PIZERO,
    CC_E_P_P_P_PIZERO,
    CC_E_P_P_P_P_PIZERO,
    CC_E_P_P_P_P_P_PIZERO,
    NC_P,
    NC_P_P,
    NC_P_P_P,
    NC_P_P_P_P,
    NC_P_P_P_P_P,
    NC_PIPLUS,
    NC_P_PIPLUS,
    NC_P_P_PIPLUS,
    NC_P_P_P_PIPLUS,
    NC_P_P_P_P_PIPLUS,
    NC_P_P_P_P_P_PIPLUS,
    NC_PIMINUS,
    NC_P_PIMINUS,
    NC_P_P_PIMINUS,
    NC_P_P_P_PIMINUS,
    NC_P_P_P_P_PIMINUS,
    NC_P_P_P_P_P_PIMINUS,
    NC_PHOTON,
    NC_P_PHOTON,
    NC_P_P_PHOTON,
    NC_P_P_P_PHOTON,
    NC_P_P_P_P_PHOTON,
    NC_P_P_P_P_P_PHOTON,
    NC_PIZERO,
    NC_P_PIZERO,
    NC_P_P_PIZERO,
    NC_P_P_P_PIZERO,
    NC_P_P_P_P_PIZERO,
    NC_P_P_P_P_P_PIZERO,
    OTHER_INTERACTION,
    ALL_INTERACTIONS // ATTN use carefully!
};

typedef std::map<InteractionType, CountingMap> InteractionCountingMap;

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief   PrimaryResult class
 */
class PrimaryResult
{
public:
    /**
     *  @brief  Default constructor  
     */
    PrimaryResult();

    unsigned int            m_nPfoMatches;              ///< The total number of pfo matches for a given primary
    unsigned int            m_nTrueHits;                ///< The number of true hits
    float                   m_trueMomentum;             ///< The true momentum
    float                   m_trueAngle;                ///< The true angle wrt the z axis
    unsigned int            m_nBestMatchedHits;         ///< The best number of matched hits
    unsigned int            m_nBestRecoHits;            ///< The number of hits in the best matched pfo
    float                   m_bestCompleteness;         ///< The best match pfo is determined by the best completeness (most matched hits)
    float                   m_bestMatchPurity;          ///< The purity of the best matched pfo
};

typedef std::map<ExpectedPrimary, PrimaryResult> PrimaryResultMap;

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief   EventResult class
 */
class EventResult
{
public:
    /**
     *  @brief  Default constructor  
     */
    EventResult();

    // ATTN Put items to count on a per-event basis here
    int                     m_fileIdentifier;           ///< The file identifier
    int                     m_eventNumber;              ///< The event number
    int                     m_mcNeutrinoNuance;         ///< The mc neutrino nuance code (interaction type details)
    int                     m_nRecoNeutrinos;           ///< The number of reconstructed neutrinos
    int                     m_nTrueNeutrinos;           ///< The number of true neutrinos
    float                   m_neutrinoPurity;           ///< The reco neutrino purity
    float                   m_neutrinoCompleteness;     ///< The reco neutrino completeness
    PrimaryResultMap        m_primaryResultMap;         ///< The primary result map
    SimpleThreeVector       m_vertexOffset;             ///< The vertex offset
};

typedef std::vector<EventResult> EventResultList; // ATTN Not terribly efficient, but that's not the main aim here
typedef std::map<InteractionType, EventResultList> InteractionEventResultMap;

//------------------------------------------------------------------------------------------------------------------------------------------

class TH1F;
class TH2F;

/**
 *  @brief  PrimaryHistogramCollection class
 */
class PrimaryHistogramCollection
{
public:
    /**
     *  @brief  Default constructor
     */
    PrimaryHistogramCollection();

    TH1F                   *m_hHitsAll;                 ///<
    TH1F                   *m_hHitsEfficiency;          ///<
    TH1F                   *m_hMomentumAll;             ///<
    TH1F                   *m_hMomentumEfficiency;      ///<
    TH1F                   *m_hAngleAll;                ///<
    TH1F                   *m_hAngleEfficiency;         ///<
    TH1F                   *m_hCompleteness;            ///<
    TH1F                   *m_hPurity;                  ///<

    TH2F                   *m_hNPfosVsPTot;             ///<
    TH2F                   *m_hBestCompVsPTot;          ///<
};

typedef std::map<ExpectedPrimary, PrimaryHistogramCollection> PrimaryHistogramMap;
typedef std::map<InteractionType, PrimaryHistogramMap> InteractionPrimaryHistogramMap;

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  EventHistogramCollection class
 */
class EventHistogramCollection
{
public:
    /**
     *  @brief  Default constructor
     */
    EventHistogramCollection();

    TH1F                   *m_hVtxDeltaX;               ///< 
    TH1F                   *m_hVtxDeltaY;               ///< 
    TH1F                   *m_hVtxDeltaZ;               ///< 
    TH1F                   *m_hVtxDeltaR;               ///< 
    TH1F                   *m_hNeutrinoPurity;          ///<
    TH1F                   *m_hNuPurityCorrect;         ///<
    TH1F                   *m_hCosmicFraction;          ///<
    TH1F                   *m_hNeutrinoCompleteness;    ///<
    TH1F                   *m_hNuCompletenessCorrect;   ///<
    TH1F                   *m_hNRecoNeutrinos;          ///<
};

typedef std::map<InteractionType, EventHistogramCollection> InteractionEventHistogramMap;

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  Validation - Main entry point for analysis
 * 
 *  @param  inputFiles the regex identifying the input root files
 *  @param  parameters the parameters
 */
void Validation(const std::string inputFiles, const Parameters parameters = Parameters());

typedef std::map<int, int> EventToNHitsMap;
typedef std::map<int, EventToNHitsMap> HitCountingMap;

/**
 *  @brief  Write hit counting map, which stores total numbers of hits (in all drift volumes) for given file ids and event numbers
 * 
 *  @param  inputFiles the regex identifying the input root files
 *  @param  outputFile the name of the output text file to create, storing hit counting information
 */
void WriteHitCountingMap(const std::string inputFiles, const std::string outputFile);

/**
 *  @brief  Fill hit counting map from the text file identified in the parameters block
 * 
 *  @param  parameters the parameters
 *  @param  hitCountingMap to receive the populated hit counting map
 */
void FillHitCountingMap(const Parameters &parameters, HitCountingMap hitCountingMap);

/**
 *  @brief  Whether a single drift volume "event" represents at least the minumum fraction of all hits in all volumes
 * 
 *  @param  simpleMCEvent the simple mc event
 *  @param  parameters the parameters
 *  @param  hitCountingMap the hit counting map
 * 
 *  @return boolean
 */
bool PassesHitCountingCheck(const SimpleMCEvent &simpleMCEvent, const Parameters &parameters, const HitCountingMap &hitCountingMap);

/**
 *  @brief  Finalise the mc primary to pfo matching, using a pfo matching map to store the results
 * 
 *  @param  simpleMCEvent the simple mc event
 *  @param  parameters the parameters
 *  @param  pfoMatchingMap to receive the populated pfo matching map
 */
void FinalisePfoMatching(const SimpleMCEvent &simpleMCEvent, const Parameters &parameters, PfoMatchingMap &pfoMatchingMap);

/**
 *  @brief  Get the strongest pfo match (most matched hits) between an available mc primary and an available pfo
 * 
 *  @param  simpleMCEvent the simple mc event
 *  @param  parameters the parameters
 *  @param  usedMCIds the list of mc primary ids with an existing match
 *  @param  usedPfoIds the list of pfo ids with an existing match
 *  @param  pfoMatchingMap to receive the populated pfo matching map
 */
bool GetStrongestPfoMatch(const SimpleMCEvent &simpleMCEvent, const Parameters &parameters, IntSet &usedMCIds, IntSet &usedPfoIds,
    PfoMatchingMap &pfoMatchingMap);

/**
 *  @brief  Get the best matches for any pfos left-over after the strong matching procedure
 * 
 *  @param  simpleMCEvent the simple mc event
 *  @param  parameters the parameters
 *  @param  usedPfoIds the list of pfo ids with an existing match
 *  @param  pfoMatchingMap to receive the populated pfo matching map
 */
void GetRemainingPfoMatches(const SimpleMCEvent &simpleMCEvent, const Parameters &parameters, const IntSet &usedPfoIds,
    PfoMatchingMap &pfoMatchingMap);

/**
 *  @brief  Whether a provided mc primary passes selection, based on number of "good" hits
 * 
 *  @param  simpleMCPrimary the simple mc primary
 *  @param  parameters the parameters
 * 
 *  @return boolean
 */
bool IsGoodMCPrimary(const SimpleMCPrimary &simpleMCPrimary, const Parameters &parameters);

/**
 *  @brief  Whether a provided mc primary has a match, of any quality (use simple matched pfo list and information in matching details map)
 * 
 *  @param  simpleMCPrimary the simple mc primary
 *  @param  pfoMatchingMap the pfo matching map
 *  @param  parameters the parameters
 * 
 *  @return boolean
 */
bool HasMatch(const SimpleMCPrimary &simpleMCPrimary, const PfoMatchingMap &pfoMatchingMap, const Parameters &parameters);

/**
 *  @brief  Whether a provided mc primary and pfo are deemed to be a good match
 * 
 *  @param  simpleMCPrimary the simple mc primary
 *  @param  simpleMatchedPfo the simple matched pfo
 *  @param  parameters the parameters
 * 
 *  @return boolean
 */
bool IsGoodMatch(const SimpleMCPrimary &simpleMCPrimary, const SimpleMatchedPfo &simpleMatchedPfo, const Parameters &parameters);

/**
 *  @brief  Print matching details to screen for a simple mc event
 * 
 *  @param  simpleMCEvent the simple mc event
 *  @param  pfoMatchingMap the pfo matching map
 *  @param  parameters the parameters
 */
void DisplaySimpleMCEventMatches(const SimpleMCEvent &simpleMCEvent, const PfoMatchingMap &pfoMatchingMap, const Parameters &parameters);

/**
 *  @brief  CountPfoMatches Relies on fact that primary list is sorted by number of true good hits
 * 
 *  @param  simpleMCEvent the simple mc event
 *  @param  pfoMatchingMap the pfo matching map
 *  @param  parameters, the parameters
 *  @param  interactionCountingMap the interaction counting map, to be populated
 *  @param  interactionEventOutcomeMap the interaction event outcome map, to be populated
 */
void CountPfoMatches(const SimpleMCEvent &simpleMCEvent, const PfoMatchingMap &pfoMatchingMap, const Parameters &parameters,
    InteractionCountingMap &interactionCountingMap, InteractionEventResultMap &interactionEventResultMap);

/**
 *  @brief  Whether a simple mc event passes a fiducial cut, applied to the mc neutrino vertex
 * 
 *  @param  simpleMCEvent the simple mc event
 * 
 *  @return boolean
 */
bool PassFiducialCut(const SimpleMCEvent &simpleMCEvent);

/**
 *  @brief  Get the event interaction type
 * 
 *  @param  simpleMCEvent the simple mc event
 *  @param  parameters the parameters
 * 
 *  @return the interaction type
 */
InteractionType GetInteractionType(const SimpleMCEvent &simpleMCEvent, const Parameters &parameters);

/**
 *  @brief  Work out which of the primary particles (expected for a given interaction types) corresponds to the provided priamry id
 *          ATTN: Relies on fact that primary list is sorted by number of true hits
 * 
 *  @param  primaryId the primary id
 *  @param  simpleMCPrimaryList the simple mc primary list
 *  @param  parameters the parameters
 * 
 *  @return the expected primary
 */
ExpectedPrimary GetExpectedPrimary(const int primaryId, const SimpleMCPrimaryList &simpleMCPrimaryList, const Parameters &parameters);

/**
 *  @brief  Print details to screen for a provided interaction type to counting map
 * 
 *  @param  interactionCountingMap the interaction counting map
 *  @param  parameters the parameters
 */
void DisplayInteractionCountingMap(const InteractionCountingMap &interactionCountingMap, const Parameters &parameters);

/**
 *  @brief  Opportunity to fill histograms, perform post-processing of information collected in main loop over ntuple, etc.
 * 
 *  @param  interactionEventResultMap the interaction event result map
 *  @param  parameters the parameters
 */
void AnalyseInteractionEventResultMap(const InteractionEventResultMap &interactionEventResultMap, const Parameters &parameters);

/**
 *  @brief  Fill histograms in the provided histogram collection, using information in the provided primary result
 * 
 *  @param  histPrefix the histogram prefix
 *  @param  primaryResult the primary result
 *  @param  primaryHistogramCollection the primary histogram collection
 */
void FillPrimaryHistogramCollection(const std::string &histPrefix, const PrimaryResult &primaryResult, PrimaryHistogramCollection &primaryHistogramCollection);

/**
 *  @brief  Fill histograms in the provided event histogram collection, using information in the provided event offset
 * 
 *  @param  histPrefix the histogram prefix
 *  @param  isCorrect whether the event is deemed correct
 *  @param  eventResult the event result
 *  @param  eventHistogramCollection the event histogram collection
 */
void FillEventHistogramCollection(const std::string &histPrefix, const bool isCorrect, const EventResult &eventResult, EventHistogramCollection &eventHistogramCollection);

/**
 *  @brief  Process histograms stored in the provided map e.g. calculating final efficiencies, normalising, etc.
 * 
 *  @param  interactionPrimaryHistogramMap the interaction primary histogram map
 */
void ProcessHistogramCollections(const InteractionPrimaryHistogramMap &interactionPrimaryHistogramMap);

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

Parameters::Parameters() :
    m_displayEvents(false),
    m_displayMatchedEvents(false),
    m_skipEvents(0),
    m_nEventsToProcess(std::numeric_limits<int>::max()),
    m_minPrimaryGoodHits(15),
    m_minHitsForGoodView(5),
    m_minPrimaryGoodViews(2),
    m_useSmallPrimaries(true),
    m_minSharedHits(5),
    m_minCompleteness(0.1f),
    m_minPurity(0.5f),
    m_applyFiducialCut(false),
    m_correctTrackShowerId(false),
    m_minNeutrinoPurity(-1.f),
    m_minNeutrinoCompleteness(-1.f),
    m_vertexXCorrection(0.495694f),
    m_histogramOutput(false),
    m_minFractionOfAllHits(0.9f)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

MatchingDetails::MatchingDetails() :
    m_matchedPrimaryId(-1),
    m_nMatchedHits(0)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

CountingDetails::CountingDetails() :
    m_nTotal(0),
    m_nMatch0(0),
    m_nMatch1(0),
    m_nMatch2(0),
    m_nMatch3Plus(0)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

PrimaryResult::PrimaryResult() :
    m_nPfoMatches(0),
    m_bestCompleteness(0.f),
    m_bestMatchPurity(0.f)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

EventResult::EventResult() :
    m_fileIdentifier(-1),
    m_eventNumber(-1),
    m_mcNeutrinoNuance(-1),
    m_nRecoNeutrinos(0),
    m_nTrueNeutrinos(0),
    m_neutrinoPurity(-1.f),
    m_neutrinoCompleteness(-1.f),
    m_vertexOffset(-999.f, -999.f, -999.f)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

PrimaryHistogramCollection::PrimaryHistogramCollection() :
    m_hHitsAll(NULL),
    m_hHitsEfficiency(NULL),
    m_hMomentumAll(NULL),
    m_hMomentumEfficiency(NULL),
    m_hAngleAll(NULL),
    m_hAngleEfficiency(NULL),
    m_hCompleteness(NULL),
    m_hPurity(NULL),
    m_hNPfosVsPTot(NULL),
    m_hBestCompVsPTot(NULL)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

EventHistogramCollection::EventHistogramCollection() :
    m_hVtxDeltaX(NULL),
    m_hVtxDeltaY(NULL),
    m_hVtxDeltaZ(NULL),
    m_hVtxDeltaR(NULL),
    m_hNeutrinoPurity(NULL),
    m_hNuPurityCorrect(NULL),
    m_hCosmicFraction(NULL),
    m_hNeutrinoCompleteness(NULL),
    m_hNuCompletenessCorrect(NULL),
    m_hNRecoNeutrinos(NULL)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  Get string representing interaction type
 * 
 *  @param  interactionType
 * 
 *  @return the interaction type string
 */
std::string ToString(const InteractionType interactionType)
{
    switch (interactionType)
    {
    case CC_MU: return "CC_MU";
    case CC_MU_P: return "CC_MU_P";
    case CC_MU_P_P: return "CC_MU_P_P";
    case CC_MU_P_P_P: return "CC_MU_P_P_P";
    case CC_MU_P_P_P_P: return "CC_MU_P_P_P_P";
    case CC_MU_P_P_P_P_P: return "CC_MU_P_P_P_P_P";
    case CC_MU_PIPLUS: return "CC_MU_PIPLUS";
    case CC_MU_P_PIPLUS: return "CC_MU_P_PIPLUS";
    case CC_MU_P_P_PIPLUS: return "CC_MU_P_P_PIPLUS";
    case CC_MU_P_P_P_PIPLUS: return "CC_MU_P_P_P_PIPLUS";
    case CC_MU_P_P_P_P_PIPLUS: return "CC_MU_P_P_P_P_PIPLUS";
    case CC_MU_P_P_P_P_P_PIPLUS: return "CC_MU_P_P_P_P_P_PIPLUS";
    case CC_MU_PHOTON: return "CC_MU_PHOTON";
    case CC_MU_P_PHOTON: return "CC_MU_P_PHOTON";
    case CC_MU_P_P_PHOTON: return "CC_MU_P_P_PHOTON";
    case CC_MU_P_P_P_PHOTON: return "CC_MU_P_P_P_PHOTON";
    case CC_MU_P_P_P_P_PHOTON: return "CC_MU_P_P_P_P_PHOTON";
    case CC_MU_P_P_P_P_P_PHOTON: return "CC_MU_P_P_P_P_P_PHOTON";
    case CC_MU_PIZERO: return "CC_MU_PIZERO";
    case CC_MU_P_PIZERO: return "CC_MU_P_PIZERO";
    case CC_MU_P_P_PIZERO: return "CC_MU_P_P_PIZERO";
    case CC_MU_P_P_P_PIZERO: return "CC_MU_P_P_P_PIZERO";
    case CC_MU_P_P_P_P_PIZERO: return "CC_MU_P_P_P_P_PIZERO";
    case CC_MU_P_P_P_P_P_PIZERO: return "CC_MU_P_P_P_P_P_PIZERO";
    case CC_E: return "CC_E";
    case CC_E_P: return "CC_E_P";
    case CC_E_P_P: return "CC_E_P_P";
    case CC_E_P_P_P: return "CC_E_P_P_P";
    case CC_E_P_P_P_P: return "CC_E_P_P_P_P";
    case CC_E_P_P_P_P_P: return "CC_E_P_P_P_P_P";
    case CC_E_PIPLUS: return "CC_E_PIPLUS";
    case CC_E_P_PIPLUS: return "CC_E_P_PIPLUS";
    case CC_E_P_P_PIPLUS: return "CC_E_P_P_PIPLUS";
    case CC_E_P_P_P_PIPLUS: return "CC_E_P_P_P_PIPLUS";
    case CC_E_P_P_P_P_PIPLUS: return "CC_E_P_P_P_P_PIPLUS";
    case CC_E_P_P_P_P_P_PIPLUS: return "CC_E_P_P_P_P_P_PIPLUS";
    case CC_E_PHOTON: return "CC_E_PHOTON";
    case CC_E_P_PHOTON: return "CC_E_P_PHOTON";
    case CC_E_P_P_PHOTON: return "CC_E_P_P_PHOTON";
    case CC_E_P_P_P_PHOTON: return "CC_E_P_P_P_PHOTON";
    case CC_E_P_P_P_P_PHOTON: return "CC_E_P_P_P_P_PHOTON";
    case CC_E_P_P_P_P_P_PHOTON: return "CC_E_P_P_P_P_P_PHOTON";
    case CC_E_PIZERO: return "CC_E_PIZERO";
    case CC_E_P_PIZERO: return "CC_E_P_PIZERO";
    case CC_E_P_P_PIZERO: return "CC_E_P_P_PIZERO";
    case CC_E_P_P_P_PIZERO: return "CC_E_P_P_P_PIZERO";
    case CC_E_P_P_P_P_PIZERO: return "CC_E_P_P_P_P_PIZERO";
    case CC_E_P_P_P_P_P_PIZERO: return "CC_E_P_P_P_P_P_PIZERO";
    case NC_P: return "NC_P";
    case NC_P_P: return "NC_P_P";
    case NC_P_P_P: return "NC_P_P_P";
    case NC_P_P_P_P: return "NC_P_P_P_P";
    case NC_P_P_P_P_P: return "NC_P_P_P_P_P";
    case NC_PIPLUS: return "NC_PIPLUS";
    case NC_P_PIPLUS: return "NC_P_PIPLUS";
    case NC_P_P_PIPLUS: return "NC_P_P_PIPLUS";
    case NC_P_P_P_PIPLUS: return "NC_P_P_P_PIPLUS";
    case NC_P_P_P_P_PIPLUS: return "NC_P_P_P_P_PIPLUS";
    case NC_P_P_P_P_P_PIPLUS: return "NC_P_P_P_P_P_PIPLUS";
    case NC_PIMINUS: return "NC_PIMINUS";
    case NC_P_PIMINUS: return "NC_P_PIMINUS";
    case NC_P_P_PIMINUS: return "NC_P_P_PIMINUS";
    case NC_P_P_P_PIMINUS: return "NC_P_P_P_PIMINUS";
    case NC_P_P_P_P_PIMINUS: return "NC_P_P_P_P_PIMINUS";
    case NC_P_P_P_P_P_PIMINUS: return "NC_P_P_P_P_P_PIMINUS";
    case NC_PHOTON: return "NC_PHOTON";
    case NC_P_PHOTON: return "NC_P_PHOTON";
    case NC_P_P_PHOTON: return "NC_P_P_PHOTON";
    case NC_P_P_P_PHOTON: return "NC_P_P_P_PHOTON";
    case NC_P_P_P_P_PHOTON: return "NC_P_P_P_P_PHOTON";
    case NC_P_P_P_P_P_PHOTON: return "NC_P_P_P_P_P_PHOTON";
    case NC_PIZERO: return "NC_PIZERO";
    case NC_P_PIZERO: return "NC_P_PIZERO";
    case NC_P_P_PIZERO: return "NC_P_P_PIZERO";
    case NC_P_P_P_PIZERO: return "NC_P_P_P_PIZERO";
    case NC_P_P_P_P_PIZERO: return "NC_P_P_P_P_PIZERO";
    case NC_P_P_P_P_P_PIZERO: return "NC_P_P_P_P_P_PIZERO";
    case OTHER_INTERACTION: return "OTHER_INTERACTION";
    case ALL_INTERACTIONS: return "ALL_INTERACTIONS";
    default: return "UNKNOWN";
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  Get string representing expected primary
 * 
 *  @param  expectedPrimary
 * 
 *  @return the expected primary string
 */
std::string ToString(const ExpectedPrimary expectedPrimary)
{
    switch (expectedPrimary)
    {
    case MUON: return "MUON";
    case ELECTRON: return "ELECTRON";
    case PROTON1: return "PROTON1";
    case PROTON2: return "PROTON2";
    case PROTON3: return "PROTON3";
    case PROTON4: return "PROTON4";
    case PROTON5: return "PROTON5";
    case PIPLUS: return "PIPLUS";
    case PIMINUS: return "PIMINUS";
    case NEUTRON: return "NEUTRON";
    case PHOTON1: return "PHOTON1";
    case PHOTON2: return "PHOTON2";
    case OTHER_PRIMARY: return "OTHER_PRIMARY";
    default: return "UNKNOWN";
    }
}

#endif // #ifndef LAR_VALIDATION_H
