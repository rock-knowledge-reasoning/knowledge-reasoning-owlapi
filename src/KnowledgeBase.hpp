#ifndef OWLAPI_KNOWLEDGE_BASE_HPP
#define OWLAPI_KNOWLEDGE_BASE_HPP

#include <string.h>
#include <map>
#include <vector>
#include <base-logging/Logging.hpp>
#include <factpp/Actor.h>
#include "OWLApi.hpp"
#include "reasoner/factpp/Types.hpp"
#include "model/OWLDataRange.hpp"
#include "model/OWLObjectOneOf.hpp"

namespace owlapi {

namespace representation {
    enum Type { UNKNOWN, LISP };
}

//! Restriction types
namespace restriction {
    enum Type { SELF, VALUE, EXISTS, FORALL, MIN_CARDINALITY, MAX_CARDINALITY, EXACT_CARDINALITY };
}

// Internationalized Resource Identifier -- simply using a string here for simplicity
typedef owlapi::model::IRI IRI;
typedef owlapi::model::IRIList IRIList;
typedef owlapi::model::IRISet IRISet;

typedef std::map<IRI, reasoner::factpp::ClassExpression > IRIClassExpressionMap;
typedef std::map<IRI, reasoner::factpp::InstanceExpression > IRIInstanceExpressionMap;
typedef std::map<IRI, reasoner::factpp::ObjectPropertyExpression > IRIObjectPropertyExpressionMap;
typedef std::map<IRI, reasoner::factpp::DataPropertyExpression > IRIDataPropertyExpressionMap;
typedef std::map<IRI, reasoner::factpp::DataTypeName > IRIDataTypeMap;
typedef std::map< std::pair<IRI, IRI>, reasoner::factpp::Axiom::List> DataValueMap;
typedef std::map<owlapi::model::OWLDataRange::Ptr, reasoner::factpp::DataRange> DataRangeMap;

typedef std::map<owlapi::model::OWLAxiom::Ptr, reasoner::factpp::Axiom::List> ReferencedAxiomsMap;

enum Representation { UNKNOWN = 0, LISP = 1 };

/**
 * \class KnowledgeBase
 * \brief This class represent the core class that manages OWL based
 * information
 * \details In its current form the KnowledgeBase wrap the Fact++ Reasoner
 * in order to provide its main functionality
 */
class KnowledgeBase
{
    ReasoningKernel* mKernel;

    IRIInstanceExpressionMap mInstances;
    IRIClassExpressionMap mClasses;
    IRIObjectPropertyExpressionMap mObjectProperties;
    IRIDataPropertyExpressionMap mDataProperties;
    IRIDataTypeMap mDataTypes;
    DataRangeMap mDataRanges;

    DataValueMap mValueOfAxioms;
    ReferencedAxiomsMap mReferencedAxiomsMap;

    bool hasClass(const IRI& klass) const { return mClasses.count(klass); }

    bool hasInstance(const IRI& instance) const { return mInstances.count(instance); }

    IRIList getResult(const Actor& actor, const IRI& filter = IRI()) const;

public:
    typedef shared_ptr<KnowledgeBase> Ptr;

    enum PropertyType { UNKNOWN_PROPERTY_TYPE, OBJECT, DATA, END_PROPERTY_TYPE };
    enum EntityType { UNKNOWN_ENTITY_TYPE, CLASS, INSTANCE, OBJECT_PROPERTY, DATA_PROPERTY, END_ENTITY_TYPE };

    static std::map<PropertyType, std::string> PropertyTypeTxt;
    static std::map<EntityType, std::string> EntityTypeTxt;

    KnowledgeBase();
    ~KnowledgeBase();

    ReasoningKernel* getReasoningKernel() { return mKernel; }

    TExpressionManager* getExpressionManager();

    const TExpressionManager* getExpressionManager() const;


    /**
     * Activate verbose output
     */
    void setVerbose();

    /**
     * Refresh status after adding new updates
     * \throw std::exception if the data base is inconsistent
     */
    void refresh();

    bool isConsistent();

    void classify();

