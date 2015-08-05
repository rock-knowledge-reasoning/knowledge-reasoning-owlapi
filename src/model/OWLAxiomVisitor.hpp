#ifndef OWLAPI_MODEL_OWL_AXIOM_VISITOR_HPP
#define OWLAPI_MODEL_OWL_AXIOM_VISITOR_HPP

#include <owlapi/model/OWLDeclarationAxiom.hpp>
#include <owlapi/model/OWLClassAxiom.hpp>
#include <owlapi/model/OWLObjectPropertyAxiom.hpp>
#include <owlapi/model/OWLDataPropertyAxiom.hpp>

//#include <owlapi/model/OWLAssertionAxiom.hpp>
//#include <owlapi/model/OWLHasKeyAxiom.hpp>
//#include <owlapi/model/OWLAssertionAxiom.hpp>

#include <owlapi/model/OWLSubClassOfAxiom.hpp>
#include <owlapi/model/OWLEquivalentClassesAxiom.hpp>
#include <owlapi/model/OWLDisjointClassesAxiom.hpp>
#include <owlapi/model/OWLDisjointUnionAxiom.hpp>

#include <owlapi/model/OWLEquivalentObjectPropertiesAxiom.hpp>
//#include <owlapi/model/OWLDisjointObjectPropertiesAxiom.hpp>
#include <owlapi/model/OWLSubObjectPropertyOfAxiom.hpp>
#include <owlapi/model/OWLObjectPropertyDomainAxiom.hpp>
#include <owlapi/model/OWLObjectPropertyRangeAxiom.hpp>
#include <owlapi/model/OWLInverseObjectPropertiesAxiom.hpp>
#include <owlapi/model/OWLFunctionalObjectPropertyAxiom.hpp>
#include <owlapi/model/OWLInverseFunctionalObjectPropertyAxiom.hpp>
#include <owlapi/model/OWLReflexiveObjectPropertyAxiom.hpp>
#include <owlapi/model/OWLIrreflexiveObjectPropertyAxiom.hpp>
#include <owlapi/model/OWLSymmetricObjectPropertyAxiom.hpp>
#include <owlapi/model/OWLAsymmetricObjectPropertyAxiom.hpp>
#include <owlapi/model/OWLTransitiveObjectPropertyAxiom.hpp>

#include <owlapi/model/OWLSubDataPropertyOfAxiom.hpp>
//#include <owlapi/model/OWLDisjointDataPropertiesAxiom.hpp>
//#include <owlapi/model/OWLEquivalentDataPropertiesAxiom.hpp>
#include <owlapi/model/OWLFunctionalDataPropertyAxiom.hpp>
#include <owlapi/model/OWLDataPropertyDomainAxiom.hpp>
#include <owlapi/model/OWLDataPropertyRangeAxiom.hpp>

#include <owlapi/model/OWLClassAssertionAxiom.hpp>
//#include <owlapi/model/OWLSameIndividualAxiom.hpp>
//#include <owlapi/model/OWLDifferentIndividualsAxiom.hpp>

#include <owlapi/model/OWLObjectPropertyAssertionAxiom.hpp>
//#include <owlapi/model/OWLNegativeObjectPropertyAssertion.hpp>

#include <owlapi/model/OWLDataPropertyAssertionAxiom.hpp>
//#include <owlapi/model/OWLNegativeDataPropertyAssertion.hpp>

//#include <owlapi/model/OWLSubAnnotationPropertyOfAxiom.hpp>
//#include <owlapi/model/OWLAnnotationPropertyDomainAxiom.hpp>
//#include <owlapi/model/OWLAnnotationPropertyRangeAxiom.hpp>
//#include <owlapi/model/OWLAnnotationAssertionAxiom.hpp>

//#include <owlapi/model/OWLSubPropertyChainOf.hpp>

#include <base/Logging.hpp>

#define AXIOM_VISITOR_VISIT(X) \
    virtual void visit(const X& axiom) { LOG_INFO_S << "OWLAxiomVisitor::visit not implemented for '" #X "' "; }

namespace owlapi {
namespace model {

/**
 * An interface for objects that can accept visits from axioms.
 *
 * Implementation based on the visitor pattern
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

    //AXIOM_VISITOR_VISIT(OWLSubAnnotationPropertyOfAxiom);
    //AXIOM_VISITOR_VISIT(OWLAnnotationPropertyDomainAxiom);
    //AXIOM_VISITOR_VISIT(OWLAnnotationPropertyRangeAxiom);
    //AXIOM_VISITOR_VISIT(OWLAnnotationAssertionAxiom);
    //
    //AXIOM_VISITOR_VISIT(OWLSubPropertyChainOf),
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_AXIOM_VISITOR_HPP
