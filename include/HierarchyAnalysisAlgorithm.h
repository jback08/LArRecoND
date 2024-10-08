/**
 *  @file   include/HierarchyAnalysisAlgorithm.h
 *
 *  @brief  Header file for the hierarchy analysis output algorithm
 *
 *  $Log: $
 */
#ifndef LAR_HIERARCHY_ANALYSIS_ALGORITHM_H
#define LAR_HIERARCHY_ANALYSIS_ALGORITHM_H 1

#include "Objects/CartesianVector.h"
#include "Objects/Cluster.h"
#include "Objects/ParticleFlowObject.h"
#include "Pandora/Algorithm.h"

#include "larpandoracontent/LArHelpers/LArHierarchyHelper.h"

class TFile;
class TTree;

namespace lar_content
{

/**
 *  @brief  HierarchyAnalysisAlgorithm class
 */
class HierarchyAnalysisAlgorithm : public pandora::Algorithm
{
public:
    /**
     *  @brief  Default constructor
     */
    HierarchyAnalysisAlgorithm();

    virtual ~HierarchyAnalysisAlgorithm();

    /**
     *  @brief  RecoMCMatch class
     */
    class RecoMCMatch
    {
    public:
        /**
         *  @brief  Constructor
	 *
	 *  @param  pNuRoot The neutrino parent MC particle
	 *  @param  pLeadingMC The leading best matched MC particle
	 *  @param  nSharedHits The number of shared hits between the matched reco and MC
	 *  @param  completeness The completeness of the match
	 *  @param  purity The purity of the match
         */
        RecoMCMatch(const pandora::MCParticle *pNuRoot, const pandora::MCParticle *pLeadingMC, const int nSharedHits,
            const float completeness, const float purity);

        const pandora::MCParticle *m_pNuRoot;    ///< The pointer to the neutrino parent MCParticle
        const pandora::MCParticle *m_pLeadingMC; ///< The pointer to the best matched (leading) MCParticle
        int m_nSharedHits;                       ///< The number of shared hits between the matched reco and MC
        float m_completeness;                    ///< The completeness of the match
        float m_purity;                          ///< The purity of the match
    };

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    /**
     *  @brief  Set the event and run numbers as well as the trigger timing
     *
     */
    void SetEventRunInfo();

    /**
     *  @brief  Create the analysis output using hierarchy tools
     *
     *  @param  matchInfo The object containing the reco and MC hierarchies
     */
    void EventAnalysisOutput(const LArHierarchyHelper::MatchInfo &matchInfo) const;

    /**
     *  @brief  Get the required cluster from the PFO
     *
     *  @param  pPfo The PFO pointer
     *  @param  hitType The cluster hit type
     *
     *  @return The cluster pointer
     */
    const pandora::Cluster *GetCluster(const pandora::ParticleFlowObject *pPfo, const pandora::HitType hitType) const;

    /**
     *  @brief  Find the best MC match for the given reco node
     *
     *  @param  pRecoNode The reco node
     *  @param  matchInfo The object storing all of the MC match hierarchy
     *  @param  rootMCParticles The root MC particles
     *
     *  @return A summary of the match info
     */
    const RecoMCMatch GetRecoMCMatch(const LArHierarchyHelper::RecoHierarchy::Node *pRecoNode,
        const LArHierarchyHelper::MatchInfo &matchInfo, pandora::MCParticleList &rootMCParticles) const;

    int m_count;                       ///< The number of times the Run() function has been called
    int m_event;                       ///< The actual event number
    int m_run;                         ///< The run number
    int m_subRun;                      ///< The subrun number
    int m_startTime;                   ///< The event trigger start time
    int m_endTime;                     ///< The event trigger end time
    std::string m_eventFileName;       ///< Name of the ROOT TFile containing the event numbers
    std::string m_eventTreeName;       ///< Name of the ROOT TTree containing the event numbers
    std::string m_eventLeafName;       ///< Name of the event number leaf/variable
    std::string m_runLeafName;         ///< Name of the run number leaf/variable
    std::string m_subRunLeafName;      ///< Name of the subrun number leaf/variable
    std::string m_startTimeLeafName;   ///< Name of the event start time leaf/variable
    std::string m_endTimeLeafName;     ///< Name of the event end time leaf/variable
    int m_eventsToSkip;                ///< The number of events to skip (from the start of the event file)
    TFile *m_eventFile;                ///< The ROOT event file pointer
    TTree *m_eventTree;                ///< The ROOT event tree pointer
    std::string m_caloHitListName;     ///< Name of input calo hit list
    std::string m_pfoListName;         ///< Name of input PFO list
    std::string m_analysisFileName;    ///< The name of the analysis ROOT file to write
    std::string m_analysisTreeName;    ///< The name of the analysis ROOT tree to write
    bool m_foldToPrimaries;            ///< Whether or not to fold the hierarchy back to primary particles
    bool m_foldDynamic;                ///< Whether or not to fold the hierarchy dynamically
    bool m_foldToLeadingShowers;       ///< Whether or not to fold the hierarchy back to leading shower particles
    float m_minPurity;                 ///< Minimum purity to tag a node as being of good quality
    float m_minCompleteness;           ///< Minimum completeness to tag a node as being of good quality
    unsigned int m_minRecoHits;        ///< Minimum number of reconstructed primary good hits
    unsigned int m_minRecoHitsPerView; ///< Minimum number of reconstructed hits for a good view
    unsigned int m_minRecoGoodViews;   ///< Minimum number of reconstructed primary good views
    bool m_removeRecoNeutrons;         ///< Whether to remove reconstructed neutrons and their downstream particles
    bool m_selectRecoHits;             ///< Whether to select reco hits that overlap with the MC particle hits
};

} // namespace lar_content

#endif // LAR_HIERARCHY_ANALYSIS_ALGORITHM_H