#ifndef OWLAPI_MODEL_OWL_DATA_PROPERTY_HPP
#define OWLAPI_MODEL_OWL_DATA_PROPERTY_HPP

#include <owlapi/model/OWLProperty.hpp>
#include <owlapi/model/OWLDataPropertyExpression.hpp>

namespace owlapi {
namespace model {

/**
 * \brief Represents a data property in the OWL 2 Specification.
 * \see http://www.w3.org/TR/owl2-syntax/#Datatypes
 */
class OWLDataProperty : public OWLDataPropertyExpression, public OWLProperty
{
public:
        typedef boost::shared_ptr<OWLDataProperty> Ptr;

        OWLDataProperty(const IRI& iri)
            : OWLDataPropertyExpression()
            , OWLProperty(iri)
        {}

        virtual ~OWLDataProperty() {}

        bool isDataPropertyExpression() const { return true; }
        bool isObjectPropertyExpression() const { return false; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_PROPERTY_HPP
