#ifndef OWLAPI_MODEL_ANNOTATION_VALUE_HPP
#define OWLAPI_MODEL_ANNOTATION_VALUE_HPP

#include "OWLAnnotationObject.hpp"

namespace owlapi {
namespace model {

class OWLLiteral;
class IRI;

class OWLAnnotationValue : public OWLAnnotationObject
{
public:
    typedef shared_ptr<OWLAnnotationValue> Ptr;

    OWLAnnotationValue() = default;

    virtual ~OWLAnnotationValue() {}


    virtual shared_ptr<OWLLiteral> asLiteral() const;
    virtual IRI asIRI() const;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_ANNOTATION_VALUE_HPP
