#ifndef OWLAPI_MODEL_OWL_INDIVIDUAL_HPP
#define OWLAPI_MODEL_OWL_INDIVIDUAL_HPP

#include <vector>
#include "OWLPropertyAssertionObject.hpp"
#include "IRI.hpp"

namespace owlapi {
namespace model {

/**
 * \class OWLIndividual
 * \brief Represents a named or anonymous individual
 */
class OWLIndividual : public OWLPropertyAssertionObject
{
public:
    typedef shared_ptr<OWLIndividual> Ptr;
    typedef std::vector<Ptr> PtrList;

    virtual ~OWLIndividual() = default;

    virtual IRI getReferenceID() const { throw std::runtime_error("owlapi::model::OWLIndividual::getReferenceID: not implemented"); }

    virtual bool isAnonymous() const { throw std::runtime_error("owlapi::model::OWLIndividual::isAnonymous: not implemented"); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_INDIVIDUAL_HPP