    void realize();

    bool isRealized();

    bool isClassSatisfiable(const IRI& klass);

    // ROLES (PROPERTIES)
    /**
     * Make an object property known
     */
    reasoner::factpp::ObjectPropertyExpression objectProperty(const IRI& property);
    /**
     * Make a data property known
     */
    reasoner::factpp::DataPropertyExpression dataProperty(const IRI& property);
    /**
     * Define a transitive object property
     */
    reasoner::factpp::Axiom transitiveProperty(const IRI& property);
    /**
     * Test if object property is transitive
     */
    bool isTransitiveProperty(const IRI& property);
    /**
     * Define a functional object/data property
     */
    reasoner::factpp::Axiom functionalProperty(const IRI& property, PropertyType type = OBJECT);
    /**
     * Test if property is functional (can be either data or object property)
     */
    bool isFunctionalProperty(const IRI& property);
    /**
     * Define an inverse functional object/data property
     */
    reasoner::factpp::Axiom inverseFunctionalProperty(const IRI& property);
    /**
     * Test if property is inverse functional
     */
    bool isInverseFunctionalProperty(const IRI& property);
    /**
     * Define a reflexive object property
     */
    reasoner::factpp::Axiom reflexiveProperty(const IRI& property);
    /**
     * Test if property is reflexive
     */
    bool isReflexiveProperty(const IRI& property);
    /**
     * Define an irreflexive object property
     */
    reasoner::factpp::Axiom irreflexiveProperty(const IRI& property);
    /**
     * Test if property is irreflexive
     */
    bool isIrreflexiveProperty(const IRI& property);
    /**
     * Define a symmetric object property
     */
    reasoner::factpp::Axiom symmetricProperty(const IRI& property);
    /**
     * Test if property is symmetric
     */
    bool isSymmetricProperty(const IRI& property);
    /*
     * Define an asymmetric object property
     */
    reasoner::factpp::Axiom asymmetricProperty(const IRI& property);
    /**
     * Test if property is symmetric
     */
    bool isAsymmetricProperty(const IRI& property);

    // CONCEPTS
    /**
     * Define a subclass relationship between two concepts / classes
     * \param subclass IRI of subclass
     * \param parentClass IRI of parentClass
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom subClassOf(const IRI& subclass, const IRI& parentClass);

    /**
     * Define a subclass relationship between two concepts / classes
     * \param subclass expression for subclass
     * \param parentClass
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom subClassOf(const reasoner::factpp::ClassExpression& subclass, const IRI& parentClass);

    /**
     * Define a subclass relationship between two concepts / classes
     * \param subclass
     * \param parentClass expression for parentClass, e.g. for restrictions
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom subClassOf(const IRI& subclass, const reasoner::factpp::ClassExpression& parentClass);

    reasoner::factpp::Axiom equals(const IRI& klass,
            const IRI& otherKlass);

    reasoner::factpp::Axiom equals(const reasoner::factpp::ClassExpression& klass,
            const reasoner::factpp::ClassExpression& otherklass);

    /**
     * Define intersection of classes under a given alias
     * \param klass A concept identifier
     * \param otherKlass Another's concept identifier
     * \return corresponding anonymous class expression
     */
    reasoner::factpp::ClassExpression intersectionOf(const IRI& klass, const IRI& otherKlass);

    reasoner::factpp::ClassExpression intersectionOf(const reasoner::factpp::ClassExpression& klass, const reasoner::factpp::ClassExpression& otherKlass);

    /**
     * Define intersection of classes under a given alias
     * \param klasses List of concept identifiers to compute the intersection
     * from
     * \return corresponding anonymous class expression
     */
    reasoner::factpp::ClassExpression intersectionOf(const IRIList& klasses);

    /**
     * Define intersection of classes under a given alias
     * \param klasses List of concept expressions to compute the intersection
     * from
     * \return corresponding anonymous class expression
     */
    reasoner::factpp::ClassExpression intersectionOf(const std::vector<reasoner::factpp::ClassExpression>& klasses);


