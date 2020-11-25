#include "HasAnnotations.hpp"
#include "OWLAnnotation.hpp"

namespace owlapi {
namespace model {

HasAnnotations::HasAnnotations(const OWLAnnotationList& annotations)
    : mAnnotations(annotations)
{}

HasAnnotations::~HasAnnotations() {}

OWLAnnotationList HasAnnotations::getAnnotations() const { return mAnnotations; }

 /**
 * Determines if any annotations exist
 * \return True if this object has annotations on it, otherwise
 *         false
 */
bool HasAnnotations::isAnnotated() const { return !mAnnotations.empty(); }



} // end namespace model
} // end namespace owlapi
