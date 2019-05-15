#pragma once

#include <string>

namespace srg
{
class SRGWorldModel;
namespace wm
{

class ConceptNet
{
public:
    ConceptNet(SRGWorldModel* wm);
    virtual ~ConceptNet();

    std::string getConcept(std::string concept);
    std::string getCompleteEdge(std::string relation, std::string fromConcept, std::string toConcept);
    std::string getOutgoingEdges(std::string relation, std::string fromConcept);
    std::string getIncomingEdges(std::string relation, std::string toConcept);
    double getRelatedness(std::string firstConcept, std::string secondConcept);

    enum CN5Relations
    {
        RelatedTo,
        FormOf,
        IsA,
        PartOf,
        HasA,
        UsedFor,
        CapableOf,
        AtLocation,
        Causes,
        HasSubevent,
        HasFirstSubevent,
        HasLastSubevent,
        HasPrerequisite,
        HasProperty,
        MotivatedByGoal,
        ObstructedBy,
        Desires,
        CreatedBy,
        Synonym,
        Antonym,
        DistinctFrom,
        DerivedFrom,
        SymbolOf,
        DefinedAs,
        Entails,
        MannerOf,
        LocatedNear,
        HasContext,
        SimilarTo,
        EtymologicallyRelatedTo,
        EtymologicallyDerivedFrom,
        CausesDesire,
        MadeOf,
        ReceivesAction,
        InstanceOf,
        NotDesires,
        NotUsedFor,
        NotCapableOf,
        NotIsA,
        NotHasProperty
    };

private:
    SRGWorldModel* wm;
    /**
     * Containts the begin of a concept net query url.
     */
    static const std::string BASE_URL;
    /**
     * Query part: /query?start=/c/en/
     */
    static const std::string URL_QUERYSTART;
    /**
     * Query part: /query?end=/c/en/
     */
    static const std::string URL_QUERYEND;
    /**
     * Query part: /query?node=/c/en/
     */
    static const std::string URL_QUERYNODE;
    /**
     * Query part: /query?other=/c/en/
     */
    static const std::string URL_QUERYOTHER;
    /**
     * Containts the cn5_ prefix.
     */
    static const std::string PREFIX;
    /**
     * Wildcard string
     */
    static const std::string WILDCARD;
};

} /* namespace wm */
} /* namespace srg */