    /**
     * Define disjunction of classes under a given alias
     * \param klass A concept identifier
     * \param otherKlass Another's concept identifier
     * \return corresponding anonymous class expression
     */
    reasoner::factpp::ClassExpression disjunctionOf(const IRI& klass, const IRI& otherKlass);

    /**
     * Define disjunction of classes under a given alias
     * \param klasses List of concept identifiers
     * \return corresponding anonymous class expression
     */
    reasoner::factpp::ClassExpression disjunctionOf(const IRIList& klasses);

    /**
     * Define intersection of classes under a given alias
     * \param alias Equivalent concept the intersection of classes
     * \param expression Expression decribing the class
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom intersectionOf(const IRI& alias, const reasoner::factpp::ClassExpression& expression);

    /**
     * Define equivalent klasses
     * \param list of klasses that are equivalent
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom equalClasses(const IRIList& klasses);

    reasoner::factpp::Axiom equalObjectProperties(const IRIList& properties);

    reasoner::factpp::Axiom equalDataProperties(const IRIList& properties);

    /**
     * Define an alias for a given class expression
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom alias(const IRI& alias, const reasoner::factpp::ClassExpression& expression);

    /**
     * Define an instance alias
     * \param alias Alias of instance
     * \param iri Name of entity that should be aliases
     * \param type Entity type that should be aliases
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom alias(const IRI& alias, const IRI& iri, EntityType type);

    /**
     * Define classes to be disjoint
     * \param klassOrInstance One concept or instance identifier
     * \param otherKlassOrInstance concept or instance identifier
     * \param type Type of entity
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom disjoint(const IRI& klassOrInstance, const IRI& otherKlassOrInstance, EntityType type);

    /**
     * Define classes to be disjoint / or instance
     * \param klassesOrInstances List of concepts or instances that will be declared disjoint
     * \param type Type of entity
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom disjoint(const IRIList& klassesOrInstances, EntityType type);

    /**
     * Define classes to be disjoint
     * \param klasses List of concepts that are declared disjoint
     * \return axiom
     */
    reasoner::factpp::Axiom disjointClasses(const IRIList& klasses) { return disjoint(klasses, CLASS); }

    /**
     * Define object property to be disjoint / or instance
     * \param properties List of properties that will be declared disjoint
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom disjointObjectProperties(const IRIList& properties);

    /**
     * Define data property to be disjoint / or instance
     * \param properties List of properties that will be declared disjoint
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom disjointDataProperties(const IRIList& properties);

    /**
     * Define a klass which represents the disjoint union of a set of classes
     * \param klass The klass name for the union
     * \param disjointKlasses List of disjoint classes
     * \see https://www.w3.org/TR/owl2-syntax/#Disjoint_Union_of_Class_Expressions
     * \verbatim
        DisjointUnion(a:Child a:Boy a:Girl)
     * \endverbatim
     */
    reasoner::factpp::Axiom disjointUnion(const IRI& klass, const IRIList&
            disjointKlasses);

    /**
     * Define an instance of a concept / class
     * \param instance IRI of the instance which shall be created
     * \param klass Class type of the instance to be created
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom instanceOf(const IRI& instance, const IRI& klass);

    /**
     * Define an instance of a concept / class
     * \param instance IRI of the instance which shall be created
     * \param e_class Class type of the instance to be created
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom instanceOf(const IRI& individual, const reasoner::factpp::ClassExpression& e_class);

    /**
     * Define an object relation between two instances / individuals
     * \param instance
     * \param relationProperty the property that related both instances
     * \param otherInstance
     * \param isTrue Set to true if relationship is true, false if is is false
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom relatedTo(const IRI& instance, const IRI& relationProperty, const IRI& otherInstance, bool isTrue = true);

    /**
     * Define negated an object relation between two instances / individuals
     * \param instance
     * \param relationProperty the property that related both instances
     * \param otherInstance
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom notRelatedTo(const IRI& instance, const IRI& relationProperty, const IRI& otherInstance);


    /**
     * Define an object relation / role and its domain
     * \param property The property for which the domain shall be specified
     * \param domain Domain of this property
     * \param propertyType Type of the property (Data, Object)
     */
    reasoner::factpp::Axiom domainOf(const IRI& property, const IRI& domain, PropertyType propertyType);

