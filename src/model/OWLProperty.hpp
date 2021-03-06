#ifndef OWLAPI_MODEL_OWL_PROPERTY_HPP
#define OWLAPI_MODEL_OWL_PROPERTY_HPP

#include "OWLPropertyExpression.hpp"
#include "OWLLogicalEntity.hpp"

namespace owlapi {
namespace model {

/**
 * \details A marker interface for properties that aren't expressions i.e. named
 * properties. By definition, properties are either data properties or object
 * properties.
 */
class OWLProperty : public OWLLogicalEntity, public virtual OWLPropertyExpression
{
public:
    typedef shared_ptr<OWLProperty> Ptr;

    OWLProperty(const IRI& iri, OWLEntity::EntityType type)
        : OWLLogicalEntity(iri, type)
    {}

    virtual ~OWLProperty() {}

    virtual std::string toString() const override { return OWLLogicalEntity::toString(); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_PROPERTY_HPP
