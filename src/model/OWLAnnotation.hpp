#ifndef OWLAPI_MODEL_OWL_ANNOTATION_HPP
#define OWLAPI_MODEL_OWL_ANNOTATION_HPP

#include <vector>
#include "OWLObject.hpp"
#include "OWLAnnotationProperty.hpp"
#include "HasAnnotations.hpp"
#include "HasAnnotationValue.hpp"

namespace owlapi {
namespace model {

class OWLAnnotation : public OWLObject, public HasAnnotations, public HasAnnotationValue
{
    OWLAnnotationProperty::Ptr mProperty;
    OWLAnnotationValue::Ptr mValue;

public:
    OWLAnnotation(const OWLAnnotationValue::Ptr& value,
            const OWLAnnotationList& annotations = OWLAnnotationList())
        : HasAnnotations(annotations)
        , HasAnnotationValue(value)
    {}
    virtual ~OWLAnnotation() {}

};

typedef std::vector<OWLAnnotation> OWLAnnotationList;

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ANNOTATION_HPP
