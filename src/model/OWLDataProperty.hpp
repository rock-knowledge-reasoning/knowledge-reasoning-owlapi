#ifndef OWLAPI_MODEL_OWL_DATA_PROPERTY_HPP
#define OWLAPI_MODEL_OWL_DATA_PROPERTY_HPP

#include <owlapi/model/OWLProperty.hpp>
#include <owlapi/model/OWLDataRange.hpp>
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
        typedef shared_ptr<OWLDataProperty> Ptr;

        OWLDataProperty(const IRI& iri);
        virtual ~OWLDataProperty();

        bool isDataPropertyExpression() const { return true; }
        bool isObjectPropertyExpression() const { return false; }

        /**
         * Add a data range
         * \throw std::invalid_argument if given range is not a data range
         */
        virtual void addRange(const OWLPropertyRange::Ptr& range);

        /**
         * Return the data ranges
         */
        OWLDataRange::PtrList getDataRanges() const;

        std::string toString() const;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_PROPERTY_HPP
