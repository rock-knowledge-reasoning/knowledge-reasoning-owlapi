#ifndef OWLAPI_MODEL_OWL_SUB_PROPERTY_AXIOM_HPP
#define OWLAPI_MODEL_OWL_SUB_PROPERTY_AXIOM_HPP

#include <owlapi/model/OWLPropertyAxiom.hpp>

namespace owlapi {
namespace model {

class OWLSubPropertyAxiom : public OWLPropertyAxiom
{
protected:
    OWLSubPropertyAxiom(const OWLPropertyExpression::Ptr subProperty,
            const OWLPropertyExpression::Ptr superProperty, 
            OWLAxiom::AxiomType type,
            OWLAnnotationList annotations)
        :  OWLPropertyAxiom(type, annotations) 
        , mSubProperty(subProperty)
        , mSuperProperty(superProperty)
    {}

public:
    typedef shared_ptr<OWLSubPropertyAxiom> Ptr;

    virtual ~OWLSubPropertyAxiom() {}

    virtual OWLPropertyExpression::Ptr getSubProperty() const { return mSubProperty; }
    virtual OWLPropertyExpression::Ptr getSuperProperty() const { return mSuperProperty; }

private:
    OWLPropertyExpression::Ptr mSubProperty;
    OWLPropertyExpression::Ptr mSuperProperty;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_SUB_PROPERTY_AXIOM_HPP
