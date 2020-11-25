#ifndef OWLAPI_MODEL_OWL_NAMED_INDIVIDUAL_HPP
#define OWLAPI_MODEL_OWL_NAMED_INDIVIDUAL_HPP

#include "OWLIndividual.hpp"
#include "OWLLogicalEntity.hpp"

namespace owlapi {
namespace model {

class OWLNamedIndividual : public OWLIndividual, public OWLLogicalEntity
{
public:
    typedef shared_ptr<OWLNamedIndividual> Ptr;

    OWLNamedIndividual( const IRI& iri)
        : OWLIndividual()
        , OWLLogicalEntity(iri, OWLEntity::NAMED_INDIVIDUAL)
    {}

    virtual ~OWLNamedIndividual() {}

    bool isAnonymous() const { return false; }

    IRI getReferenceID() const { return getIRI(); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_NAMED_INDIVIDUAL_HPP
