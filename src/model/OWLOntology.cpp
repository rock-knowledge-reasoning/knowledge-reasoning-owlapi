#include "OWLOntology.hpp"
#include <owlapi/KnowledgeBase.hpp>
#include <owlapi/io/OWLOntologyIO.hpp>
#include "OWLOntologyChange.hpp"
#include "OWLOntologyChangeFilter.hpp"

namespace owlapi {
namespace model {

OWLOntology::OWLOntology(const KnowledgeBase::Ptr& kb)
    : mpKnowledgeBase(kb)
{
    if(!mpKnowledgeBase)
    {
        mpKnowledgeBase = make_shared<KnowledgeBase>();
    }
}

OWLOntology::~OWLOntology()
{}

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
    OWLAxiom::PtrList& axioms = mAxiomsByType[axiom->getAxiomType()];

    if( axioms.end() == std::find(axioms.begin(), axioms.end(), axiom) )
    {
        axioms.push_back(axiom);
    } else {
        LOG_WARN_S << "Axiom: '" << axiom->toString() << "' has already been added to ontology loaded from '" << getAbsolutePath() << "'";
    }
}

OWLOntology::Ptr OWLOntology::fromFile(const std::string& filename)
{
    return io::OWLOntologyIO::fromFile(filename);
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

void OWLOntology::removeAxiom(const OWLAxiom::Ptr& axiom)
{
    // remove axiom from reverse map
    OWLAxiom::PtrList& axioms = mAxiomsByType[axiom->getAxiomType()];
    OWLAxiom::PtrList::iterator ait = std::find(axioms.begin(), axioms.end(), axiom);
    if(ait != axioms.end())
    {
        LOG_INFO_S << "Removing axiom: " << (*ait)->toString();
    }

}

void OWLOntology::retractIndividual(const OWLIndividual::Ptr& individual)
{
    if(!individual->isAnonymous())
    {
        OWLNamedIndividual::Ptr namedIndividual = dynamic_pointer_cast<OWLNamedIndividual>(individual);
        retractNamedIndividual(namedIndividual);
    } else {
        OWLAnonymousIndividual::Ptr anonymousIndividual = dynamic_pointer_cast<OWLAnonymousIndividual>(individual);
        retractAnonymousIndividual(anonymousIndividual);
    }

    throw std::invalid_argument("owlapi::model::OWLOntology::retractIndividual: individual not found");
}

void OWLOntology::retractNamedIndividual(const OWLNamedIndividual::Ptr& individual)
{
    std::map<OWLNamedIndividual::Ptr, std::vector<OWLAxiom::Ptr> >::const_iterator it = mNamedIndividualAxioms.find(individual);
    if(it != mNamedIndividualAxioms.end())
    {
        std::vector<OWLAxiom::Ptr> axioms = it->second;

        std::vector<OWLAxiom::Ptr>::const_iterator cit = axioms.begin();
        for(; cit != axioms.end(); ++cit)
        {
            LOG_DEBUG_S << "Retracting Axiom: " << (*cit)->toString();
            removeAxiom(*cit);
        }
        mNamedIndividualAxioms.erase(it->first);
        mNamedIndividuals.erase(individual->getIRI());
    }
}

void OWLOntology::retractAnonymousIndividual(const OWLAnonymousIndividual::Ptr& individual)
{
    std::map<OWLAnonymousIndividual::Ptr, std::vector<OWLAxiom::Ptr> >::const_iterator it = mAnonymousIndividualAxioms.find(individual);
    if(it != mAnonymousIndividualAxioms.end())
    {
        std::vector<OWLAxiom::Ptr> axioms = it->second;

        std::vector<OWLAxiom::Ptr>::const_iterator cit = axioms.begin();
        for(; cit != axioms.end(); ++cit)
        {
            LOG_DEBUG_S << "Retracting Axiom: " << (*cit)->toString();
            removeAxiom(*cit);
        }
        mAnonymousIndividualAxioms.erase(it->first);
        mAnonymousIndividuals.erase(individual->getReferenceID());
    }
}

void OWLOntology::retractValueOf(const OWLIndividual::Ptr& individual, const OWLDataProperty::Ptr& property)
{
    OWLNamedIndividual::Ptr namedIndividual = dynamic_pointer_cast<OWLNamedIndividual>(individual);
    std::string individualName = "anonymous";
    if(namedIndividual)
    {
        individualName = namedIndividual->getIRI().toString();
    }

    std::vector<OWLAxiom::Ptr>& axioms = mAxiomsByType[OWLAxiom::DataPropertyAssertion];
    std::vector<OWLAxiom::Ptr>::iterator it = axioms.begin();
    for(; it != axioms.end(); ++it)
    {
        OWLDataPropertyAssertionAxiom::Ptr axiom = dynamic_pointer_cast<OWLDataPropertyAssertionAxiom>(*it);
        if(axiom->getSubject() == individual && axiom->getProperty() == property)
        {
            LOG_DEBUG_S << "Retracted DataPropertyAssertionAxiom: s: '" << individualName << "'"
                    << ", p: " << property->getIRI().toString() << "'";
            axioms.erase(it);
            return;
        }
    }
    LOG_DEBUG_S << "No value to be retracted";
}

void OWLOntology::addDirectImportsDocument(const IRI& iri)
{
    if( mDirectImportsDocuments.end() == std::find(mDirectImportsDocuments.begin(), mDirectImportsDocuments.end(), iri) )
    {
        mDirectImportsDocuments.push_back(iri);
        addImportsDocument(iri);
    }
}

void OWLOntology::addImportsDocument(const IRI& iri)
{
    if( mImportsDocuments.end() == std::find(mImportsDocuments.begin(), mImportsDocuments.end(), iri) )
    {
        mImportsDocuments.push_back(iri);
    }
}

OWLAxiom::PtrList OWLOntology::getReferencingAxioms(const OWLEntity::Ptr& entity, bool excludeImports)
{
    OWLAxiom::PtrList axioms;

    return axioms;
}

ChangeApplied OWLOntology::applyChange(const OWLOntologyChange::Ptr& change)
{
    OWLOntologyChangeFilter::Ptr filter(new OWLOntologyChangeFilter());
    return change->accept(filter);
}

} // end namespace model
} // end namespace owlapi
