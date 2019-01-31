#ifndef OWLAPI_MODEL_OWL_ONTOLOGY_ASK_HPP
#define OWLAPI_MODEL_OWL_ONTOLOGY_ASK_HPP

#include <owlapi/model/OWLOntology.hpp>
#include <owlapi/model/OWLCardinalityRestriction.hpp>
#include <owlapi/model/OWLLiteral.hpp>
#include <owlapi/model/OWLDataProperty.hpp>

namespace owlapi {
namespace model {

/**
 * OWLOntologyAsk will cache query results
 */
class OWLOntologyAsk
{
    OWLOntology::Ptr mpOntology;

public:
    typedef shared_ptr<OWLOntologyAsk> Ptr;

    OWLOntologyAsk(OWLOntology::Ptr ontology);

    /**
     * Get the OWLClass instance by IRI
     * \return OWLClass::Ptr
     * \throw std::runtime_error if object cannot be found in ontology
     */
    OWLClass::Ptr getOWLClass(const IRI& iri) const;

    /**
     * Get an individual by IRI
     * \return OWLIndividual::Ptr
     * \throw std::runtime_error if neither a anonymous nor a named individual
     * is registered with this IRI
     */
    OWLIndividual::Ptr getOWLIndividual(const IRI& iri) const;

    /**
     * Get the OWLAnonymousIndividual by IRI
     * \return OWLAnonymousIndividual::Ptr
     * \throw std::runtime_error if object cannot be found in ontology
     */
    OWLAnonymousIndividual::Ptr getOWLAnonymousIndividual(const IRI& iri) const;

    /**
     * Get the OWLNamedIndividual by IRI
     * \return OWLNamedIndividual::Ptr
     * \throw std::runtime_error if object cannot be found in ontology
     */
    OWLNamedIndividual::Ptr getOWLNamedIndividual(const IRI& iri) const;

    /**
     * Get or create the OWLObjectProperty by IRI
     * \return OWLObjectProperty::Ptr
     * \throw std::runtime_error if object cannot be found in ontology
     */
    OWLObjectProperty::Ptr getOWLObjectProperty(const IRI& iri) const;

    /**
     * Get or create the OWLDataProperty by IRI
     * \return OWLDataProperty::Ptr
     * \throw std::runtime_error if object cannot be found in ontology
     */
    OWLDataProperty::Ptr getOWLDataProperty(const IRI& iri) const;


    /**
     * Retrieve the cardinality restrictions of a given class expression
     * \param ce Classexpression
     * \param objectProperty IRI of the relation that this restriction applies to
     * \return a compact list of cardinality restrictions
     * \see OWLCardinalityRestriction::compact
     */
    std::vector<OWLCardinalityRestriction::Ptr> getCardinalityRestrictions(const
            owlapi::model::OWLClassExpression::Ptr& ce,
            const IRI& objectProperty = IRI()) const;

    /**
     * Retrieve the cardinality restrictions of a given class (by IRI)
     * \param iri IRI identifying the class
     * \param objectProperty object property to account for
     * \throws std::invalid_argument if class cannot be identified
     * \return a compact list of cardinality restrictions
     * \see OWLCardinalityRestriction::compact
     */
    std::vector<OWLCardinalityRestriction::Ptr> getCardinalityRestrictions(const IRI& iri,
            const IRI& objectProperty = IRI()) const;

    /**
     * Retrieve the cardinality restrictions for a given set of classes
     * Will retrieve the restrictions per class and then join the (potentially
     * overlapping results)
     * \param klasses Set of klasses to get restrictions for
     * \param objectProperty The object property to which the cardinality
     * restriction applies to
     * \param operationType Use the given operation type to join the set of
     * restrictions
     * \see OWLCardinalityRestriction::join
     */
    std::vector<OWLCardinalityRestriction::Ptr> getCardinalityRestrictions(const std::vector<IRI>& klasses,
            const IRI& objectProperty = IRI(),
            OWLCardinalityRestriction::OperationType operationType = OWLCardinalityRestriction::SUM_OP) const;

