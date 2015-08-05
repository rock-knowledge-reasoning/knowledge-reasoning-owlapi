#ifndef OWLAPI_MODEL_OWL_OBJECT_HPP
#define OWLAPI_MODEL_OWL_OBJECT_HPP

#include <owlapi/Exceptions.hpp>
#include <boost/shared_ptr.hpp>

namespace owlapi {
namespace model {

template<typename T, typename S>
typename T::Ptr ptr_cast(typename S::Ptr source, bool assert = false)
{
    typename T::Ptr ptr = boost::dynamic_pointer_cast<T>(source);
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
    typedef boost::shared_ptr<OWLObject> Ptr;

    virtual ~OWLObject() {}

    virtual bool isBottomEntity();
    virtual bool isTopEntity();
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_HPP
