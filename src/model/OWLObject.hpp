#ifndef OWLAPI_MODEL_OWL_OBJECT_HPP
#define OWLAPI_MODEL_OWL_OBJECT_HPP

#include "../Exceptions.hpp"
#include "../SharedPtr.hpp"

namespace owlapi {
namespace model {

template<typename T, typename S>
typename T::Ptr ptr_cast(typename S::Ptr source, bool assert = false)
{
    typename T::Ptr ptr = dynamic_pointer_cast<T>(source);
    if(assert && !ptr)
    {
        throw std::runtime_error("owlapi::model::ptr_cast assertion failed");
    }
    return ptr;
}

class OWLAxiomVisitor;

class OWLObject
{
public:
    typedef shared_ptr<OWLObject> Ptr;

    enum Type {
        /// Unkown type -- for internal use only
        UNKOWN,
        AnonymousIndividual,
        AnnotationProperty,
        DataPropertyExpression,
        ObjectPropertyExpression,
        IRIType,
        Literal,
        END
    };

    virtual ~OWLObject() {}

    virtual Type getObjectType() const { return UNKOWN; }

    virtual bool isBottomEntity() const;
    virtual bool isTopEntity() const;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_HPP