    /**
     * Define an object relation / role and its range
     * \param property The property for which the range shall be specified
     * \param range Range type for this relation
     * \param propertyType Type of the property (Data, Object)
     */
    reasoner::factpp::Axiom objectRangeOf(const IRI& property,
            const IRI& range);

    /**
     * Define an object relation / role and its range
     * \param property The property for which the range shall be specified
     * \param range type for this relation
     * \param propertyType Type of the property (Data, Object)
     */
    reasoner::factpp::Axiom objectRangeOf(const IRI& property,
            const reasoner::factpp::ClassExpression& expression);

    /**
     * Define a data relation / role and its range
     * \param property The data property for which the range shall be specified
     * \param range Data range type for
     */
    reasoner::factpp::Axiom dataRangeOf(const IRI& property,
            reasoner::factpp::DataRange& range);

    /**
     * Define associated value of an individual
     * \param individual The IRI of the individual
     * \param property The property for which a value shall be set
     * \param dataValue The data value to set
     * \return axiom representing the valueOf assertion
     */
    reasoner::factpp::Axiom valueOf(const IRI& individual, const IRI& property, const reasoner::factpp::DataValue& dataValue);

    reasoner::factpp::Axiom valueOf(const IRI& individual, const IRI& property, const owlapi::model::OWLLiteral::Ptr& literal);

    /**
     * Get all known datatypes
     * \return Mapping of IRI to datatypes
     */
    std::map<IRI, reasoner::factpp::DataTypeName> dataTypes() const { return mDataTypes; }

    /**
     * Define the associated datatype (or retrieve the existing)
     * \return datatype
     */
    reasoner::factpp::DataTypeName dataType(const IRI& dataType);

    /**
     * Define a dataType using a value and the corresponding datatype
     * \param value The 'raw' string representation as found in a serialized
     * ontology
     * \param dataType The datatype of the value
     * \return A datavalue as tuple of string and datatype
     */
    reasoner::factpp::DataValue dataValue(const std::string& value, const std::string& dataType);

    /**
     * Define a dataType using an OWLLiteral
     * \param literal The owl literal
     * \return A datavalue as tuple of string and datatype
     */
    reasoner::factpp::DataValue dataValue(const shared_ptr<owlapi::model::OWLLiteral>& literal);

    reasoner::factpp::DataRange dataRange(const shared_ptr<owlapi::model::OWLDataRange>& range);

    /**
     * Resolve a class expression with corresponding id (anonymous) or klass name
     */
    reasoner::factpp::ClassExpression classExpression(const IRI& expressionId);

    /**
     * Define one of a class relationship
     * (enumerated classes): Classes can be described by enumeration of the individuals that make up the class. The members of the class are exactly the set of enumerated individuals; no more, no less. For example, the class of daysOfTheWeek can be described by simply enumerating the individuals Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday. From this a reasoner can deduce the maximum cardinality (7) of any property that has daysOfTheWeek as its allValuesFrom
     * restriction.
     *
     * \param instanceList list of classes
     * \return corresponding class expression
     */
    reasoner::factpp::ClassExpression objectOneOf(
            const IRI& id,
            const IRIList& instanceList
    );

    void objectOneOf(const IRI& id, const shared_ptr<owlapi::model::OWLObjectOneOf>& oneOf);
    /**
     * Define the union (Or) of a set of klasses
     */
    reasoner::factpp::ClassExpression objectUnionOf(
            const IRI& id,
            const IRIList& klassList
    );

    /**
     * Define the intersection (And) of a set of klasses
     */
    reasoner::factpp::ClassExpression objectIntersectionOf(
            const IRI& id,
            const IRIList& klassList
    );

    reasoner::factpp::ClassExpression objectComplementOf(
            const IRI& id,
            const IRI& klass
    );

    reasoner::factpp::ClassExpression objectSomeValuesFrom(
            const IRI& klassId,
            const IRI& property,
            const IRI& expressionId
    );

