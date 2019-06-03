#pragma once

namespace srg
{
namespace container
{

#define UNUSED_VAR     __attribute__ ((unused))

#define CN_RELATIONS(REL)                                                                                                                                      \
    REL(RelatedTo)                                                                                                                                             \
    REL(FormOf)                                                                                                                                                \
    REL(IsA)                                                                                                                                                   \
    REL(PartOf)                                                                                                                                                \
    REL(HasA)                                                                                                                                                  \
    REL(UsedFor)                                                                                                                                               \
    REL(CapableOf)                                                                                                                                             \
    REL(AtLocation)                                                                                                                                            \
    REL(Causes)                                                                                                                                                \
    REL(HasSubevent)                                                                                                                                           \
    REL(HasFirstSubevent)                                                                                                                                      \
    REL(HasLastSubevent)                                                                                                                                       \
    REL(HasPrerequisite)                                                                                                                                       \
    REL(HasProperty)                                                                                                                                           \
    REL(MotivatedByGoal)                                                                                                                                       \
    REL(ObstructedBy)                                                                                                                                          \
    REL(Desires)                                                                                                                                               \
    REL(CreatedBy)                                                                                                                                             \
    REL(Synonym)                                                                                                                                               \
    REL(Antonym)                                                                                                                                               \
    REL(DistinctFrom)                                                                                                                                          \
    REL(DerivedFrom)                                                                                                                                           \
    REL(SymbolOf)                                                                                                                                              \
    REL(DefinedAs)                                                                                                                                             \
    REL(Entails)                                                                                                                                               \
    REL(MannerOf)                                                                                                                                              \
    REL(LocatedNear)                                                                                                                                           \
    REL(HasContext)                                                                                                                                            \
    REL(SimilarTo)                                                                                                                                             \
    REL(EtymologicallyRelatedTo)                                                                                                                               \
    REL(EtymologicallyDerivedFrom)                                                                                                                             \
    REL(CausesDesire)                                                                                                                                          \
    REL(MadeOf)                                                                                                                                                \
    REL(ReceivesAction)                                                                                                                                        \
    REL(InstanceOf)                                                                                                                                            \
    REL(NotDesires)                                                                                                                                            \
    REL(NotUsedFor)                                                                                                                                            \
    REL(NotCapableOf)                                                                                                                                          \
    REL(NotIsA)                                                                                                                                                \
    REL(NotHasProperty)                                                                                                                                        \
    REL(UNDEFINED)

// Enum
#define MAKE_ENUM(VAR) VAR,

enum Relation
{
    CN_RELATIONS(MAKE_ENUM)
};

// Strings
#define MAKE_STRINGS(VAR) #VAR,
UNUSED_VAR static const char* relations[] = {CN_RELATIONS(MAKE_STRINGS)};

} // namespace container
} // namespace srg