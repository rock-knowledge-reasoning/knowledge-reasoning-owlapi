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

OWLClass::Ptr OWLOntology::getClass(const IRI& iri) const
{
    std::map<IRI, OWLClass::Ptr>::const_iterator cit = mClasses.find(iri);
    if(cit != mClasses.end())
    {
        return cit->second;
    }
    throw std::invalid_argument("owlapi::model::OWLOntology::getClass: "
            " no class '" + iri.toString() + "' known");
}

OWLDataProperty::Ptr OWLOntology::getDataProperty(const IRI& iri) const
{
    std::map<IRI, OWLDataProperty::Ptr>::const_iterator cit = mDataProperties.find(iri);
    if(cit != mDataProperties.end())
    {
        return cit->second;
    }
    throw std::invalid_argument("owlapi::model::OWLOntology::getDataProperty: "
            " no data property '" + iri.toString() + "' known");
}

OWLObjectProperty::Ptr OWLOntology::getObjectProperty(const IRI& iri) const
{
    std::map<IRI, OWLObjectProperty::Ptr>::const_iterator cit = mObjectProperties.find(iri);
    if(cit != mObjectProperties.end())
    {
        return cit->second;
    }
    throw std::invalid_argument("owlapi::model::OWLOntology::getObjectProperty: "
            " no object property '" + iri.toString() + "' known");
}

} // end namespace model
} // end namespace owlapi
