#ifndef OWLAPI_MODEL_OWL_LITERAL_NUMERIC_HPP
#define OWLAPI_MODEL_OWL_LITERAL_NUMERIC_HPP

#include <string>
#include <boost/lexical_cast.hpp>

namespace owlapi {
namespace model {

template<typename T>
class OWLLiteralNumeric
{
public:
    typedef T value_t;

protected:
    T mNumericValue;

    value_t fromString(const std::string& stringValue) { return boost::lexical_cast< value_t >(stringValue); }

    OWLLiteralNumeric(T value)
        : mNumericValue(value)
    {}

    value_t getValue() const { return mNumericValue; }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_LITERAL_NUMERIC_HPP
