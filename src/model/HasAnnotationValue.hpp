#ifndef OWLAPI_MODEL_HAS_ANNOTATION_VALUE_HPP
#define OWLAPI_MODEL_HAS_ANNOTATION_VALUE_HPP

#include "OWLAnnotationValue.hpp"

namespace owlapi {
namespace model {

class HasAnnotationValue
{
public:
    HasAnnotationValue(const OWLAnnotationValue::Ptr& value)
        : mAnnotationValue(value)
    {}

    OWLAnnotationValue::Ptr annotationValue() const { return mAnnotationValue; }

private:
    OWLAnnotationValue::Ptr mAnnotationValue;

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_HAS_ANNOTATION_VALUE_HPP
