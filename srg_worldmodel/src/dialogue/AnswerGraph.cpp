#include "srg/dialogue/AnswerGraph.h"
#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/Edge.h"

#include <gvc.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <vector>

namespace srg
{
namespace dialogue
{

AnswerGraph::AnswerGraph()
{
}

AnswerGraph::~AnswerGraph()
{
    for (auto edgeEntry : edges) {
        delete edgeEntry.second;
    }
    for (auto conceptEntry : concepts) {
        delete conceptEntry.second;
    }
}

void AnswerGraph::setRoot(srg::conceptnet::Concept *root) {
    this->root = root;
}

std::string AnswerGraph::toString()
{
    std::stringstream ret;
    ret << "root: " << root->term << std::endl;
    ret << "edges: " << std::endl;
    for (conceptnet::Edge* edge : root->getEdges()) {
        ret << "\t" << edge->toString() << std::endl;
    }
    //    ret << "Synonyms: " << std::endl;
    //    for (conceptnet::Edge* edge1 : graph->incomingEdges) {
    //        for (conceptnet::Edge* edge2 : edge1->fromConcept->outgoingEdges) {
    //            ret << "\t" << edge2->toString() << std::endl;
    //        }
    //    }
    //    ret << "UsedFor: " << std::endl;
    //    for (conceptnet::Edge* edge1 : graph->incomingEdges) {
    //        for (conceptnet::Edge* edge2 : edge1->fromConcept->outgoingEdges) {
    //            for (conceptnet::Edge* edge3 : edge2->toConcept->incomingEdges) {
    //                ret << "\t" << edge3->toString() << std::endl;
    //            }
    //        }
    //    }
    //    ret << "Synonyms: " << std::endl;
    //    for (conceptnet::Edge* edge1 : graph->incomingEdges) {
    //        for (conceptnet::Edge* edge2 : edge1->fromConcept->outgoingEdges) {
    //            for (conceptnet::Edge* edge3 : edge2->toConcept->incomingEdges) {
    //                for (conceptnet::Edge* edge4 : edge3->fromConcept->outgoingEdges) {
    //                    ret << "\t" << edge4->toString() << std::endl;
    //                }
    //            }
    //        }
    //    }
    //    ret << "CausesDesire Branch:" << std::endl;
    //    ret << "CausesDesire edges: " << std::endl;
    //    for (conceptnet::Edge* edge : graph->outgoingEdges) {
    //        ret << "\t" << edge->toString() << std::endl;
    //    }
    //    ret << "Synonyms: " << std::endl;
    //    for (conceptnet::Edge* edge1 : graph->outgoingEdges) {
    //        for (conceptnet::Edge* edge2 : edge1->toConcept->outgoingEdges) {
    //            ret << "\t" << edge2->toString() << std::endl;
    //        }
    //    }
    //    ret << "UsedFor: " << std::endl;
    //    for (conceptnet::Edge* edge1 : graph->outgoingEdges) {
    //        for (conceptnet::Edge* edge2 : edge1->toConcept->outgoingEdges) {
    //            for (conceptnet::Edge* edge3 : edge2->toConcept->incomingEdges) {
    //                ret << "\t" << edge3->toString() << std::endl;
    //            }
    //        }
    //    }
    //    ret << "Synonyms: " << std::endl;
    //    for (conceptnet::Edge* edge1 : graph->outgoingEdges) {
    //        for (conceptnet::Edge* edge2 : edge1->toConcept->outgoingEdges) {
    //            for (conceptnet::Edge* edge3 : edge2->toConcept->incomingEdges) {
    //                for (conceptnet::Edge* edge4 : edge3->fromConcept->outgoingEdges) {
    //                    ret << "\t" << edge4->toString() << std::endl;
    //                }
    //            }
    //        }
    //    }
    //
    //    ret << "Answer Concepts:" << std::endl;
    //    for (auto& concept : answerConcepts) {
    //        ret << "\t" << concept->term << std::endl;
    //    }
    return ret.str();
}

void AnswerGraph::renderDot()
{
    Agraph_t* g;
    std::vector<conceptnet::Concept*> openNodes;
    std::vector<conceptnet::Concept*> closedNodes;

    /* set up a graphviz context - but only once even for multiple graphs */
    static GVC_t* gvc;

    if (!gvc)
        gvc = gvContext();

    /* Create a simple digraph */
    g = agopen("g", Agdirected, NULL);
    agsafeset(g, "rankdir", "RL", "");
    openNodes.push_back(this->root);

    while (!openNodes.empty()) {
        conceptnet::Concept* node = openNodes[0];
        std::cout << "AnswerGraph:renderDot: " << node->term << " " << node << std::endl;
        openNodes.erase(openNodes.begin());
        if (std::find(closedNodes.begin(), closedNodes.end(), node) != closedNodes.end()) {
            continue;
        }
        closedNodes.push_back(node);

        for (const conceptnet::Edge* edge : node->getEdges()) {
            Agnode_t* to;
            Agnode_t* from;
            std::cout << "AnswerGraph:rendetDot: " << node->id << " " << node << " " << edge->fromConcept->id << " " << edge->fromConcept << std::endl;
            if (edge->fromConcept == node) {
                to = agnode(g, strdup(edge->toConcept->term.c_str()), TRUE);
                from = agnode(g, strdup(node->term.c_str()), TRUE);
                openNodes.push_back(edge->toConcept);
            } else {
                to = agnode(g, strdup(node->term.c_str()), TRUE);
                from = agnode(g, strdup(edge->fromConcept->term.c_str()), TRUE);
                openNodes.push_back(edge->fromConcept);
            }
            Agedge_t* ed = agedge(g, from, to, strdup(conceptnet::relations[edge->relation]), TRUE);
            agsafeset(ed, "label", strdup(std::string(conceptnet::relations[edge->relation]).append(" / " + std::to_string(edge->weight)).c_str()), "");
        }
    }

    for (conceptnet::Concept* concept : this->answerConcepts) {
        Agnode_t* node = agnode(g, strdup(concept->term.c_str()), TRUE);
        agsafeset(node, "color", "red", "");
    }

    /* Set an attribute - in this case one that affects the visible rendering */

    /* Use the directed graph layout engine */
    gvLayout(gvc, g, "dot");

    /* Output in .dot format */
    FILE* fptr;
    fptr = fopen("test.dot", "w");
    gvRender(gvc, g, "dot", fptr);
    fclose(fptr);

    gvFreeLayout(gvc, g);

    agclose(g);

// call this to translate into ps format and open with evince
//    dot -Tps ~/test.dot -o outfile.ps
}

conceptnet::Concept* AnswerGraph::getConcept(std::string conceptId) const
{
    auto mapEntry = this->concepts.find(conceptId);
    if (mapEntry != this->concepts.end()) {
        return mapEntry->second;
    } else {
        return nullptr;
    }
}

conceptnet::Concept* AnswerGraph::createConcept(std::string conceptId, std::string term, std::string senseLabel)
{
    auto mapEntry = this->concepts.find(conceptId);
    if (mapEntry != this->concepts.end()) {
        return mapEntry->second;
    } else {
        return this->concepts.emplace(conceptId, new conceptnet::Concept(conceptId, term, senseLabel)).first->second;
    }
}

conceptnet::Edge* AnswerGraph::getEdge(std::string edgeId) const
{
    auto mapEntry = this->edges.find(edgeId);
    if (mapEntry != this->edges.end()) {
        return mapEntry->second;
    } else {
        return nullptr;
    }
}

conceptnet::Edge* AnswerGraph::createEdge(std::string edgeId, std::string language, srg::conceptnet::Concept* fromConcept,
                                                srg::conceptnet::Concept* toConcept, srg::conceptnet::Relation relation, double weight)
{
    auto mapEntry = this->edges.find(edgeId);
    if (mapEntry != this->edges.end()) {
        return mapEntry->second;
    } else {
        return this->edges.emplace(edgeId, new conceptnet::Edge(edgeId, language, fromConcept, toConcept, relation, weight)).first->second;
    }
}

} // namespace dialogue

} // namespace srg