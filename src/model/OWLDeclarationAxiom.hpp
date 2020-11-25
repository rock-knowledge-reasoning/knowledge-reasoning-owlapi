#ifndef OWLAPI_MODEL_OWL_DECLARATION_AXIOM_HPP
#define OWLAPI_MODEL_OWL_DECLARATION_AXIOM_HPP

#include "OWLEntity.hpp"
#include "OWLAxiom.hpp"

namespace owlapi {
namespace model {

/**
 *  Represents a Declaration in the OWL 2 Specification.
 *
 *  A declaration axiom declares an entity in an ontology. It doesn't affect the
 *  logical meaning of the ontology.
 *  \see http://www.w3.org/TR/2009/REC-owl2-syntax-20091027/#Entity_Declarations_and_Typing
 */
class OWLDeclarationAxiom : public OWLAxiom
{
    OWLEntity::Ptr mEntity;

public:
    OWLDeclarationAxiom(OWLEntity::Ptr entity, OWLAnnotationList annotations = OWLAnnotationList())
        : OWLAxiom(Declaration, annotations)
        , mEntity(entity)
    {}

    virtual ~OWLDeclarationAxiom() {}

    OWLEntity::Ptr getEntity() const { return mEntity; }

    std::string toString() const;

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DECLARATION_AXIOM_HPP
