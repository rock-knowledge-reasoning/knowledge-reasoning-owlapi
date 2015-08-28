#include "OWLObject.hpp"
#include "OWLAxiomVisitor.hpp"

namespace owlapi {
namespace model {

bool OWLObject::isBottomEntity() const
{
    throw NotImplemented("owlapi::model::OWLObject::isBottomEntity");
}

bool OWLObject::isTopEntity() const
{
    throw NotImplemented("owlapi::model::OWLObject::isTopEntity");
}

} // end namespace model
} // end namespace owlapi