    /* Get all cardinality restrictions whose qualification is a (direct)
     * subclass of one of the classes list in the filter
     * \param qualification klass
     * \return filtered list
     */
    std::vector<OWLCardinalityRestriction::Ptr> getCardinalityRestrictions(const IRI& klass,
            const IRI& objectProperty,
            const IRI& qualificationKlass,
            bool direct = false) const;

    /* Get all cardinality restrictions whose qualification is a (direct)
     * subclass of one of the classes list in the filter
     * \param qualification klass
     * \return filtered list
     */
    std::vector<OWLCardinalityRestriction::Ptr> getCardinalityRestrictions(const IRIList& klasses,
            const IRI& objectProperty,
            const IRI& qualificationKlass,
            bool direct = false) const;

    /**
     * Get all cardinality restrictions whose qualification is a (direct)
     * subclass of one of the classes list in the filter
     * \return filtered list
     */
    std::vector<OWLCardinalityRestriction::Ptr> getCardinalityRestrictions(const IRIList& klasses,
            const IRI& objectProperty,
            const IRIList& qualificationKlasses,
            bool direct = false) const;

    /**
     * Get all cardinality restrictions which are set for a given object
     * property and which should apply to a given klass/instance
     * \param klass The qualification of the cardinality restriction
     * \param objectProperty The objectProperty for this description
     * \param targetKlass The queried target class/instance to identify applied
     * restrictions
     * \return The list of restrictions that are applied to the targetKlass
     */
    std::vector<OWLCardinalityRestriction::Ptr> getCardinalityRestrictionsForTarget(const IRI& klass,
            const IRI& objectProperty,
            const IRI& targetKlass) const;


    /**
     * Check if object identified by iri is a subclass of the given superclass
     * \param iri A class identifier
     * \param superclass Another class
     * \return If iri is a subclass of superclass
     */
    bool isSubClassOf(const IRI& iri, const IRI& superclass) const;

    /**
     * Check if object identified by iri is a direct subclass of the given superclass
     * \param iri A class identifier
     * \param superclass Another class
     * \return If iri is a direct subclass of superclass
     */
    bool isDirectSubClassOf(const IRI& iri, const IRI& superclass) const;

    /**
     * Check if given class expression is subclass of superclass
     * \return true if superclass is parent of subclass
     */
    bool isSubClassOf(const OWLClassExpression::Ptr& subclass, const OWLClassExpression::Ptr& superclass) const;

    /**
     * Check if iri represents a known class
     */
    bool isOWLClass(const IRI& iri) const;

    /**
     * Retrieve all known classes
     * \return list of all classes
     */
    IRIList allClasses(bool excludeBottomClass = true) const;

    /**
     * Retrieve all known instances
     * \return list of all instances
     */
    IRIList allInstances() const;

    /**
     * Retrieve all subclasses of a given klass
     * \return list of all subclasses
     */
    IRIList allSubClassesOf(const IRI& klass, bool direct = false) const;

    /**
     * Retrieve all subclasses of a given klass
     * \return hierachical list of all subclasses with increasing link distance
     */
    std::vector<IRIList> allSubClassesOfWithDistance(const IRI& klass) const;

    /**
     * Retrieve all underived subclasses, i.e. the leaves in the inheritance
     * tree
     * \return the list of underived subclassed of a particular class type
     */
    IRIList allUnderivedSubClassesOf(const IRI& classType) const;

    /**
     * Retrieve all known instance of the given class type
     */
    IRIList allInstancesOf(const IRI& classType, bool direct = false) const;

    /**
     * Retrieve all known object properties
     */
    IRIList allObjectProperties() const;

    /**
     * Retrieve all known data(type) properties
     */
    IRIList allDataProperties() const;

    /**
     * Test if instance is type of a class
     * \param instance Instance identifier
     * \param klass Class identifier
     * \return true if instance is of given klass type, false otherwise
     */
    bool isInstanceOf(const IRI& instance, const IRI& klass) const;

    /**
     * Test if instances are related via given a given property
     * \param instance Instance identifier
     * \param relationProperty relation identifier:
     * \param otherInstance Another instance
     * \return True if instance is related with otherInstance via
     * relationProperty
     */
    bool isRelatedTo(const IRI& instance, const IRI& relationProperty, const IRI& otherInstance) const;

