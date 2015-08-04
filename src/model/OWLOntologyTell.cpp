#include "OWLOntologyTell.hpp"
#include <owlapi/KnowledgeBase.hpp>
#include <owlapi/OWLApi.hpp>
#include <owlapi/model/OWLDataType.hpp>
#include <base/Logging.hpp>

namespace owlapi {
namespace model {

OWLOntologyTell::OWLOntologyTell(OWLOntology::Ptr ontology)
    : mpOntology(ontology)
    , mAsk(ontology)
{}

void OWLOntologyTell::initializeDefaultClasses()
{
    klass(vocabulary::OWL::Class());
    klass(vocabulary::OWL::Thing());

//    // http://www.w3.org/TR/2009/REC-owl2-syntax-20091027/#Entity_Declarations_and_Typing
//    // Declarations for the built-in entities of OWL 2, listed in Table 5, are implicitly present in every OWL 2 ontology.
//    OWLClass thing(vocabulary::OWL::Thing());
//    // vocabulary::OWL::Nothing()
//    // ObjectProperty
//    // vocabulary::OWL::topObjectProperty()
//    // vocabulary::OWL::bottomObjectProperty()
//    // DataProperty
//    // vocubulary::OWL::topDataProperty()
//    // vocabulary::OWL::bottomDataProperty()
//    // Datatypes:
//    // vocabulary::RDFS::Literal()
//    // ... each datatype in OWL 2
//    // AnnotationProperty
//    // vocabulary::OWL::AnnotationProperty()
//
//    // Trigger instanciation of the following classes
//    getClassLazy(vocabulary::OWL::Class());

}

OWLClass::Ptr OWLOntologyTell::klass(const IRI& iri)
{
    LOG_DEBUG_S << "Retrieve class: " << iri;
    std::map<IRI, OWLClass::Ptr>::const_iterator it = mpOntology->mClasses.find(iri);
    if(it != mpOntology->mClasses.end())
    {
        return it->second;
    } else {
        OWLClass::Ptr klass(new OWLClass(iri));
        mpOntology->mClasses[iri] = klass;

        // Update kb
        mpOntology->kb()->getClassLazy(iri);

        OWLEntity::Ptr entity(new OWLEntity(iri, OWLEntity::CLASS));
        OWLAxiom::Ptr axiom(new OWLDeclarationAxiom(entity));
        mpOntology->addAxiom(axiom);

        return klass;
    }
}

OWLAnonymousIndividual::Ptr OWLOntologyTell::anonymousIndividual(const IRI& iri)
{
    std::map<IRI, OWLAnonymousIndividual::Ptr>::const_iterator it = mpOntology->mAnonymousIndividuals.find(iri);
    if(it != mpOntology->mAnonymousIndividuals.end())
    {
        return it->second;
    } else {
        NodeID node(iri.toString(), true);
        OWLAnonymousIndividual::Ptr individual(new OWLAnonymousIndividual(node));
        mpOntology->mAnonymousIndividuals[iri] = individual;
        return individual;
    }
}

OWLNamedIndividual::Ptr OWLOntologyTell::namedIndividual(const IRI& iri)
{
    std::map<IRI, OWLNamedIndividual::Ptr>::const_iterator it = mpOntology->mNamedIndividuals.find(iri);
    if(it != mpOntology->mNamedIndividuals.end())
    {
        return it->second;
    } else {
        OWLNamedIndividual::Ptr individual(new OWLNamedIndividual(iri));
        mpOntology->mNamedIndividuals[iri] = individual;

        //Update kb
        mpOntology->kb()->getInstanceLazy(iri);

        return individual;
    }
}

OWLObjectProperty::Ptr OWLOntologyTell::objectProperty(const IRI& iri)
{
    std::map<IRI, OWLObjectProperty::Ptr>::const_iterator it = mpOntology->mObjectProperties.find(iri);
    if(it != mpOntology->mObjectProperties.end())
    {
        return it->second;
    } else {
        OWLObjectProperty::Ptr property(new OWLObjectProperty(iri));
        mpOntology->mObjectProperties[iri] = property;

        //Update kb
        mpOntology->kb()->getObjectPropertyLazy(iri);

        OWLEntity::Ptr entity(new OWLEntity(iri, OWLEntity::OBJECT_PROPERTY));
        OWLAxiom::Ptr axiom(new OWLDeclarationAxiom(entity));
        mpOntology->addAxiom(axiom);

        return property;
    }
}

OWLDataProperty::Ptr OWLOntologyTell::dataProperty(const IRI& iri)
{
    std::map<IRI, OWLDataProperty::Ptr>::const_iterator it = mpOntology->mDataProperties.find(iri);
    if(it != mpOntology->mDataProperties.end())
    {
        return it->second;
    } else {
        OWLDataProperty::Ptr property(new OWLDataProperty(iri));
        mpOntology->mDataProperties[iri] = property;

        //Update kb
        mpOntology->kb()->getDataPropertyLazy(iri);

        OWLEntity::Ptr entity(new OWLEntity(iri, OWLEntity::DATA_PROPERTY));
        OWLAxiom::Ptr axiom(new OWLDeclarationAxiom(entity));
        mpOntology->addAxiom(axiom);

        return property;
    }
}

OWLSubClassOfAxiom::Ptr OWLOntologyTell::subClassOf(const IRI& subclass, OWLClassExpression::Ptr superclass)
{
    OWLClass::Ptr e_subclass = klass(subclass);
    return subClassOf(e_subclass, superclass);
}

OWLSubClassOfAxiom::Ptr OWLOntologyTell::subClassOf(const IRI& subclass, const IRI& superclass)
{
    // All classes inherit from top concept, i.e. owl:Thing
    OWLClass::Ptr e_subclass = klass(subclass);
    OWLClass::Ptr e_superclass = klass(superclass);

    return subClassOf(e_subclass, e_superclass);
}

OWLSubClassOfAxiom::Ptr OWLOntologyTell::subClassOf(OWLClass::Ptr subclass, OWLClass::Ptr superclass)
{
    mpOntology->kb()->subClassOf(subclass->getIRI(), superclass->getIRI());
    return subClassOf(boost::dynamic_pointer_cast<OWLClassExpression>(subclass),boost::dynamic_pointer_cast<OWLClassExpression>(superclass));
}


OWLSubClassOfAxiom::Ptr OWLOntologyTell::subClassOf(OWLClassExpression::Ptr subclassExpression, OWLClassExpression::Ptr superclassExpression)
{
    OWLSubClassOfAxiom::Ptr axiom(new OWLSubClassOfAxiom(subclassExpression, superclassExpression));
    mpOntology->mSubClassAxiomBySubPosition[subclassExpression].push_back(axiom);
    mpOntology->mSubClassAxiomBySuperPosition[superclassExpression].push_back(axiom);

    LOG_DEBUG_S << "Added SubClassOfAxiom:" << subclassExpression->toString() << " axiom: " << axiom->toString();

    mpOntology->addAxiom(axiom);
    return axiom;
}

OWLCardinalityRestriction::Ptr OWLOntologyTell::cardinalityRestriction(OWLPropertyExpression::Ptr property, uint32_t cardinality, const OWLQualification& qualification, OWLCardinalityRestriction::CardinalityRestrictionType restrictionType)
{
    return OWLCardinalityRestriction::getInstance(property, cardinality, qualification, restrictionType);

}

void OWLOntologyTell::addAxiom(OWLAxiom::Ptr axiom)
{
    mpOntology->mAxiomsByType[axiom->getAxiomType()].push_back( axiom );
}

OWLClassAssertionAxiom::Ptr OWLOntologyTell::instanceOf(const IRI& instance, const IRI& classType)
{
    // Update reasoner kb
    mpOntology->kb()->instanceOf(instance, classType);

    // ClassAssertion
    OWLNamedIndividual::Ptr e_individual = namedIndividual(instance);
    OWLClassExpression::Ptr e_class = klass(classType);
    OWLClassAssertionAxiom::Ptr axiom(new OWLClassAssertionAxiom(e_individual, e_class));

    // Register
    mpOntology->mClassAssertionAxiomsByClass[e_class].push_back(axiom);
    mpOntology->mClassAssertionAxiomsByIndividual[e_individual].push_back(axiom);
    mpOntology->mNamedIndividualAxioms[e_individual].push_back(axiom);

    LOG_DEBUG_S << "NamedIndividual '" << instance << "' of class: '" << classType << "'";
    return axiom;
}

void OWLOntologyTell::inverseFunctionalProperty(const IRI& property)
{
    mpOntology->kb()->inverseFunctionalProperty(property);
}

void OWLOntologyTell::symmetricProperty(const IRI& property)
{
    mpOntology->kb()->symmetricProperty(property);

    {
        OWLObjectProperty::Ptr oProperty = mpOntology->getObjectProperty(property);
        OWLSymmetricObjectPropertyAxiom::Ptr axiom(new OWLSymmetricObjectPropertyAxiom(oProperty));
        mpOntology->addAxiom(axiom);
    }
}


void OWLOntologyTell::transitiveProperty(const IRI& property)
{
    mpOntology->kb()->transitiveProperty(property);

    {
        OWLObjectProperty::Ptr oProperty = mpOntology->getObjectProperty(property);
        OWLTransitiveObjectPropertyAxiom::Ptr axiom(new OWLTransitiveObjectPropertyAxiom(oProperty));
        mpOntology->addAxiom(axiom);
    }
}

void OWLOntologyTell::functionalObjectProperty(const IRI& property)
{
    mpOntology->kb()->functionalProperty(property, KnowledgeBase::OBJECT);

    {
        OWLObjectProperty::Ptr oProperty = mpOntology->getObjectProperty(property);
        OWLFunctionalObjectPropertyAxiom::Ptr axiom(new OWLFunctionalObjectPropertyAxiom(oProperty));
        mpOntology->addAxiom(axiom);
    }
}

void OWLOntologyTell::functionalDataProperty(const IRI& property)
{
    mpOntology->kb()->functionalProperty(property, KnowledgeBase::DATA);

    {
        OWLDataProperty::Ptr dProperty = mpOntology->getDataProperty(property);
        OWLFunctionalDataPropertyAxiom::Ptr axiom(new OWLFunctionalDataPropertyAxiom(dProperty));
        mpOntology->addAxiom(axiom);
    }
}

void OWLOntologyTell::relatedTo(const IRI& subject, const IRI& relation, const IRI& object)
{
    mpOntology->kb()->relatedTo(subject, relation, object);
}

OWLSubPropertyAxiom::Ptr OWLOntologyTell::subPropertyOf(const IRI& subProperty, const IRI& parentProperty)
{
    mpOntology->kb()->subPropertyOf(subProperty, parentProperty);

    OWLSubPropertyAxiom::Ptr axiom;
    if(mAsk.isObjectProperty(parentProperty))
    {
        OWLObjectProperty::Ptr subOProperty = mpOntology->getObjectProperty(subProperty);
        OWLObjectProperty::Ptr superOProperty = mpOntology->getObjectProperty(parentProperty);

        axiom = OWLSubPropertyAxiom::Ptr(new OWLSubObjectPropertyOfAxiom(subOProperty, superOProperty));
    } else {
        OWLDataProperty::Ptr subDProperty = mpOntology->getDataProperty(subProperty);
        OWLDataProperty::Ptr superDProperty = mpOntology->getDataProperty(parentProperty);

        axiom = OWLSubPropertyAxiom::Ptr(new OWLSubDataPropertyOfAxiom(subDProperty, superDProperty));
    }

    mpOntology->addAxiom(axiom);
    return axiom;
}

void OWLOntologyTell::dataPropertyDomainOf(const IRI& property, const IRI& classType)
{
    mpOntology->kb()->domainOf(property, classType, KnowledgeBase::DATA);

    /*OWLDataProperty::Ptr dataProperty = mpOntology->getDataProperty(property);
    OWLClass::Ptr domain = mpOntology->getClass(classType);
    dataProperty->addDomain(domain);
    */
}

void OWLOntologyTell::dataPropertyRangeOf(const IRI& property, const IRI& classType)
{
    // cannot use the following since that is not implemented in the reasoner
    // mpOntology->kb()->rangeOf(relation, classType, KnowledgeBase::OBJECT);

    OWLDataProperty::Ptr dataProperty = mpOntology->getDataProperty(property);
    OWLDataRange::Ptr range(new OWLDataType(classType));
    dataProperty->addRange(range);
}

void OWLOntologyTell::objectPropertyDomainOf(const IRI& relation, const IRI& classType)
{
    mpOntology->kb()->domainOf(relation, classType, KnowledgeBase::OBJECT);

    /*OWLObjectProperty::Ptr objectProperty = mpOntology->getObjectProperty(relation);
    OWLClass::Ptr domain = mpOntology->getClass(classType);
    objectProperty->addDomain(domain);
    */
}

void OWLOntologyTell::objectPropertyRangeOf(const IRI& relation, const IRI& classType)
{
    mpOntology->kb()->rangeOf(relation, classType, KnowledgeBase::OBJECT);

    /*OWLObjectProperty::Ptr objectProperty = mpOntology->getObjectProperty(relation);
    OWLClass::Ptr range = mpOntology->getClass(classType);
    objectProperty->addRange(range);
    */
}

void OWLOntologyTell::inverseOf(const IRI& relation, const IRI& inverseRelation)
{
    mpOntology->kb()->inverseOf(relation, inverseRelation);

    if( mAsk.isObjectProperty(relation) || mAsk.isObjectProperty(inverseRelation) )
    {
        OWLObjectProperty::Ptr first = mpOntology->getObjectProperty(relation);
        OWLObjectProperty::Ptr second = mpOntology->getObjectProperty(inverseRelation);

        OWLInverseObjectPropertiesAxiom::Ptr axiom(new OWLInverseObjectPropertiesAxiom(first, second));
        mpOntology->addAxiom(axiom);
    }
}

void OWLOntologyTell::valueOf(const IRI& instance, const IRI& dataProperty, OWLLiteral::Ptr literal)
{
    reasoner::factpp::DataValue dataValue = mpOntology->kb()->dataValue(literal->getValue(), literal->getType());
    mpOntology->kb()->valueOf(instance, dataProperty, dataValue);
}

void OWLOntologyTell::restrictClass(const IRI& klass, OWLCardinalityRestriction::Ptr restriction)
{
    subClassOf(klass, restriction);
}

} // end namespace model
} // end namespace owlapi
