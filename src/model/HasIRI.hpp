#ifndef OWLAPI_MODEL_HAS_IRI_HPP
#define OWLAPI_MODEL_HAS_IRI_HPP

#include "../Exceptions.hpp"
#include "IRI.hpp"

namespace owlapi {
namespace model {

class HasIRI
{
public:
    virtual ~HasIRI() {}

    virtual IRI getIRI() const { throw NotImplemented("owlapi::model::HasIRI::getIRI()"); }
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_HAS_IRI_HPP


