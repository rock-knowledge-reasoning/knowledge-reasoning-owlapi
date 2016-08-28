#include "OWLOntologyTell.hpp"
#include <owlapi/KnowledgeBase.hpp>
#include <owlapi/OWLApi.hpp>
#include <owlapi/model/OWLDataType.hpp>
#include <base-logging/Logging.hpp>

namespace owlapi {
namespace model {

OWLOntologyTell::OWLOntologyTell(OWLOntology::Ptr ontology, const IRI& origin)
    : mpOntology(ontology)
    , mAsk(ontology)
    , mOrigin(origin)
{
    if(mOrigin.empty())
    {
        mOrigin = ontology->getIRI();
    }
}

void OWLOntologyTell::initializeDefaultClasses()
{
    klass(vocabulary::OWL::Class());
    klass(vocabulary::OWL::Thing());

    klass(vocabulary::OWL::Ontology());
    klass(vocabulary::RDFS::Resource());
    klass(vocabulary::RDFS::Datatype());

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
        addAxiom(axiom);

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
        addAxiom( OWLAxiom::declare(entity) );

        return individual;
    }
}

void OWLOntologyTell::directlyImports(const IRI& iri)
{
    mpOntology->addDirectImportsDocument(iri);
}


void OWLOntologyTell::imports(const IRI& iri)
{
    mpOntology->addImportsDocument(iri);
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
        addAxiom( OWLAxiom::declare(entity) );

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

        addAxiom( OWLAxiom::declare(entity) );
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
        addAxiom( OWLAxiom::declare(entity) );

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

    addAxiom(axiom);
    // to return the exact type
    return axiom;
}

OWLCardinalityRestriction::Ptr OWLOntologyTell::cardinalityRestriction(OWLPropertyExpression::Ptr property, uint32_t cardinality, const OWLQualification& qualification, OWLCardinalityRestriction::CardinalityRestrictionType restrictionType)
{
    return OWLCardinalityRestriction::getInstance(property, cardinality, qualification, restrictionType);

}

OWLAxiom::Ptr OWLOntologyTell::addAxiom(const OWLAxiom::Ptr& axiom)
{
    axiom->setOrigin(mOrigin);
    mpOntology->addAxiom(axiom);
    return axiom;
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

    addAxiom( axiom );
    return axiom;
}

OWLAxiom::Ptr OWLOntologyTell::inverseFunctionalProperty(const IRI& property)
{
    mpOntology->kb()->inverseFunctionalProperty(property);
    return addUnaryObjectPropertyAxiom<OWLInverseFunctionalObjectPropertyAxiom>(property);
}

OWLAxiom::Ptr OWLOntologyTell::reflexiveProperty(const IRI& property)
{
    mpOntology->kb()->reflexiveProperty(property);
    return addUnaryObjectPropertyAxiom<OWLReflexiveObjectPropertyAxiom>(property);
}

OWLAxiom::Ptr OWLOntologyTell::irreflexiveProperty(const IRI& property)
{
    mpOntology->kb()->irreflexiveProperty(property);
    return addUnaryObjectPropertyAxiom<OWLIrreflexiveObjectPropertyAxiom>(property);
}

OWLAxiom::Ptr OWLOntologyTell::symmetricProperty(const IRI& property)
{
    mpOntology->kb()->symmetricProperty(property);
    return addUnaryObjectPropertyAxiom<OWLSymmetricObjectPropertyAxiom>(property);
}

OWLAxiom::Ptr OWLOntologyTell::asymmetricProperty(const IRI& property)
{
    mpOntology->kb()->asymmetricProperty(property);
    return addUnaryObjectPropertyAxiom<OWLAsymmetricObjectPropertyAxiom>(property);
}

OWLAxiom::Ptr OWLOntologyTell::transitiveProperty(const IRI& property)
{
    mpOntology->kb()->transitiveProperty(property);
    return addUnaryObjectPropertyAxiom<OWLTransitiveObjectPropertyAxiom>(property);
}

OWLAxiom::Ptr OWLOntologyTell::functionalObjectProperty(const IRI& property)
{
    mpOntology->kb()->functionalProperty(property, KnowledgeBase::OBJECT);
    return addUnaryObjectPropertyAxiom<OWLFunctionalObjectPropertyAxiom>(property);
}

OWLAxiom::Ptr OWLOntologyTell::functionalDataProperty(const IRI& property)
{
    mpOntology->kb()->functionalProperty(property, KnowledgeBase::DATA);
    return addUnaryDataPropertyAxiom<OWLFunctionalDataPropertyAxiom>(property);
}

OWLAxiom::Ptr OWLOntologyTell::relatedTo(const IRI& subject, const IRI& relation, const IRI& object)
{
    mpOntology->kb()->relatedTo(subject, relation, object);

    LOG_DEBUG_S << "Add relation: " << std::endl
        << "    s: " << subject << std::endl
        << "    p: " << relation << std::endl
        << "    o: " << object << std::endl;

    OWLIndividual::Ptr individual;
    try {
         individual = mpOntology->getIndividual(subject);
    } catch(...)
    {
        // punning is allowed -- so auto-declare named individual
        if(mAsk.isOWLClass(subject) || mAsk.isDataProperty(subject) || mAsk.isObjectProperty(subject))
        {
            individual = namedIndividual(subject);
        } else {
            LOG_DEBUG_S << "Checked for punning: '" << subject << "', but its not a known class or property";
            throw;
        }
    }

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
        return addAxiom(axiom);

    } else if(mAsk.isDataProperty(relation))
    {
        OWLDataPropertyAssertionAxiom::Ptr axiom(new OWLDataPropertyAssertionAxiom(
                    individual,
                    mpOntology->getDataProperty(relation),
                    dynamic_pointer_cast<OWLLiteral>(assertionObject) ));
        return addAxiom(axiom);
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

    addAxiom(axiom);
    return axiom;
}

OWLAxiom::Ptr OWLOntologyTell::dataPropertyDomainOf(const IRI& property, const IRI& classType)
{
    mpOntology->kb()->domainOf(property, classType, KnowledgeBase::DATA);

    OWLDataProperty::Ptr dataProperty = mpOntology->getDataProperty(property);
    OWLClassExpression::Ptr domain = mpOntology->getClass(classType);

    OWLDataPropertyExpression::Ptr e_dataProperty = ptr_cast<OWLDataPropertyExpression, OWLDataProperty>(dataProperty);
    OWLDataPropertyDomainAxiom::Ptr axiom(new OWLDataPropertyDomainAxiom(e_dataProperty, domain));
    return addAxiom(axiom);
}

OWLAxiom::Ptr OWLOntologyTell::dataPropertyRangeOf(const IRI& property, const IRI& classType)
{
    // cannot use the following since that is not implemented in the reasoner
    // mpOntology->kb()->rangeOf(relation, classType, KnowledgeBase::OBJECT);

    OWLDataProperty::Ptr dataProperty = mpOntology->getDataProperty(property);
    OWLDataRange::Ptr range(new OWLDataType(classType));
    dataProperty->addRange(range);

    OWLDataPropertyExpression::Ptr e_dataProperty = dynamic_pointer_cast<OWLDataPropertyExpression>(dataProperty);
    OWLDataPropertyRangeAxiom::Ptr axiom(new OWLDataPropertyRangeAxiom(e_dataProperty, range));
    return addAxiom(axiom);
}

OWLAxiom::Ptr OWLOntologyTell::objectPropertyDomainOf(const IRI& relation, const IRI& classType)
{
    mpOntology->kb()->domainOf(relation, classType, KnowledgeBase::OBJECT);

    OWLObjectProperty::Ptr oProperty = mpOntology->getObjectProperty(relation);
    OWLClassExpression::Ptr klass = mpOntology->getClass(classType);

    OWLObjectPropertyDomainAxiom::Ptr axiom(new OWLObjectPropertyDomainAxiom(oProperty, klass));
    return addAxiom(axiom);
}

OWLAxiom::Ptr OWLOntologyTell::objectPropertyRangeOf(const IRI& relation, const IRI& classType)
{
    mpOntology->kb()->rangeOf(relation, classType, KnowledgeBase::OBJECT);

    OWLObjectProperty::Ptr oProperty = mpOntology->getObjectProperty(relation);
    OWLClassExpression::Ptr klass = mpOntology->getClass(classType);

    OWLObjectPropertyRangeAxiom::Ptr axiom(new OWLObjectPropertyRangeAxiom(oProperty, klass));
    return addAxiom(axiom);
}

OWLAxiom::Ptr OWLOntologyTell::inverseOf(const IRI& relation, const IRI& inverseRelation)
{
    if( mAsk.isObjectProperty(relation) || mAsk.isObjectProperty(inverseRelation) )
    {
        mpOntology->kb()->inverseOf(relation, inverseRelation);

        OWLObjectProperty::Ptr first = mpOntology->getObjectProperty(relation);
        OWLObjectProperty::Ptr second = mpOntology->getObjectProperty(inverseRelation);

        OWLInverseObjectPropertiesAxiom::Ptr axiom(new OWLInverseObjectPropertiesAxiom(first, second));
        return addAxiom(axiom);
    } else {
        throw std::invalid_argument("owlapi::model::OWLOntologyTell::inverseOf: '" + relation.toString() + "'"
                + inverseRelation.toString() + "' -- not an object property");
    }
}

OWLAxiom::Ptr OWLOntologyTell::valueOf(const IRI& instance, const IRI& dataProperty, OWLLiteral::Ptr literal)
{
    reasoner::factpp::DataValue dataValue = mpOntology->kb()->dataValue(literal->getValue(), literal->getType());
    mpOntology->kb()->valueOf(instance, dataProperty, dataValue);

    OWLIndividual::Ptr individual = mAsk.getOWLIndividual(instance);
    OWLDataProperty::Ptr property = mAsk.getOWLDataProperty(dataProperty);
    OWLDataPropertyAssertionAxiom::Ptr axiom(new OWLDataPropertyAssertionAxiom(individual, property, literal));

    mpOntology->retractValueOf(individual, property);
    return addAxiom(axiom);
}

OWLAxiom::Ptr OWLOntologyTell::restrictClass(const IRI& klass, OWLCardinalityRestriction::Ptr restriction)
{
    OWLProperty::Ptr property = dynamic_pointer_cast<OWLProperty>( restriction->getProperty());
    if(!property)
    {
        throw std::invalid_argument("OWLOntologyTell::restrictClass: cardinalityRestriction on anonymous properties is currently not supported");
    }

    switch(restriction->getCardinalityRestrictionType())
    {
        case OWLCardinalityRestriction::MIN:
            mpOntology->kb()->objectPropertyRestriction(restriction::MIN_CARDINALITY,property->getIRI(), restriction->getQualification(), restriction->getCardinality());
            break;
        case OWLCardinalityRestriction::MAX:
            mpOntology->kb()->objectPropertyRestriction(restriction::MAX_CARDINALITY, property->getIRI(), restriction->getQualification(), restriction->getCardinality());
            break;
        case OWLCardinalityRestriction::EXACT:
            mpOntology->kb()->objectPropertyRestriction(restriction::EXACT_CARDINALITY, property->getIRI(), restriction->getQualification(), restriction->getCardinality());
            break;
        default:
            throw std::invalid_argument("OWLOntologyTell::restrictClass: unknown cardinality restriction type given");
    }
    return subClassOf(klass, restriction);
}

void OWLOntologyTell::ontology(const IRI& iri)
{
    // allowing punning
    instanceOf(iri, vocabulary::OWL::Ontology());
}

void OWLOntologyTell::datatype(const IRI& iri)
{
    // http://www.w3.org/TR/owl-ref/#rdf-datatype
    //
    // but also
    // <http://www.linkedmodel.org/schema/vaem#dateUnion>
    //      <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>
    //      <http://www.w3.org/2000/01/rdf-schema#Datatype> .
    // <http://www.linkedmodel.org/schema/vaem#dateUnion>
    //      <http://purl.org/dc/elements/1.1/description> "A datatype that is the
    //      union of date xsd data types. 'dateUnion' is equivalent to the xsd
    //      specification that uses an xsd:union of memberTypes='xsd:date
    //      xsd:dateTime xsd:float
    //      xsd:gYear'."^^<http://www.w3.org/2001/XMLSchema#string> .
    // <http://www.linkedmodel.org/schema/vaem#dateUnion>
    //      <http://www.w3.org/2000/01/rdf-schema#label> "date
    //      union"^^<http://www.w3.org/2001/XMLSchema#string> .
    // <http://www.linkedmodel.org/schema/vaem#dateUnion>
    //      <http://www.w3.org/2000/01/rdf-schema#subClassOf>
    //      <http://www.w3.org/2000/01/rdf-schema#Resource> .
    // <http://www.linkedmodel.org/schema/vaem#dateUnion>
    //      <http://www.w3.org/2002/07/owl#equivalentClass> _:genid1 .
    //
    subClassOf(iri, vocabulary::RDFS::Datatype());
}

} // end namespace model
} // end namespace owlapi
