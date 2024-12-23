#include "OWLOntologyAsk.hpp"
#include "../KnowledgeBase.hpp"
#include "../Vocabulary.hpp"
#include "OWLObjectIntersectionOf.hpp"

namespace owlapi {
namespace model {

OWLOntologyAsk::OWLOntologyAsk(OWLOntology::Ptr ontology)
    : mpOntology(ontology)
{}


OWLClassExpression::Ptr OWLOntologyAsk::getOWLClassExpression(const IRI& iri) const
{

    try {
        OWLAnonymousClassExpression::Ptr klass = getOWLAnonymousClassExpression(iri);
        return dynamic_pointer_cast<OWLClassExpression>(klass);
    } catch(...)
    {
        ///
    }

    try {
        OWLClass::Ptr klass = getOWLClass(iri);
        return dynamic_pointer_cast<OWLClassExpression>(klass);
    } catch(...)
    {
        ///
    }
    throw std::invalid_argument("owlapi::model::OWLOntologyAsk::getClassExpression: "
            "could not find a class expression for iri '" + iri.toString() + "'");
}
const IRI& OWLOntologyAsk::getOWLClassExpressionIRI(const OWLClassExpression::Ptr& expression) const
{
    OWLAnonymousClassExpression::Ptr aClassExpression = dynamic_pointer_cast<OWLAnonymousClassExpression>(expression);

    if(aClassExpression)
    {
        std::map<IRI, OWLAnonymousClassExpression::Ptr>::const_iterator it = std::find_if(
                mpOntology->mAnonymousClassExpressions.begin(),
                mpOntology->mAnonymousClassExpressions.end(),
                [&aClassExpression](const std::pair<IRI, OWLAnonymousClassExpression::Ptr>& other)
                {
                    return other.second == aClassExpression;
                });
        if(it != mpOntology->mAnonymousClassExpressions.end())
        {
            return it->first;
        }
        throw std::runtime_error("owlapi::model::OWLOntologyAsk::getOWLClassExpressionIRI:"
                " failed to identify IRI for AnonymousClassExpression."
                " Did you already define it via OWLOntologyTell?"
        );
    }

    OWLClass::Ptr aClass = dynamic_pointer_cast<OWLClass>(expression);
    if(aClass)
    {
        std::map<IRI, OWLClass::Ptr>::const_iterator it = std::find_if(
                mpOntology->mClasses.begin(),
                mpOntology->mClasses.end(),
                [&aClass](const std::pair<IRI, OWLClass::Ptr>& other)
                {
                    return other.second == aClass;
                });

        if(it != mpOntology->mClasses.end())
        {
            return it->first;
        }
        throw std::runtime_error(
            "owlapi::model::OWLOntologyAsk::getOWLClassExpressionIRI:"
            " failed to identify IRI for Class " + aClass->getIRI().toString() + "' "
            " Did you already define it via OWLOntologyTell?"
        );
    }

    throw std::invalid_argument(
            "owlapi::model::OWLOntologyAsk::getOWLClassExpressionIRI:"
            " given expression is neither Class nor AnonymousClassExpression"
    );
}

OWLClass::Ptr OWLOntologyAsk::getOWLClass(const IRI& iri) const
{
    std::map<IRI, OWLClass::Ptr>::const_iterator it = mpOntology->mClasses.find(iri);
    if(it != mpOntology->mClasses.end())
    {
        return it->second;
    }

    throw std::runtime_error("OWLOntologyAsk::getOWLClass: '" + iri.toString() + "' is not a known OWLClass");
}

OWLAnonymousClassExpression::Ptr OWLOntologyAsk::getOWLAnonymousClassExpression(const IRI& iri) const
{
    std::map<IRI, OWLAnonymousClassExpression::Ptr>::const_iterator it = mpOntology->mAnonymousClassExpressions.find(iri);
    if(it != mpOntology->mAnonymousClassExpressions.end())
    {
        return it->second;
    }

    throw std::runtime_error("OWLOntologyAsk::getOWLAnonymousClassExpression: '" + iri.toString() + "' is not a known OWLAnonymousClassExpression");
}


OWLIndividual::Ptr OWLOntologyAsk::getOWLIndividual(const IRI& iri) const
{
    try {
        return getOWLAnonymousIndividual(iri);
    } catch(const std::runtime_error& e)
    {}

    try {
        return getOWLNamedIndividual(iri);
    } catch(const std::runtime_error& e)
    {}

    throw std::runtime_error("OWLOntologyAsk::getOWLIndividual: '" + iri.toString() + "' is not a known OWLIndividual");
}

OWLAnonymousIndividual::Ptr OWLOntologyAsk::getOWLAnonymousIndividual(const IRI& iri) const
{
    std::map<IRI, OWLAnonymousIndividual::Ptr>::const_iterator it = mpOntology->mAnonymousIndividuals.find(iri);
    if(it != mpOntology->mAnonymousIndividuals.end())
    {
        return it->second;
    }
    throw std::runtime_error("OWLOntologyAsk::getAnonymousIndividual: '" + iri.toString() + "' is not a known OWLAnonymousIndividual");
}

OWLNamedIndividual::Ptr OWLOntologyAsk::getOWLNamedIndividual(const IRI& iri) const
{
    std::map<IRI, OWLNamedIndividual::Ptr>::const_iterator it = mpOntology->mNamedIndividuals.find(iri);
    if(it != mpOntology->mNamedIndividuals.end())
    {
        return it->second;
    }

    throw std::runtime_error("OWLOntologyAsk::getNamedIndividual: '" + iri.toString() + "' is not a known OWLNamedIndividual");
}

OWLObjectProperty::Ptr OWLOntologyAsk::getOWLObjectProperty(const IRI& iri) const
{
    std::map<IRI, OWLObjectProperty::Ptr>::const_iterator it = mpOntology->mObjectProperties.find(iri);
    if(it != mpOntology->mObjectProperties.end())
    {
        return it->second;
    }
    throw std::runtime_error("OWLOntologyAsk::getOWLObjectProperty: '" + iri.toString() + "' is not a known OWLObjectProperty");
}

OWLDataProperty::Ptr OWLOntologyAsk::getOWLDataProperty(const IRI& iri) const
{
    std::map<IRI, OWLDataProperty::Ptr>::const_iterator it = mpOntology->mDataProperties.find(iri);
    if(it != mpOntology->mDataProperties.end())
    {
        return it->second;
    }

    throw std::runtime_error("OWLOntologyAsk::getOWLDataProperty: '" + iri.toString() + "' is not a known OWLDataProperty");
}

OWLAnnotationProperty::Ptr OWLOntologyAsk::getOWLAnnotationProperty(const IRI& iri) const
{
    std::map<IRI, OWLAnnotationProperty::Ptr>::const_iterator it = mpOntology->mAnnotationProperties.find(iri);
    if(it != mpOntology->mAnnotationProperties.end())
    {
        return it->second;
    }

    throw std::runtime_error("OWLOntologyAsk::getOWLAnnotationProperty: '" + iri.toString() + "' is not a known OWLAnnotationProperty");
}

std::vector<OWLCardinalityRestriction::Ptr>
OWLOntologyAsk::getCardinalityRestrictions(const owlapi::model::OWLClassExpression::Ptr& ce,
        const IRI& objectProperty,
        bool includeAncestors) const
{
    owlapi::model::OWLProperty::Ptr property;
    if(objectProperty != IRI())
    {
        try {
            property = getOWLObjectProperty(objectProperty);
        } catch(const std::runtime_error& e)
        {
            throw std::invalid_argument("owlapi::model::OWLOntologyAsk::getCardinalityRestrictions: "
                    "could not retrieve restriction since object property '" + objectProperty.toString() + "'"
                    " does not exist -- " + e.what());
        }
    }

    // In order to find a restriction for a given class
    //    1. check class assertions for individuals
    // -> 2. check subclass axioms for classes
    //      - find superclass definitions, collect all restrictions
    //        - (including the ones for the superclasses -- identify restrictions)
    std::vector<OWLSubClassOfAxiom::Ptr> subclassAxioms = mpOntology->mSubClassAxiomBySubPosition[ce];

    std::vector<OWLCardinalityRestriction::Ptr> restrictionsFromAxioms;
    for(const OWLSubClassOfAxiom::Ptr& subclassAxiomPtr : subclassAxioms)
    {
        OWLClassExpression::Ptr superClass = subclassAxiomPtr->getSuperClass();
        OWLCardinalityRestriction::Ptr restriction = dynamic_pointer_cast<OWLCardinalityRestriction>(superClass);
        if(restriction)
        {
            restrictionsFromAxioms.push_back(restriction->clone());
        }
    }

    std::vector<OWLClassExpression::Ptr> equivalentClasses = allEquivalentClassExpressions(ce);
    for(const OWLClassExpression::Ptr& equivalentClass : equivalentClasses)
    {
        switch(equivalentClass->getClassExpressionType())
        {
            case OWLClassExpression::OBJECT_EXACT_CARDINALITY:
            case OWLClassExpression::OBJECT_MAX_CARDINALITY:
            case OWLClassExpression::OBJECT_MIN_CARDINALITY:
            {
                OWLCardinalityRestriction::Ptr r =
                    dynamic_pointer_cast<OWLCardinalityRestriction>(equivalentClass);
                if(!r)
                {
                    throw
                        std::runtime_error("owlapi::model::OWLOntologyAsk::getCardinalityRestrictions:"
                                " internal error: equivalent class cast to object cardinality failed");
                }
                restrictionsFromAxioms.push_back(r->clone());
                break;
            }
            case OWLClassExpression::OBJECT_INTERSECTION_OF:
            {
                OWLObjectIntersectionOf::Ptr intersectionOf =
                    dynamic_pointer_cast<OWLObjectIntersectionOf>(equivalentClass);
                for(OWLClassExpression::Ptr element : intersectionOf->getOperands())
                {
                    switch(element->getClassExpressionType())
                    {
                        case OWLClassExpression::OBJECT_MAX_CARDINALITY:
                        case OWLClassExpression::OBJECT_MIN_CARDINALITY:
                        case OWLClassExpression::OBJECT_EXACT_CARDINALITY:
                        {
                            OWLCardinalityRestriction::Ptr r =
                                dynamic_pointer_cast<OWLCardinalityRestriction>(element);
                            if(!r)
                            {
                                throw
                                    std::runtime_error("owlapi::model::OWLOntologyAsk::getCardinalityRestrictions:"
                                            " internal error in object intersection: cast to cardinality restriction failed");
                            }

                            restrictionsFromAxioms.push_back(r->clone());
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
            default:
                break;
        }
    }

    std::vector<OWLCardinalityRestriction::Ptr> restrictions;
    for(OWLCardinalityRestriction::Ptr restriction : restrictionsFromAxioms)
    {
        if(!restriction)
        {
            throw
                std::runtime_error("owlapi::model::OWLOntologyAsk::getCardinalityRestrictions:"
                        " internal error: object is not an OWLCardinalityRestriction");
        }
        switch(restriction->getClassExpressionType())
        {
            case OWLClassExpression::OBJECT_MIN_CARDINALITY:
            case OWLClassExpression::OBJECT_MAX_CARDINALITY:
            case OWLClassExpression::OBJECT_EXACT_CARDINALITY:
            {
                // Only handle restriction which are matching the object
                // property
                if(property && restriction->getProperty() != property)
                {
                    // property is not equal
                    break;
                }

                OWLObjectCardinalityRestriction::Ptr oRestriction =
                    dynamic_pointer_cast<OWLObjectCardinalityRestriction>(restriction);
                std::vector<OWLCardinalityRestriction::Ptr>
                    inheritedRestrictions = getCardinalityRestrictions(oRestriction->getFiller(), objectProperty);

                if(inheritedRestrictions.empty())
                {
                    restrictions.push_back(restriction->clone());
                } else {

                    restrictions =
                        OWLCardinalityRestrictionOps::intersection(restrictions,
                                inheritedRestrictions);
                }
                break;
            }
            case OWLClassExpression::DATA_EXACT_CARDINALITY:
            case OWLClassExpression::DATA_MIN_CARDINALITY:
            case OWLClassExpression::DATA_MAX_CARDINALITY:
                throw std::invalid_argument("owlapi::model::OWLOntologyAsk::getCardinalityRestrictions: cannot handle data restriction, when dealing with class qualification");
                break;
            default:
                break;
        }
    }

    if(includeAncestors)
    {
        const IRI& klassIri = getOWLClassExpressionIRI(ce);
        for(const IRI& ancestor : ancestors(klassIri))
        {
            std::vector<OWLCardinalityRestriction::Ptr> inheritedRestrictions =
                getCardinalityRestrictions(ancestor, objectProperty, false);
            restrictions = OWLCardinalityRestrictionOps::intersection(restrictions, inheritedRestrictions);
        }
    }

    return OWLCardinalityRestriction::compact(restrictions);
}

std::vector<OWLCardinalityRestriction::Ptr>
OWLOntologyAsk::getCardinalityRestrictions(const IRI& iri,
        const IRI& objectProperty,
        bool includeAncestors) const
{
    // In order to find a restriction for a given class
    //    1. check class assertions for individuals
    // -> 2. check subclass axioms for classes
    //      - find superclass definitions, collect all restrictions
    //        - (including the ones for the superclasses -- identify restrictions)
    std::pair<OWLCardinalityRestriction::PtrList, bool> result = mpOntology->mQueryCache.getCardinalityRestrictions(iri, objectProperty);

    if(result.second)
    {
        return result.first;
    } else {
        OWLClass::Ptr klass = getOWLClass(iri);
        OWLCardinalityRestriction::PtrList restrictions =
            getCardinalityRestrictions(klass, objectProperty, includeAncestors);
        if(includeAncestors)
        {
            mpOntology->mQueryCache.cacheCardinalityRestrictions(iri, objectProperty, restrictions);
        }
        return restrictions;
    }
}

std::vector<OWLCardinalityRestriction::Ptr> OWLOntologyAsk::getCardinalityRestrictions(const std::vector<IRI>& klasses, const IRI& objectProperty,
        OWLCardinalityRestrictionOps::OperationType operationType) const
{
    std::pair<OWLCardinalityRestriction::PtrList, bool> result =
        mpOntology->mQueryCache.getCardinalityRestrictions(klasses,
                objectProperty,
                operationType);
    if(result.second)
    {
        return result.first;
    } else {
        std::vector<IRI>::const_iterator cit = klasses.begin();
        std::vector<OWLCardinalityRestriction::Ptr> restrictions;

        for(; cit != klasses.end(); ++cit)
        {
            IRI iri = *cit;
            std::vector<OWLCardinalityRestriction::Ptr> klassRestrictions = getCardinalityRestrictions(iri, objectProperty);
            restrictions = OWLCardinalityRestrictionOps::join(restrictions, klassRestrictions, operationType);
        }
        mpOntology->mQueryCache.cacheCardinalityRestrictions(klasses,
                objectProperty,
                operationType,
                restrictions);
        return restrictions;
    }
}

std::vector<OWLCardinalityRestriction::Ptr> OWLOntologyAsk::getCardinalityRestrictions(const IRI& klass,
        const IRI& objectProperty,
        const IRI& qualificationKlass,
        bool direct) const
{
    IRIList klasses;
    klasses.push_back(klass);
    return getCardinalityRestrictions(klasses, objectProperty, qualificationKlass, direct);
}

std::vector<OWLCardinalityRestriction::Ptr> OWLOntologyAsk::getCardinalityRestrictions(const IRIList& klasses, const IRI& objectProperty, const IRI& qualificationKlass, bool direct) const
{
    IRIList qualificationKlasses;
    qualificationKlasses.push_back(qualificationKlass);
    return getCardinalityRestrictions(klasses, objectProperty, qualificationKlasses, direct);
}

std::vector<OWLCardinalityRestriction::Ptr> OWLOntologyAsk::getCardinalityRestrictions(const IRIList& klasses, const IRI& objectProperty, const IRIList& qualificationKlasses, bool direct) const
{
    std::vector<OWLCardinalityRestriction::Ptr> filteredRestrictions;
    std::vector<OWLCardinalityRestriction::Ptr> restrictions = getCardinalityRestrictions(klasses, objectProperty);
    std::vector<OWLCardinalityRestriction::Ptr>::const_iterator rit = restrictions.begin();
    for(; rit != restrictions.end(); ++rit)
    {
        const OWLObjectCardinalityRestriction::Ptr& restriction = dynamic_pointer_cast<OWLObjectCardinalityRestriction>(*rit);

        if(!restriction)
        {
            throw std::invalid_argument("owlapi::model::OWLOntologyAsk::getCardinalityRestrictions:"
                    " failed to handle data restriction - expected object restriction");
        }

        std::vector<IRI>::const_iterator qit = qualificationKlasses.begin();
        for(; qit != qualificationKlasses.end(); ++qit)
        {
            const IRI& allowedQualification = *qit;
            OWLClass::Ptr klass = dynamic_pointer_cast<OWLClass>(restriction->getFiller());
            if(!klass)
            {
                throw std::invalid_argument("owlapi::model::OWLOntologyAsk::getCardinalityRestrictions"
                        " currently only supporting qualification with plain klasses");
            }

            const IRI& qualification = klass->getIRI();
            if(qualification == allowedQualification
                    || (!direct && isSubClassOf(qualification, allowedQualification)) )
            {
                filteredRestrictions.push_back(restriction);
            }
        }
    }
    return filteredRestrictions;
}

std::vector<OWLCardinalityRestriction::Ptr> OWLOntologyAsk::getCardinalityRestrictionsForTarget(const IRI& klass,
        const IRI& objectProperty,
        const IRI& targetKlass) const
{
    std::vector<OWLCardinalityRestriction::Ptr> filteredRestrictions;

    owlapi::model::OWLProperty::Ptr property;
    try {
        property = getOWLObjectProperty(objectProperty);
    } catch(const std::runtime_error& e)
    {
        throw std::invalid_argument("owlapi::model::OWLOntologyAsk::getCardinalityRestrictionsForTarget: "
                "could not retrieve restriction since object property '" + objectProperty.toString() + "'"
                " does not exist -- " + e.what());
    }

    std::vector<OWLCardinalityRestriction::Ptr> restrictions = getCardinalityRestrictions(klass);
    for(const OWLCardinalityRestriction::Ptr& r : restrictions)
    {
        if( r->getProperty() == property)
        {
            OWLObjectCardinalityRestriction::Ptr oRestriction = dynamic_pointer_cast<OWLObjectCardinalityRestriction>(r);

            OWLClassExpression::Ptr klassExpression = oRestriction->getFiller();
            if(isSubClassOf(targetKlass, klassExpression))
            {
                filteredRestrictions.push_back(r);
            }
        }
    }
    return filteredRestrictions;
}


bool OWLOntologyAsk::isSubClassOf(const IRI& iri, const IRI& superclass) const
{
    std::pair<bool, bool> result = mpOntology->mQueryCache.isSubClassOf(iri, superclass);
    if(result.second)
    {
        return result.first;
    } else {
        result.first = mpOntology->kb()->isSubClassOf(iri, superclass);
        result.second = true;

        mpOntology->mQueryCache.cacheIsSubClassOf(iri, superclass,
                result.first);

        return result.first;
    }
}

bool OWLOntologyAsk::isDirectSubClassOf(const IRI& iri, const IRI& superclass) const
{
    IRIList list = allSubClassesOf(superclass, true);
    return list.end() != std::find(list.begin(), list.end(), iri);
}

bool OWLOntologyAsk::isSubClassOf(const OWLClassExpression::Ptr& subclass, const OWLClassExpression::Ptr& superclass) const
{
    IRI subclassIRI = getOWLClassExpressionIRI(subclass);
    IRI superclassIRI = getOWLClassExpressionIRI(superclass);

    return isSubClassOf(subclassIRI, superclassIRI);
}

bool OWLOntologyAsk::isSubClassOf(const IRI& subclass, const OWLClassExpression::Ptr& superclass) const
{
    IRI superclassIRI = getOWLClassExpressionIRI(superclass);
    return isSubClassOf(subclass, superclassIRI);
}

bool OWLOntologyAsk::isOWLClass(const IRI& iri) const
{
    std::map<IRI, OWLClass::Ptr>::const_iterator it = mpOntology->mClasses.find(iri);
    return it != mpOntology->mClasses.end();
}

bool OWLOntologyAsk::isOWLAnonymousClassExpression(const IRI& iri) const
{
    std::map<IRI, OWLAnonymousClassExpression::Ptr>::const_iterator it = mpOntology->mAnonymousClassExpressions.find(iri);
    return it != mpOntology->mAnonymousClassExpressions.end();
}

bool OWLOntologyAsk::isOWLIndividual(const IRI& iri) const
{
    try {
        mpOntology->getIndividual(iri);
        return true;
    } catch(...)
    {
        return false;
    }
}

bool OWLOntologyAsk::isOWLAnonymousIndividual(const IRI& iri) const
{
    try {
        mpOntology->getAnonymousIndividual(iri);
        return true;
    } catch(...)
    {
        return false;
    }
}

bool OWLOntologyAsk::isRDFProperty(const IRI& iri) const
{
    return mpOntology->mRDFProperties.find(iri) != mpOntology->mRDFProperties.end();
}

IRIList OWLOntologyAsk::allInstancesOf(const IRI& classType, bool direct) const
{
    return mpOntology->kb()->allInstancesOf(classType, direct);
}

IRIList OWLOntologyAsk::allSubClassesOf(const IRI& classType, bool direct) const
{
    return mpOntology->kb()->allSubClassesOf(classType, direct);
}

std::vector<IRIList> OWLOntologyAsk::allSubClassesOfWithDistance(const IRI& classType) const
{
    std::vector<IRIList> list;
    IRIList allDirectSubclasses = allSubClassesOf(classType, true);
    list.push_back(allDirectSubclasses);

    while(!allDirectSubclasses.empty())
    {
        IRIList subclasses;
        for(const IRI& iri : allDirectSubclasses)
        {
            IRIList directSubclasses = allSubClassesOf(iri, true);
            subclasses.insert(subclasses.begin(), directSubclasses.begin(), directSubclasses.end());
        }
        list.push_back(subclasses);
        allDirectSubclasses = subclasses;
    }
    return list;
}

IRIList OWLOntologyAsk::allUnderivedSubClassesOf(const IRI& classType) const
{
    IRIList noChildrenSubclass;
    IRIList subclasses = allSubClassesOf(classType, false);
    IRIList::const_iterator cit = subclasses.begin();
    for(; cit != subclasses.end(); ++cit)
    {
        const IRI& subclass = *cit;
        IRIList subclassesOfSubclass = allSubClassesOf(subclass, true);
        if(subclassesOfSubclass.empty())
        {
            noChildrenSubclass.push_back(subclass);
        }
    }
    return noChildrenSubclass;
}

IRIList OWLOntologyAsk::allInstances() const
{
    return mpOntology->kb()->allInstances();
}

IRIList OWLOntologyAsk::allClasses(bool excludeBottomClass) const
{
    return mpOntology->kb()->allClasses(excludeBottomClass);
}

IRIList OWLOntologyAsk::allEquivalentClasses(const IRI& klass) const
{
    return mpOntology->kb()->allEquivalentClasses(klass);
}

std::vector<OWLClassExpression::Ptr> OWLOntologyAsk::allEquivalentClassExpressions(const IRI& klass) const
{
    OWLClassExpression::Ptr klassExpression = getOWLClassExpression(klass);
    if(!klassExpression)
    {
        throw std::invalid_argument("owlapi::model::equivalentClasses: failed to"
                " get class expression for '" + klass.toString() + "'");
    }
    return allEquivalentClassExpressions(klassExpression);
}


std::vector<OWLClassExpression::Ptr> OWLOntologyAsk::allEquivalentClassExpressions(const OWLClassExpression::Ptr& klassExpression) const
{
    OWLClassExpression::PtrList allClasses;

    for(const OWLAxiom::Ptr& axiom :
            mpOntology->mAxiomsByType[OWLAxiom::EquivalentClasses])
    {
        const OWLEquivalentClassesAxiom::Ptr& equivalentClassesAxiom =
            dynamic_pointer_cast<OWLEquivalentClassesAxiom>(axiom);

        if(equivalentClassesAxiom->contains(klassExpression))
        {
            OWLClassExpression::PtrList equivalentClasses =
                equivalentClassesAxiom->getEquivalentClasses();

            equivalentClasses.erase(std::remove_if(equivalentClasses.begin(), equivalentClasses.end(),
                    [klassExpression](const OWLClassExpression::Ptr& other)
                    {
                        return other == klassExpression;
                    }), equivalentClasses.end());

            allClasses.insert(allClasses.end(), equivalentClasses.begin(),
                    equivalentClasses.end());
        }
    }
    return allClasses;
}

IRIList OWLOntologyAsk::allRDFProperties() const
{
    return IRIList(mpOntology->mRDFProperties.begin(), mpOntology->mRDFProperties.end());
}

IRIList OWLOntologyAsk::allObjectProperties() const
{
    IRIList list;
    std::map<IRI, OWLObjectProperty::Ptr>::const_iterator cit = mpOntology->mObjectProperties.begin();
    for(; cit != mpOntology->mObjectProperties.end(); ++cit)
    {
        list.push_back(cit->first);
    }
    return list;
}

IRIList OWLOntologyAsk::allDataProperties() const
{
    IRIList list;
    std::map<IRI, OWLDataProperty::Ptr>::const_iterator cit = mpOntology->mDataProperties.begin();
    for(; cit != mpOntology->mDataProperties.end(); ++cit)
    {
        list.push_back(cit->first);
    }
    return list;
}

IRIList OWLOntologyAsk::allDataTypes() const
{
    IRIList list;
    for(const auto& p : mpOntology->kb()->dataTypes())
    {
        list.push_back(p.first);
    }
    return list;
}

IRIList OWLOntologyAsk::allAnnotationProperties() const
{
    IRIList list;
    std::map<IRI, OWLAnnotationProperty::Ptr>::const_iterator cit = mpOntology->mAnnotationProperties.begin();
    for(; cit != mpOntology->mAnnotationProperties.end(); ++cit)
    {
        list.push_back(cit->first);
    }
    return list;
}

bool OWLOntologyAsk::isInstanceOf(const IRI& instance, const IRI& klass) const
{
    return mpOntology->kb()->isInstanceOf(instance, klass);
}

bool OWLOntologyAsk::isRelatedTo(const IRI& instance, const IRI& relationProperty, const IRI& otherInstance) const
{
    return mpOntology->kb()->isRelatedTo(instance, relationProperty, otherInstance);
}

IRIList OWLOntologyAsk::allTypesOf(const IRI& instance, bool direct) const
{
    return mpOntology->kb()->typesOf(instance, direct);
}

IRI OWLOntologyAsk::typeOf(const IRI& instance) const
{
    return mpOntology->kb()->typeOf(instance);
}

IRIList OWLOntologyAsk::allRelatedInstances(const IRI& instance, const IRI& relationProperty, const IRI& klass) const
{
    return mpOntology->kb()->allRelatedInstances(instance, relationProperty, klass);
}

IRIList OWLOntologyAsk::allInverseRelatedInstances(const IRI& instance, const IRI& relationProperty, const IRI& klass) const
{
    return mpOntology->kb()->allInverseRelatedInstances(instance, relationProperty, klass);
}


OWLAnnotationValue::Ptr OWLOntologyAsk::getAnnotationValue(const IRI& instance,
        const IRI& annotationProperty,
        bool includeAncestors) const
{
    for(const auto& p : mpOntology->mAnnotationAxioms)
    {
        const OWLAnnotationProperty::Ptr& aProperty = p.first;
        const std::vector<OWLAxiom::Ptr>& axioms = p.second;
        if(aProperty->getIRI() != annotationProperty)
        {
            continue;
        }

        for(const OWLAxiom::Ptr& axiom : axioms)
        {
            OWLAnnotationAssertionAxiom::Ptr assertionAxiom = dynamic_pointer_cast<OWLAnnotationAssertionAxiom>(axiom);
            if(assertionAxiom)
            {
                OWLAnnotationSubject::Ptr subject = assertionAxiom->getSubject();
                switch(subject->getObjectType())
                {
                    case OWLObject::IRIType:
                    {
                        IRI target = *dynamic_pointer_cast<IRI>(subject).get();
                        if(target == instance)
                        {
                            return assertionAxiom->annotationValue();
                        }
                        break;
                    }
                    case OWLObject::AnonymousIndividual:
                        if(dynamic_pointer_cast<OWLAnonymousIndividual>(subject)->getReferenceID() == instance)
                        {
                            return assertionAxiom->annotationValue();
                        }
                        break;
                    default:
                        throw std::invalid_argument("owlapi::model::OWLOntologyAsk::getAnnotationValue:"
                                " encountered unknown type for AnnotationSubject");
                }
            }
        }
        throw std::invalid_argument("owlapi::model::OWLOntologyAsk::getAnnotationValue:"
                " failed to extract annotation '" +
                annotationProperty.toString() +
                "' for " +
                instance.toString()
                );

    }
    return OWLAnnotationValue::Ptr();
}

OWLLiteral::Ptr OWLOntologyAsk::getDataValue(const IRI& instance,
        const IRI& dataProperty,
        bool includeAncestors) const
{
    std::string msg;
    try {
        reasoner::factpp::DataValue dataValue = mpOntology->kb()->getDataValue(instance, dataProperty);

        std::string valueRepresentation = dataValue.getValue();
        if(dataValue.getType().empty())
        {
            valueRepresentation += "^^" + dataValue.getType();
        }
        return OWLLiteral::create(valueRepresentation);
    } catch(const std::runtime_error& e)
    {
        // ignore
    }

    // check for parent value when punning is active
    // and perform a greedy match
    if(includeAncestors && isOWLClass(instance))
    {
        IRIList parentClasses = ancestors(instance);
        LOG_DEBUG_S << "For instance: " << instance
            << " check datavalue : " << dataProperty
            << " of ancestors: " << parentClasses;
        for(const IRI& parentClass : parentClasses)
        {
            try {
                OWLLiteral::Ptr literal = getDataValue(parentClass, dataProperty, false);
                return literal;
            } catch(const std::exception& e)
            {
                // ignore
            }
        }
        throw std::runtime_error("owlapi::model:OWLOntologyAsk::getDataValue: "
                "meta modelling instance " + instance.toQuotedString() + " has no value related via "
                + dataProperty.toQuotedString());
    } else {
        throw std::runtime_error("owlapi::model:OWLOntologyAsk::getDataValue: "
                "instance " + instance.toQuotedString() + " has no value related via "
                + dataProperty.toQuotedString());
    }
}

IRIList OWLOntologyAsk::getDataPropertyDomain(const IRI& dataProperty, bool direct) const
{
    return mpOntology->kb()->getDataPropertyDomain(dataProperty, direct);
}

OWLDataRange::PtrList OWLOntologyAsk::getDataRange(const IRI& dataProperty) const
{
    std::map<IRI, OWLDataProperty::Ptr>::const_iterator cit = mpOntology->mDataProperties.find(dataProperty);
    if(cit != mpOntology->mDataProperties.end())
    {
        const OWLDataProperty::Ptr& property = cit->second;
        return property->getDataRanges();
    }
    throw std::invalid_argument("owlapi::model::OWLOntologyAsk::getDataRange: "
            " no data property '" + dataProperty.toString() + "' found");
}

OWLDataType OWLOntologyAsk::getDataType(const IRI& dataProperty, const std::string& value) const
{
    OWLDataRange::PtrList dataRanges = getDataRange(dataProperty);
    for(const OWLDataRange::Ptr& range : dataRanges)
    {
        OWLDataRange::Type rangeType = range->getDataRangeType();
        switch(rangeType)
        {
            case OWLDataRange::DATATYPE:
            case OWLDataRange::DATA_ONE_OF:
                return OWLDataType::fromRange(range, value);
            case OWLDataRange::DATA_UNION_OF:
            case OWLDataRange::DATA_COMPLEMENT_OF:
            case OWLDataRange::DATA_INTERSECTION_OF:
            case OWLDataRange::DATATYPE_RESTRICTION:
                throw std::invalid_argument("owlapi::model::OWLOntologyAsk::getDataType: "
                        " data range '" + OWLDataRange::TypeTxt[rangeType] + "' for '" + dataProperty.toString() + "' is unsupported");
        }
    }

    throw std::invalid_argument("owlapi::model::OWLOntologyAsk::getDataType: "
            " no data property '" + dataProperty.toString() + "' found");
}

IRIList OWLOntologyAsk::getObjectPropertyDomain(const IRI& objectProperty, bool direct) const
{
    return mpOntology->kb()->getObjectPropertyDomain(objectProperty, direct);
}

IRIList OWLOntologyAsk::ancestors(const IRI& iri, bool direct) const
{
    if(isOWLClass(iri))
    {
        return mpOntology->kb()->allAncestorsOf(iri, direct);
    } else if(isObjectProperty(iri))
    {
        return mpOntology->kb()->allAncestorObjectProperties(iri, direct);

    } else if(isDataProperty(iri))
    {
        return mpOntology->kb()->allAncestorDataProperties(iri, direct);
    } else {
        throw std::invalid_argument("owlapi::model::OWLOntology::ancestors: '"
                + iri.toString() + "' is not a known class, object or data property");
    }
}

bool OWLOntologyAsk::areEquivalent(const IRI& klassA, const IRI& klassB) const
{
    return mpOntology->kb()->isEquivalentClass(klassA, klassB);
}

bool OWLOntologyAsk::isDatatype(const IRI& iri) const
{
    return mpOntology->kb()->isDatatype(iri);
}

bool OWLOntologyAsk::isObjectProperty(const IRI& property) const
{
    try {
        mpOntology->kb()->getObjectProperty(property);
        return true;
    } catch(const std::exception& e)
    {
        return false;
    }
}

bool OWLOntologyAsk::isDataProperty(const IRI& property) const
{
    try {
        mpOntology->kb()->getDataProperty(property);
        return true;
    } catch(const std::exception& e)
    {
        return false;
    }
}

bool OWLOntologyAsk::isAnnotationProperty(const IRI& property) const
{
    return mpOntology->mAnnotationProperties.find(property) != mpOntology->mAnnotationProperties.end();
}

IRIList OWLOntologyAsk::getObjectPropertiesForDomain(const IRI& domain) const
{
    IRIList validProperties;
    IRIList objectProperties = mpOntology->kb()->allObjectProperties();
    IRIList::const_iterator cit = objectProperties.begin();
    for(; cit != objectProperties.end(); ++cit)
    {
        const IRI& property = *cit;
        IRIList domains = domainOf(property);
        if( isSubClassOfIntersection(domain, domains) )
        {
            validProperties.push_back(property);
        }
    }
    return validProperties;
}

IRIList OWLOntologyAsk::getDataPropertiesForDomain(const IRI& domain) const
{
    IRIList validProperties;
    IRIList dataProperties = mpOntology->kb()->allDataProperties();
    IRIList::const_iterator cit = dataProperties.begin();
    for(; cit != dataProperties.end(); ++cit)
    {
        const IRI& property = *cit;
        IRIList domains = domainOf(property);
        if(isSubClassOfIntersection(domain, domains) )
        {
            validProperties.push_back(property);
        }
    }
    return validProperties;
}


IRISet OWLOntologyAsk::getRelatedObjectProperties(const IRI& instance, bool needInverse) const
{
    return mpOntology->kb()->getRelatedObjectProperties(instance, needInverse);
}

IRISet OWLOntologyAsk::getRelatedDataProperties(const IRI& instance) const
{
    return mpOntology->kb()->getRelatedDataProperties(instance);
}

bool OWLOntologyAsk::isSubClassOfIntersection(const IRI& klass, const IRIList& intersection) const
{
    // TODO: Proper check on intersection
    IRIList::const_iterator cit = intersection.begin();
    for(; cit != intersection.end(); ++cit)
    {
        if(isSubClassOf(klass, *cit))
        {
            return true;
        }
    }

    return false;
}

IRIList OWLOntologyAsk::domainOf(const IRI& iri, bool direct) const
{
    IRIList domains;
    if(isObjectProperty(iri))
    {
        return getObjectPropertyDomain(iri, direct);
    } else if(isDataProperty(iri))
    {
        return getDataPropertyDomain(iri, direct);
    }

    throw std::invalid_argument("owlapi::model::OWLOntologyAsk::domainOf:"
            " property '" + iri.toString() + "' is not known");
}

IRIList OWLOntologyAsk::imports() const
{
    return mpOntology->mDirectImportsDocuments;
}

} // end namespace model
} // end namespace owlapi