    reasoner::factpp::ClassExpression objectAllValuesFrom(
            const IRI& klassId,
            const IRI& property,
            const IRI& expressionId
    );

    reasoner::factpp::ClassExpression objectHasValue(
            const IRI& klassId,
            const IRI& property,
            const IRI& individual
    );

    /**
     * Define a oneOf relationship
     */
    reasoner::factpp::DataRange dataOneOf(const owlapi::model::OWLDataOneOf::Ptr& oneOf);

    reasoner::factpp::DataRange dataOneOf(const owlapi::model::OWLLiteral::PtrList& literals);

    /**
     * Define a dataTypeRestriction
     */
    reasoner::factpp::DataRange dataTypeRestriction(const owlapi::model::OWLDataTypeRestriction::Ptr& restriction);


    reasoner::factpp::ClassExpression dataSomeValuesFrom(
            const IRI& klassId,
            const IRI& property,
            const owlapi::model::OWLDataRange::Ptr& restriction
    );

    reasoner::factpp::ClassExpression dataAllValuesFrom(
            const IRI& klassId,
            const IRI& property,
            const owlapi::model::OWLDataRange::Ptr& restriction
    );

    reasoner::factpp::ClassExpression dataHasValue(
            const IRI& klassId,
            const IRI& property,
            const owlapi::model::OWLLiteral::Ptr& literal
    );

    /**
     * Define inverse of a given object property
     * \param baseProperty A property
     * \param inverseProperty The corresponding inverse property
     */
    reasoner::factpp::Axiom inverseOf(const IRI& baseProperty, const IRI& inverseProperty);


    /// PROPERTIES
    /**
     * Add a (potentially not existing) subproperty to an existing parent property
     * Create the subproperty instance if it does not yet exist
     * \throws std::invalid_argument if parent property does not exist
     * \return corresponding axiom
     */
    reasoner::factpp::Axiom subPropertyOf(const IRI& subproperty, const IRI& parentProperty);

    /**
     * Add an object subproperty to an existing parent property
     */
    reasoner::factpp::Axiom subObjectPropertyOf(const IRI& subproperty, const reasoner::factpp::ObjectPropertyExpression& parentProperty);

    /**
     * Add a data subproperty to an existing parent property
     */
    reasoner::factpp::Axiom subDataPropertyOf(const IRI& subproperty, const reasoner::factpp::DataPropertyExpression& parentProperty);

    /**
     * Adds an object property restriction
     * \param type Type, e.g., depending of existential or universal quantifier is required
     * \param relationProperty the property to which the restriction will refer
     * \param klassOrInstance property value referring to the qualificaiton of
     * this restriction
     * \param cardinality if cardinality type is selected this defines the min,max or exact cardinality to use
     * \return corresponding class expression
     */
    reasoner::factpp::ClassExpression objectPropertyRestriction(
            restriction::Type type,
            const IRI& relationProperty,
            const IRI& klassOrInstance,
            int cardinality = -1
    );

    reasoner::factpp::ClassExpression objectMinCardinality(const IRI& klassId,
            size_t n,
            const IRI& property,
            const IRI& qualification = vocabulary::OWL::Thing()
    );

    reasoner::factpp::ClassExpression objectMaxCardinality(const IRI& klassId,
            size_t n,
            const IRI& property,
            const IRI& qualification = vocabulary::OWL::Thing()
    );

    reasoner::factpp::ClassExpression objectExactCardinality(const IRI& klassId,
            size_t n,
            const IRI& property,
            const IRI& qualification = vocabulary::OWL::Thing()
    );

    reasoner::factpp::ClassExpression dataMinCardinality(const IRI& klassId,
            size_t n,
            const IRI& property,
            const owlapi::model::OWLDataRange::Ptr& range
    );

    reasoner::factpp::ClassExpression dataMaxCardinality(const IRI& klassId,
            size_t n,
            const IRI& property,
            const owlapi::model::OWLDataRange::Ptr& range
    );

