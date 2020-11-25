#ifndef OWLAPI_MODEL_OWL_CLASS_HPP
#define OWLAPI_MODEL_OWL_CLASS_HPP

#include "../Vocabulary.hpp"
#include "OWLClassExpression.hpp"
#include "OWLLogicalEntity.hpp"

namespace owlapi {
namespace model {

/**
 * \brief OWLClass
 * \see http://www.w3.org/TR/owl2-syntax/#Class_Expression_Axioms
 */
class OWLClass : public OWLClassExpression, public OWLLogicalEntity
{
    bool mIsThing;
    bool mIsNothing;

public:
    typedef shared_ptr<OWLClass> Ptr;

    /**
     * Default constructor 
     */
    OWLClass(const IRI& iri);

    virtual ~OWLClass() {}

    /**
     * Test if this class is the top entity, i.e. owl:Thing
     * \return true, if this is the top entity, false otherwise
     */
    bool isTopEntity() const { return mIsThing; }

    /**
     * Test if this class is the bottom entity, i.e. owl:Nothing
     * \return true, if this is the bottom entity, false otherwise
     */
    bool isBottomEntity() const { return mIsNothing; }

    ClassExpressionType getClassExpressionType() const { return OWLClassExpression::OWL_CLASS; }

    bool isAnonymous() const { return false; }

    bool isClassExpressionLiteral() const { return true; }

    virtual std::string toString() const;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_CLASS_HPP
