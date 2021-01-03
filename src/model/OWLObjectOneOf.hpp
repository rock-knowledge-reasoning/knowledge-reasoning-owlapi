#ifndef OWLAPI_MODEL_OWL_OBJECT_ONE_OF_HPP
#define OWLAPI_MODEL_OWL_OBJECT_ONE_OF_HPP

#include "OWLAnonymousClassExpression.hpp"
#include "OWLNamedIndividual.hpp"

namespace owlapi {
namespace model  {

class OWLObjectOneOf : public OWLAnonymousClassExpression
{
public:
    typedef shared_ptr<OWLObjectOneOf> Ptr;

    OWLObjectOneOf(const OWLNamedIndividual::PtrList& individuals)
        : OWLAnonymousClassExpression()
        , mNamedIndividuals(individuals)
    {}

    virtual ~OWLObjectOneOf() = default;

    const OWLNamedIndividual::PtrList& getNamedIndividuals() const { return mNamedIndividuals; }

    ClassExpressionType getClassExpressionType() const override { return OWLClassExpression::OBJECT_ONE_OF; }

private:
        OWLNamedIndividual::PtrList mNamedIndividuals;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_ONE_OF_HPP