    reasoner::factpp::ClassExpression dataExactCardinality(const IRI& klassId,
            size_t n,
            const IRI& property,
            const owlapi::model::OWLDataRange::Ptr& range
    );

    /**
     * Test if class / concept is a subclass of another
     */
    bool isSubClassOf(const IRI& subclass, const IRI& parentClass);
    /**
     * Test if class expression defines subclass of another
     */
    bool isSubClassOf(const reasoner::factpp::ClassExpression& subclass, const IRI& parentClass);
    /**
     * Test if an instance is of the same type as other instance
     */
    bool isSameClass(const IRI& instance, const IRI& otherInstance) { return typeOf(instance) == typeOf(otherInstance); }

    /**
     * Test if class is equivalent to another
     */
    bool isEquivalentClass(const IRI& klass0, const IRI& klass1);

    /**
     * Test if class is disjoint with another
     */
    bool isDisjointClass(const IRI& klass0, const IRI& klass1);

    /**
     * Test if instance is type of a class
     * \param instance Instance identifier
     * \param klass Class identifier
     * \return true if instance is of given klass type, false otherwise
     */
    bool isInstanceOf(const IRI& instance, const IRI& klass);

    /**
     * Test if instances are related via given a given property
     * \param instance Instance identifier
     * \param relationProperty relation identifier:
     * \param otherInstance Identifier of the related instance
     */
    bool isRelatedTo(const IRI& instance, const IRI& relationProperty, const IRI& otherInstance);

    /**
     * Test if instances are referring to the same
     * \param instance Instance IRI
     * \param otherInstance Another IRI for checking
     * \return true upon success, false otherwise
     */
    bool isSameInstance(const IRI& instance, const IRI& otherInstance) const;

    /**
     * Test if given IRI refers to a datatype
     */
    bool isDatatype(const IRI& name) const;

    /**
     * Get ClassExpression for given IRI if it exists
     * \param klass
     * \throw std::invalid_argument if class does not exist
     */
    reasoner::factpp::ClassExpression getClass(const IRI& klass) const;

    /**
     * Get existing ClassExpression for given IRI or initialize if it does not exist yet
     * \param klass
     */
    reasoner::factpp::ClassExpression getClassLazy(const IRI& klass);

    /**
     * Retrieve all known classes
     * \return list of all classes
     */
    IRIList allClasses(bool excludeBottomClass = true) const;

    /**
     * Retrieve all subclasses of a given klass
     * \return list of all subclasses
     */
    IRIList allSubClassesOf(const IRI& klass, bool direct = false);

    /**
     * Retrieve all ancestors of a given klass
     * \return list of all ancestors
     */
    IRIList allAncestorsOf(const IRI& klass, bool direct = false);

    /**
     * Retrieve all equivalent classes
     * \param klass
     * \return list of all equivalent classes
     */
    IRIList allEquivalentClasses(const IRI& klass);

    /**
     * Retrieve all disjoint class
     * \param klass Reference class for this query
     * \return list of classes that are disjoint with the given one
     */
    IRIList allDisjointClasses(const IRI& klass);

    /**
     * Retrieve instance  and perform lazy initialization if requested
     * \param instance
     * \throw if the instance cannot be found and will not be lazily initialized
     * \return InstanceExpression for that instance
     */
    reasoner::factpp::InstanceExpression getInstance(const IRI& instance) const;

    /**
     * Get existing InstanceExpression for a given IRI or initialize if it does not exist yet
     * \param instance Identifer of instance
     * \return InstanceExpression for that instance
     */
    reasoner::factpp::InstanceExpression getInstanceLazy(const IRI& instance);

    reasoner::factpp::ObjectPropertyExpression getObjectProperty(const IRI& property) const;
    reasoner::factpp::ObjectPropertyExpression getObjectPropertyLazy(const IRI& property);

    /**
     * Get property domain
     * \param property Name of property
     * \param direct If direct is true, an exact domain is retrieved if available, otherwise the most specific concept set
     * default is true
     */
    IRIList getObjectPropertyDomain(const IRI& property, bool direct = true) const;

