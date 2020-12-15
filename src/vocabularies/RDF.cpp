#include "../Vocabulary.hpp"

namespace owlapi {
namespace vocabulary {

owlapi::model::IRISet RDF::mDataTypes = {
    CompoundLiteral(),
    JSON(),
    HTML(),
    PlainLiteral(),
    XMLLiteral()
};

bool RDF::isDatatype(const owlapi::model::IRI& iri)
{
    return mDataTypes.find(iri) != mDataTypes.end();
}

} // end namespace vocabulary
} // end namespace owlapi
