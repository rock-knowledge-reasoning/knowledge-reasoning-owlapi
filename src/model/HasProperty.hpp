#ifndef OWLAPI_MODEL_HAS_PROPERTY_HPP
#define OWLAPI_MODEL_HAS_PROPERTY_HPP

#include "../SharedPtr.hpp"

namespace owlapi {
namespace model {

template<typename P>
class HasProperty
{
public:
    typedef shared_ptr<P> property_t;

    HasProperty(property_t property)
        :  mProperty(property)
    {}

    virtual ~HasProperty() {}

    property_t getProperty() const { return mProperty; }

private:
    property_t mProperty;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_HAS_PROPERTY_HPP

