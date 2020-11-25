#ifndef OWLAPI_MODEL_OWL_AXIOM_HPP
#define OWLAPI_MODEL_OWL_AXIOM_HPP

#include <vector>
#include "OWLObject.hpp"
#include "OWLEntity.hpp"
#include "HasAnnotations.hpp"

namespace owlapi {
namespace model {

/**
 * \class OWLAxiom
 * \brief Represents an axiom in the OWL 2 Specification
 * \see http://www.w3.org/TR/owl2-syntax/#Axioms
 */
class OWLAxiom : public OWLObject, public HasAnnotations
{
public:
    typedef shared_ptr<OWLAxiom> Ptr;
    typedef std::vector<Ptr> PtrList;

    /// Defines the types of axioms that can be used with OWL
    enum AxiomType {
                /// Unkown type -- for internal use only
                UNKOWN,
               /// General declaration
                Declaration = 0,            // TDLAxiomDeclaration
                ClassAxiom,                 // TDLConceptName
                ObjectPropertyAxiom,        // TDLObjectRoleName
                DataPropertyAxiom,          // TDLDataRoleName
                DatatypeDefinition,         // n/a
                Assertion,                  // n/a
                HasKey,
                AnnotationAxiom,            // not part of the reasoner
                // ClassAxiom:
                SubClassOf,                 // TDLAxiomConceptInclusion
                EquivalentClasses,          // TDLAxiomEquivalentConcepts
                DisjointClasses,            // TDLAxiomDisjointConcepts
                DisjointUnion,              // TDLAxiomDisjointUnion
                // ObjectPropertyAxiom
                EquivalentObjectProperties, // TDLAxiomEquivalentORoles
                DisjointObjectProperties,   // TDLAxiomDisjointORoles
                SubObjectPropertyOf,        // TDLAxiomORoleSubsumption
                ObjectPropertyDomain,       // TDLAxiomORoleDomain
                ObjectPropertyRange,        // TDLAxiomDRoleRange
                InverseObjectProperties,    // TDLAxiomRoleInverse
                FunctionalObjectProperty,   // TDLAxiomORoleFunctional
                ReflexiveObjectProperty,    // TDLAxiomRoleReflexive
                InverseFunctionalObjectProperty, // TDLAxiomRoleInverseFunctional
                IrreflexiveObjectProperty,  // TDLAxiomRoleIrreflexive
                SymmetricObjectProperty,    // TDLAxiomRoleSymmetric
                TransitiveObjectProperty,   // TDLAxiomRoleTransitive
                AsymmetricObjectProperty,   // TDLAxiomRoleAsymmetric
                // DataPropertyAxiom
                SubDataPropertyOf,          // TDLDRoleSubsumption
                DisjointDataProperties,     // TDLAxiomDisjointDRoles
                EquivalentDataProperties,   // TDLAxiomEquivalentDRoles
                FunctionalDataProperty,   // TDLAxiomDRoleFunctional
                DataPropertyDomain,         // TDLAxiomDRoleDomain
                DataPropertyRange,          // TDLAxiomDRoleRange
                // Assertion
                ClassAssertion,             // TDLAxiomInstanceOf
                SameIndividual,             // TDLAxiomSameIndividuals
                DifferentIndividuals,       // TDLAxiomDifferentIndividuals
                ObjectPropertyAssertion,    // TDLAxiomRelatedTo
                NegativeObjectPropertyAssertion,  // TDLAxiomRelatedToNot
                /// Triple to assert a data property value to an individual
                DataPropertyAssertion,      // TDLAxiomValueOf
                /// Triple to assert that a individual has not the given value for
                /// a relation
                NegativeDataPropertyAssertion,    // TDLAxiomValueOfNot
                // AnnotationAxiom
                SubAnnotationPropertyOf,    // n/a
                AnnotationPropertyDomain,   // n/a
                AnnotationPropertyRange,    // n/a
                AnnotationAssertion,        // n/a
                // Not explicitly stated in OWL 2 but for convenience
                SubPropertyChainOf
    };

    static std::map<AxiomType, std::string> AxiomTypeTxt;

    OWLAxiom(AxiomType type, const OWLAnnotationList annotations)
        : HasAnnotations(annotations)
        , mAxiomType(type)
        , mRetract(false)
    {}

    virtual ~OWLAxiom() {}

    /**
     * \param visitor
     *        visitor to accept
     */
    void accept(OWLAxiomVisitor* visitor) const;

    /**
     * Determines if this axiom is a logical axiom. Logical axioms are defined
     * to be axioms other than both declaration axioms (including imports
     * declarations) and annotation axioms.
     *
     * \return True if the axiom is a logical axiom, False if
     *         the axiom is not a logical axiom.
     */
    virtual bool isLogicalAxiom() const { return false; }

    /**
     * Determines if this axioms in an annotation axiom (an instance of
     * OWLAnnotationAxiom)
     *
     * \return True if this axiom is an instance of
     *         OWLAnnotationAxiom, otherwise false.
     */
     virtual bool isAnnotationAxiom() const { return false; }

    /**
     * Gets the axiom type for this axiom.
     * \return The axiom type that corresponds to the type of this axiom.
     */
    AxiomType getAxiomType() const { return mAxiomType; }

    /**
     * Determines if this axiom is one of the specified types
     *
     * \param axiomType
     *        The axiom type to check for
     * \return True, if this axiom is one of the specified types,
     *         otherwise false
     */
    bool isOfType(AxiomType axiomType) const { return axiomType == mAxiomType; }

    /**
     * Determines if this axiom is one of the specified types
     *
     * \param types
     *        The axiom types to check for
     * \return True if this axioms is one of the specified types,
     *         otherwise false
     */
    bool isOfType(const std::set<AxiomType>& types) const { return types.end() != types.find(mAxiomType); }

    virtual std::string toString() const;

    static OWLAxiom::Ptr declare(const OWLEntity::Ptr& entity);

    /**
     * Set the origin of this axiom
     */
    void setOrigin(const IRI& iri) { mOrigin = iri; }

    /**
     * Get the origin of this axiom, i.e. in which ontology this axiom was
     * stated
     */
    const IRI& getOrigin() const { return mOrigin; }

    /**
     * Check if axiom is marked for retraction
     */
    bool isMarkedForRetraction() const { return mRetract; }

    void markForRetraction() const { mRetract = true; }

private:
    AxiomType mAxiomType;

    IRI mOrigin;

    mutable bool mRetract;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_AXIOM_HPP