    /**
     * Get property range
     * \param property Name of property
     * \param direct If direct is true, an exact range is retrieved if available, otherwise the most specific concept set
     * default is true
     */
    IRIList getObjectPropertyRange(const IRI& property, bool direct = true) const;

    /**
     * Get data property
     * \param property Name of property
     * \throw std::invalid_argument when property is not known
     */
    reasoner::factpp::DataPropertyExpression getDataProperty(const IRI& property) const;

    /**
     * Get data property or register if it is unknown
     * \param property Name of property
     */
    reasoner::factpp::DataPropertyExpression getDataPropertyLazy(const IRI& property);

    /**
     * Retrieve all known instances
     * \return list of all instances
     */
    IRIList allInstances() const;

    /**
     * Retrieve all known instance of a given klass type
     * \param klass Name of klass
     * \param direct Set to true if only direct instances of the given class shall be retrieved
     * \return list of all known instances
     */
    IRIList allInstancesOf(const IRI& klass, bool direct = false);

    /**
     * Retrieve all related instances regarding a single instance and a selected property
     * which have a certain klass type
     * \param instance Instance name
     * \param relationProperty the relation property
     * \param klass Klass type the relation should map to
     * \return list of instances that are related to instance via the relationProperty
     */
    IRIList allRelatedInstances(const IRI& instance, const IRI& relationProperty, const IRI& klass = IRI());

    /**
     * Retrieve the first found related instance regarding a single instance and a selected property
     * \param instance Instance name
     * \param relationProperty
     * \param klass Klass type the relation should map to
     * \return instance that is related to instance via the relationProperty
     */
    IRI relatedInstance(const IRI& instance, const IRI& relationProperty, const IRI& klass = IRI());

    /**
     * Retrieve all instances regarding a single instance and a selected property
     * \param instance Instance where this relation is bound to
     * \param relationProperty Property for this relation (which will be inverted)
     * \param klass Optional filter klass for the result list, check if result is instance of given klass
     * \return list of instance that are related to instance via the inverse of the given
     * relationProperty
     */
    IRIList allInverseRelatedInstances(const IRI& instance, const IRI& relationProperty, const IRI& klass = IRI());

    /**
     * Test if given property is a subproperty of a given parent property
     * \return True upon sucess, false otherwise
     */
    bool isSubProperty(const IRI& relationProperty, const IRI& parentRelationProperty);
    /**
     * Test if given properties are disjoint
     */
    bool isDisjointProperties(const IRI& relationProperty0, const IRI& relationProperty1);

    /**
     * Retrieve property domain
     */
    IRIList getPropertyDomain(const IRI& property, bool direct = true) const;

    /**
     * Retrieve all object properties
     * Note that the top and bottom object properties are excluded from this list
     */
    IRIList allObjectProperties() const;

    /**
     * Retrieve all ancestors of a given object property
     * \param property Name of object property
     * \param direct True if only the direct parent should be returned, false
     * otherwise
     */
    IRIList allAncestorObjectProperties(const IRI& property, bool direct = false);

    /**
     * Retrieve all sub object properties
     */
    IRIList allSubObjectProperties(const IRI& relationProperty, bool direct = false );

    /**
     * Retrieve all equivalent object properties
     */
    IRIList allEquivalentObjectProperties(const IRI& propertyRelation);

    bool isSubObjectProperty(const IRI& relationProperty, const IRI& parentRelationProperty);
    bool isDisjointObjectProperties(const IRI& relationProperty0, const IRI& relationProperty1);

    /**
     * Retrieve all data properties,
     * Note that the top and bottom data properties are excluded from this list
     */
    IRIList allDataProperties() const;

    /**
     * Retrieve all ancestors of a given data property
     * \param property Name of object property
     * \param direct True if only the direct parent should be returned, false
     * otherwise
     */
    IRIList allAncestorDataProperties(const IRI& property, bool direct = false);

    /**
     * Retrieve all sub data properties
     */
    IRIList allSubDataProperties(const IRI& relationProperty, bool direct = false );

