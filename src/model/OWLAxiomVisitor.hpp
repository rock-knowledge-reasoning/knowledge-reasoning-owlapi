#ifndef OWLAPI_MODEL_OWL_AXIOM_VISITOR_HPP
#define OWLAPI_MODEL_OWL_AXIOM_VISITOR_HPP

#include "OWLDeclarationAxiom.hpp"
#include "OWLClassAxiom.hpp"
#include "OWLObjectPropertyAxiom.hpp"
#include "OWLDataPropertyAxiom.hpp"

//#include "OWLAssertionAxiom.hpp"
//#include "OWLHasKeyAxiom.hpp"
//#include "OWLAnnotationAxiom.hpp"

#include "OWLSubClassOfAxiom.hpp"
#include "OWLEquivalentClassesAxiom.hpp"
#include "OWLDisjointClassesAxiom.hpp"
#include "OWLDisjointUnionAxiom.hpp"

#include "OWLEquivalentObjectPropertiesAxiom.hpp"
//#include "OWLDisjointObjectPropertiesAxiom.hpp"
#include "OWLSubObjectPropertyOfAxiom.hpp"
#include "OWLObjectPropertyDomainAxiom.hpp"
#include "OWLObjectPropertyRangeAxiom.hpp"
#include "OWLInverseObjectPropertiesAxiom.hpp"
#include "OWLFunctionalObjectPropertyAxiom.hpp"
#include "OWLInverseFunctionalObjectPropertyAxiom.hpp"
#include "OWLReflexiveObjectPropertyAxiom.hpp"
#include "OWLIrreflexiveObjectPropertyAxiom.hpp"
#include "OWLSymmetricObjectPropertyAxiom.hpp"
#include "OWLAsymmetricObjectPropertyAxiom.hpp"
#include "OWLTransitiveObjectPropertyAxiom.hpp"

#include "OWLSubDataPropertyOfAxiom.hpp"
//#include "OWLDisjointDataPropertiesAxiom.hpp"
//#include "OWLEquivalentDataPropertiesAxiom.hpp"
#include "OWLFunctionalDataPropertyAxiom.hpp"
#include "OWLDataPropertyDomainAxiom.hpp"
#include "OWLDataPropertyRangeAxiom.hpp"

#include "OWLClassAssertionAxiom.hpp"
//#include "OWLSameIndividualAxiom.hpp"
//#include "OWLDifferentIndividualsAxiom.hpp"

#include "OWLObjectPropertyAssertionAxiom.hpp"
//#include "OWLNegativeObjectPropertyAssertion.hpp"

#include "OWLDataPropertyAssertionAxiom.hpp"
//#include "OWLNegativeDataPropertyAssertion.hpp"

#include "OWLSubAnnotationPropertyOfAxiom.hpp"
#include "OWLAnnotationAssertionAxiom.hpp"
#include "OWLAnnotationPropertyDomainAxiom.hpp"
#include "OWLAnnotationPropertyRangeAxiom.hpp"

//#include "OWLSubPropertyChainOf.hpp"

#include <base-logging/Logging.hpp>

#define AXIOM_VISITOR_VISIT(X) \
    virtual void visit(const X& axiom) = 0;

#define AXIOM_VISITOR_VISIT_DECL(X) \
    virtual void visit(const X& axiom);

