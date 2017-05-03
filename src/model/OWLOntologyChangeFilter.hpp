#ifndef OWLAPI_MODEL_OWL_ONTOLOGY_CHANGE_FILTER_HPP
#define OWLAPI_MODEL_OWL_ONTOLOGY_CHANGE_FILTER_HPP

#include "../SharedPtr.hpp"
#include "OWLOntologyChangeVisitor.hpp"

namespace owlapi {
namespace model {

class OWLOntologyChangeFilter : public OWLOntologyChangeVisitor
{
public:
    typedef shared_ptr<OWLOntologyChangeFilter> Ptr;

    NO_IMPL_CHANGE_VISITOR_VISIT(AddAxiom);
    NO_IMPL_CHANGE_VISITOR_VISIT(RemoveAxiom);
    NO_IMPL_CHANGE_VISITOR_VISIT(SetOntologyID);
    NO_IMPL_CHANGE_VISITOR_VISIT(AddImport);
    NO_IMPL_CHANGE_VISITOR_VISIT(RemoveImport);
    NO_IMPL_CHANGE_VISITOR_VISIT(AddOntologyAnnotation);
    NO_IMPL_CHANGE_VISITOR_VISIT(RemoveOntologyAnnotation);

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ONTOLOGY_CHANGE_FILTER_HPP
