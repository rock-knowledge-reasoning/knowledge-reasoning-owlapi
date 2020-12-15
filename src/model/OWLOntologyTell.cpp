#include "OWLOntologyTell.hpp"
#include "../OWLApi.hpp"
#include "../KnowledgeBase.hpp"
#include "OWLDataType.hpp"
#include "OWLEquivalentClassesAxiom.hpp"
#include "OWLDisjointClassesAxiom.hpp"
#include "OWLDisjointUnionAxiom.hpp"
#include "OWLEquivalentObjectPropertiesAxiom.hpp"
#include "OWLEquivalentDataPropertiesAxiom.hpp"
#include "OWLDisjointObjectPropertiesAxiom.hpp"
#include "OWLDisjointDataPropertiesAxiom.hpp"
#include "OWLDataOneOf.hpp"
#include "OWLDataTypeRestriction.hpp"
#include "OWLSubAnnotationPropertyOfAxiom.hpp"
#include "OWLAnnotationAssertionAxiom.hpp"
#include "OWLDataSomeValuesFrom.hpp"
#include "OWLDataRange.hpp"
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
    klass(vocabulary::OWL::DeprecatedClass());
    klass(vocabulary::OWL::Thing());
    subClassOf(vocabulary::OWL::Nothing(), vocabulary::OWL::Thing());

    klass(vocabulary::OWL::Ontology());
    klass(vocabulary::RDFS::Resource());
    klass(vocabulary::RDFS::Datatype());

    klass(vocabulary::RDF::Alt());
    klass(vocabulary::RDF::Bag());
    klass(vocabulary::RDF::List());
    klass(vocabulary::RDF::Property());
    klass(vocabulary::RDF::Seq());
    klass(vocabulary::RDFS::Class());
    klass(vocabulary::OWL::ObjectProperty());
    klass(vocabulary::OWL::DatatypeProperty());

    //datatype(vocabulary::RDF::Statement());
    //datatype(vocabulary::RDF::XMLLiteral());
    //klass(vocabulary::RDF::PlainLiteral()); // datatype

    // http://www.w3.org/TR/2009/REC-owl2-syntax-20091027/#Entity_Declarations_and_Typing
    // Declarations for the built-in entities of OWL 2, listed in Table 5, are implicitly present in every OWL 2 ontology.
    //  vocabulary::OWL::Thing()
    // vocabulary::OWL::Nothing()
    // ObjectProperty
    // vocabulary::OWL::topObjectProperty()
    // vocabulary::OWL::bottomObjectProperty()
    // DataProperty
    // vocubulary::OWL::topDataProperty()
    // vocabulary::OWL::bottomDataProperty()
    // Datatypes:
    // vocabulary::RDFS::Literal()
    // ... each datatype in OWL 2
    // AnnotationProperty
    // vocabulary::OWL::AnnotationProperty()

    IRIList defaultProperties = {
        vocabulary::OWL::FunctionalProperty(),
        vocabulary::OWL::EquivalentProperty(),
    };

    for(const IRI& iri : defaultProperties)
    {
        klass(iri);
        subClassOf(iri, vocabulary::RDF::Property());
    }

    IRIList defaultPropertyClasses = {
        vocabulary::OWL::ReflexiveProperty(),
        vocabulary::OWL::IrreflexiveProperty(),
        vocabulary::OWL::SymmetricProperty(),
        vocabulary::OWL::AsymmetricProperty(),
        vocabulary::OWL::TransitiveProperty(),
        vocabulary::OWL::TransitiveProperty(),
        vocabulary::OWL::InverseFunctionalProperty(),
    };

    for(const IRI& iri : defaultPropertyClasses)
    {
        klass(iri);
    }

    IRIList defaultObjectProperties = {
        vocabulary::OWL::topObjectProperty(),
        vocabulary::OWL::bottomObjectProperty(),
        // Utility Properties: https://www.w3.org/TR/rdf-schema/#ch_utilvocab
        vocabulary::RDFS::isDefinedBy(),
        vocabulary::RDFS::seeAlso(),
        vocabulary::RDF::value()
    };

    for(const IRI& iri : defaultObjectProperties)
    {
        objectProperty(iri);
    }

    IRIList defaultDataProperties = {
        vocabulary::OWL::topDataProperty(),
        vocabulary::OWL::bottomDataProperty(),
    };

    for(const IRI& iri : defaultDataProperties)
    {
        dataProperty(iri);
    }

    // http://www.w3.org/TR/2009/REC-owl2-syntax-20091027/#Entity_Declarations_and_Typing
    // Declarations for the built-in entities of OWL 2, listed in Table 5, are implicitly present in every OWL 2 ontology.
    subClassOf(vocabulary::OWL::AnnotationProperty(), vocabulary::RDF::Property());
    klass(vocabulary::OWL::Nothing());
    klass(vocabulary::RDFS::Literal());


    IRIList annotationProperties = {
        vocabulary::OWL::deprecated(),
        vocabulary::OWL::incompatibleWith(),
        vocabulary::OWL::versionInfo(),
        vocabulary::RDFS::comment(),
        vocabulary::RDFS::label(),
        vocabulary::RDFS::isDefinedBy(),
        vocabulary::RDFS::seeAlso()
    };

    for(const IRI& iri : annotationProperties)
    {
        annotationProperty(iri);
    }

    // https://www.w3.org/TR/owl-ref/#Header
    klass(vocabulary::OWL::OntologyProperty());
    // Instances of owl:OntologyProperty
    IRIList ontologyProperties = {
        vocabulary::OWL::backwardCompatibleWith(),
        vocabulary::OWL::imports(),
        vocabulary::OWL::incompatibleWith(),
        vocabulary::OWL::priorVersion(),
    };

    for(const IRI& iri : ontologyProperties)
    {
        // TODO: handle ontologyProperties
        annotationProperty(iri);
    }

    // Add known datatypes
    for(const IRI& iri : vocabulary::XSD::getDatatypes())
    {
        datatype(iri);
    }
    for(const IRI& iri : vocabulary::RDF::getDatatypes())
    {
        datatype(iri);
    }
}

