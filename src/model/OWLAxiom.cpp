#include "OWLAxiom.hpp"
#include <boost/assign/list_of.hpp>
#include <owlapi/OWLApi.hpp>

#define VISIT_CASE_STATEMENT(X,Y) \
        case X: \
        { \
            const Y* ptr = dynamic_cast<const Y*>(this); \
            if(!ptr) \
            { \
                throw std::runtime_error("owlapi::model::Axiom: could not cast axiom to " #Y " "); \
            } \
            visitor->visit( *ptr ); \
            break; \
        }

namespace owlapi {
namespace model {

std::map<OWLAxiom::AxiomType, std::string> OWLAxiom::AxiomTypeTxt = boost::assign::map_list_of
                (UNKOWN,                            "UNKNOWN")
                (Declaration,                       "Declaration")
                (ClassAxiom,                        "ClassAxiom")
                (ObjectPropertyAxiom,               "ObjectPropertyAxiom")
                (DataPropertyAxiom,                 "DataPropertyAxiom")
                (DatatypeDefinition,                "DatatypeDefinition")
                (Assertion,                         "Assertion")
                (HasKey,                            "HasKey")
                (AnnotationAxiom,                   "AnnotationAxiom")
                (SubClassOf,                        "SubClassOf")
                (EquivalentClasses,                 "EquivalentClasses")
                (DisjointClasses,                   "DisjointClasses")
                (DisjointUnion,                     "DisjointUnion")
                (EquivalentObjectProperties,        "EquivalentObjectProperties")
                (DisjointObjectProperties,          "DisjointObjectProperties")
                (SubObjectPropertyOf,               "SubObjectPropertyOf")
                (ObjectPropertyDomain,              "ObjectPropertyDomain")
                (ObjectPropertyRange,               "ObjectPropertyRange")
                (InverseObjectProperties,           "InverseObjectProperties")
                (FunctionalObjectProperty,          "FunctionalObjectProperty")
                (ReflexiveObjectProperty,           "ReflexiveObjectProperty")
                (InverseFunctionalObjectProperty,   "InverseFunctionalObjectProperty")
                (IrreflexiveObjectProperty,         "IrreflexiveObjectProperty")
                (SymmetricObjectProperty,           "SymmetricObjectProperty")
                (TransitiveObjectProperty,          "TransitiveObjectProperty")
                (AsymmetricObjectProperty,          "AsymmetricObjectProperty")
                (SubDataPropertyOf,                 "SubDataPropertyOf")
                (DisjointDataProperties,            "DisjointDataProperties")
                (EquivalentDataProperties,          "EquivalentDataProperties")
                (FunctionalDataProperty,            "FunctionalDataProperties")
                (DataPropertyDomain,                "DataPropertyDomain")
                (DataPropertyRange,                 "DataPropertyRange")
                (ClassAssertion,                    "ClassAssertion")
                (SameIndividual,                    "SameIndividual")
                (DifferentIndividuals,              "DifferentIndividuals")
                (ObjectPropertyAssertion,           "ObjectPropertyAssertion")
                (NegativeObjectPropertyAssertion,   "NegativeObjectPropertyAssertion")
                (DataPropertyAssertion,             "DataPropertyAssertion")
                (NegativeDataPropertyAssertion,     "NegativeDataPropertyAssertion")
                (SubAnnotationPropertyOf,           "SubAnnotationPropertyOf")
                (AnnotationPropertyDomain,          "AnnotationPropertyDomain")
                (AnnotationPropertyRange,           "AnnotationPropertyRange")
                (AnnotationAssertion,               "AnnotationAssertion")
                (SubPropertyChainOf,                "SubPropertyChainOf");

std::string OWLAxiom::toString() const
{
    return "OWLAxiom: " + AxiomTypeTxt[mAxiomType];
}

void OWLAxiom::accept(OWLAxiomVisitor* visitor) const
{
    switch(getAxiomType())
    {
        // keep case statements in sync with OWLAxiomVisitor implementations
        VISIT_CASE_STATEMENT(Declaration, OWLDeclarationAxiom);
        VISIT_CASE_STATEMENT(ClassAxiom, OWLClassAxiom);
        VISIT_CASE_STATEMENT(ObjectPropertyAxiom, OWLObjectPropertyAxiom);
        VISIT_CASE_STATEMENT(DataPropertyAxiom, OWLObjectPropertyAxiom);

        //VISIT_CASE_STATEMENT(Assertion, OWLAssertionAxiom);
        //VISIT_CASE_STATEMENT(HasKey, OWLHasKeyAxiom);
        //VISIT_CASE_STATEMENT(AnnotationAxiom, OWLAnnotationAxiom);

        VISIT_CASE_STATEMENT(SubClassOf, OWLSubClassOfAxiom);
        VISIT_CASE_STATEMENT(EquivalentClasses, OWLEquivalentClassesAxiom);
        VISIT_CASE_STATEMENT(DisjointClasses, OWLDisjointClassesAxiom);
        VISIT_CASE_STATEMENT(DisjointUnion, OWLDisjointUnionAxiom);

        VISIT_CASE_STATEMENT(EquivalentObjectProperties, OWLEquivalentObjectPropertiesAxiom);
        //VISIT_CASE_STATEMENT(DisjointObjectProperties, OWLDisjointObjectPropertiesAxiom);
        VISIT_CASE_STATEMENT(SubObjectPropertyOf, OWLSubObjectPropertyOfAxiom);
        //VISIT_CASE_STATEMENT(ObjectPropertyDomain, OWLObjectPropertyDomainAxiom);
        //VISIT_CASE_STATEMENT(ObjectPropertyRange, OWLObjectPropertyRangeAxiom);
        VISIT_CASE_STATEMENT(InverseObjectProperties, OWLInverseObjectPropertiesAxiom);
        VISIT_CASE_STATEMENT(FunctionalObjectProperty, OWLFunctionalObjectPropertyAxiom);
        // VISIT_CASE_STATEMENT(ReflexiveObjectProperty, OWLReflexiveObjectPropertyAxiom);
        // VISIT_CASE_STATEMENT(InverseFunctionalObjectProperty, OWLInverseFunctionalObjectPropertyAxiom);
        // VISIT_CASE_STATEMENT(IrreflexiveObjectProperty, OWLIrreflexiveObjectPropertyAxiom);
        VISIT_CASE_STATEMENT(SymmetricObjectProperty, OWLSymmetricObjectPropertyAxiom);
        VISIT_CASE_STATEMENT(TransitiveObjectProperty, OWLTransitiveObjectPropertyAxiom);
        // VISIT_CASE_STATEMENT(AsymmetricObjectProperty, OWLAsymmetricObjectPropertyAxiom);

        VISIT_CASE_STATEMENT(SubDataPropertyOf, OWLSubDataPropertyOfAxiom);
        //VISIT_CASE_STATEMENT(DisjointDataProperties, OWLDisjointDataPropertiesAxiom);
        //VISIT_CASE_STATEMENT(EquivalentDataProperties, OWLEquivalentDataPropertiesAxiom);
        VISIT_CASE_STATEMENT(FunctionalDataProperty, OWLFunctionalDataPropertyAxiom);
        //VISIT_CASE_STATEMENT(DataPropertyDomain, OWLDataPropertyDomainAxiom);
        //VISIT_CASE_STATEMENT(DataPropertyRange, OWLDataPropertyRangeAxiom);

        //VISIT_CASE_STATEMENT(ClassAssertion, OWLClassAssertionAxiom);
        //VISIT_CASE_STATEMENT(SameIndividual, OWLSameIndividualAxiom);
        //VISIT_CASE_STATEMENT(DifferentIndividuals, OWLDifferentIndividualsAxiom);

        VISIT_CASE_STATEMENT(ObjectPropertyAssertion, OWLObjectPropertyAssertionAxiom);
        //VISIT_CASE_STATEMENT(NegativeObjectPropertyAssertion, OWLNegativeObjectPropertyAssertion);

        VISIT_CASE_STATEMENT(DataPropertyAssertion, OWLDataPropertyAssertionAxiom);
        //VISIT_CASE_STATEMENT(NegativeObjectPropertyAssertion, OWLNegativeDataPropertyAssertion);

        //VISIT_CASE_STATEMENT(SubAnnotationPropertyOf, OWLSubAnnotationPropertyOfAxiom);
        //VISIT_CASE_STATEMENT(AnnotationPropertyDomain, OWLAnnotationPropertyDomainAxiom);
        //VISIT_CASE_STATEMENT(AnnotationPropertyRange, OWLAnnotationPropertyRangeAxiom);
        //VISIT_CASE_STATEMENT(AnnotationAssertion, OWLAnnotationAssertionAxiom);

        //VISIT_CASE_STATEMENT(SubPropertyChainOf, OWLSubPropertyChainOf),

        default:
            break;
    }
}


} // end namespace model
} // end namespace owlapi
