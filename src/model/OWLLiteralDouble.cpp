#include "OWLLiteralDouble.hpp"
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <owlapi/Vocabulary.hpp>

namespace owlapi {
namespace model {

OWLLiteralDouble::OWLLiteralDouble(double value)
    : OWLLiteral("", vocabulary::XSD::resolve("double").toString())
    , OWLLiteralNumeric<double>(value)
{
    std::stringstream ss;
    ss << value;
    mValue = ss.str();
}

OWLLiteralDouble::OWLLiteralDouble(const std::string& value)
    : OWLLiteral(value)
    , OWLLiteralNumeric<double>(0.0)
{
    mNumericValue = fromString(mValue);
}

} // end namespace model
} // end namespace owlapi
