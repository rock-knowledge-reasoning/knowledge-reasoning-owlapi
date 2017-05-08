#ifndef OWLAPI_MODEL_OWL_ONTOLOGY_CHANGE_VISITOR_HPP
#define OWLAPI_MODEL_OWL_ONTOLOGY_CHANGE_VISITOR_HPP

#include "../SharedPtr.hpp"
#include "ChangeApplied.hpp"

#define CHANGE_VISITOR_VISIT(X) \
    virtual ChangeApplied visit(const X& change) = 0;

#define NO_IMPL_CHANGE_VISITOR_VISIT(X) \
    virtual ChangeApplied visit(const X& change) { LOG_WARN_S << "No implementation of visitor for : '" #X "'"; return CHANGE_NO_OPERATION; }

namespace owlapi {
namespace model {

class AddAxiom;
class RemoveAxiom;
class SetOntologyID;
class AddImport;
class RemoveImport;
class AddOntologyAnnotation;
class RemoveOntologyAnnotation;

class OWLOntologyChangeVisitor
{
public:
    typedef shared_ptr< OWLOntologyChangeVisitor > Ptr;

    virtual ~OWLOntologyChangeVisitor() {}

    CHANGE_VISITOR_VISIT(AddAxiom);
    CHANGE_VISITOR_VISIT(RemoveAxiom);
    CHANGE_VISITOR_VISIT(SetOntologyID);
    CHANGE_VISITOR_VISIT(AddImport);
    CHANGE_VISITOR_VISIT(RemoveImport);
    CHANGE_VISITOR_VISIT(AddOntologyAnnotation);
    CHANGE_VISITOR_VISIT(RemoveOntologyAnnotation);

};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ONTOLOGY_CHANGE_VISITOR_HPP
