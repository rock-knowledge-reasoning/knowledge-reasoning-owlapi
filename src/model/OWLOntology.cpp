#include "OWLOntology.hpp"
#include <owlapi/KnowledgeBase.hpp>

namespace owlapi {
namespace model {

OWLOntology::OWLOntology()
    : mpKnowledgeBase(new KnowledgeBase())
{}

OWLOntology::~OWLOntology()
{
    delete mpKnowledgeBase;
}

void OWLOntology::refresh()
{
    if(mpKnowledgeBase)
    {
        mpKnowledgeBase->refresh();
        return;
    }

    throw std::runtime_error("owlapi::model::OWLOntology: KnowledgeBase not initialized");
}

} // end namespace model
} // end namespace owlapi
