#include "../Vocabulary.hpp"

namespace owlapi {
namespace vocabulary {

bool OWL::isQualifiedCardinality(const owlapi::model::IRI& predicate)
{
    return predicate == qualifiedCardinality() ||
        predicate == maxQualifiedCardinality() ||
        predicate == minQualifiedCardinality();
}

} // end namespace vocabulary
} // end namespace owlapi
