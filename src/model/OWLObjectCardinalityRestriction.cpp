#include "OWLObjectCardinalityRestriction.hpp"
#include "OWLObjectMinCardinality.hpp"
#include "OWLObjectMaxCardinality.hpp"
#include "OWLObjectExactCardinality.hpp"
#include "OWLClass.hpp"

#include <base-logging/Logging.hpp>

namespace owlapi {
namespace model {

OWLObjectCardinalityRestriction::OWLObjectCardinalityRestriction(
        const OWLPropertyExpression::Ptr& property,
        uint32_t cardinality,
        const OWLClassExpression::Ptr& qualification,
        OWLCardinalityRestriction::CardinalityRestrictionType restrictionType
)
    : OWLCardinalityRestriction(property, cardinality, restrictionType)
    , OWLQuantifiedObjectRestriction(dynamic_pointer_cast<OWLObjectPropertyExpression>(property), qualification)
{
    if(!property || !getProperty())
    {
        throw std::invalid_argument("owlapi::model::OWLObjectCardinalityRestriction"
                " cannot construct without property");
    }

}

OWLCardinalityRestriction::Ptr OWLObjectCardinalityRestriction::createInstance(
        const OWLObjectPropertyExpression::Ptr& property,
        uint32_t cardinality,
        const OWLClassExpression::Ptr& qualification,
        OWLCardinalityRestriction::CardinalityRestrictionType type
)
{
    if(!property)
    {
        throw std::invalid_argument("owlapi::model::OWLObjectCardinalityRestriction"
                " cannot construct without property");
    }

    switch(type)
    {
        case OWLCardinalityRestriction::MIN:
            return make_shared<OWLObjectMinCardinality>(property, cardinality, qualification);
        case OWLCardinalityRestriction::MAX:
            return make_shared<OWLObjectMaxCardinality>(property, cardinality, qualification);
        case OWLCardinalityRestriction::EXACT:
            return make_shared<OWLObjectExactCardinality>(property, cardinality, qualification);
        default:
            break;
    }
    throw std::invalid_argument("owlapi::model::OWLObjectCardinalityRestriction::createInstance"
            " creation of UNKNOWN cardinality type requested");
}

OWLCardinalityRestriction::Ptr OWLObjectCardinalityRestriction::doClone() const
{
    return createInstance(dynamic_pointer_cast<OWLObjectPropertyExpression>(OWLObjectRestriction::getProperty()), getCardinality(), getFiller(), getCardinalityRestrictionType());
}

OWLObjectCardinalityRestriction::Ptr OWLObjectCardinalityRestriction::intersectionExact(
        const OWLObjectCardinalityRestriction& exact,
        const OWLObjectCardinalityRestriction& b
)
{
    const OWLObjectRestriction* aRestriction = dynamic_cast<const OWLObjectRestriction*>(&exact);
    const OWLObjectRestriction* bRestriction = dynamic_cast<const OWLObjectRestriction*>(&b);

    if(aRestriction->getClassExpressionType() != OWLClassExpression::OBJECT_EXACT_CARDINALITY)
    {
        throw std::invalid_argument("owlapi::model::OWLObjectCardinalityRestriction::intersectionExact:"
                " first argument has be a class expression OBJECT_EXACT_CARDINALITY");
    }

    switch(b.getCardinalityRestrictionType())
    {
        case MIN:
            if(exact.getCardinality() >= b.getCardinality())
            {
                return dynamic_pointer_cast<OWLObjectCardinalityRestriction>(exact.doClone());
            }
            throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction::intersectionExact: incompatible \
                    cardinality restriction of exact and min : " + aRestriction->toString() + " vs. " + bRestriction->toString());
        case MAX:
            if(exact.getCardinality() <= b.getCardinality())
            {
                return dynamic_pointer_cast<OWLObjectCardinalityRestriction>(exact.doClone());
            }
            throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction::intersectionExact: incompatible \
                    cardinality restriction of exact and max : " + aRestriction->toString() + " vs. " + bRestriction->toString());
        case EXACT:
            if(exact.getCardinality() == b.getCardinality())
            {
                return dynamic_pointer_cast<OWLObjectCardinalityRestriction>(exact.doClone());
            }
            throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction::intersectionExact: incompatible \
                    cardinality restriction of exact and exact: " + aRestriction->toString() + " vs. " + bRestriction->toString());
        default:
            throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction::intersectionExact: \
                    cannot handle UNKNOWN type");
    }
}

OWLObjectCardinalityRestriction::Ptr OWLObjectCardinalityRestriction::intersectionMinMax(
        const OWLObjectCardinalityRestriction& a,
        const OWLObjectCardinalityRestriction& b)
{
    const OWLObjectRestriction* aRestriction = dynamic_cast<const OWLObjectRestriction*>(&a);
    const OWLObjectRestriction* bRestriction = dynamic_cast<const OWLObjectRestriction*>(&b);

    if(aRestriction->getClassExpressionType() != OWLClassExpression::OBJECT_MIN_CARDINALITY)
    {
        throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction::intersectionMinMax"
                " first argument is not a min cardinality restriction");
    }
    if(bRestriction->getClassExpressionType() != OWLClassExpression::OBJECT_MAX_CARDINALITY)
    {
        throw std::invalid_argument("owlapi::model::OWLCardinalityRestriction::intersectionMinMax"
                " first argument is not a max cardinality restriction");
    }

