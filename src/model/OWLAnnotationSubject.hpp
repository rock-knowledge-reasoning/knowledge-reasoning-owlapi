#ifndef OWLAPI_MODEL_ANNOTATION_SUBJECT_HPP
#define OWLAPI_MODEL_ANNOTATION_SUBJECT_HPP

#include "OWLAnnotationObject.hpp"

namespace owlapi {
namespace model {

class OWLAnnotationSubject : public OWLAnnotationObject
{
public:
    typedef shared_ptr<OWLAnnotationSubject> Ptr;

    OWLAnnotationSubject() = default;

    virtual ~OWLAnnotationSubject() {}

    virtual std::string toString() const { throw std::runtime_error("owlapi::model::OWLAnnotationSubject::toString: not implemented by subclass"); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_ANNOTATION_SUBJECT_HPP