OWLClass::Ptr OWLOntologyTell::klass(const IRI& iri)
{
    LOG_DEBUG_S << "Retrieve class: " << iri;
    std::map<IRI, OWLClass::Ptr>::const_iterator it = mpOntology->mClasses.find(iri);
    if(it != mpOntology->mClasses.end())
    {
        return it->second;
    } else {
        OWLClass::Ptr klass = make_shared<OWLClass>(iri);
        mpOntology->mClasses[iri] = klass;

        // Update kb
        mpOntology->kb()->getClassLazy(iri);

        OWLEntity::Ptr entity = OWLEntity::klass(iri);
        OWLAxiom::Ptr axiom = make_shared<OWLDeclarationAxiom>(entity);
        addAxiom(axiom);

        return klass;
    }
}

OWLAnonymousClassExpression::Ptr OWLOntologyTell::anonymousClass(const IRI& iri, const OWLAnonymousClassExpression::Ptr expression)
{
    std::map<IRI, OWLAnonymousClassExpression::Ptr>::iterator it = mpOntology->mAnonymousClassExpressions.find(iri);
    if(expression)
    {
        mpOntology->mAnonymousClassExpressions[iri] = expression;
        return expression;
    } else {
        if(it != mpOntology->mAnonymousClassExpressions.end())
        {
            return it->second;
        }
        OWLAnonymousClassExpression::Ptr aClass = make_shared<OWLAnonymousClassExpression>();
        mpOntology->mAnonymousClassExpressions[iri] = aClass;
        return aClass;
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
        OWLAnonymousIndividual::Ptr individual = make_shared<OWLAnonymousIndividual>(node);
        mpOntology->mAnonymousIndividuals[iri] = individual;

        mpOntology->kb()->getInstanceLazy(iri);
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
        OWLNamedIndividual::Ptr individual = make_shared<OWLNamedIndividual>(iri);
        mpOntology->mNamedIndividuals[iri] = individual;

        //Update kb
        mpOntology->kb()->getInstanceLazy(iri);

        OWLEntity::Ptr entity = OWLEntity::namedIndividual(iri);
        OWLAxiom::Ptr axiom = OWLAxiom::declare(entity);
        addAxiom(axiom);

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

void OWLOntologyTell::rdfProperty(const IRI& iri)
{
    mpOntology->mRDFProperties.insert(iri);
}

OWLObjectProperty::Ptr OWLOntologyTell::objectProperty(const IRI& iri)
{
    std::map<IRI, OWLObjectProperty::Ptr>::const_iterator it = mpOntology->mObjectProperties.find(iri);
    if(it != mpOntology->mObjectProperties.end())
    {
        return it->second;
    } else {
        OWLObjectProperty::Ptr property = make_shared<OWLObjectProperty>(iri);
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

        OWLEntity::Ptr entity = OWLEntity::annotationProperty(iri);
        addAxiom( OWLAxiom::declare(entity) );

        OWLAnnotationProperty::Ptr property = ptr_cast<OWLAnnotationProperty, OWLEntity>(entity, true);
        mpOntology->mAnnotationProperties[iri] = property;

        //instanceOf(iri, vocabulary::OWL::AnnotationProperty());

        return property;
    }
}

void OWLOntologyTell::removeAnnotationProperty(const IRI& iri)
{
    OWLAnnotationProperty::Ptr property = mpOntology->mAnnotationProperties[iri];
    mpOntology->mAnnotationProperties.erase(iri);

    std::map<OWLAxiom::AxiomType, OWLAxiom::PtrList>::iterator it = mpOntology->mAxiomsByType.find(OWLAxiom::AnnotationAssertion);
    if(it == mpOntology->mAxiomsByType.end())
    {
        return;
    }

    OWLAxiom::PtrList& axioms = it->second;
    OWLAxiom::PtrList::iterator ait = std::find_if(axioms.begin(), axioms.end(), [iri](OWLAxiom::Ptr other)
            {
                return dynamic_pointer_cast<OWLAnnotationAssertionAxiom>(other)->getProperty()->getIRI() == iri;
            });
    if(ait != axioms.end())
    {
        axioms.erase(ait);
    }
}

OWLSubClassOfAxiom::Ptr OWLOntologyTell::subClassOf(const IRI& subclass, const OWLClassExpression::Ptr& superclass)
{
    OWLClass::Ptr e_subclass = klass(subclass);
    return subClassOf(e_subclass, superclass);
}

OWLSubClassOfAxiom::Ptr OWLOntologyTell::subClassOf(const IRI& subclass, const IRI& superclass)
{
    // All classes inherit from top concept, i.e. owl:Thing
    OWLClass::Ptr e_subclass = klass(subclass);
    if(mAsk.isOWLAnonymousClassExpression(superclass))
    {
        OWLAnonymousClassExpression::Ptr e_superclass = mAsk.getOWLAnonymousClassExpression(superclass);
        return subClassOf(e_subclass, dynamic_pointer_cast<OWLClassExpression>(e_superclass));

    } else {
        OWLClass::Ptr e_superclass = klass(superclass);
        return subClassOf(e_subclass, e_superclass);
    }
}

OWLSubClassOfAxiom::Ptr OWLOntologyTell::subClassOf(const OWLClass::Ptr& subclass, const OWLClass::Ptr& superclass)
{
    mpOntology->kb()->subClassOf(subclass->getIRI(), superclass->getIRI());
    return subClassOf(ptr_cast<OWLClassExpression,OWLClass>(subclass),
            ptr_cast<OWLClassExpression, OWLClass>(superclass));
}


OWLSubClassOfAxiom::Ptr OWLOntologyTell::subClassOf(const OWLClassExpression::Ptr& subclassExpression, const OWLClassExpression::Ptr& superclassExpression)
{
    OWLSubClassOfAxiom::Ptr axiom = make_shared<OWLSubClassOfAxiom>(subclassExpression, superclassExpression);
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

OWLAxiom::Ptr OWLOntologyTell::addAxiom(const OWLAxiom::Ptr& axiom, const reasoner::factpp::Axiom& kbAxiom)
{
    mpOntology->kb()->addReference(axiom, kbAxiom);
    addAxiom(axiom);
    return axiom;
}

OWLAxiom::Ptr OWLOntologyTell::equalClasses(const IRIList& klasses)
{
    OWLClassExpression::PtrList pKlasses;
    for(const IRI& classType : klasses)
    {
        OWLClassExpression::Ptr pKlass = klass(classType);
        pKlasses.push_back(pKlass);
    }
    OWLEquivalentClassesAxiom::Ptr axiom = make_shared<OWLEquivalentClassesAxiom>(pKlasses);
    reasoner::factpp::Axiom kb_axiom = mpOntology->kb()->equalClasses(klasses);
    addAxiom(axiom, kb_axiom);
    return axiom;
}

OWLAxiom::Ptr OWLOntologyTell::equalObjectProperties(const IRIList& properties)
{
    reasoner::factpp::Axiom kb_axiom = mpOntology->kb()->equalObjectProperties(properties);
    OWLObjectPropertyExpression::PtrList pProperties;
    for(const IRI& p : properties)
    {
        OWLObjectPropertyExpression::Ptr pProperty = objectProperty(p);
        pProperties.push_back(pProperty);
    }
    OWLEquivalentObjectPropertiesAxiom::Ptr axiom = make_shared<OWLEquivalentObjectPropertiesAxiom>(pProperties);
    addAxiom(axiom, kb_axiom);
    return axiom;
}

OWLAxiom::Ptr OWLOntologyTell::equalDataProperties(const IRIList& properties)
{
    reasoner::factpp::Axiom kb_axiom = mpOntology->kb()->equalDataProperties(properties);
    OWLDataPropertyExpression::PtrList pProperties;
    for(const IRI& p : properties)
    {
        OWLDataPropertyExpression::Ptr pProperty = dataProperty(p);
        pProperties.push_back(pProperty);
    }
    OWLEquivalentDataPropertiesAxiom::Ptr axiom = make_shared<OWLEquivalentDataPropertiesAxiom>(pProperties);
    addAxiom(axiom, kb_axiom);
    return axiom;
}

OWLAxiom::Ptr OWLOntologyTell::disjointObjectProperties(const IRIList& properties)
{
    reasoner::factpp::Axiom kb_axiom = mpOntology->kb()->disjointObjectProperties(properties);
    OWLObjectPropertyExpression::PtrList pProperties;
    for(const IRI& p : properties)
    {
        OWLObjectPropertyExpression::Ptr pProperty = objectProperty(p);
        pProperties.push_back(pProperty);
    }
    OWLDisjointObjectPropertiesAxiom::Ptr axiom = make_shared<OWLDisjointObjectPropertiesAxiom>(pProperties);
    addAxiom(axiom, kb_axiom);
    return axiom;
}

OWLAxiom::Ptr OWLOntologyTell::disjointDataProperties(const IRIList& properties)
{
    reasoner::factpp::Axiom kb_axiom = mpOntology->kb()->disjointDataProperties(properties);
    OWLDataPropertyExpression::PtrList pProperties;
    for(const IRI& p : properties)
    {
        OWLDataPropertyExpression::Ptr pProperty = dataProperty(p);
        pProperties.push_back(pProperty);
    }
    OWLDisjointDataPropertiesAxiom::Ptr axiom = make_shared<OWLDisjointDataPropertiesAxiom>(pProperties);
    addAxiom(axiom, kb_axiom);
    return axiom;
}

OWLAxiom::Ptr OWLOntologyTell::disjointClasses(const IRIList& klasses)
{
    reasoner::factpp::Axiom kb_axiom = mpOntology->kb()->disjoint(klasses, KnowledgeBase::CLASS);
    OWLClassExpression::PtrList pKlasses;
    for(const IRI& classType : klasses)
    {
        OWLClassExpression::Ptr pKlass = klass(classType);
        pKlasses.push_back(pKlass);
    }
    OWLDisjointClassesAxiom::Ptr axiom = make_shared<OWLDisjointClassesAxiom>(pKlasses);
    addAxiom(axiom, kb_axiom);
    return axiom;
}

OWLAxiom::Ptr OWLOntologyTell::disjointUnion(const IRI& unionClass, const IRIList& disjointKlasses)
{
    reasoner::factpp::Axiom kb_axiom = mpOntology->kb()->disjointUnion(unionClass, disjointKlasses);
    OWLClassExpression::PtrList pKlasses;
    for(const IRI& classType : disjointKlasses)
    {
        OWLClassExpression::Ptr pKlass = klass(classType);
        pKlasses.push_back(pKlass);
    }

    OWLDisjointUnionAxiom::Ptr axiom = make_shared<OWLDisjointUnionAxiom>(klass(unionClass), pKlasses);
    addAxiom(axiom, kb_axiom);
    return axiom;
}


OWLClassAssertionAxiom::Ptr OWLOntologyTell::instanceOf(const IRI& instance, const IRI& classType)
{
    // Update reasoner kb
    reasoner::factpp::Axiom kb_axiom = mpOntology->kb()->instanceOf(instance, classType);

    // ClassAssertion
    OWLNamedIndividual::Ptr e_individual = namedIndividual(instance);
    OWLClassExpression::Ptr e_class = klass(classType);
    OWLClassAssertionAxiom::Ptr axiom = make_shared<OWLClassAssertionAxiom>(e_individual, e_class);

    // Register
    mpOntology->mClassAssertionAxiomsByClass[e_class].push_back(axiom);
    mpOntology->mClassAssertionAxiomsByIndividual[e_individual].push_back(axiom);
    mpOntology->mNamedIndividualAxioms[e_individual].push_back(axiom);

    LOG_DEBUG_S << "NamedIndividual '" << instance << "' of class: '" << classType << "'";

    addAxiom( axiom, kb_axiom );

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
    bool isAnnotationProperty = mAsk.isAnnotationProperty(relation);
    if(isAnnotationProperty)
    {
        return annotationOf(subject, relation, object);
    }

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
        OWLObjectPropertyAssertionAxiom::Ptr axiom = make_shared<OWLObjectPropertyAssertionAxiom>(
                    individual,
                    mpOntology->getObjectProperty(relation),
                    assertionObject);
        return addAxiom(axiom);

    } else if(mAsk.isDataProperty(relation))
    {
        OWLDataPropertyAssertionAxiom::Ptr axiom = make_shared<OWLDataPropertyAssertionAxiom>(
                    individual,
                    mpOntology->getDataProperty(relation),
                    dynamic_pointer_cast<OWLLiteral>(assertionObject) );
        return addAxiom(axiom);
    } else {
        throw std::runtime_error("owlapi::model::OWLOntologyTell::relatedTo: "
                "'" + relation.toString() + "' is not a known relation/property");
    }
}

OWLSubPropertyAxiom::Ptr OWLOntologyTell::subPropertyOf(const IRI& subProperty, const IRI& parentProperty)
{

    OWLSubPropertyAxiom::Ptr axiom;
    if(mAsk.isObjectProperty(parentProperty))
    {
        objectProperty(subProperty);

        OWLObjectProperty::Ptr subOProperty = mpOntology->getObjectProperty(subProperty);
        OWLObjectProperty::Ptr superOProperty = mpOntology->getObjectProperty(parentProperty);

        axiom = make_shared<OWLSubObjectPropertyOfAxiom>(subOProperty, superOProperty);

        mpOntology->kb()->subPropertyOf(subProperty, parentProperty);
    } else if(mAsk.isDataProperty(parentProperty))
    {
        dataProperty(subProperty);

        OWLDataProperty::Ptr subDProperty = mpOntology->getDataProperty(subProperty);
        OWLDataProperty::Ptr superDProperty = mpOntology->getDataProperty(parentProperty);

        axiom = make_shared<OWLSubDataPropertyOfAxiom>(subDProperty, superDProperty);

        mpOntology->kb()->subPropertyOf(subProperty, parentProperty);

    } else if(mAsk.isAnnotationProperty(parentProperty))
    {
        OWLAnnotationProperty::Ptr subAProperty;
        try {
            subAProperty = mpOntology->getAnnotationProperty(subProperty);
        } catch(const std::invalid_argument& e)
        {
            subAProperty = annotationProperty(subProperty);
        }

        OWLAnnotationProperty::Ptr superAProperty;
        try {
            superAProperty = mpOntology->getAnnotationProperty(parentProperty);
        } catch(const std::invalid_argument& e)
        {
            superAProperty = annotationProperty(parentProperty);
        }

        axiom = make_shared<OWLSubAnnotationPropertyOfAxiom>(subAProperty, superAProperty);
    } else {
        throw std::runtime_error("OWLOntologyTell::subPropertyOf: could not identify property type for " +
                parentProperty.toString());
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
    OWLDataPropertyDomainAxiom::Ptr axiom = make_shared<OWLDataPropertyDomainAxiom>(e_dataProperty, domain);
    return addAxiom(axiom);
}

OWLAxiom::Ptr OWLOntologyTell::dataPropertyDomainOf(const IRI& property, const OWLClassExpression::Ptr& domain)
{
    // TODO: implement ClassExpression support
    //mpOntology->kb()->domainOf(property, classType, KnowledgeBase::DATA);

    OWLDataProperty::Ptr dataProperty = mpOntology->getDataProperty(property);

    OWLDataPropertyExpression::Ptr e_dataProperty = ptr_cast<OWLDataPropertyExpression, OWLDataProperty>(dataProperty);
    OWLDataPropertyDomainAxiom::Ptr axiom = make_shared<OWLDataPropertyDomainAxiom>(e_dataProperty, domain);
    return addAxiom(axiom);
}

OWLClassExpression::Ptr OWLOntologyTell::dataPropertyRestriction(const IRI& id, const OWLDataRestriction::Ptr& r)
{
    std::map<IRI, OWLAnonymousClassExpression::Ptr>::const_iterator cit =
        mpOntology->mAnonymousClassExpressions.find(iri);
    if(cit != mAnonymousClassExpressions.end())
    {
        return cit->second;
    }

    switch(r->getClassExpressionType())
    {
        case OWLClassExpression::DATA_SOME_VALUES_FROM:
        {
            OWLDataSomeValuesFrom::Ptr someValuesFrom = dynamic_pointer_cast<OWLDataSomeValuesFrom>(r);

            OWLPropertyExpression::Ptr property = dynamic_pointer_cast<OWLDataRestriction>(someValuesFrom)->getProperty();
            if(!property)
            {
                throw
                    std::invalid_argument("owlapi::model::OWLOntologyTell::dataPropertyRestriction:"
                            " could not extract property from SomeValuesFrom"
                            " Restriction ");
            }
            OWLDataProperty::Ptr dataProperty = dynamic_pointer_cast<OWLDataProperty>(property);
            if(!dataProperty)
            {
                throw
                    std::invalid_argument("owlapi::model::OWLOntologyTell::dataPropertyRestriction:"
                            " failed to identify data property for restriction '"
                            + id.toString() + "'" );
            }

            IRI dataPropertyIRI = mpOntology->iriOfDataProperty(dataProperty);
            OWLDataRange::Ptr filler = someValuesFrom->getFiller();
            OWLDataTypeRestriction::Ptr dataTypeRestriction = dynamic_pointer_cast<OWLDataTypeRestriction>(filler);
            if(!dataTypeRestriction)
            {
                throw std::invalid_argument("owlapi::model::OWLOntologyTell::dataPropertyRestriction:"
                        "failed to extract data type restriction");
            }


            mpOntology->kb()->dataSomeValuesFrom(id, dataPropertyIRI, dataTypeRestriction);
            break;
        }
        case OWLClassExpression::DATA_ALL_VALUES_FROM:
            break;
        case OWLClassExpression::DATA_HAS_VALUE:
            break;
        case OWLClassExpression::DATA_MIN_CARDINALITY:
            break;
        case OWLClassExpression::DATA_MAX_CARDINALITY:
            break;
        case OWLClassExpression::DATA_EXACT_CARDINALITY:
            break;
        default:
            break;
    }

    mpOntology->mAnonymousClassExpressions[id] = r;
}

OWLAxiom::Ptr OWLOntologyTell::dataPropertyRangeOf(const IRI& property, const IRI& classType)
{
    // cannot use the following since that is not implemented in the reasoner
    // mpOntology->kb()->rangeOf(relation, classType, KnowledgeBase::OBJECT);

    OWLDataProperty::Ptr dataProperty = mpOntology->getDataProperty(property);

    std::map<IRI, OWLDataRange::PtrList>::const_iterator cit = mpOntology->mAnonymousDataRanges.find(classType);
    if(cit != mpOntology->mAnonymousDataRanges.end())
    {
        OWLDataRange::PtrList ranges = cit->second;
        for(const OWLDataRange::Ptr& range : ranges)
        {
            dataProperty->addRange(range);
            if(range->getDataRangeType() == OWLDataRange::ONE_OF)
            {
                OWLDataOneOf::Ptr oneOf = dynamic_pointer_cast<OWLDataOneOf>(range);
                reasoner::factpp::DataRange range = mpOntology->kb()->dataOneOf(oneOf->getLiterals());
                mpOntology->kb()->dataRangeOf(property, range);
                return addAxiom( make_shared<OWLDataPropertyRangeAxiom>(
                            dataProperty,
                            oneOf) );
            }
        }
        return OWLAxiom::Ptr();
    } else{
        OWLDataRange::Ptr range = make_shared<OWLDataType>(classType);
        dataProperty->addRange(range);
        OWLDataPropertyExpression::Ptr e_dataProperty = dynamic_pointer_cast<OWLDataPropertyExpression>(dataProperty);
        OWLDataPropertyRangeAxiom::Ptr axiom = make_shared<OWLDataPropertyRangeAxiom>(e_dataProperty, range);
        return addAxiom(axiom);
    }
}

OWLAxiom::Ptr OWLOntologyTell::objectPropertyDomainOf(const IRI& relation, const IRI& classType)
{
    mpOntology->kb()->domainOf(relation, classType, KnowledgeBase::OBJECT);

    OWLObjectProperty::Ptr oProperty = mpOntology->getObjectProperty(relation);
    OWLClassExpression::Ptr klass = mpOntology->getClass(classType);

    OWLObjectPropertyDomainAxiom::Ptr axiom = make_shared<OWLObjectPropertyDomainAxiom>(oProperty, klass);
    return addAxiom(axiom);
}

OWLAxiom::Ptr OWLOntologyTell::objectPropertyRangeOf(const IRI& relation, const IRI& classType)
{
    if(mAsk.isDatatype(classType))
    {
        throw std::invalid_argument("owlapi::model::OWLOntologyTell::objectPropertyRangeOf: "
                "cannot use datarange '" + classType.toString() + "' for object property "
                + relation.toString());
    }
    mpOntology->kb()->objectRangeOf(relation, classType);

    OWLObjectProperty::Ptr oProperty = mpOntology->getObjectProperty(relation);
    OWLClassExpression::Ptr klass = mpOntology->getClass(classType);

    OWLObjectPropertyRangeAxiom::Ptr axiom = make_shared<OWLObjectPropertyRangeAxiom>(oProperty, klass);
    return addAxiom(axiom);
}


//OWLAxiom::Ptr OWLOntologyTell::annotationPropertyDomainOf(const IRI& property, const IRI& classType)
//{
//    mpOntology->kb()->domainOf(property, classType, KnowledgeBase::DATA);
//
//    OWLDataProperty::Ptr dataProperty = mpOntology->getDataProperty(property);
//    OWLClassExpression::Ptr domain = mpOntology->getClass(classType);
//
//    OWLDataPropertyExpression::Ptr e_dataProperty = ptr_cast<OWLDataPropertyExpression, OWLDataProperty>(dataProperty);
//    OWLDataPropertyDomainAxiom::Ptr axiom = make_shared<OWLDataPropertyDomainAxiom>(e_dataProperty, domain);
//    return addAxiom(axiom);
//}
//
//OWLAxiom::Ptr OWLOntologyTell::dataPropertyDomainOf(const IRI& property, const OWLClassExpression::Ptr& domain)
//{
//    // TODO: implement ClassExpression support
//    //mpOntology->kb()->domainOf(property, classType, KnowledgeBase::DATA);
//
//    OWLDataProperty::Ptr dataProperty = mpOntology->getDataProperty(property);
//
//    OWLDataPropertyExpression::Ptr e_dataProperty = ptr_cast<OWLDataPropertyExpression, OWLDataProperty>(dataProperty);
//    OWLDataPropertyDomainAxiom::Ptr axiom = make_shared<OWLDataPropertyDomainAxiom>(e_dataProperty, domain);
//    return addAxiom(axiom);
//}

OWLAxiom::Ptr OWLOntologyTell::annotationPropertyRangeOf(const IRI& property,
        const IRI& rangeIRI)
{
    // cannot use the following since that is not implemented in the reasoner
    // mpOntology->kb()->rangeOf(relation, classType, KnowledgeBase::OBJECT);

    OWLAnnotationProperty::Ptr annotationProperty = mpOntology->getAnnotationProperty(property);
    //annotationProperty->addRange(rangeIRI)
    OWLAnnotationPropertyRangeAxiom::Ptr axiom =
            make_shared<OWLAnnotationPropertyRangeAxiom>(annotationProperty, rangeIRI);
    return addAxiom(axiom);
}

OWLAxiom::Ptr OWLOntologyTell::inverseOf(const IRI& relation, const IRI& inverseRelation)
{
    if( mAsk.isObjectProperty(relation) || mAsk.isObjectProperty(inverseRelation) )
    {
        mpOntology->kb()->inverseOf(relation, inverseRelation);

        OWLObjectProperty::Ptr first = mpOntology->getObjectProperty(relation);
        OWLObjectProperty::Ptr second = mpOntology->getObjectProperty(inverseRelation);

        OWLInverseObjectPropertiesAxiom::Ptr axiom = make_shared<OWLInverseObjectPropertiesAxiom>(first, second);
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

    OWLIndividual::Ptr individual;
    if(mAsk.isOWLIndividual(instance) || mAsk.isOWLAnonymousIndividual(instance))
    {
        individual = mAsk.getOWLIndividual(instance);
    } else {
        individual = anonymousIndividual(instance);
    }
    OWLDataProperty::Ptr property = mAsk.getOWLDataProperty(dataProperty);
    OWLDataPropertyAssertionAxiom::Ptr axiom = make_shared<OWLDataPropertyAssertionAxiom>(individual, property, literal);

    mpOntology->retractValueOf(individual, property);
    return addAxiom(axiom);
}


OWLAxiom::Ptr OWLOntologyTell::annotationOf(const IRI& subject,
        const IRI& relation,
        const IRI& object)
{
        OWLAnnotationSubject::Ptr annotationSubject = make_shared<IRI>(subject);
        // Setting of AnnotationAssertions
        std::string value = object.toString();
        if(value.empty())
        {
            LOG_WARN_S << "Encountered empty property value for subject: " << annotationSubject
                << " and relation: " << relation;
            throw
                std::invalid_argument("owlapi::model::OWLOntologyTell::annotationOf:"
                        " Encountered empty property value for subject '"
                        + subject.toString() +
                        "' and relation '" +
                        relation.toString() + "'"
                    );
        }

        try {
            shared_ptr<IRI> annotationObjectIRI = make_shared<IRI>(value);
            // TODO: improve checking for IRI
            annotationObjectIRI->toURI();
            return annotationOf(annotationSubject, relation, annotationObjectIRI);
        } catch(const std::invalid_argument& e)
        {
            LOG_INFO_S << "AnnotationValue is not an IRI (currently limited to URI checking)";
        }

        OWLLiteral::Ptr literal = OWLLiteral::create(value);
        if(!literal->isTyped())
        {
            // check if range type is known
            //OWLDataType dataType = OWLDataType::fromRange(,value);
            literal = OWLLiteral::create(object.toString());
        }
        return annotationOf(annotationSubject, relation, literal);
}

OWLAxiom::Ptr OWLOntologyTell::annotationOf(const OWLAnnotationSubject::Ptr& subject,
        const IRI& annotationProperty,
        const OWLAnnotationValue::Ptr& annotationValue)
{
    OWLAnnotationProperty::Ptr property = mAsk.getOWLAnnotationProperty(annotationProperty);
    OWLAnnotationAssertionAxiom::Ptr axiom = make_shared<OWLAnnotationAssertionAxiom>(subject, property, annotationValue);
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

    mpOntology->kb()->dataType(iri);
}

void OWLOntologyTell::dataOneOf(const IRI& id, const IRIList& iris)
{
    OWLDataOneOf::Ptr dataRange =
        make_shared<OWLDataOneOf>(iris);
    mpOntology->mAnonymousDataRanges[id].push_back(dataRange);
}

void OWLOntologyTell::dataTypeRestriction(const IRI& id, const OWLDataTypeRestriction::Ptr& restriction)
{
    reasoner::factpp::DataRange range = mpOntology->kb()->dataTypeRestriction(restriction);
    mpOntology->mAnonymousDataRanges[id].push_back(restriction);
}

void OWLOntologyTell::removeIndividual(const IRI& iri)
{
    mpOntology->retractIndividual(iri);
}

} // end namespace model
} // end namespace owlapi
