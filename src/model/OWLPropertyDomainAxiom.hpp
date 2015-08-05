#ifndef OWLAPI_MODEL_OWL_PROPERTY_DOMAIN_AXIOM_HPP
#define OWLAPI_MODEL_OWL_PROPERTY_DOMAIN_AXIOM_HPP

namespace owlapi {
namespace model {

template<typename T>
class OWLPropertyDomainAxiom
{
public:
    typedef T domain_t;

    OWLPropertyDomainAxiom(const T& domain)
        : mDomain(domain)
    {}

    virtual ~OWLPropertyDomainAxiom() {}

    domain_t getDomain() const { return mDomain; }

private:
    domain_t mDomain;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_PROPERTY_DOMAIN_AXIOM_HPP
