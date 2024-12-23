#ifndef OWLAPI_MODEL_OWL_OBJECT_PROPERTY_HPP
#define OWLAPI_MODEL_OWL_OBJECT_PROPERTY_HPP

#include "OWLProperty.hpp"
#include "OWLObjectPropertyExpression.hpp"

namespace owlapi {
namespace model {

class OWLObjectProperty : public virtual OWLObjectPropertyExpression, public OWLProperty
{
public:
    typedef shared_ptr<OWLObjectProperty> Ptr;

    OWLObjectProperty(const IRI& iri)
        : OWLObjectPropertyExpression()
        , OWLProperty(iri, OWLEntity::OBJECT_PROPERTY)
    {}

    virtual ~OWLObjectProperty() {}

    /**
     * Convert OWLObjectProperty to a string -- based on the underlying iri
     */
    std::string toString() const { return getIRI().toString(); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_OBJECT_PROPERTY_HPP
