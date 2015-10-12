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

        OWLEntity::Ptr entity = OWLEntity::klass(iri);
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

        OWLEntity::Ptr entity = OWLEntity::namedIndividual(iri);
        mpOntology->addAxiom( OWLAxiom::declare(entity) );

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

        OWLEntity::Ptr entity = OWLEntity::objectProperty(iri);
        mpOntology->addAxiom( OWLAxiom::declare(entity) );

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
        //Update kb
        mpOntology->kb()->getDataPropertyLazy(iri);

        OWLEntity::Ptr entity = OWLEntity::dataProperty(iri);
        OWLDataProperty::Ptr property = ptr_cast<OWLDataProperty, OWLEntity>(entity);
        mpOntology->mDataProperties[iri] = property;

        mpOntology->addAxiom( OWLAxiom::declare(entity) );
        return property;
    }
}

OWLAnnotationProperty::Ptr OWLOntologyTell::annotationProperty(const IRI& iri)
{
    std::map<IRI, OWLAnnotationProperty::Ptr>::const_iterator it = mpOntology->mAnnotationProperties.find(iri);
    if(it != mpOntology->mAnnotationProperties.end())
    {
        return it->second;
    } else {

        OWLEntity::Ptr entity = OWLEntity::dataProperty(iri);
        mpOntology->addAxiom( OWLAxiom::declare(entity) );

        OWLAnnotationProperty::Ptr property = ptr_cast<OWLAnnotationProperty, OWLEntity>(entity);
        mpOntology->mAnnotationProperties[iri] = property;
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
    return subClassOf(ptr_cast<OWLClassExpression,OWLClass>(subclass),
            ptr_cast<OWLClassExpression, OWLClass>(superclass));
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

    mpOntology->addAxiom( axiom );
    return axiom;
}

void OWLOntologyTell::inverseFunctionalProperty(const IRI& property)
{
    mpOntology->kb()->inverseFunctionalProperty(property);
    addUnaryObjectPropertyAxiom<OWLInverseFunctionalObjectPropertyAxiom>(property);
}

void OWLOntologyTell::reflexiveProperty(const IRI& property)
{
    mpOntology->kb()->reflexiveProperty(property);
    addUnaryObjectPropertyAxiom<OWLReflexiveObjectPropertyAxiom>(property);
}

void OWLOntologyTell::irreflexiveProperty(const IRI& property)
{
    mpOntology->kb()->irreflexiveProperty(property);
    addUnaryObjectPropertyAxiom<OWLIrreflexiveObjectPropertyAxiom>(property);
}

void OWLOntologyTell::symmetricProperty(const IRI& property)
{
    mpOntology->kb()->symmetricProperty(property);
    addUnaryObjectPropertyAxiom<OWLSymmetricObjectPropertyAxiom>(property);
}

void OWLOntologyTell::asymmetricProperty(const IRI& property)
{
    mpOntology->kb()->asymmetricProperty(property);
    addUnaryObjectPropertyAxiom<OWLAsymmetricObjectPropertyAxiom>(property);
}

void OWLOntologyTell::transitiveProperty(const IRI& property)
{
    mpOntology->kb()->transitiveProperty(property);
    addUnaryObjectPropertyAxiom<OWLTransitiveObjectPropertyAxiom>(property);
}

void OWLOntologyTell::functionalObjectProperty(const IRI& property)
{
    mpOntology->kb()->functionalProperty(property, KnowledgeBase::OBJECT);
    addUnaryObjectPropertyAxiom<OWLFunctionalObjectPropertyAxiom>(property);
}

void OWLOntologyTell::functionalDataProperty(const IRI& property)
{
    mpOntology->kb()->functionalProperty(property, KnowledgeBase::DATA);
    addUnaryDataPropertyAxiom<OWLFunctionalDataPropertyAxiom>(property);
}

void OWLOntologyTell::relatedTo(const IRI& subject, const IRI& relation, const IRI& object)
{
    mpOntology->kb()->relatedTo(subject, relation, object);

    LOG_DEBUG_S << "Add relation: " << std::endl
        << "    s: " << subject << std::endl
        << "    p: " << relation << std::endl
        << "    o: " << object << std::endl;

    OWLIndividual::Ptr individual = mpOntology->getIndividual(subject);
    OWLPropertyAssertionObject::Ptr assertionObject;
    try {
        assertionObject = mpOntology->getIndividual(object);
        LOG_DEBUG_S << "    o: from known individual" << std::endl;
    } catch(...)
    {
        assertionObject = OWLLiteral::create(object.toString());
        LOG_DEBUG_S << "    o: as literal" << std::endl;
    }

    if(mAsk.isObjectProperty(relation))
    {
        OWLObjectPropertyAssertionAxiom::Ptr axiom(new OWLObjectPropertyAssertionAxiom(
                    individual,
                    mpOntology->getObjectProperty(relation),
                    assertionObject));
        mpOntology->addAxiom(axiom);

    } else if(mAsk.isDataProperty(relation))
    {
        OWLDataPropertyAssertionAxiom::Ptr axiom(new OWLDataPropertyAssertionAxiom(
                    individual,
                    mpOntology->getDataProperty(relation),
                    boost::dynamic_pointer_cast<OWLLiteral>(assertionObject) ));
        mpOntology->addAxiom(axiom);
    } else {
        throw std::runtime_error("owlapi::model::OWLOntologyTell::relatedTo: "
                "'" + relation.toString() + "' is not a known relation/property");
    }
}

OWLSubPropertyAxiom::Ptr OWLOntologyTell::subPropertyOf(const IRI& subProperty, const IRI& parentProperty)
{
    mpOntology->kb()->subPropertyOf(subProperty, parentProperty);

    OWLSubPropertyAxiom::Ptr axiom;
    if(mAsk.isObjectProperty(parentProperty))
    {
        objectProperty(subProperty);

        OWLObjectProperty::Ptr subOProperty = mpOntology->getObjectProperty(subProperty);
        OWLObjectProperty::Ptr superOProperty = mpOntology->getObjectProperty(parentProperty);

        axiom = OWLSubPropertyAxiom::Ptr(new OWLSubObjectPropertyOfAxiom(subOProperty, superOProperty));
    } else {
        dataProperty(subProperty);

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

    OWLDataProperty::Ptr dataProperty = mpOntology->getDataProperty(property);
    OWLClassExpression::Ptr domain = mpOntology->getClass(classType);

    OWLDataPropertyExpression::Ptr e_dataProperty = ptr_cast<OWLDataPropertyExpression, OWLDataProperty>(dataProperty);
    OWLDataPropertyDomainAxiom::Ptr axiom(new OWLDataPropertyDomainAxiom(e_dataProperty, domain));
    mpOntology->addAxiom(axiom);
}

void OWLOntologyTell::dataPropertyRangeOf(const IRI& property, const IRI& classType)
{
    // cannot use the following since that is not implemented in the reasoner
    // mpOntology->kb()->rangeOf(relation, classType, KnowledgeBase::OBJECT);

    OWLDataProperty::Ptr dataProperty = mpOntology->getDataProperty(property);
    OWLDataRange::Ptr range(new OWLDataType(classType));
    dataProperty->addRange(range);

    OWLDataPropertyExpression::Ptr e_dataProperty = boost::dynamic_pointer_cast<OWLDataPropertyExpression>(dataProperty);
    OWLDataPropertyRangeAxiom::Ptr axiom(new OWLDataPropertyRangeAxiom(e_dataProperty, range));
    mpOntology->addAxiom(axiom);
}

void OWLOntologyTell::objectPropertyDomainOf(const IRI& relation, const IRI& classType)
{
    mpOntology->kb()->domainOf(relation, classType, KnowledgeBase::OBJECT);

    OWLObjectProperty::Ptr oProperty = mpOntology->getObjectProperty(relation);
    OWLClassExpression::Ptr klass = mpOntology->getClass(classType);

    OWLObjectPropertyDomainAxiom::Ptr axiom(new OWLObjectPropertyDomainAxiom(oProperty, klass));
    mpOntology->addAxiom(axiom);
}

void OWLOntologyTell::objectPropertyRangeOf(const IRI& relation, const IRI& classType)
{
    mpOntology->kb()->rangeOf(relation, classType, KnowledgeBase::OBJECT);

    OWLObjectProperty::Ptr oProperty = mpOntology->getObjectProperty(relation);
    OWLClassExpression::Ptr klass = mpOntology->getClass(classType);

    OWLObjectPropertyRangeAxiom::Ptr axiom(new OWLObjectPropertyRangeAxiom(oProperty, klass));
    mpOntology->addAxiom(axiom);
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

    OWLIndividual::Ptr individual = mAsk.getOWLIndividual(instance);
    OWLDataProperty::Ptr property = mAsk.getOWLDataProperty(dataProperty);
    OWLDataPropertyAssertionAxiom::Ptr axiom(new OWLDataPropertyAssertionAxiom(individual, property, literal));

    mpOntology->retractValueOf(individual, property);
    mpOntology->addAxiom(axiom);
}

void OWLOntologyTell::restrictClass(const IRI& klass, OWLCardinalityRestriction::Ptr restriction)
{
    subClassOf(klass, restriction);
}

} // end namespace model
} // end namespace owlapi
