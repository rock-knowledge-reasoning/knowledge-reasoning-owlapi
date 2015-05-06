#ifndef OWLAPI_MODEL_OWL_ANNOTATION_HPP
#define OWLAPI_MODEL_OWL_ANNOTATION_HPP

#include <vector>
#include <owlapi/model/OWLObject.hpp>
#include <owlapi/model/OWLAnnotationProperty.hpp>
#include <owlapi/model/OWLAnnotationValue.hpp>

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
