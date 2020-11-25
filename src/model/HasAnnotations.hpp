#ifndef OWLAPI_MODEL_HAS_ANNOTATIONS_HPP
#define OWLAPI_MODEL_HAS_ANNOTATIONS_HPP

#include <vector>

namespace owlapi {
namespace model {

class OWLAnnotation;
typedef std::vector<OWLAnnotation> OWLAnnotationList;

class HasAnnotations
{
protected:
    OWLAnnotationList mAnnotations;

public:
    HasAnnotations(const OWLAnnotationList& annotations);

    virtual ~HasAnnotations();

    OWLAnnotationList getAnnotations() const;

     /**
     * Determines if any annotations exist
     * \return True if this object has annotations on it, otherwise
     *         false
     */
    bool isAnnotated() const;

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_HAS_ANNOTATIONS_HPP
