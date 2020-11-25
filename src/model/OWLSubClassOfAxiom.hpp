#ifndef OWLAPI_MODEL_OWL_SUBCLASS_OF_AXIOM_HPP
#define OWLAPI_MODEL_OWL_SUBCLASS_OF_AXIOM_HPP

#include "OWLClassExpression.hpp"
#include "OWLClassAxiom.hpp"

namespace owlapi {
namespace model {

class OWLSubClassOfAxiom : public OWLClassAxiom
{
    OWLClassExpression::Ptr mSubClass;
    OWLClassExpression::Ptr mSuperClass;
public:
    typedef shared_ptr<OWLSubClassOfAxiom> Ptr;

    OWLSubClassOfAxiom(OWLClassExpression::Ptr subClass, OWLClassExpression::Ptr superClass, OWLAnnotationList annotations = OWLAnnotationList())
        : OWLClassAxiom(OWLAxiom::SubClassOf, annotations)
        , mSubClass(subClass)
        , mSuperClass(superClass)
    {}

    virtual ~OWLSubClassOfAxiom() {}

    OWLClassExpression::Ptr getSubClass() const { return mSubClass; }
    OWLClassExpression::Ptr getSuperClass() const { return mSuperClass; }

    /**
     * Determines if this subclass axiom has a subclass that is anonymous.
     */
    bool isGCI() const { throw std::runtime_error("OWLSubClassOfAxiom::isGCI: not implemented"); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_SUBCLASS_OF_AXIOM_HPP
