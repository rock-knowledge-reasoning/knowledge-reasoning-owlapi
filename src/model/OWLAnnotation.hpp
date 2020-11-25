#ifndef OWLAPI_MODEL_OWL_ANNOTATION_HPP
#define OWLAPI_MODEL_OWL_ANNOTATION_HPP

#include <vector>
#include "OWLObject.hpp"
#include "OWLAnnotationProperty.hpp"

namespace owlapi {
namespace model {

class OWLAnnotation : public OWLObject
{
    OWLAnnotationProperty mProperty;
    OWLAnnotationValue mValue;

public:
    virtual ~OWLAnnotation() {}

};

typedef std::vector<OWLAnnotation> OWLAnnotationList;

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ANNOTATION_HPP
