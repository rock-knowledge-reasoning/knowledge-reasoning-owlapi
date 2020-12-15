#include "OWLOntology.hpp"
#include "../KnowledgeBase.hpp"
#include "../io/OWLOntologyIO.hpp"
#include "OWLOntologyChange.hpp"
#include "OWLOntologyChangeFilter.hpp"
#include "OWLAnnotationAssertionAxiom.hpp"

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

void OWLOntology::cleanup()
{
    if(mpKnowledgeBase)
    {
        mpKnowledgeBase->cleanup();
        return;
    }
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

OWLAnonymousClassExpression::Ptr OWLOntology::getAnonymousClassExpression(const IRI& iri) const
{
    std::map<IRI, OWLAnonymousClassExpression::Ptr>::const_iterator cit = mAnonymousClassExpressions.find(iri);
    if(cit != mAnonymousClassExpressions.end())
    {
        return cit->second;
    }
    throw std::invalid_argument("owlapi::model::OWLOntology::getAnonymousClassExpression: "
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

const IRI& OWLOntology::iriOfDataProperty(const OWLDataProperty::Ptr& property) const
{
    for(const auto& p : mDataProperties)
    {
        if(property == p.second)
        {
            return p.first;
        }
    }
    throw std::invalid_argument("owlapi::model::OWLOntology::iriOfDataProperty: not matching"
            " IRI found for dataproperty");
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

OWLAnnotationProperty::Ptr OWLOntology::getAnnotationProperty(const IRI& iri) const
{
    std::map<IRI, OWLAnnotationProperty::Ptr>::const_iterator cit = mAnnotationProperties.find(iri);
    if(cit != mAnnotationProperties.end())
    {
        return cit->second;
    }
    throw std::invalid_argument("owlapi::model::OWLOntology::getAnnotationProperty: "
            " no annotation property '" + iri.toString() + "' known");
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

OWLAnonymousIndividual::Ptr OWLOntology::getAnonymousIndividual(const IRI& iri) const
{
    std::map<IRI, OWLAnonymousIndividual::Ptr>::const_iterator cit = mAnonymousIndividuals.find(iri);
    if(cit != mAnonymousIndividuals.end())
    {
        return cit->second;
    }
    throw std::invalid_argument("owlapi::model::OWLOntology::getAnonymousIndividual: no anonymous individual '" + iri.toString() + "' known");
}

void OWLOntology::addAxiom(const OWLAxiom::Ptr& axiom)
{
    OWLAxiom::PtrList& axioms = mAxiomsByType[axiom->getAxiomType()];

    switch(axiom->getAxiomType())
    {
        case OWLAxiom::AnnotationAssertion:
            {
                OWLAnnotationAssertionAxiom::Ptr annotationAxiom =
                    dynamic_pointer_cast<OWLAnnotationAssertionAxiom>(axiom);
                mAnnotationAxioms[annotationAxiom->getProperty()].push_back(annotationAxiom);
            }
            break;
        default:
            break;

    }

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
    OWLAxiomRetractVisitor visitor(this);
    OWLAxiom::PtrList::iterator ait = std::find(axioms.begin(), axioms.end(), axiom);
    if(ait != axioms.end())
    {
        LOG_INFO_S << "Removing axiom: " << (*ait)->toString();
        (*ait)->accept(&visitor);
    }
    axioms.erase(ait);
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

void OWLOntology::retractIndividual(const IRI& iri)
{
    OWLIndividual::Ptr individual = getIndividual(iri);
    std::vector<OWLClassAssertionAxiom::Ptr>& axioms = mClassAssertionAxiomsByIndividual[individual];
    for(OWLClassAssertionAxiom::Ptr& axiom : axioms)
    {
        OWLAxiomRetractVisitor v(this);
        axiom->accept(&v);
    }
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

OWLEntity::PtrList OWLOntology::getSignature() const
{
    OWLEntity::PtrList entities;
    for(const auto& p : mDeclarationsByEntity)
    {
        entities.push_back(p.first);
    }
    return entities;
}

} // end namespace model
} // end namespace owlapi
