#include "OWLDataOneOf.hpp"

namespace owlapi {
namespace model {

OWLDataOneOf::OWLDataOneOf(const owlapi::model::IRIList& list)
    : OWLDataRange(OWLDataRange::DATA_ONE_OF)
    , mLiterals()
{
    for(const IRI& iri : list)
    {
        OWLLiteral::Ptr literal = OWLLiteral::create(iri.toString());
        mLiterals.push_back(literal);
    }
}

OWLDataOneOf::OWLDataOneOf(const OWLLiteral::PtrList& literals)
    : OWLDataRange(OWLDataRange::DATA_ONE_OF)
    , mLiterals(literals)
{}

} // end namespace model
} // end namespace owlapi
