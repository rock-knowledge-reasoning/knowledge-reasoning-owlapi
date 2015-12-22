#ifndef OWLAPI_MODEL_OWL_PROPERTY_EXPRESSION_HPP
#define OWLAPI_MODEL_OWL_PROPERTY_EXPRESSION_HPP

#include <owlapi/model/OWLObject.hpp>
#include <owlapi/model/OWLPropertyRange.hpp>
#include <owlapi/model/OWLClassExpression.hpp>

namespace owlapi {
namespace model {

/**
 *  Expressions represent complex notions in the domain being described. For example, a class expression describes a set of individuals in terms of the restrictions on the individuals' characteristics. 
 */
class OWLPropertyExpression : public OWLObject
{
public:
    typedef boost::shared_ptr<OWLPropertyExpression> Ptr;

    virtual ~OWLPropertyExpression();

    virtual bool isDataPropertyExpression() const;
    virtual bool isObjectPropertyExpression() const;

    virtual void addRange(const OWLPropertyRange::Ptr& range) { mRanges.push_back(range); }
    void setRanges(const std::vector<OWLPropertyRange::Ptr>& ranges);

    virtual std::vector<OWLPropertyRange::Ptr> getRanges() const { return mRanges; }

    void addDomain(const OWLClassExpression::Ptr& classExpression) { mDomains.push_back(classExpression); }
    void setDomains(const std::vector<OWLClassExpression::Ptr>& classExpressions) { mDomains = classExpressions; }

    std::vector<OWLClassExpression::Ptr> getDomains() const { return mDomains; }

    virtual std::string toString() const { throw std::runtime_error("OWLPropertyExpression::toString: not implemented"); }

protected:
    std::vector<OWLPropertyRange::Ptr> mRanges;
    std::vector<OWLClassExpression::Ptr> mDomains;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_PROPERTY_EXPRESSION_HPP