    OWLObjectPropertyExpression::Ptr oProperty = dynamic_pointer_cast<OWLObjectPropertyExpression>(aRestriction->getProperty());
    if(a.getCardinality() == b.getCardinality())
    {
        return dynamic_pointer_cast<OWLObjectCardinalityRestriction>( createInstance(oProperty,
                    a.getCardinality(),
                    a.getFiller(),
                    EXACT) );

    } else if(a.getCardinality() > b.getCardinality())
    {
        std::stringstream ss;
        ss << "owlapi::model::OWLCardinalityRestriction::intersection ";
        ss << "incompatible MIN/MAX cardinality restrictions on property ";
        ss << "'" << oProperty->toString() << "' ";
        //ss << " qualification ";
        //ss << "'" << a->getQualification() << "' ";
        ss << " min cardinality: ";
        ss << a.getCardinality();
        ss << " vs. max cardinality: ";
        ss << b.getCardinality();

        throw std::invalid_argument(ss.str());
    }

    return OWLObjectCardinalityRestriction::Ptr();
}

OWLCardinalityRestriction::Ptr OWLObjectCardinalityRestriction::intersection(
        const OWLCardinalityRestriction::Ptr& other
)
{
    if(!other)
    {
        throw std::invalid_argument("owlapi::model::OWLObjectCardinalityRestriction::intersection"
                " cannot intersection with null pointer");
    }

    if(isOverlapping(other))
    {
        CardinalityRestrictionType aType = getCardinalityRestrictionType();
        CardinalityRestrictionType bType = other->getCardinalityRestrictionType();

        OWLObjectCardinalityRestriction::Ptr b = dynamic_pointer_cast<OWLObjectCardinalityRestriction>(other);

        OWLObjectPropertyExpression::Ptr oProperty = dynamic_pointer_cast<OWLObjectPropertyExpression>(OWLObjectRestriction::getProperty());

        // Same cardinality type
        if(aType == bType)
        {
            switch(aType)
            {
                case MIN:
                    return createInstance(oProperty,
                              std::max(getCardinality(), other->getCardinality()),
                              getFiller(),
                              MIN);
                case MAX:
                    return createInstance(oProperty,
                            std::min(getCardinality(), other->getCardinality()),
                            getFiller(),
                            MAX);
                case EXACT:
                    {
                        if(getCardinality() != b->getCardinality())
                        {
                            std::stringstream ss;
                            ss << "owlapi::model::OWLObjectCardinalityRestriction::intersection ";
                            ss << "incompatible EXACT cardinality restrictions on property ";
                            ss << "'" << oProperty->toString() << "' ";
                            ss << " qualification ";
                            ss << "'" << getFiller() << "' ";
                            ss << " cardinality: ";
                            ss << getCardinality();
                            ss << " vs. ";
                            ss << b->getCardinality();

                            throw std::invalid_argument(ss.str());
                        }

                        return createInstance(oProperty,
                                getCardinality(),
                                getFiller(),
                                EXACT);
                    }
                default:
                    throw std::runtime_error("owlapi::model::OWLObjectCardinalityRestriction::intersection  \
                             invalid cardinality restriction of type UNKNOWN");
            }
        } else if(aType == MIN && bType == MAX)
        {
            return intersectionMinMax(*this,*b.get());
        } else if(aType == MAX && bType == MIN)
        {
            return intersectionMinMax(*b.get(),*this);
        } else if(aType == EXACT)
        {
            return intersectionExact(*this,*b.get());
        } else if(bType == EXACT)
        {
            return intersectionExact(*b.get(),*this);
        }
    }

    return OWLObjectCardinalityRestriction::Ptr();
}

bool OWLObjectCardinalityRestriction::isOverlapping(
        const OWLCardinalityRestriction::Ptr& other
)
{
    if(other->isObjectRestriction())
    {
        OWLPropertyExpression::Ptr oProperty = getProperty();
        if(!oProperty)
        {
            throw std::runtime_error("owlapi::model::OWLObjectCardinalityRestriction::isOverlapping:"
                    " failed to cast to object property");
        }

        if(oProperty->toString() != other->getProperty()->toString() )
        {
            LOG_DEBUG_S << "Different properties";
            return false;
        }

        OWLObjectCardinalityRestriction::Ptr otherRestriction = dynamic_pointer_cast<OWLObjectCardinalityRestriction>(other);

        if(getFiller() != otherRestriction->getFiller())
        {
            LOG_DEBUG_S << "Different qualification";
            return false;
        }
        return true;
    }

    LOG_DEBUG_S << "Object restriction does not overlap with data restriction";
    return false;
}

OWLCardinalityRestriction::Ptr OWLObjectCardinalityRestriction::join(const OWLCardinalityRestriction::Ptr& other,
        OperationType operationType)
{
    if(isOverlapping(other))
    {
        if(getCardinalityRestrictionType() == other->getCardinalityRestrictionType())
        {
            uint32_t cardinality = 0;
            switch(operationType)
            {
                case SUM_OP:
                    cardinality = getCardinality() + other->getCardinality();
                    break;
                case MIN_OP:
                    cardinality = std::min(getCardinality(), other->getCardinality());
                    break;
                case MAX_OP:
                    cardinality = std::max(getCardinality(), other->getCardinality());
                    break;
            }

            return getInstance(dynamic_pointer_cast<OWLObjectPropertyExpression>(getProperty()),
                            cardinality,
                            getFiller(),
                            getCardinalityRestrictionType());
        }
    } else {
        LOG_DEBUG_S << "Restrictions are not overlapping: "
            << toString() << std::endl
            << other->toString() << std::endl;
    }
    return OWLCardinalityRestriction::Ptr();
}

std::map<IRI, OWLCardinalityRestriction::MinMax> OWLObjectCardinalityRestriction::getBounds(
        const std::vector<OWLCardinalityRestriction::Ptr>& restrictions
)
{
    owlapi::model::OWLPropertyExpression::Ptr property;
    std::map<IRI, OWLCardinalityRestriction::MinMax> modelCount;

    // We assume a compact list of the query restrictions, but
    // have to generate an intermediate bounded representation for each
    // possible resource model
    std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
    for(; cit != restrictions.end(); ++cit)
    {
        OWLObjectCardinalityRestriction::Ptr restriction = dynamic_pointer_cast<OWLObjectCardinalityRestriction>(*cit);

        // Check if the same property is used for all cardinality restrictions
        // Note: might to too restrictive, but need to verify lateron
        if(!property)
        {
            property = (*cit)->getProperty();
        } else {
            if(property->toString() != (*cit)->getProperty()->toString())
            {
                throw std::invalid_argument("owlapi::model::getBounds cardinality restrictions are inconsistent, i.e. apply to different properties: " + \
                        property->toString() + " vs. " + (*cit)->getProperty()->toString());
            }
        }

        uint32_t cardinality = restriction->getCardinality();

        OWLClassExpression::Ptr classExpression = restriction->getFiller();
        OWLClass::Ptr klass = dynamic_pointer_cast<OWLClass>(classExpression);
        if(!klass)
        {
            throw std::invalid_argument("owlapi::model::OWLObjectCardinalityRestriction::convertToExactMapping:"
                    " can only handle OWLClass (and not complex OWLClassExpression(s))");
        }
        owlapi::model::IRI qualification = klass->getIRI();
        std::pair<uint32_t,uint32_t>& minMax = modelCount[qualification];

        if(restriction->getCardinalityRestrictionType() == owlapi::model::OWLCardinalityRestriction::MAX)
        {
            minMax.first = std::max(static_cast<uint32_t>(0), minMax.first);
            if(minMax.second == 0) // has not been initialized yet
            {
                minMax.second = cardinality;
            } else {
                minMax.second = std::min(cardinality, minMax.second);
            }
        } else if(restriction->getCardinalityRestrictionType() == owlapi::model::OWLCardinalityRestriction::MIN)
        {
            minMax.first = std::max(cardinality, minMax.first);
            minMax.second = std::numeric_limits<uint32_t>::max();
        } else if(restriction->getCardinalityRestrictionType() == owlapi::model::OWLCardinalityRestriction::EXACT)
        {
            minMax.first = cardinality;
            minMax.second = cardinality;
        }
    }

    return modelCount;
}


IRI OWLObjectCardinalityRestriction::getQualification() const
{
    OWLClassExpression::Ptr klassExpression = getFiller();
    if(klassExpression)
    {
        OWLClass::Ptr klass = dynamic_pointer_cast<OWLClass>(klassExpression);
        return klass->getIRI();
    }
    throw std::runtime_error("owlapi::model::OWLObjectCardinalityRestriction::getQualification:"
            " failed to extract qualification");
}





} // end namespace model
} // end namespace owlapi
