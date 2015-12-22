#ifndef OWLAPI_MODEL_ANNOTATION_OBJECT_HPP
#define OWLAPI_MODEL_ANNOTATION_OBJECT_HPP

#include <owlapi/model/OWLObject.hpp>

namespace owlapi {
namespace model {

/**
 * A marker interface for the values (objects) of annotations.
 */
class OWLAnnotationObject : public OWLObject
{
public:
    boost::shared_ptr<OWLAnnotationObject> Ptr;

    virtual ~OWLAnnotationObject() {}
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_ANNOTATION_OBJECT_HPP
