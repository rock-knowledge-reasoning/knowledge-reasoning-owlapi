#ifndef OWLAPI_MODEL_OWL_NO_IMPL_AXIOM_RETRACT_VISITOR_HPP
#define OWLAPI_MODEL_OWL_NO_IMPL_AXIOM_RETRACT_VISITOR_HPP

#include "OWLAxiomVisitor.hpp"

namespace owlapi {
namespace model {

class OWLOntology;

class OWLAxiomRetractVisitor : public OWLAxiomVisitor
{
    OWLOntology* mpOntology;

public:
    OWLAxiomRetractVisitor(OWLOntology* ontology);

    virtual ~OWLAxiomRetractVisitor() {}

    AXIOM_VISITOR_VISIT_DECL(OWLDeclarationAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLClassAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLObjectPropertyAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLDataPropertyAxiom);

    //NO_IMPL_AXIOM_VISITOR_VISIT(OWLAssertionAxiom);
    //NO_IMPL_AXIOM_VISITOR_VISIT(OWLHasKeyAxiom);
    //NO_IMPL_AXIOM_VISITOR_VISIT(OWLAnnotationAxiom);

    NO_IMPL_AXIOM_VISITOR_VISIT(OWLSubClassOfAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLEquivalentClassesAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLDisjointClassesAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLDisjointUnionAxiom);

    NO_IMPL_AXIOM_VISITOR_VISIT(OWLEquivalentObjectPropertiesAxiom);
    //NO_IMPL_AXIOM_VISITOR_VISIT(OWLDisjointObjectPropertiesAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLSubObjectPropertyOfAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLObjectPropertyDomainAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLObjectPropertyRangeAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLInverseObjectPropertiesAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLFunctionalObjectPropertyAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLInverseFunctionalObjectPropertyAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLReflexiveObjectPropertyAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLIrreflexiveObjectPropertyAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLSymmetricObjectPropertyAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLAsymmetricObjectPropertyAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLTransitiveObjectPropertyAxiom);

    NO_IMPL_AXIOM_VISITOR_VISIT(OWLSubDataPropertyOfAxiom);
    //NO_IMPL_AXIOM_VISITOR_VISIT(OWLDisjointDataPropertiesAxiom);
    //NO_IMPL_AXIOM_VISITOR_VISIT(OWLEquivalentDataPropertiesAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLFunctionalDataPropertyAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLDataPropertyDomainAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLDataPropertyRangeAxiom);

    AXIOM_VISITOR_VISIT_DECL(OWLClassAssertionAxiom);
    //NO_IMPL_AXIOM_VISITOR_VISIT(OWLSameIndividualAxiom);
    //NO_IMPL_AXIOM_VISITOR_VISIT(OWLDifferentIndividualsAxiom);

    NO_IMPL_AXIOM_VISITOR_VISIT(OWLObjectPropertyAssertionAxiom);
    //NO_IMPL_AXIOM_VISITOR_VISIT(OWLNegativeObjectPropertyAssertion);

    NO_IMPL_AXIOM_VISITOR_VISIT(OWLDataPropertyAssertionAxiom);
    //NO_IMPL_AXIOM_VISITOR_VISIT(OWLNegativeDataPropertyAssertion);

    NO_IMPL_AXIOM_VISITOR_VISIT(OWLSubAnnotationPropertyOfAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLAnnotationPropertyDomainAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLAnnotationPropertyRangeAxiom);
    NO_IMPL_AXIOM_VISITOR_VISIT(OWLAnnotationAssertionAxiom);
    //
    //NO_IMPL_AXIOM_VISITOR_VISIT(OWLSubPropertyChainOf),

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_NO_IMPL_AXIOM_RETRACT_VISITOR_HPP
