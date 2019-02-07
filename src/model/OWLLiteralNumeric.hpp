#ifndef OWLAPI_MODEL_OWL_LITERAL_NUMERIC_HPP
#define OWLAPI_MODEL_OWL_LITERAL_NUMERIC_HPP

#include <string>
#include <boost/lexical_cast.hpp>
#include <typeinfo>

namespace owlapi {
namespace model {

template<typename T>
class OWLLiteralNumeric
{
public:
    typedef T value_t;

protected:
    T mNumericValue;

    value_t fromString(const std::string& stringValue) {
        try {
            return boost::lexical_cast< value_t >(stringValue);
        } catch(const std::bad_cast& e)
        {
            throw std::runtime_error("owlapi::model::OWLLiteralNumeric::fromString:"
                    " failed to cast '" + stringValue + "' to " +
                    typeid(value_t).name());
        }
    }

    OWLLiteralNumeric(T value)
        : mNumericValue(value)
    {}

    value_t getValue() const { return mNumericValue; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_LITERAL_NUMERIC_HPP
