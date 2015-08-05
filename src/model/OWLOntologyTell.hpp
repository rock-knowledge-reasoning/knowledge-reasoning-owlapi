#ifndef OWLAPI_MODEL_OWL_ONTOLOGY_TELL_HPP
#define OWLAPI_MODEL_OWL_ONTOLOGY_TELL_HPP

#include <owlapi/model/OWLOntology.hpp>
#include <owlapi/model/OWLLiteral.hpp>
#include <owlapi/model/OWLCardinalityRestriction.hpp>
#include <owlapi/model/OWLOntologyAsk.hpp>
#include <owlapi/model/OWLSubPropertyAxiom.hpp>

namespace owlapi {
namespace model {

/**
 * This class is allowed to add new information to an ontology
 */
class OWLOntologyTell
{
    OWLOntology::Ptr mpOntology;
    OWLOntologyAsk mAsk;

public:
    typedef boost::shared_ptr<OWLOntologyTell> Ptr;

    OWLOntologyTell(OWLOntology::Ptr ontology);

    template<typename T>
    bool addUnaryObjectPropertyAxiom(const IRI& property)
    {
        if(!mAsk.isObjectProperty(property))
        {
            return false;
        }

        OWLObjectProperty::Ptr oProperty = mpOntology->getObjectProperty(property);
        boost::shared_ptr<T> axiom(new T(oProperty));
        mpOntology->addAxiom(axiom);
        return true;
    }

    template<typename T>
    bool addUnaryDataPropertyAxiom(const IRI& property)
    {
        if(!mAsk.isDataProperty(property))
        {
            return false;
        }

        OWLDataProperty::Ptr dProperty = mpOntology->getDataProperty(property);
        boost::shared_ptr<T> axiom(new T(dProperty));
        mpOntology->addAxiom(axiom);
        return true;
    }

    /**
     * Initialize all default classes for this ontology
     */
    void initializeDefaultClasses();

    /**
     * Get or create the OWLClass instance by IRI
     * \return OWLClass::Ptr
     */
    OWLClass::Ptr klass(const IRI& iri);

    /**
     * Get or create the OWLAnonymousIndividual by IRI
     * \return OWLAnonymousIndividual::Ptr
     */
    OWLAnonymousIndividual::Ptr anonymousIndividual(const IRI& iri);

    /**
     * Get or create the OWLNamedIndividual by IRI
     * \return OWLNamedIndividual::Ptr
     */
    OWLNamedIndividual::Ptr namedIndividual(const IRI& iri);

    /**
     * Get or create the OWLObjectProperty by IRI
     * \return OWLObjectProperty::Ptr
     */
    OWLObjectProperty::Ptr objectProperty(const IRI& iri);

    /**
     * Get or create the OWLDataProperty by IRI
     * \return OWLDataProperty::Ptr
     */
    OWLDataProperty::Ptr dataProperty(const IRI& iri);

    OWLAnnotationProperty::Ptr annotationProperty(const IRI& iri);

    OWLSubClassOfAxiom::Ptr subClassOf(OWLClass::Ptr subclass, OWLClass::Ptr superclass);
    OWLSubClassOfAxiom::Ptr subClassOf(const IRI& subclass, OWLClassExpression::Ptr superclass);

    /**
     * Declare the subclass relationship between two classes
     */
    OWLSubClassOfAxiom::Ptr subClassOf(const IRI& subclass, const IRI& superclass);

    /**
     * Declare the subclass relationship between two classes
     */
    OWLSubClassOfAxiom::Ptr subClassOf(OWLClassExpression::Ptr subclassExpression, OWLClassExpression::Ptr superclassExpression);

    /**
     * Set instance of class type
     */
    OWLClassAssertionAxiom::Ptr instanceOf(const IRI& instance, const IRI& classType);

    /**
     * Create a cardinality restriction (for consistency reasons)
     * \see OWLCardinalityRestriction
     * \return restriction
     */
    OWLCardinalityRestriction::Ptr cardinalityRestriction(OWLPropertyExpression::Ptr property, uint32_t cardinality, const OWLQualification& qualification, OWLCardinalityRestriction::CardinalityRestrictionType restrictionType);

    /**
     * Add an axiom
     * \param axiom
     */
    void addAxiom(OWLAxiom::Ptr axiom);

    void inverseFunctionalProperty(const IRI& property);
    void reflexiveProperty(const IRI& property);
    void irreflexiveProperty(const IRI& property);
    void symmetricProperty(const IRI& property);
    void asymmetricProperty(const IRI& property);
    void transitiveProperty(const IRI& property);
    void functionalObjectProperty(const IRI& property);
    void functionalDataProperty(const IRI& property);
    void relatedTo(const IRI& subject, const IRI& relation, const IRI& object);
    void dataPropertyDomainOf(const IRI& relation, const IRI& classType);
    void dataPropertyRangeOf(const IRI& relation, const IRI& classType);
    void objectPropertyDomainOf(const IRI& relation, const IRI& classType);
    void objectPropertyRangeOf(const IRI& relation, const IRI& classType);

    /**
     * Define a subproperty of an existing property
     * \param subProperty IRI of the subproperty
     * \param parentProperty IRI of the (already existing) parent property
     * \throw std::invalid_argument if the parent property already exists
     */
    OWLSubPropertyAxiom::Ptr subPropertyOf(const IRI& subProperty, const IRI& parentProperty);
    void inverseOf(const IRI& relation, const IRI& inverseType);

    // DataPropertyAssert
    /**
     * Setting the data property for a given instance to the value given by
     * literal
     * \param instance Instance name
     * \param dataProperty DataProperty name
     * \param literal Literal containing the value
     */
    void valueOf(const IRI& instance, const IRI& dataProperty, OWLLiteral::Ptr literal);
    void restrictClass(const IRI& klass, OWLCardinalityRestriction::Ptr restriction);

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ONTOLOGY_TELL_HPP