#define NO_IMPL_AXIOM_VISITOR_VISIT(X) \
    virtual void visit(const X& axiom) { LOG_WARN_S << "No implementation of visitor for : '" #X "'"; }


namespace owlapi {
namespace model {

/**
 * An interface for objects that can accept visits from axioms.
 *
 * Implementation based on the visitor pattern
 *\verbatim

    // Visit all axioms that are part of an ontology
    const owlapi::model::OWLAxiom::PtrList& axioms = ontology->getAxioms();

    OWLAxiom::PtrList::const_iterator cit = axioms.begin();
    for(; cit != axioms.end(); ++cit)
    {
        const OWLAxiom::Ptr& axiom = *cit;
        axiom->accept(&visitor);
    }
 \endverbatim
 *
 */
class OWLAxiomVisitor
{
public:
    virtual ~OWLAxiomVisitor() {}

    AXIOM_VISITOR_VISIT(OWLDeclarationAxiom);
    AXIOM_VISITOR_VISIT(OWLClassAxiom);
    AXIOM_VISITOR_VISIT(OWLObjectPropertyAxiom);
    AXIOM_VISITOR_VISIT(OWLDataPropertyAxiom);

    //AXIOM_VISITOR_VISIT(OWLAssertionAxiom);
    //AXIOM_VISITOR_VISIT(OWLHasKeyAxiom);
    //AXIOM_VISITOR_VISIT(OWLAnnotationAxiom);

    AXIOM_VISITOR_VISIT(OWLSubClassOfAxiom);
    AXIOM_VISITOR_VISIT(OWLEquivalentClassesAxiom);
    AXIOM_VISITOR_VISIT(OWLDisjointClassesAxiom);
    AXIOM_VISITOR_VISIT(OWLDisjointUnionAxiom);

    AXIOM_VISITOR_VISIT(OWLEquivalentObjectPropertiesAxiom);
    //AXIOM_VISITOR_VISIT(OWLDisjointObjectPropertiesAxiom);
    AXIOM_VISITOR_VISIT(OWLSubObjectPropertyOfAxiom);
    AXIOM_VISITOR_VISIT(OWLObjectPropertyDomainAxiom);
    AXIOM_VISITOR_VISIT(OWLObjectPropertyRangeAxiom);
    AXIOM_VISITOR_VISIT(OWLInverseObjectPropertiesAxiom);
    AXIOM_VISITOR_VISIT(OWLFunctionalObjectPropertyAxiom);
    AXIOM_VISITOR_VISIT(OWLInverseFunctionalObjectPropertyAxiom);
    AXIOM_VISITOR_VISIT(OWLReflexiveObjectPropertyAxiom);
    AXIOM_VISITOR_VISIT(OWLIrreflexiveObjectPropertyAxiom);
    AXIOM_VISITOR_VISIT(OWLSymmetricObjectPropertyAxiom);
    AXIOM_VISITOR_VISIT(OWLAsymmetricObjectPropertyAxiom);
    AXIOM_VISITOR_VISIT(OWLTransitiveObjectPropertyAxiom);

    AXIOM_VISITOR_VISIT(OWLSubDataPropertyOfAxiom);
    //AXIOM_VISITOR_VISIT(OWLDisjointDataPropertiesAxiom);
    //AXIOM_VISITOR_VISIT(OWLEquivalentDataPropertiesAxiom);
    AXIOM_VISITOR_VISIT(OWLFunctionalDataPropertyAxiom);
    AXIOM_VISITOR_VISIT(OWLDataPropertyDomainAxiom);
    AXIOM_VISITOR_VISIT(OWLDataPropertyRangeAxiom);

    AXIOM_VISITOR_VISIT(OWLClassAssertionAxiom);
    //AXIOM_VISITOR_VISIT(OWLSameIndividualAxiom);
    //AXIOM_VISITOR_VISIT(OWLDifferentIndividualsAxiom);

    AXIOM_VISITOR_VISIT(OWLObjectPropertyAssertionAxiom);
    //AXIOM_VISITOR_VISIT(OWLNegativeObjectPropertyAssertion);

    AXIOM_VISITOR_VISIT(OWLDataPropertyAssertionAxiom);
    //AXIOM_VISITOR_VISIT(OWLNegativeDataPropertyAssertion);

    AXIOM_VISITOR_VISIT(OWLSubAnnotationPropertyOfAxiom);
    AXIOM_VISITOR_VISIT(OWLAnnotationPropertyDomainAxiom);
    AXIOM_VISITOR_VISIT(OWLAnnotationPropertyRangeAxiom);
    AXIOM_VISITOR_VISIT(OWLAnnotationAssertionAxiom);
    //
    //AXIOM_VISITOR_VISIT(OWLSubPropertyChainOf),
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_AXIOM_VISITOR_HPP
