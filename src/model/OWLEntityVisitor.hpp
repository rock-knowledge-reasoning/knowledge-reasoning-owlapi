#ifndef OWLAPI_MODEL_OWL_ENTITY_VISITOR_HPP
#define OWLAPI_MODEL_OWL_ENTITY_VISITOR_HPP

namespace owlapi {
namespace model {

class OWLClass;
class OWLNamedIndividual;
class OWLDataProperty;
class OWLObjectProperty;
class OWLAnnotationProperty;

class OWLEntityVisitor
{
public:
    virtual ~OWLEntityVisitor() {}

    virtual void visit(const OWLClass* klass) = 0;
    virtual void visit(const OWLNamedIndividual* individual) = 0;
    virtual void visit(const OWLDataProperty* property) = 0;
    virtual void visit(const OWLObjectProperty* property) = 0;
    virtual void visit(const OWLAnnotationProperty* property) = 0;


};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_ENTITIY_VISITOR_HPP
