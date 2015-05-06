#ifndef OWLAPI_MODEL_ANNOTATION_VALUE_HPP
#define OWLAPI_MODEL_ANNOTATION_VALUE_HPP

#include <owlapi/model/OWLAnnotationObject.hpp>

namespace owlapi {
namespace model {

class OWLAnnotationValue : public OWLAnnotationObject
{
public:
    virtual ~OWLAnnotationValue() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_ANNOTATION_VALUE_HPP
