#include "srg/dialogue/AnswerGraph.h"
#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/Edge.h"

#include <iostream>
#include <sstream>

namespace srg
{
namespace dialogue
{

AnswerGraph::AnswerGraph(srg::conceptnet::Concept* root)
        : graph(root)
{
    this->graph = root;
}

AnswerGraph::~AnswerGraph() {
    for(conceptnet::Edge* edge : graph->outgoingEdges) {
        delete edge;
    }
    for(conceptnet::Edge* edge : graph->incomingEdges) {
        delete edge;
    }
}

std::string AnswerGraph::toString() {
    std::stringstream ret;
    ret << "root: " << graph->term << std::endl;
    ret << "MotivatedByGoal Branch:" << std::endl;
    ret << "MotivatedByGoal edges: " << std::endl;
    for(conceptnet::Edge* edge : graph->incomingEdges) {
        ret << "\t" << edge->toString() << std::endl;
    }
    ret << "Synonyms: " << std::endl;
    for(conceptnet::Edge* edge1 : graph->incomingEdges) {
        for(conceptnet::Edge* edge2 : edge1->fromConcept->outgoingEdges) {
            ret << "\t" << edge2->toString() << std::endl;
        }

    }
    ret << "UsedFor: " << std::endl;
    for(conceptnet::Edge* edge1 : graph->incomingEdges) {
        for(conceptnet::Edge* edge2 : edge1->fromConcept->outgoingEdges) {
            for(conceptnet::Edge* edge3 : edge2->toConcept->incomingEdges) {
                ret << "\t" << edge3->toString() << std::endl;
            }
        }

    }
    ret << "Synonyms: " << std::endl;
    for(conceptnet::Edge* edge1 : graph->incomingEdges) {
        for(conceptnet::Edge* edge2 : edge1->fromConcept->outgoingEdges) {
            for(conceptnet::Edge* edge3 : edge2->toConcept->incomingEdges) {
                for(conceptnet::Edge* edge4 : edge3->fromConcept->outgoingEdges) {
                    ret << "\t" << edge4->toString() << std::endl;
                }
            }
        }

    }
    ret << "CausesDesire Branch:" << std::endl;
    ret << "CausesDesire edges: " << std::endl;
    for(conceptnet::Edge* edge : graph->outgoingEdges) {
        ret << "\t" << edge->toString() << std::endl;
    }
    ret << "Synonyms: " << std::endl;
    for(conceptnet::Edge* edge1 : graph->outgoingEdges) {
        for(conceptnet::Edge* edge2 : edge1->fromConcept->outgoingEdges) {
            ret << "\t" << edge2->toString() << std::endl;
        }

    }
    ret << "UsedFor: " << std::endl;
    for(conceptnet::Edge* edge1 : graph->outgoingEdges) {
        for(conceptnet::Edge* edge2 : edge1->fromConcept->outgoingEdges) {
            for(conceptnet::Edge* edge3 : edge2->toConcept->incomingEdges) {
                ret << "\t" << edge3->toString() << std::endl;
            }
        }

    }
    ret << "Synonyms: " << std::endl;
    for(conceptnet::Edge* edge1 : graph->outgoingEdges) {
        for(conceptnet::Edge* edge2 : edge1->fromConcept->outgoingEdges) {
            for(conceptnet::Edge* edge3 : edge2->toConcept->incomingEdges) {
                for(conceptnet::Edge* edge4 : edge3->fromConcept->outgoingEdges) {
                    ret << "\t" << edge4->toString() << std::endl;
                }
            }
        }

    }
    return ret.str();
}
} // namespace conceptnet

} // namespace srg