    /**
     * Retrieve the type of the provided instance
     * \return type
     */
    IRI typeOf(const IRI& instance) const;

    /**
     * Retrieve all related instances regarding a single instance and a selected property
     * which have a certain klass type
     * \param instance Instance name
     * \param relationProperty the relation property
     * \param klass Klass type the relation should map to
     * \return list of instances that are related to instance via the relationProperty
     */
    IRIList allRelatedInstances(const IRI& instance, const IRI& relationProperty, const IRI& klass = IRI()) const;

    /**
     * Retrieve all instances regarding a single instance and a selected property
     * \param instance Instance where this relation is bound to
     * \param relationProperty Property for this relation (which will be inverted)
     * \param klass Optional filter klass for the result list, check if result is instance of given klass
     * \return list of instance that are related to instance via the inverse of the given
     * relationProperty
     */
    IRIList allInverseRelatedInstances(const IRI& instance, const IRI& relationProperty, const IRI& klass = IRI()) const;

    /**
     * Retrieve data value associated with instance
     * \param instance Name of the instance to get the data property for
     * \param dataProperty Name of the dataProperty to check the relation for
     * \param includeAncestors When using punning allow to retrieve associated
     * property from a parent class
     * \return data value
     */
    OWLLiteral::Ptr getDataValue(const IRI& instance, const IRI& dataProperty,
            bool includeAncestors = true) const;

    /**
     * Get the data domain for the given data propery
     * \param dataProperty name of the propery
     * \param direct set to true if only the direct domain association should be
     * used, false if inherited domains should be considered as well
     * \throw std::invalid_argument if data property is not known
     */
    IRIList getDataPropertyDomain(const IRI& dataProperty, bool direct = true) const;

    /**
     * Get the data range for the given data property
     * \throw std::invalid_argument if data property is not known
     */
    OWLDataRange::PtrList getDataRange(const IRI& dataProperty) const;

    /**
     * Get the data type for the given data property and a particular value
     * based on the available data ranges (if available)
     * \throw std::invalid_argument if data property is not known
     */
    OWLDataType getDataType(const IRI& dataProperty, const std::string& value) const;

    /**
     * Get the object domain for the given propery
     * \param objectProperty name of the propery
     * \param direct set to true if only the direct domain association should be
     * used, false if inherited domains should be considered as well
     * \throw std::invalid_argument if object property is not known
     */
    IRIList getObjectPropertyDomain(const IRI& objectProperty, bool direct = true) const;

    /**
     * Retrieve the list of ancestors
     * \return list of ancestors
     */
    IRIList ancestors(const IRI& instance) const;

    /**
     * Check is the given property is an object property, i.e.
     * check if property is an instance of ObjectProperty
     *
     * \see  http://www.w3.org/TR/owl-ref/
     */
    bool isObjectProperty(const IRI& property) const;

    /**
     * Check if the given property is a data(type) property, i.e.
     * check if property is an instance of DatatypeProperty
     *
     * \see http://www.w3.org/TR/owl-ref
     */
    bool isDataProperty(const IRI& property) const;

    /**
     * Retrieve all object properties that apply to a given domain
     * \return property list
     */
    IRIList getObjectPropertiesForDomain(const IRI& klass) const;

    /**
     * Retrieve all data properties that apply to a given domain
     */
    IRIList getDataPropertiesForDomain(const IRI& klass) const;

    /**
     * Test if klass is a subclass of intersection of classes
     * \return true if it does, false otherwise
     */
    bool isSubClassOfIntersection(const IRI& klass, const IRIList& intersection) const;

    /**
     * Get the list of domains for a given property
     */
    IRIList domainOf(const IRI& iri, bool direct = true) const;

    /**
     * Get the list of imports of other ontologies
     * \see http://www.w3.org/TR/owl2-syntax/#Imports
     */
    IRIList imports() const;

    /**
      * Retrieve the underlying ontology reference
      */
    const OWLOntology::Ptr& getOntology() const { return mpOntology; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ONTOLOGY_ASK_HPP
