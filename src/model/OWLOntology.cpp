#include "OWLOntology.hpp"
#include <owlapi/KnowledgeBase.hpp>
#include <owlapi/io/OWLOntologyIO.hpp>

namespace owlapi {
namespace model {

OWLOntology::OWLOntology(const KnowledgeBase::Ptr& kb)
    : mpKnowledgeBase(kb)
{
    if(!mpKnowledgeBase)
    {
        mpKnowledgeBase = boost::make_shared<KnowledgeBase>();
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

void OWLOntology::retractValueOf(const OWLIndividual::Ptr& individual, const OWLDataProperty::Ptr& property)
{
    OWLNamedIndividual::Ptr namedIndividual = boost::dynamic_pointer_cast<OWLNamedIndividual>(individual);
    std::string individualName = "anonymous";
    if(namedIndividual)
    {
        individualName = namedIndividual->getIRI().toString();
    }

    std::vector<OWLAxiom::Ptr>& axioms = mAxiomsByType[OWLAxiom::DataPropertyAssertion];
    std::vector<OWLAxiom::Ptr>::iterator it = axioms.begin();
    for(; it != axioms.end(); ++it)
    {
        OWLDataPropertyAssertionAxiom::Ptr axiom = boost::dynamic_pointer_cast<OWLDataPropertyAssertionAxiom>(*it);
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

} // end namespace model
} // end namespace owlapi
