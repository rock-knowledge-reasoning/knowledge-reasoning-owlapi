#ifndef OWLAPI_MODEL_OWL_ONTOLOGY_TELL_HPP
#define OWLAPI_MODEL_OWL_ONTOLOGY_TELL_HPP

#include <stdexcept>
#include <owlapi/model/OWLOntology.hpp>
#include <owlapi/model/OWLLiteral.hpp>
#include <owlapi/model/OWLCardinalityRestriction.hpp>
#include <owlapi/model/OWLOntologyAsk.hpp>
#include <owlapi/model/OWLSubPropertyAxiom.hpp>

namespace owlapi {
namespace model {

/**
 * \class OWLOntologyTell
 * This class is allowed to add new information to an ontology
 */
class OWLOntologyTell
{
    OWLOntology::Ptr mpOntology;
    OWLOntologyAsk mAsk;

    IRI mOrigin;

public:
    typedef boost::shared_ptr<OWLOntologyTell> Ptr;

    /**
     * \brief
     * \param ontology OWLOntology to populate with information
     * \param origin optional source IRI to provide information 'who' the
     * information originates from
     */
    OWLOntologyTell(OWLOntology::Ptr ontology, const IRI& origin = IRI());

    /**
     * Get the origin of the data provided by this OWLOntologyTell object
     * \return IRI to describe the origin
     */
    const IRI& getOrigin() const { return mOrigin; }

    template<typename T>
    OWLAxiom::Ptr addUnaryObjectPropertyAxiom(const IRI& property)
    {
        if(!mAsk.isObjectProperty(property))
        {
            throw std::invalid_argument("owlapi::model::OWLOntologyTell: '" +
                    property.toString() + "' is not an object property");
        }

        OWLObjectProperty::Ptr oProperty = mpOntology->getObjectProperty(property);
        boost::shared_ptr<T> axiom(new T(oProperty));
        return addAxiom(axiom);
    }

    template<typename T>
    OWLAxiom::Ptr addUnaryDataPropertyAxiom(const IRI& property)
    {
        if(!mAsk.isDataProperty(property))
        {
            throw std::invalid_argument("owlapi::model::OWLOntologyTell: '" +
                    property.toString() + "' is not an data property");
        }

        OWLDataProperty::Ptr dProperty = mpOntology->getDataProperty(property);
        boost::shared_ptr<T> axiom(new T(dProperty));
        return addAxiom(axiom);
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
     * Define an import of another ontology defined by the given IRI
     */
    void imports(const IRI& iri);

    void directlyImports(const IRI& iri);

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
     * \return the added axiom
     */
    OWLAxiom::Ptr addAxiom(const OWLAxiom::Ptr& axiom);

    OWLAxiom::Ptr inverseFunctionalProperty(const IRI& property);
    OWLAxiom::Ptr reflexiveProperty(const IRI& property);
    OWLAxiom::Ptr irreflexiveProperty(const IRI& property);
    OWLAxiom::Ptr symmetricProperty(const IRI& property);
    OWLAxiom::Ptr asymmetricProperty(const IRI& property);
    OWLAxiom::Ptr transitiveProperty(const IRI& property);
    OWLAxiom::Ptr functionalObjectProperty(const IRI& property);
    OWLAxiom::Ptr functionalDataProperty(const IRI& property);
    OWLAxiom::Ptr relatedTo(const IRI& subject, const IRI& relation, const IRI& object);
    OWLAxiom::Ptr dataPropertyDomainOf(const IRI& relation, const IRI& classType);
    OWLAxiom::Ptr dataPropertyRangeOf(const IRI& relation, const IRI& classType);
    OWLAxiom::Ptr objectPropertyDomainOf(const IRI& relation, const IRI& classType);
    OWLAxiom::Ptr objectPropertyRangeOf(const IRI& relation, const IRI& classType);

    /**
     * Define a subproperty of an existing property
     * \param subProperty IRI of the subproperty
     * \param parentProperty IRI of the (already existing) parent property
     * \throw std::invalid_argument if the parent property already exists
     */
    OWLSubPropertyAxiom::Ptr subPropertyOf(const IRI& subProperty, const IRI& parentProperty);

    /**
     * Define the inverse of an object property
     * \throws if relation of inverseType are not object properties
     * \return newly added axiom
     */
    OWLAxiom::Ptr inverseOf(const IRI& relation, const IRI& inverseType);

    // DataPropertyAssert
    /**
     * Setting the data property for a given instance to the value given by
     * literal
     * \param instance Instance name
     * \param dataProperty DataProperty name
     * \param literal Literal containing the value
     */
    OWLAxiom::Ptr valueOf(const IRI& instance, const IRI& dataProperty, OWLLiteral::Ptr literal);
    OWLAxiom::Ptr restrictClass(const IRI& klass, OWLCardinalityRestriction::Ptr restriction);

    /**
     * Make ontology known
     */
    void ontology(const IRI& ontology);
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ONTOLOGY_TELL_HPP
