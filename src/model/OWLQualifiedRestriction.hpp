#ifndef OWLAPI_MODEL_OWL_QUALIFIED_RESTRICTION_HPP
#define OWLAPI_MODEL_OWL_QUALIFIED_RESTRICTION_HPP

#include "IRI.hpp"
#include "OWLRestriction.hpp"
#include <owlapi/Vocabulary.hpp>

namespace owlapi {
namespace io {

class OWLOntologyReader;

}
namespace model {

typedef IRI OWLQualification;

class OWLQualifiedRestriction : virtual public OWLRestriction
{
    friend class owlapi::io::OWLOntologyReader;
    OWLQualification mQualification;
    bool mQualified;

public:
    typedef shared_ptr<OWLQualifiedRestriction> Ptr;
    /**
     * \brief A qualified restriction allows to associate a property with a qualification
     * \details According to w3c the qualification here should be:
     * for ObjectProperties this has to be a ClassExpression (:= Class | ObjectIntersectionOf | ObjectUnionOf | ObjectComplementOf | ObjectOneOf | ObjectSomeValuesFrom | ObjectAllValuesFrom | ObjectHasValue | ObjectHasSelf)
     * for DataProperties this has to be a DataRange (:= Datatype | DataIntersectionOf | DataUnionOf | DataComplementOf | DataOneOf | DatatypeRestriction )
     * However, to facilitate implementation, we relax/generalize the requirement to an IRI, so that OWLQualification := IRI
     *
     * To qualify ObjectProperties and DataProperties they have to differ from the TOP concepts, i.e. owl:Thing and rdfs::Literal respectively
     *
     */
    OWLQualifiedRestriction(const OWLPropertyExpression::Ptr& property,
            const OWLQualification& qualification);
    /**
     * Default constructor to allow usage of this class in a map
     */
    OWLQualifiedRestriction() { mQualified = false; }

    virtual ~OWLQualifiedRestriction() = default;

    /**
     * Retrieve the qualification for this restriction
     * \return Given qualification
     */
    OWLQualification getQualification() const { return mQualification; }

    /**
     * \details Determines if this restriction is qualified. Qualified cardinality restrictions are defined to be cardinality restrictions that have fillers which aren't TOP (owl:Thing or rdfs:Literal). An object restriction is unqualified if it has a filler that is owl:Thing. A data restriction is unqualified if it has a filler which is the top data type (rdfs:Literal).
     * \return true if the given qualification differs from the TOP concepts, false otherwise
     */
    bool isQualified() const { return mQualified; }

    /**
     * Stringify the qualified restriction
     * \return stringified version fo the qualified restriction
     */
    virtual std::string toString() const;

protected:
    /**
     * Allow to set the qualification for this restriction to
     * allow incremental construction
     */
    void setQualification(const OWLQualification& qualification) {
        mQualification = qualification;
        mQualified = !( mQualification == owlapi::vocabulary::OWL::Thing() || mQualification == owlapi::vocabulary::RDFS::Literal() );
    }

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_QUALIFIED_RESTRICTION_HPP
