#include "OWLOntology.hpp"
#include <owlapi/KnowledgeBase.hpp>
#include "OWLOntologyReader.hpp"

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

OWLIndividual::Ptr OWLOntology::getIndividual(const IRI& iri) const
{
    {
        std::map<IRI, OWLNamedIndividual::Ptr>::const_iterator cit = mNamedIndividuals.find(iri);
        if(cit != mNamedIndividuals.end())
        {
            return cit->second;
        }
    }
    {
        std::map<IRI, OWLAnonymousIndividual::Ptr>::const_iterator cit = mAnonymousIndividuals.find(iri);
        if(cit != mAnonymousIndividuals.end())
        {
            return cit->second;
        }
    }
    throw std::invalid_argument("owlapi::model::OWLOntology::getIndividual: no individual '" + iri.toString() + "' known");
}

void OWLOntology::addAxiom(const OWLAxiom::Ptr& axiom)
{
    // TODO check if axiom already exists
    mAxiomsByType[axiom->getAxiomType()].push_back(axiom);
}

OWLOntology::Ptr OWLOntology::fromFile(const std::string& filename)
{
    OWLOntologyReader reader;
    return reader.fromFile(filename);
}

OWLAxiom::PtrList OWLOntology::getAxioms() const
{
    OWLAxiom::PtrList axioms;
    AxiomMap::const_iterator cit = mAxiomsByType.begin();
    for(; cit != mAxiomsByType.end(); ++cit)
    {
        const OWLAxiom::PtrList& typedAxioms = cit->second;
        axioms.insert(axioms.begin(), typedAxioms.begin(), typedAxioms.end());
    }
    return axioms;
}

} // end namespace model
} // end namespace owlapi