    /**
     * Retrieve all equivalent data properties
     */
    IRIList allEquivalentDataProperties(const IRI& propertyRelation);

    bool isSubDataProperty(const IRI& relationProperty, const IRI& parentRelationProperty);
    bool isDisjointDataProperties(const IRI& relationProperty0, const IRI& relationProperty1);

    /**
     * Get property domain
     * \param property Name of property
     * \param direct If direct is true, an exact domain is retrieved if available, otherwise the most specific concept set
     * default is true
     */
    IRIList getDataPropertyDomain(const IRI& property, bool direct = true) const;

    // TODO: isSubPropertyChainOf
    // mKernel->isSubChain( objectproperty ) --> with reference to given argument list

    /**
     * Get all types of a given instance
     * \return list of types of a given instance
     */
    IRIList typesOf(const IRI& instance, bool direct = false) const;

    /**
     * Get the direct type of a given instance
     * \param instance Instance name
     * \return klassname for the type of this instance if known
     */
    IRI typeOf(const IRI& instance) const;

    /**
     * Resolve an alias / instance name
     * \param instanceOrAlias  IRI to find an alias for a given instance
     * \return Resolved aliases or a list containing only the instance name if there is no alias
     */
    IRIList getSameAs(const IRI& instanceOrAlias);

    /**
     * Make the list of instances unique, i.e., remove redundant information (aliases)
     * \param instances List of instances (not necessarily a set)
     * \return list of unique IRI, i.e. a set
     */
    IRIList uniqueList(const IRIList& instances);

    /**
     * Remove axiom from the knowledge base
     * \param a Axiom to retract
     */
    void retract(const reasoner::factpp::Axiom& a);

    /**
     * Remove all axioms which are related to/referenced by the given axiom
     * \param a Axiom to retract related ones for
     */
    void retractRelated(const owlapi::model::OWLAxiom::Ptr& a);

    /**
     * Add a reference between the frontend axiom and the knowledge base axiom
     */
    void addReference(const owlapi::model::OWLAxiom::Ptr& modelAxiom, const reasoner::factpp::Axiom& kbAxiom);

    /**
     * Test relation i.e. check if knowledge base remains consistent and add result if successfully asserted
     * \return true if relation has been successfully been added
     */
    bool assertAndAddRelation(const IRI& instance, const IRI& relation, const IRI& otherInstance);

    /**
     * Retrieve data value associated with instance
     * \return data value
     */
    reasoner::factpp::DataValue getDataValue(const IRI& instance, const IRI& dataProperty);

    /**
     * Print the ontology
     * \return
     */
    std::string toString(representation::Type representation = representation::LISP) const;

    //
    // Exploration
    //
    /**
     * Get node for exploration
     */
    reasoner::factpp::ExplorationNode getExplorationNode(const IRI& klass);

    /**
     * Retrieve object properties that are related to the given instance
     * \param instance Name of the instance
     * \param needInverse True if the inverse relation should be included, false otherwise
     * \return List of object properties
     */
    owlapi::model::IRISet getRelatedObjectProperties(const IRI& instance, bool needInverse = false);

    /**
     * Retrieve object properties that are related to the given class
     * NOTE: not sure whether that is what Fact++ actually provides here
     */
    reasoner::factpp::ObjectPropertyExpressionList getRelatedObjectPropertiesByKlass(const IRI& klass);

    /**
     * Retrieve data properties
     */
    owlapi::model::IRISet getRelatedDataProperties(const IRI& instance, bool needInverse = false);

    /**
     * Retrieve object properties that are related to the given class
     * NOTE: not sure whether that is what Fact++ actually provides here
     */
    reasoner::factpp::DataPropertyExpressionList getRelatedDataPropertiesByKlass(const IRI& klass);

    /**
     * Retrieve the datatype name
     */
    reasoner::factpp::DataTypeName getDataTypeName(const IRI& dataType) const;

    /**
     * Cleanup all axiom that have been marked for retraction
     */
    void cleanup();
};

} // namespace owlapi
#endif // OWLAPI_KNOWLEDGE_BASE_HPP
