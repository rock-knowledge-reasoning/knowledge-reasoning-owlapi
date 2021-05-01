#include "KnowledgeBase.hpp"

#include <base-logging/Logging.hpp>

#include <factpp/Kernel.h>
#include <factpp/tOntologyPrinterLISP.h>

#include "Vocabulary.hpp"
#include "vocabularies/OWL.hpp"
#include "model/OWLFacetRestriction.hpp"

using namespace owlapi::reasoner::factpp;
using namespace owlapi::model;

namespace owlapi {

std::map<KnowledgeBase::PropertyType, std::string> KnowledgeBase::PropertyTypeTxt = {
    { KnowledgeBase::UNKNOWN_PROPERTY_TYPE, "UNKNOWN PROPERTY TYPE"},
    { KnowledgeBase::OBJECT, "OBJECT"},
    { KnowledgeBase::DATA, "DATA"}
};

std::map<KnowledgeBase::EntityType, std::string> KnowledgeBase::EntityTypeTxt = {
    { KnowledgeBase::UNKNOWN_ENTITY_TYPE, "UNKNOWN ENTITY TYPE"},
    { KnowledgeBase::CLASS, "CLASS"},
    { KnowledgeBase::INSTANCE, "INSTANCE"},
    { KnowledgeBase::OBJECT_PROPERTY, "OBJECT_RELATION"},
    { KnowledgeBase::DATA_PROPERTY, "DATA_RELATION"}
};

IRIList KnowledgeBase::getResult(const Actor& actor, const IRI& filter) const
{
    IRIList list;
    Actor::Array1D result;

    actor.getFoundData(result);
    Actor::Array1D::const_iterator cit = result.begin();
    for(; cit != result.end(); ++cit)
    {
        const ClassifiableEntry* entry = *cit;
        IRI iri = IRI::create(entry->getName());
        if(iri != filter)
        {
            if(iri == IRI("TOP"))
            {
                list.push_back(vocabulary::OWL::Thing());
            } else if(iri == IRI("BOTTOM"))
            {
                list.push_back(vocabulary::OWL::Nothing());
            } else {
                list.push_back(iri);
            }
        }
    }

    return list;
}

TExpressionManager* KnowledgeBase::getExpressionManager()
{
    return mKernel->getExpressionManager();
}

const TExpressionManager* KnowledgeBase::getExpressionManager() const
{
    return mKernel->getExpressionManager();
}

ClassExpression KnowledgeBase::getClass(const IRI& klass) const
{
    IRIClassExpressionMap::const_iterator cit = mClasses.find(klass);
    if(cit == mClasses.end())
    {
        throw std::invalid_argument("KnowledgeBase::getClass: Class '" + klass.toString() + "' does not exist");
    }

    return cit->second;
}

ClassExpression KnowledgeBase::getClassLazy(const IRI& klass)
{
    try {
        return getClass(klass);
    } catch(const std::invalid_argument& e)
    {
        TDLConceptExpression* f_class = getExpressionManager()->Concept(klass.toString());
        ClassExpression expression(f_class);
        mClasses[klass] = expression;
        return expression;
    }
}

InstanceExpression KnowledgeBase::getInstance(const IRI& instance) const
{
    IRIInstanceExpressionMap::const_iterator cit = mInstances.find(instance);
    if(cit == mInstances.end())
    {
        throw std::invalid_argument("KnowledgeBase::getInstance: Instance '" + instance.toString() + "' does not exist");
    }

    return cit->second;
}

InstanceExpression KnowledgeBase::getInstanceLazy(const IRI& instance)
{
    try {
        return getInstance(instance);
    } catch(const std::invalid_argument& e)
    {
        TDLIndividualExpression* f_individual = getExpressionManager()->Individual(instance.toString());
        InstanceExpression expression(f_individual);
        mInstances[instance] = expression;
        return expression;
    }
}

ObjectPropertyExpression KnowledgeBase::getObjectProperty(const IRI& property) const
{
    IRIObjectPropertyExpressionMap::const_iterator cit = mObjectProperties.find(property);
    if(cit == mObjectProperties.end())
    {
        throw std::invalid_argument("KnowledgeBase::getObjectProperty: ObjectProperty '" + property.toString() + "' does not exist");
    }
    return cit->second;
}

ObjectPropertyExpression KnowledgeBase::getObjectPropertyLazy(const IRI& property)
{
    try {
        return getObjectProperty(property);
    } catch(const std::invalid_argument& e)
    {
        TDLObjectRoleExpression* f_property = getExpressionManager()->ObjectRole(property.toString());
        ObjectPropertyExpression expression(f_property);
        mObjectProperties[property] = expression;
        return expression;
    }
}

IRIList KnowledgeBase::getObjectPropertyDomain(const IRI& property, bool direct) const
{
    ObjectPropertyExpression e_property = getObjectProperty(property);
    IRIList classes;

    Actor actor;
    actor.needConcepts();
    mKernel->getORoleDomain( e_property.get(), direct, actor);

    return getResult(actor);
}

IRIList KnowledgeBase::getObjectPropertyRange(const IRI& property, bool direct) const
{
    ObjectPropertyExpression e_property = getObjectProperty(property);
    IRIList classes;

    Actor actor;
    actor.needConcepts();
    mKernel->getRoleRange( e_property.get(), direct, actor);

    return getResult(actor);
}

DataPropertyExpression KnowledgeBase::getDataProperty(const IRI& property) const
{
    IRIDataPropertyExpressionMap::const_iterator cit = mDataProperties.find(property);
    if(cit == mDataProperties.end())
    {
        throw std::invalid_argument("KnowledgeBase::getDataProperty: DataProperty '" + property.toString() + "' does not exist");
    }
    return cit->second;
}

DataPropertyExpression KnowledgeBase::getDataPropertyLazy(const IRI& property)
{
    try {
        return getDataProperty(property);
    } catch(const std::invalid_argument& e)
    {
        TDLDataRoleExpression* f_property = getExpressionManager()->DataRole(property.toString());
        DataPropertyExpression expression(f_property);
        mDataProperties[property] = expression;
        return expression;
    }
}

KnowledgeBase::KnowledgeBase()
    : mKernel(new ReasoningKernel())
{
    mKernel->setVerboseOutput(false);
    mKernel->newKB();
    mKernel->setTopBottomRoleNames (
            vocabulary::OWL::topObjectProperty().toString().c_str(),
            vocabulary::OWL::bottomObjectProperty().toString().c_str(),
            vocabulary::OWL::topDataProperty().toString().c_str(),
            vocabulary::OWL::bottomDataProperty().toString().c_str());

    // registry
    // default data types
    mDataTypes["string"] = getExpressionManager()->getStrDataType();
    mDataTypes["int"] = getExpressionManager()->getIntDataType();
    mDataTypes["double"] = getExpressionManager()->getRealDataType();
    mDataTypes["bool"] = getExpressionManager()->getBoolDataType();
    mDataTypes["time"] = getExpressionManager()->getTimeDataType();


     using namespace owlapi::vocabulary;

     mDataTypes[vocabulary::RDF::PlainLiteral()] = getExpressionManager()->getStrDataType();
     mDataTypes[XSD::resolve("double")] = getExpressionManager()->getRealDataType();
     mDataTypes[XSD::resolve("float")] = getExpressionManager()->getRealDataType();

     mDataTypes[XSD::resolve("long")] = getExpressionManager()->getIntDataType();
     mDataTypes[XSD::resolve("int")] = getExpressionManager()->getIntDataType();
     mDataTypes[XSD::resolve("short")] = getExpressionManager()->getIntDataType();

     mDataTypes[XSD::nonNegativeInteger()] = getExpressionManager()->getIntDataType();
     mDataTypes[XSD::nonPositiveInteger()] = getExpressionManager()->getIntDataType();
     mDataTypes[XSD::positiveInteger()] = getExpressionManager()->getIntDataType();
     mDataTypes[XSD::negativeInteger()] = getExpressionManager()->getIntDataType();
     mDataTypes[XSD::integer()] = getExpressionManager()->getIntDataType();

     mDataTypes[XSD::byte()] = getExpressionManager()->getIntDataType();
     mDataTypes[XSD::unsignedLong()] = getExpressionManager()->getIntDataType();
     mDataTypes[XSD::unsignedInt()] = getExpressionManager()->getIntDataType();
     mDataTypes[XSD::unsignedShort()] = getExpressionManager()->getIntDataType();
     mDataTypes[XSD::unsignedByte()] = getExpressionManager()->getIntDataType();

     mDataTypes[XSD::decimal()] = getExpressionManager()->getRealDataType();
     mDataTypes[XSD::string()] = getExpressionManager()->getStrDataType();

     mDataTypes[XSD::dateTime()] = getExpressionManager()->getTimeDataType();
     mDataTypes[XSD::dateTimeStamp()] = getExpressionManager()->getTimeDataType();

}

KnowledgeBase::~KnowledgeBase()
{
    delete mKernel;
}

void KnowledgeBase::setVerbose()
{
    mKernel->setVerboseOutput(true);
}

void KnowledgeBase::refresh()
{
    mKernel->realiseKB();
}

bool KnowledgeBase::isConsistent()
{
    return mKernel->isKBConsistent();
}

void KnowledgeBase::classify()
{
    mKernel->classifyKB();
}

void KnowledgeBase::realize()
{
    mKernel->realiseKB();
}

bool KnowledgeBase::isRealized()
{
    return mKernel->isKBRealised();
}

bool KnowledgeBase::isClassSatisfiable(const IRI& klass)
{
    ClassExpression e_klass = getClass(klass);
    return mKernel->isSatisfiable(e_klass.get());
}


ObjectPropertyExpression KnowledgeBase::objectProperty(const IRI& property)
{
    return getObjectPropertyLazy(property);
}

DataPropertyExpression KnowledgeBase::dataProperty(const IRI& property)
{
    return getDataPropertyLazy(property);
}

Axiom KnowledgeBase::transitiveProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectPropertyLazy(property);
    return Axiom( mKernel->setTransitive(e_property.get()) );
}

bool KnowledgeBase::isTransitiveProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectProperty(property);
    return mKernel->isTransitive(e_property.get());
}

Axiom KnowledgeBase::functionalProperty(const IRI& property, PropertyType type)
{
    switch(type)
    {
        case OBJECT:
        {
            ObjectPropertyExpression e_property = getObjectPropertyLazy(property);
            return Axiom( mKernel->setOFunctional(e_property.get()) );
        }
        case DATA:
        {
            DataPropertyExpression e_property = getDataPropertyLazy(property);
            return Axiom( mKernel->setDFunctional(e_property.get()) );
        }
        default:
            throw std::invalid_argument("KnowledgeBase::functionalProperty: Functional property can only be created for object or data property");
    }
}

bool KnowledgeBase::isFunctionalProperty(const IRI& property)
{
    try {
        ObjectPropertyExpression e_property = getObjectProperty(property);
        return mKernel->isFunctional(e_property.get());
    } catch(const std::invalid_argument& e)
    {
        // Not a known object property
    } catch(const EFaCTPlusPlus& e)
    {
        // Known object property, but not registered as role
        return false;
    }

    try {
        DataPropertyExpression e_property = getDataProperty(property);
        return mKernel->isFunctional(e_property.get());
    } catch(const std::invalid_argument& e)
    {
        // Not a known object property
    } catch(const EFaCTPlusPlus& e)
    {
        // Known object property, but not registered as role
        return false;
    }

    LOG_INFO_S << "KnowledgeBase::isFunctionalProperty: Property '" << property.toString() << "' is not a known data or object property";
    throw std::invalid_argument("KnowledgeBase::isFunctionalProperty: Property '" + property.toString() + "' is not a known data or object property");
}

Axiom KnowledgeBase::inverseFunctionalProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectPropertyLazy(property);
    return Axiom( mKernel->setInverseFunctional(e_property.get()) );
}

bool KnowledgeBase::isInverseFunctionalProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectProperty(property);
    return mKernel->isInverseFunctional(e_property.get());
}

Axiom KnowledgeBase::reflexiveProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectPropertyLazy(property);
    return Axiom( mKernel->setReflexive(e_property.get()) );
}

bool KnowledgeBase::isReflexiveProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectProperty(property);
    return mKernel->isReflexive(e_property.get());
}

Axiom KnowledgeBase::irreflexiveProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectPropertyLazy(property);
    return Axiom( mKernel->setIrreflexive(e_property.get()) );
}

bool KnowledgeBase::isIrreflexiveProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectProperty(property);
    return mKernel->isIrreflexive(e_property.get());
}

Axiom KnowledgeBase::symmetricProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectPropertyLazy(property);
    return Axiom( mKernel->setSymmetric(e_property.get()) );
}

bool KnowledgeBase::isSymmetricProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectProperty(property);
    return mKernel->isSymmetric(e_property.get());
}

Axiom KnowledgeBase::asymmetricProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectPropertyLazy(property);
    return Axiom( mKernel->setAsymmetric(e_property.get()) );
}

bool KnowledgeBase::isAsymmetricProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectProperty(property);
    return mKernel->isAsymmetric(e_property.get());
}

Axiom KnowledgeBase::equalClasses(const IRIList& klasses)
{
    getExpressionManager()->newArgList();
    for(const IRI& iri : klasses)
    {
        ClassExpression e_class = getClassLazy(iri);
        getExpressionManager()->addArg(e_class.get());
    }
    return Axiom(mKernel->equalConcepts());
}

Axiom KnowledgeBase::equalObjectProperties(const IRIList& properties)
{
    getExpressionManager()->newArgList();
    for(const IRI& iri : properties)
    {
        ObjectPropertyExpression e_property = getObjectPropertyLazy(iri);
        getExpressionManager()->addArg(e_property.get());
    }
    return Axiom(mKernel->equalORoles());
}

Axiom KnowledgeBase::equalDataProperties(const IRIList& properties)
{
    getExpressionManager()->newArgList();
    for(const IRI& iri : properties)
    {
        DataPropertyExpression e_property = getDataPropertyLazy(iri);
        getExpressionManager()->addArg(e_property.get());
    }
    return Axiom(mKernel->equalORoles());
}

Axiom KnowledgeBase::subClassOf(const IRI& subclass, const IRI& parentClass)
{
    ClassExpression ce = getClassLazy(subclass);
    return subClassOf( ce, parentClass );
}

Axiom KnowledgeBase::subClassOf(const ClassExpression& subclass, const IRI& parentClass)
{
    ClassExpression e_parentClass = getClassLazy(parentClass);
    return Axiom( mKernel->impliesConcepts(subclass.get(), e_parentClass.get()) );
}

Axiom KnowledgeBase::subClassOf(const IRI& subclass, const ClassExpression& parentClass)
{
    ClassExpression e_subclass = getClassLazy(subclass);
    return Axiom( mKernel->impliesConcepts(e_subclass.get(), parentClass.get()) );
}

Axiom KnowledgeBase::subPropertyOf(const IRI& subProperty, const IRI& parentProperty)
{
    try {
        ObjectPropertyExpression e_property = getObjectProperty(parentProperty);
        return subObjectPropertyOf(subProperty, e_property);
    } catch(const std::exception& e)
    {
        // not an object property
    }

    try {
        DataPropertyExpression e_property = getDataProperty(parentProperty);
        return subDataPropertyOf(subProperty, e_property);
    } catch(const std::exception& e)
    {
        // not an object property
    }

    throw std::invalid_argument("owlapi::KnowledgeBase::subPropertyOf: "
            "parent property '" + parentProperty.toString() + "' is not known");
}

Axiom KnowledgeBase::subObjectPropertyOf(const IRI& subProperty, const ObjectPropertyExpression& parentProperty)
{
    ObjectPropertyExpression e_property = getObjectPropertyLazy(subProperty);
    //TDLAxiom* impliesORoles ( TORoleComplexExpr* R, TORoleExpr* S )
    return Axiom( mKernel->impliesORoles(e_property.get(), parentProperty.get()));
}

Axiom KnowledgeBase::subDataPropertyOf(const IRI& subProperty, const DataPropertyExpression& parentProperty)
{
    DataPropertyExpression e_property = getDataPropertyLazy(subProperty);
    return Axiom( mKernel->impliesDRoles(e_property.get(), parentProperty.get()) );
}

Axiom KnowledgeBase::equals(const IRI& klass, const IRI& otherKlass)
{
    ClassExpression e_class = getClassLazy(klass);
    ClassExpression e_other_class = getClassLazy(otherKlass);
    return equals(e_class, e_other_class);
}

Axiom KnowledgeBase::equals(const reasoner::factpp::ClassExpression& klass,
            const reasoner::factpp::ClassExpression& otherklass)
{
    getExpressionManager()->newArgList();
    getExpressionManager()->addArg(klass.get());
    getExpressionManager()->addArg(otherklass.get());

    return Axiom( mKernel->equalConcepts() );

}

ClassExpression KnowledgeBase::intersectionOf(const IRI& klass, const IRI& otherKlass)
{
    IRIList klasses;
    klasses.push_back(klass);
    klasses.push_back(otherKlass);
    return intersectionOf(klasses);
}

ClassExpression KnowledgeBase::intersectionOf(const reasoner::factpp::ClassExpression& klass, const reasoner::factpp::ClassExpression& otherKlass)
{
    std::vector<reasoner::factpp::ClassExpression> klasses;
    klasses.push_back(klass);
    klasses.push_back(otherKlass);
    return intersectionOf(klasses);
}

ClassExpression KnowledgeBase::intersectionOf(const IRIList& klasses)
{
    IRIList::const_iterator cit = klasses.begin();
    std::vector<reasoner::factpp::ClassExpression> e_klasses;
    for(; cit != klasses.end(); ++cit)
    {
        ClassExpression e_class = getClassLazy(*cit);
        e_klasses.push_back(e_class);
    }
    return intersectionOf(e_klasses);
}

ClassExpression KnowledgeBase::intersectionOf(const std::vector<reasoner::factpp::ClassExpression>& klasses)
{
    std::vector<reasoner::factpp::ClassExpression>::const_iterator cit = klasses.begin();
    getExpressionManager()->newArgList();
    for(; cit != klasses.end(); ++cit)
    {
        getExpressionManager()->addArg(cit->get());
    }
    TDLConceptExpression* f_and = getExpressionManager()->And();
    return ClassExpression( f_and );
}


ClassExpression KnowledgeBase::disjunctionOf(const IRI& klass, const IRI& otherKlass)
{
    IRIList klasses;
    klasses.push_back(klass);
    klasses.push_back(otherKlass);
    return disjunctionOf(klasses);
}

ClassExpression KnowledgeBase::disjunctionOf(const IRIList& klasses)
{
    IRIList::const_iterator cit = klasses.begin();
    getExpressionManager()->newArgList();
    for(; cit != klasses.end(); ++cit)
    {
        ClassExpression e_class = getClassLazy(*cit);
        getExpressionManager()->addArg(e_class.get());
    }
    TDLConceptExpression* f_or = getExpressionManager()->Or();
    return ClassExpression( f_or );
}

Axiom KnowledgeBase::alias(const IRI& alias, const ClassExpression& expression)
{
    ClassExpression e_aliasClass = getClassLazy(alias);

    getExpressionManager()->newArgList();
    getExpressionManager()->addArg(e_aliasClass.get());
    getExpressionManager()->addArg(expression.get());
    return Axiom( mKernel->equalConcepts() );
}

Axiom KnowledgeBase::alias(const IRI& aliasName, const IRI& iri, EntityType type)
{
    switch(type)
    {
        case INSTANCE:
        {
            instanceOf(aliasName, typeOf(iri));

            InstanceExpression e_aliasInstance = getInstanceLazy(aliasName);
            InstanceExpression e_instance = getInstance(iri);

            getExpressionManager()->newArgList();
            getExpressionManager()->addArg(e_aliasInstance.get());
            getExpressionManager()->addArg(e_instance.get());
            return Axiom( mKernel->processSame() );
        }
        case CLASS:
        {
            ClassExpression e_class = getClass(iri);
            return alias(aliasName, e_class);
        }
        default:
            throw std::invalid_argument("KnowledgeBase::alias: Alias for entity type '" + EntityTypeTxt[type] + "' not supported");
    }
}


Axiom KnowledgeBase::disjoint(const IRI& klassOrInstance, const IRI& otherKlassOrInstance, KnowledgeBase::EntityType type)
{
    IRIList klassesOrInstances;
    klassesOrInstances.push_back(klassOrInstance);
    klassesOrInstances.push_back(otherKlassOrInstance);
    return disjoint(klassesOrInstances, type);
}

Axiom KnowledgeBase::disjoint(const IRIList& klassesOrInstances, KnowledgeBase::EntityType type)
{
    IRIList::const_iterator cit = klassesOrInstances.begin();
    getExpressionManager()->newArgList();
    for(; cit != klassesOrInstances.end(); ++cit)
    {
        if(type == CLASS)
        {
            ClassExpression e_class = getClass(*cit);
            getExpressionManager()->addArg(e_class.get());
        } else if(type == INSTANCE)
        {
            InstanceExpression e_instance = getInstance(*cit);
            getExpressionManager()->addArg(e_instance.get());
        }
    }

    if(type == CLASS)
    {
        TDLAxiom* axiom = mKernel->disjointConcepts();
        return Axiom(axiom);
    } else if(type == INSTANCE)
    {
        TDLAxiom* axiom = mKernel->processDifferent();
        return Axiom(axiom);
    }

    throw std::runtime_error("owlapi::KnowledgeBase::disjoint requires either list of classes or instances");
}

Axiom KnowledgeBase::disjointObjectProperties(const IRIList& properties)
{
    getExpressionManager()->newArgList();
    for(const IRI& iri : properties)
    {
        ObjectPropertyExpression e_property = getObjectPropertyLazy(iri);
        getExpressionManager()->addArg(e_property.get());
    }
    return Axiom(mKernel->disjointORoles());
}

Axiom KnowledgeBase::disjointDataProperties(const IRIList& properties)
{
    getExpressionManager()->newArgList();
    for(const IRI& iri : properties)
    {
        DataPropertyExpression e_property = getDataPropertyLazy(iri);
        getExpressionManager()->addArg(e_property.get());
    }
    return Axiom(mKernel->disjointORoles());
}

Axiom KnowledgeBase::disjointUnion(const IRI& klass, const IRIList& disjointClasses)
{
    getExpressionManager()->newArgList();
    for(const IRI& disjointClass : disjointClasses)
    {
        ClassExpression d_class = getClass(disjointClass);
        getExpressionManager()->addArg(d_class.get());
    }

    ClassExpression e_class = getClass(klass);
    TDLAxiom* axiom = mKernel->disjointUnion(e_class.get());
    return Axiom(axiom);
}

Axiom KnowledgeBase::instanceOf(const IRI& individual, const IRI& klass)
{
    ClassExpression e_class = getClass(klass);
    return instanceOf(individual, e_class);
}

Axiom KnowledgeBase::instanceOf(const IRI& individual, const ClassExpression& e_class)
{
    InstanceExpression e_instance = getInstanceLazy(individual);
    return Axiom( mKernel->instanceOf(e_instance.get(), e_class.get()) );
}

Axiom KnowledgeBase::relatedTo(const IRI& instance, const IRI& relationProperty, const IRI& otherInstance, bool isTrue)
{
    ObjectPropertyExpression e_relation = getObjectPropertyLazy(relationProperty);

    InstanceExpression e_instance = getInstanceLazy(instance);
    InstanceExpression e_otherInstance = getInstanceLazy(otherInstance);

    if(isTrue)
    {
        return Axiom( mKernel->relatedTo(e_instance.get(), e_relation.get(), e_otherInstance.get()) );
    } else {
        return Axiom( mKernel->relatedToNot(e_instance.get(), e_relation.get(), e_otherInstance.get()) );
    }
}

Axiom KnowledgeBase::domainOf(const IRI& property, const IRI& domain, PropertyType propertyType)
{
    switch(propertyType)
    {
        case OBJECT:
        {
            ObjectPropertyExpression e_role = getObjectPropertyLazy(property);
            ClassExpression e_domain = getClassLazy(domain);

            return Axiom( mKernel->setODomain(e_role.get(), e_domain.get()) );
        }
        case DATA:
        {
            DataPropertyExpression e_role = getDataPropertyLazy(property);
            ClassExpression e_domain = getClassLazy(domain);

            return Axiom( mKernel->setDDomain(e_role.get(), e_domain.get()) );
        }
        default:
        {
            throw std::invalid_argument("Invalid property type '" + PropertyTypeTxt[propertyType] + "' for domainOf");
        }
    }
}

Axiom KnowledgeBase::objectRangeOf(const IRI& property, const IRI& range)
{
    ObjectPropertyExpression e_role = getObjectProperty(property);
    ClassExpression e_range = getClassLazy(range);
    return Axiom( mKernel->setORange(e_role.get(), e_range.get()) );
}

Axiom KnowledgeBase::objectRangeOf(const IRI& property, const ClassExpression& expression)
{
    ObjectPropertyExpression e_role = getObjectProperty(property);
    return Axiom( mKernel->setORange(e_role.get(), expression.get()) );
}

reasoner::factpp::Axiom KnowledgeBase::dataRangeOf(const IRI& property,
        reasoner::factpp::DataRange& range)
{
    DataPropertyExpression e_role = getDataProperty(property);
    return Axiom( mKernel->setDRange(e_role.get(), range.get()) );
}

Axiom KnowledgeBase::valueOf(const IRI& individual, const IRI& property, const DataValue& dataValue)
{
    // Make sure the database remains consistent when handling functional
    // properties, i.e. you cannot associate two valueOf statement with a
    // functional property
    std::pair<IRI,IRI> key(individual, property);
    if(isFunctionalProperty(property))
    {
        DataValueMap::iterator it = mValueOfAxioms.find(key);
        if(it != mValueOfAxioms.end())
        {
            Axiom::List& list = it->second;
            Axiom::List::const_iterator cit = list.begin();
            for(; cit != list.end(); ++cit)
            {
                LOG_DEBUG_S << "Retracting existing value: " << individual << " " << property;
                retract(*cit);
            }
            mValueOfAxioms.erase(it);
        }
    }

    TDLAxiom* tdlAxiom = mKernel->valueOf( getInstance(individual).get(), getDataProperty(property).get(), dataValue.get());
    Axiom axiom(tdlAxiom);
    mValueOfAxioms[key].push_back(axiom);

    return axiom;
}

Axiom KnowledgeBase::valueOf(const IRI& individual, const IRI& property, const owlapi::model::OWLLiteral::Ptr& literal)
{
    reasoner::factpp::DataValue value = dataValue(literal);
    return valueOf(individual, property, value);
}

DataTypeName KnowledgeBase::dataType(const IRI& dataType)
{
    IRIDataTypeMap::const_iterator cit = mDataTypes.find(dataType);
    if(cit != mDataTypes.end())
    {
        return cit->second;
    }

    // Guess reasoner datatype
    DataTypeName dataTypeName(getExpressionManager()->DataType(dataType.toString()));
    mDataTypes[dataType] = dataTypeName;
    return dataTypeName;
}

DataValue KnowledgeBase::dataValue(const std::string& value, const std::string& dataType)
{
    IRIDataTypeMap::const_iterator cit = mDataTypes.find(dataType);
    if(cit == mDataTypes.end())
    {
        throw std::invalid_argument("owlapi::KnowledgeBase::dataValue: dataType '" + dataType + "' is unknown");
    }

    DataTypeName dataTypeName = cit->second;
    const TDLDataValue* dataValue = getExpressionManager()->DataValue(value, dataTypeName.get());
    return DataValue(dataValue);
}

DataValue KnowledgeBase::dataValue(const owlapi::model::OWLLiteral::Ptr& literal)
{
    return dataValue(literal->getValue(), literal->getType());
}

Axiom KnowledgeBase::inverseOf(const IRI& base, const IRI& inverse)
{
    ObjectPropertyExpression e_role = getObjectPropertyLazy(base);
    ObjectPropertyExpression e_inverse = getObjectPropertyLazy(inverse);

    return Axiom( mKernel->setInverseRoles(e_role.get(), e_inverse.get()) );
}

ClassExpression KnowledgeBase::objectOneOf(const IRI& id,
        const IRIList& instanceList)
{
    getExpressionManager()->newArgList();
    for(const IRI& instance : instanceList)
    {
        InstanceExpression e_instance = getInstanceLazy(instance);
        getExpressionManager()->addArg(e_instance.get());
    }
    ClassExpression ce( getExpressionManager()->OneOf() );
    mClasses[id] = ce;
    return ce;
}

void KnowledgeBase::objectOneOf(const IRI& id, const OWLObjectOneOf::Ptr& oneOf)
{
    getExpressionManager()->newArgList();
    for(const OWLNamedIndividual::Ptr& instance : oneOf->getNamedIndividuals())
    {
        InstanceExpression e_instance = getInstanceLazy(instance->getIRI());
        getExpressionManager()->addArg(e_instance.get());
    }
    ClassExpression ce( getExpressionManager()->OneOf() );
    mClasses[id] = ce;
}

ClassExpression KnowledgeBase::objectComplementOf(const IRI& id, const IRI& klass)
{
    ClassExpression e_klass = getClassLazy(klass);
    ClassExpression ce( getExpressionManager()->Not( e_klass.get() ) ) ;
    mClasses[id] = ce;
    return ce;
}

ClassExpression KnowledgeBase::objectIntersectionOf(const IRI& id, const IRIList& klassList)
{
    getExpressionManager()->newArgList();
    for(const IRI& klass : klassList)
    {
        ClassExpression e_klass = getClassLazy(klass);
        getExpressionManager()->addArg(e_klass.get());
    }

    ClassExpression ce(getExpressionManager()->And());
    mClasses[id] = ce;
    return ce;
}

ClassExpression KnowledgeBase::objectUnionOf(const IRI& id, const IRIList& klassList)
{
    getExpressionManager()->newArgList();
    for(const IRI& klass : klassList)
    {
        ClassExpression e_klass = getClassLazy(klass);
        getExpressionManager()->addArg(e_klass.get());
    }

    ClassExpression ce(getExpressionManager()->Or());
    mClasses[id] = ce;
    return ce;
}


reasoner::factpp::DataRange KnowledgeBase::dataRange(const owlapi::model::OWLDataRange::Ptr& range)
{
    DataRangeMap::const_iterator cit = mDataRanges.find(range);
    if(cit != mDataRanges.end())
    {
        return cit->second;
    }

    switch(range->getDataRangeType())
    {
        case OWLDataRange::DATATYPE:
            break;
        case OWLDataRange::DATA_INTERSECTION_OF:
            break;
        case OWLDataRange::DATA_UNION_OF:
            break;
        case OWLDataRange::DATA_COMPLEMENT_OF:
        //    return dataComplementOf(dynamic_pointer_cast<OWLDataComplementOf>(range));
        case OWLDataRange::DATA_ONE_OF:
            return dataOneOf(dynamic_pointer_cast<OWLDataOneOf>(range));
        case OWLDataRange::DATATYPE_RESTRICTION:
            break;
        default:
            break;
    }

    throw std::invalid_argument("owlapi::KnowledgeBase::dataRange: failed to"
            "convert OWLDataRange into reasoner type");
}

reasoner::factpp::DataRange KnowledgeBase::dataOneOf(const owlapi::model::OWLDataOneOf::Ptr& oneOf)
{
    reasoner::factpp::DataRange range = dataOneOf(oneOf->getLiterals());
    mDataRanges[oneOf] = range;
    return range;
}

reasoner::factpp::DataRange KnowledgeBase::dataOneOf(const owlapi::model::OWLLiteral::PtrList& literals)
{
    getExpressionManager()->newArgList();
    for(const owlapi::model::OWLLiteral::Ptr& literal : literals)
    {
        DataValue dValue = dataValue(literal->getValue(), literal->getType());
        getExpressionManager()->addArg(dValue.get());
    }
    return reasoner::factpp::DataRange( getExpressionManager()->DataOneOf() );
}

reasoner::factpp::DataRange KnowledgeBase::dataTypeRestriction(const owlapi::model::OWLDataTypeRestriction::Ptr& restriction)
{
    if(!restriction)
    {
        throw std::invalid_argument("owlapi::KnowledgeBase::dataTypeRestriction: "
                " restriction is NULL");
    }
    using namespace owlapi::model;

    TDLDataTypeRestriction* dataTypeRestriction = NULL;
    for(const OWLFacetRestriction& fr : restriction->getFacetRestrictions())
    {
        const OWLLiteral::Ptr& literal = fr.getFacetValue();
        DataValue dValue = dataValue(literal->getValue(), literal->getType());
        const TDLFacetExpression* facetExpression;

        switch(fr.getFacet().getFacetType())
        {
            case OWLFacet::MIN_INCLUSIVE:
                facetExpression = getExpressionManager()->FacetMinInclusive( dValue.get());
                break;
            case OWLFacet::MIN_EXCLUSIVE:
                facetExpression = getExpressionManager()->FacetMinExclusive( dValue.get());
                break;
            case OWLFacet::MAX_INCLUSIVE:
                facetExpression = getExpressionManager()->FacetMaxInclusive( dValue.get());
                break;
            case OWLFacet::MAX_EXCLUSIVE:
                facetExpression = getExpressionManager()->FacetMaxExclusive( dValue.get());
                break;
            default:
                break;
        }

        // TDLDataTypeExpression
        if(!dataTypeRestriction)
        {
            DataTypeName dataTypeName = dataType(literal->getType());
            dataTypeRestriction = getExpressionManager()->RestrictedType(dataTypeName.get(), facetExpression);
        } else {
            dataTypeRestriction = getExpressionManager()->RestrictedType(dataTypeRestriction, facetExpression);
        }
    }
    return reasoner::factpp::DataRange(dataTypeRestriction);
}

reasoner::factpp::ClassExpression KnowledgeBase::classExpression(
        const IRI& expressionId
)
{
    IRIClassExpressionMap::const_iterator it = mClasses.find(expressionId);
    if(it != mClasses.end())
    {
        return it->second;
    }

    throw std::invalid_argument("owlapi::KnowledgeBase::classExpression: failed"
            " to resolve class expression for iri '" + expressionId.toString() + "'");
}

reasoner::factpp::ClassExpression KnowledgeBase::objectSomeValuesFrom(const IRI& klassId,
        const IRI& property,
        const IRI& expressionId
)
{
    reasoner::factpp::ClassExpression classExpr = classExpression(expressionId);
    reasoner::factpp::ObjectPropertyExpression opExpr = objectProperty(property);
    TDLConceptExpression* someValuesFromConcept =
        getExpressionManager()->Exists(opExpr.get(), classExpr.get());

    ClassExpression ce(someValuesFromConcept);
    mClasses[klassId] = ce;
    return ce;
}

reasoner::factpp::ClassExpression KnowledgeBase::objectAllValuesFrom(const IRI& klassId,
    const IRI& property,
    const IRI& expressionId
)
{
    reasoner::factpp::ClassExpression classExpr = classExpression(expressionId);
    reasoner::factpp::ObjectPropertyExpression opExpr = objectProperty(property);
    TDLConceptExpression* allValuesFromConcept =
        getExpressionManager()->Forall(opExpr.get(), classExpr.get());

    ClassExpression ce(allValuesFromConcept);
    mClasses[klassId] = ce;
    return ce;
}

reasoner::factpp::ClassExpression KnowledgeBase::objectHasValue(
        const IRI& id,
        const IRI& property,
        const IRI& individual
)
{
    reasoner::factpp::ObjectPropertyExpression opExpr = objectProperty(property);
    reasoner::factpp::InstanceExpression e_instance = getInstanceLazy(individual);

    TDLConceptExpression* hasValue = getExpressionManager()->Value(opExpr.get(), e_instance.get());
    ClassExpression ce(hasValue);

    mClasses[id] = ce;
    return ce;
}

reasoner::factpp::ClassExpression KnowledgeBase::dataSomeValuesFrom(const IRI& klassId,
        const IRI& property,
        const owlapi::model::OWLDataRange::Ptr& range)
{
    const TDLDataExpression* dataExpression;
    switch(range->getDataRangeType())
    {
        case OWLDataRange::DATATYPE:
        {
            owlapi::model::IRI name = dynamic_pointer_cast<OWLLogicalEntity>(range)->getIRI();
            reasoner::factpp::DataTypeName dt = dataType(name);
            dataExpression = dt.get();
            break;
        }
        case OWLDataRange::DATATYPE_RESTRICTION:
        {
            OWLDataTypeRestriction::Ptr restriction = dynamic_pointer_cast<OWLDataTypeRestriction>(range);
            reasoner::factpp::DataRange e_range = dataTypeRestriction(restriction);
            dataExpression = e_range.get();
            break;
        }
        default:
            throw std::runtime_error("owlapi::KnowledgeBase:dataSomeValuesFrom:"
                    " missing support this data range type");
    }

    DataPropertyExpression dpExpression = dataProperty(property);
    TDLConceptExpression* someValuesFromConcept = getExpressionManager()->Exists(dpExpression.get(),
     dataExpression);

    ClassExpression ce(someValuesFromConcept);
    mClasses[klassId] = ce;
    return ce;
}

ClassExpression KnowledgeBase::dataAllValuesFrom(const IRI& klassId,
        const IRI& property,
        const owlapi::model::OWLDataRange::Ptr& range)
{
    const TDLDataExpression* dataExpression;
    switch(range->getDataRangeType())
    {
        case OWLDataRange::DATATYPE:
        {
            owlapi::model::IRI name = dynamic_pointer_cast<OWLLogicalEntity>(range)->getIRI();
            reasoner::factpp::DataTypeName dt = dataType(name);
            dataExpression = dt.get();
            break;
        }
        case OWLDataRange::DATATYPE_RESTRICTION:
        {
            OWLDataTypeRestriction::Ptr restriction = dynamic_pointer_cast<OWLDataTypeRestriction>(range);
            reasoner::factpp::DataRange e_range = dataTypeRestriction(restriction);
            dataExpression = e_range.get();
            break;
        }
        default:
            throw std::runtime_error("owlapi::KnowledgeBase:dataSomeValuesFrom:"
                    " missing support this data range type");
    }
    DataPropertyExpression dpExpression = dataProperty(property);

    TDLConceptExpression* allValuesFromConcept = getExpressionManager()->Forall(dpExpression.get(),
                    dataExpression);

    ClassExpression ce(allValuesFromConcept);
    mClasses[klassId] = ce;
    return ce;
}

ClassExpression KnowledgeBase::dataHasValue(const IRI& klassId,
        const IRI& property,
        const owlapi::model::OWLLiteral::Ptr& literal)
{
    reasoner::factpp::DataValue value = dataValue(literal);
    DataPropertyExpression dpExpression = dataProperty(property);
    TDLConceptExpression* hasValueConcept = getExpressionManager()->Value(dpExpression.get(),
                    value.get());

    ClassExpression ce(hasValueConcept);
    mClasses[klassId] = ce;
    return ce;
}

ClassExpression KnowledgeBase::dataMinCardinality(const IRI& klassId,
        size_t n,
        const IRI& property,
        const owlapi::model::OWLDataRange::Ptr& range)
{
    DataPropertyExpression dpProperty = dataProperty(property);
    TDLConceptExpression* cardinalityConcept;
    if(range)
    {
        reasoner::factpp::DataRange dataExpression = dataRange(range);
        cardinalityConcept =
            getExpressionManager()->MinCardinality(n, dpProperty.get(), dataExpression.get());
    } else {
        reasoner::factpp::DataRange range(getExpressionManager()->getStrDataType());
        cardinalityConcept =
            getExpressionManager()->MinCardinality(n, dpProperty.get(), range.get());
    }

    ClassExpression ce(cardinalityConcept);
    mClasses[klassId] = ce;
    return ce;
}

ClassExpression KnowledgeBase::dataMaxCardinality(const IRI& klassId,
        size_t n,
        const IRI& property,
        const owlapi::model::OWLDataRange::Ptr& range)
{
    DataPropertyExpression dpProperty = dataProperty(property);
    TDLConceptExpression* cardinalityConcept;
    if(range)
    {
        reasoner::factpp::DataRange dataExpression = dataRange(range);
        cardinalityConcept =
            getExpressionManager()->MaxCardinality(n, dpProperty.get(), dataExpression.get());
    } else {
        reasoner::factpp::DataRange range(getExpressionManager()->getStrDataType());
        cardinalityConcept =
            getExpressionManager()->MaxCardinality(n, dpProperty.get(), range.get());
    }

    ClassExpression ce(cardinalityConcept);
    mClasses[klassId] = ce;
    return ce;
}

ClassExpression KnowledgeBase::dataExactCardinality(const IRI& klassId,
        size_t n,
        const IRI& property,
        const owlapi::model::OWLDataRange::Ptr& range)
{
    DataPropertyExpression dpProperty = dataProperty(property);
    TDLConceptExpression* cardinalityConcept;
    if(range)
    {
        reasoner::factpp::DataRange dataExpression = dataRange(range);
        cardinalityConcept =
            getExpressionManager()->Cardinality(n, dpProperty.get(), dataExpression.get());
    } else {
        reasoner::factpp::DataRange range(getExpressionManager()->getStrDataType());
        cardinalityConcept =
            getExpressionManager()->Cardinality(n, dpProperty.get(), range.get());
    }

    ClassExpression ce(cardinalityConcept);
    mClasses[klassId] = ce;
    return ce;
}

ClassExpression KnowledgeBase::objectMinCardinality(
        const IRI& id,
        size_t cardinality,
        const IRI& relationProperty,
        const IRI& qualification
)
{
    ClassExpression ce = objectPropertyRestriction(restriction::MIN_CARDINALITY,
            relationProperty,
            qualification,
            cardinality);
    mClasses[id] = ce;
    return ce;
}

ClassExpression KnowledgeBase::objectMaxCardinality(
        const IRI& id,
        size_t cardinality,
        const IRI& relationProperty,
        const IRI& qualification
)
{
    ClassExpression ce = objectPropertyRestriction(restriction::MAX_CARDINALITY,
            relationProperty,
            qualification,
            cardinality);
    mClasses[id] = ce;
    return ce;
}

ClassExpression KnowledgeBase::objectExactCardinality(
        const IRI& id,
        size_t cardinality,
        const IRI& relationProperty,
        const IRI& qualification
)
{
    ClassExpression ce = objectPropertyRestriction(restriction::EXACT_CARDINALITY,
            relationProperty,
            qualification,
            cardinality);
    mClasses[id] = ce;
    return ce;
}

ClassExpression KnowledgeBase::objectPropertyRestriction(
        restriction::Type type,
        const IRI& relationProperty,
        const IRI& klassOrInstance,
        int cardinality)
{
    ObjectPropertyExpression e_relation = getObjectPropertyLazy(relationProperty);
    switch(type)
    {
        case restriction::SELF:
        {
            return ClassExpression( getExpressionManager()->SelfReference(e_relation.get()) );
        }
        case restriction::VALUE:
        {
            InstanceExpression e_instance = getInstanceLazy(klassOrInstance);
            return ClassExpression( getExpressionManager()->Value(e_relation.get(), e_instance.get()) );
        }
        case restriction::EXISTS:
        {
            ClassExpression e_class = getClassLazy(klassOrInstance);
            return ClassExpression( getExpressionManager()->Exists(e_relation.get(), e_class.get()) );
        }
        case restriction::FORALL:
        {
            ClassExpression e_class = getClassLazy(klassOrInstance);
            return ClassExpression( getExpressionManager()->Forall(e_relation.get(), e_class.get()) );
        }
        case restriction::MIN_CARDINALITY:
        {
            ClassExpression e_class = getClassLazy(klassOrInstance);
            return ClassExpression( getExpressionManager()->MinCardinality(cardinality, e_relation.get(), e_class.get()) );
        }
        case restriction::MAX_CARDINALITY:
        {
            ClassExpression e_class = getClassLazy(klassOrInstance);
            return ClassExpression( getExpressionManager()->MaxCardinality(cardinality, e_relation.get(), e_class.get()) );
        }
        case restriction::EXACT_CARDINALITY:
        {
            ClassExpression e_class = getClassLazy(klassOrInstance);
            return ClassExpression( getExpressionManager()->Cardinality(cardinality, e_relation.get(), e_class.get()) );
        }
    }

    throw std::runtime_error("owlapi::KnowledgeBase::objectPropertyRestriction: Unknown restriction type");
}

bool KnowledgeBase::isSubClassOf(const IRI& subclass, const IRI& parentClass)
{
    try {
        ClassExpression e_class = getClass(subclass);
        return isSubClassOf( e_class, parentClass );
    } catch(const std::exception& e)
    {
        LOG_WARN_S << "owlapi::KnowledgeBase::isSubClassOf: subclass test failed"
            << subclass << " isSubClassof " << parentClass
            << " -- "
            << e.what();
        throw;
    }
}

bool KnowledgeBase::isSubClassOf(const ClassExpression& subclass, const IRI& parentClass)
{
    ClassExpression e_class = getClass(parentClass);
    return mKernel->isSubsumedBy(subclass.get(), e_class.get());
}


bool KnowledgeBase::isEquivalentClass(const IRI& klass0, const IRI& klass1)
{
    ClassExpression e_class0 = getClass(klass0);
    ClassExpression e_class1 = getClass(klass1);

    return mKernel->isEquivalent( e_class0.get(), e_class1.get() );
}

bool KnowledgeBase::isDisjointClass(const IRI& klass0, const IRI& klass1)
{
    ClassExpression e_class0 = getClass(klass0);
    ClassExpression e_class1 = getClass(klass1);

    return mKernel->isDisjoint( e_class0.get(), e_class1.get() );
}

bool KnowledgeBase::isInstanceOf(const IRI& instance, const IRI& klass)
{
    InstanceExpression e_instance = getInstance(instance);
    ClassExpression e_class = getClass(klass);
    return mKernel->isInstance(e_instance.get(), e_class.get());
}

bool KnowledgeBase::isRelatedTo(const IRI& instance, const IRI& relationProperty, const IRI& otherInstance)
{
    InstanceExpression e_instance = getInstance(instance);
    InstanceExpression e_otherInstance = getInstance(otherInstance);

    ObjectPropertyExpression e_relation = getObjectProperty(relationProperty);
    return mKernel->isRelated(e_instance.get(), e_relation.get(), e_otherInstance.get());
}

bool KnowledgeBase::isSameInstance(const IRI& instance, const IRI& otherInstance) const
{
    InstanceExpression e_instance = getInstance(instance);
    InstanceExpression e_otherInstance = getInstance(otherInstance);
    return mKernel->isSameIndividuals(e_instance.get(), e_otherInstance.get());
}

bool KnowledgeBase::isDatatype(const IRI& name) const
{
    return mDataTypes.find(name) != mDataTypes.end();
}

IRIList KnowledgeBase::allClasses(bool excludeBottomClass) const
{
    IRIList klasses;
    IRIClassExpressionMap::const_iterator cit = mClasses.begin();
    for(; cit != mClasses.end(); ++cit)
    {
        klasses.push_back(cit->first);
    }
    return klasses;
}

IRIList KnowledgeBase::allSubClassesOf(const IRI& klass, bool direct)
{
    ClassExpression e_class = getClass(klass);

    Actor actor;
    actor.needConcepts();
    mKernel->getSubConcepts(e_class.get(), direct, actor);
    // vocabulary::OWL::Nothing() is bottom concept, which is represented as "BOTTOM" in Factpp
    return getResult(actor, IRI("BOTTOM"));
}

IRIList KnowledgeBase::allAncestorsOf(const IRI& klass, bool direct)
{
    ClassExpression e_class = getClass(klass);

    Actor actor;
    actor.needConcepts();
    mKernel->getSupConcepts(e_class.get(), direct, actor);

    IRIList directClassAncestors = getResult(actor);
    IRIList punningBasedAncestors;
    try {
        punningBasedAncestors = typesOf(klass);
    } catch(...)
    {
        /// ignore if there is no instance for punning
    }
    IRISet ancestors(directClassAncestors.begin(), directClassAncestors.end());
    for(const IRI& ancestor : ancestors)
    {
        IRIList equivalenctClasses = allEquivalentClasses(ancestor);
        punningBasedAncestors.insert(punningBasedAncestors.end(),
                equivalenctClasses.begin(), equivalenctClasses.end());

    }
    ancestors.insert(punningBasedAncestors.begin(), punningBasedAncestors.end());
    return IRIList(ancestors.begin(), ancestors.end());
}

IRIList KnowledgeBase::allEquivalentClasses(const IRI& klass)
{
    ClassExpression e_class = getClass(klass);

    Actor actor;
    actor.needConcepts();
    mKernel->getEquivalentConcepts(e_class.get(), actor);
    return getResult(actor);
}

IRIList KnowledgeBase::allDisjointClasses(const IRI& klass)
{
    ClassExpression e_class = getClass(klass);

    Actor actor;
    actor.needConcepts();
    mKernel->getDisjointConcepts(e_class.get(), actor);

    return getResult(actor);
}

IRIList KnowledgeBase::allInstances() const
{
    IRIList instances;
    IRIInstanceExpressionMap::const_iterator cit = mInstances.begin();
    for(; cit != mInstances.end(); ++cit)
    {
        instances.push_back(cit->first);
    }
    return instances;
}

IRIList KnowledgeBase::allInstancesOf(const IRI& klass, bool direct)
{
    ClassExpression e_class = getClass(klass);
    IRIList instances;

    Actor actor;
    actor.needIndividuals();
    mKernel->getInstances(e_class.get(), actor);

    Actor::Array1D result;
    actor.getFoundData(result);
    Actor::Array1D::const_iterator cit = result.begin();
    for(; cit != result.end(); ++cit)
    {
        // Fact does seem to fail at extracting direct instances
        const ClassifiableEntry* entry = *cit;
        IRI instanceName = IRI::create(entry->getName());
        if(direct && ! (typeOf(instanceName) == klass) )
        {
            continue;
        }
        instances.push_back(instanceName);
    }
    return instances;
}

IRIList KnowledgeBase::allRelatedInstances(const IRI& individual, const IRI& relationProperty, const IRI& klass)
{
    IRIList individuals;
    try {
        InstanceExpression e_instance = getInstance(individual);
        ObjectPropertyExpression e_relation = getObjectProperty(relationProperty);

        ReasoningKernel::IndividualSet relatedIndividuals;
        mKernel->getRoleFillers(e_instance.get(), e_relation.get(), relatedIndividuals);

        ReasoningKernel::IndividualSet::const_iterator cit = relatedIndividuals.begin();
        for(; cit != relatedIndividuals.end(); ++cit)
        {
            const TNamedEntry* entry = *cit;
            IRI iri( entry->getName() );

            if( klass.empty() || isInstanceOf(iri, klass) )
            {
                individuals.push_back(iri);
            }
        }
    } catch(const std::exception& e)
    {
        LOG_WARN_S << "'" << individual << "' not related to any via '" << relationProperty << "': " << e.what();
        // There is no such relation defined, thus return an empty list
    }
    return individuals;
}

IRI KnowledgeBase::relatedInstance(const IRI& individual, const IRI& relationProperty, const IRI& klass)
{
    IRIList instances = allRelatedInstances(individual, relationProperty, klass);
    if(instances.empty())
    {
        throw std::invalid_argument("KnowledgeBase::relatedInstance: no instance related to '" + individual.toString() + "' via property '" + relationProperty.toString() + "'");
    }
    return instances.front();
}

IRIList KnowledgeBase::allInverseRelatedInstances(const IRI& individual, const IRI& relationProperty, const IRI& klass)
{
    IRIList individuals;
    try {
        InstanceExpression e_instance = getInstance(individual);
        ObjectPropertyExpression e_relation = getObjectProperty(relationProperty);
        TDLObjectRoleExpression* f_inverse = mKernel->getExpressionManager()->Inverse(e_relation.get());

        ReasoningKernel::IndividualSet relatedIndividuals;
        mKernel->getRoleFillers(e_instance.get(), f_inverse, relatedIndividuals);

        ReasoningKernel::IndividualSet::const_iterator cit = relatedIndividuals.begin();
        for(; cit != relatedIndividuals.end(); ++cit)
        {
            const TNamedEntry* entry = *cit;
            IRI iri( entry->getName() );

            if( klass.empty() || isInstanceOf(iri, klass) )
            {
                individuals.push_back(iri);
            }
        }
    } catch(const std::exception& e)
    {
        LOG_WARN_S << "'" << individual << "' not related to any via '" << relationProperty << "': " << e.what();
        // There is no such relation defined, thus return an empty list
    }
    return individuals;
}

IRIList KnowledgeBase::allObjectProperties() const
{
    IRIList properties;
    IRIObjectPropertyExpressionMap::const_iterator cit = mObjectProperties.begin();
    for(; cit != mObjectProperties.end(); ++cit)
    {
        const owlapi::model::IRI& iri = cit->first;
        if(iri == vocabulary::OWL::topObjectProperty() || iri == vocabulary::OWL::bottomObjectProperty())
        {
            continue;
        }
        properties.push_back(iri);
    }
    return properties;
}


IRIList KnowledgeBase::allAncestorObjectProperties(const IRI& property, bool direct)
{
    ObjectPropertyExpression e_role = getObjectProperty(property);

    Actor actor;
    actor.needObjectRoles();
    mKernel->getSupRoles(e_role.get(), direct, actor);

    IRIList directAncestors = getResult(actor);
    IRISet ancestors(directAncestors.begin(), directAncestors.end());

    for(const IRI& ancestor : ancestors)
    {
        IRIList equivalenctRoles = allEquivalentObjectProperties(ancestor);
        ancestors.insert(equivalenctRoles.begin(), equivalenctRoles.end());

    }
    return IRIList(ancestors.begin(), ancestors.end());
}

IRIList KnowledgeBase::allSubObjectProperties(const IRI& propertyRelation, bool direct)
{
    ObjectPropertyExpression e_relation = getObjectProperty(propertyRelation);

    Actor actor;
    actor.needObjectRoles();
    mKernel->getSubRoles(e_relation.get(), direct, actor);

    return getResult(actor);
}

IRIList KnowledgeBase::allEquivalentObjectProperties(const IRI& propertyRelation)
{
    ObjectPropertyExpression e_relation = getObjectProperty(propertyRelation);
    IRIList relations;

    Actor actor;
    actor.needObjectRoles();
    mKernel->getEquivalentRoles(e_relation.get(), actor);
    return getResult(actor);
}

bool KnowledgeBase::isSubProperty(const IRI& relationProperty, const IRI& parentRelationProperty)
{
    try {
        return isSubObjectProperty(relationProperty, parentRelationProperty);
    } catch(...)
    {}

    try {
        return isSubDataProperty(relationProperty, parentRelationProperty);
    } catch(...)
    {}

    throw std::invalid_argument("KnowledgeBase::isSubObjectProperty: Property '" + relationProperty.toString() + "' is not a known data or object property");
}

bool KnowledgeBase::isSubObjectProperty(const IRI& relationProperty, const IRI& parentRelationProperty)
{
    ObjectPropertyExpression e_child = getObjectProperty(relationProperty);
    ObjectPropertyExpression e_parent = getObjectProperty(parentRelationProperty);
    return mKernel->isSubRoles(e_child.get(), e_parent.get());
}

bool KnowledgeBase::isSubDataProperty(const IRI& relationProperty, const IRI& parentRelationProperty)
{
    DataPropertyExpression e_child = getDataProperty(relationProperty);
    DataPropertyExpression e_parent = getDataProperty(parentRelationProperty);
    return mKernel->isSubRoles(e_child.get(), e_parent.get());
}

bool KnowledgeBase::isDisjointProperties(const IRI& relationProperty0, const IRI& relationProperty1)
{
    try {
        return isDisjointObjectProperties(relationProperty0, relationProperty1);
    } catch(...)
    {}

    try {
        return isDisjointDataProperties(relationProperty0, relationProperty1);
    } catch(...)
    {}

    throw std::invalid_argument("KnowledgeBase::isDisjointProperties: Property '" + relationProperty0.toString() + "/" + relationProperty1.toString() + "' is not a known data or object property");
}

bool KnowledgeBase::isDisjointObjectProperties(const IRI& relationProperty0, const IRI& relationProperty1)
{
    ObjectPropertyExpression e_child = getObjectProperty(relationProperty0);
    ObjectPropertyExpression e_parent = getObjectProperty(relationProperty1);
    return mKernel->isDisjointRoles(e_child.get(), e_parent.get());
}

bool KnowledgeBase::isDisjointDataProperties(const IRI& relationProperty0, const IRI& relationProperty1)
{
    DataPropertyExpression e_child = getDataProperty(relationProperty0);
    DataPropertyExpression e_parent = getDataProperty(relationProperty1);
    return mKernel->isDisjointRoles(e_child.get(), e_parent.get());
}

IRIList KnowledgeBase::allDataProperties() const
{
    IRIList properties;
    IRIDataPropertyExpressionMap::const_iterator cit = mDataProperties.begin();
    for(; cit != mDataProperties.end(); ++cit)
    {
        const owlapi::model::IRI& iri = cit->first;
        if(iri == vocabulary::OWL::topDataProperty() || iri == vocabulary::OWL::bottomDataProperty())
        {
            continue;
        }
        properties.push_back(cit->first);
    }
    return properties;
}

IRIList KnowledgeBase::allAncestorDataProperties(const IRI& property, bool direct)
{
    DataPropertyExpression e_role = getDataProperty(property);

    Actor actor;
    actor.needDataRoles();
    mKernel->getSupRoles(e_role.get(), direct, actor);

    IRIList directAncestors = getResult(actor);
    IRISet ancestors(directAncestors.begin(), directAncestors.end());

    for(const IRI& ancestor : directAncestors)
    {
        IRIList equivalenctRoles = allEquivalentDataProperties(ancestor);
        ancestors.insert(equivalenctRoles.begin(), equivalenctRoles.end());

    }
    return IRIList(ancestors.begin(), ancestors.end());
}

IRIList KnowledgeBase::allSubDataProperties(const IRI& propertyRelation, bool direct)
{
    DataPropertyExpression e_relation = getDataProperty(propertyRelation);
    IRIList relations;

    Actor actor;
    actor.needDataRoles();
    mKernel->getSubRoles(e_relation.get(), direct, actor);

    return getResult(actor);
}

IRIList KnowledgeBase::allEquivalentDataProperties(const IRI& propertyRelation)
{
    DataPropertyExpression e_relation = getDataProperty(propertyRelation);
    IRIList relations;

    Actor actor;
    actor.needDataRoles();
    mKernel->getEquivalentRoles(e_relation.get(), actor);

    return getResult(actor);
}

IRIList KnowledgeBase::getPropertyDomain(const IRI& property, bool direct) const
{
    try {
        return getObjectPropertyDomain(property, direct);
    } catch(...)
    {}

    try {
        return getDataPropertyDomain(property, direct);
    } catch(...)
    {}

    throw std::invalid_argument("KnowledgeBase::isPropertyDomain: Property '" + property.toString() + "' is not a known data or object property");
}

IRIList KnowledgeBase::getDataPropertyDomain(const IRI& property, bool direct) const
{
    DataPropertyExpression e_property = getDataProperty(property);

    Actor actor;
    actor.needConcepts();
    mKernel->getDRoleDomain( e_property.get(), direct, actor);

    return getResult(actor);
}

IRIList KnowledgeBase::typesOf(const IRI& instance, bool direct) const
{
    InstanceExpression e_instance = getInstance(instance);

    Actor actor;
    actor.needConcepts();
    mKernel->getTypes(e_instance.get(), direct, actor);

    return getResult(actor);
}

IRI KnowledgeBase::typeOf(const IRI& instance) const
{
    IRIList types = typesOf(instance, true);
    assert(!types.empty());
    return types[0];
}

IRIList KnowledgeBase::getSameAs(const IRI& aliasOrInstance)
{
    InstanceExpression e_instance = getInstance(aliasOrInstance);

    Actor actor;
    actor.needIndividuals();
    mKernel->getSameAs(e_instance.get(), actor);

    IRIList alias = getResult(actor);

    if(alias.empty())
    {
        IRIList list;
        list.push_back(aliasOrInstance);
        return list;
    } else {
        return alias;
    }
}

IRIList KnowledgeBase::uniqueList(const IRIList& individuals)
{
    IRIList base = individuals;
    IRIList unique = individuals;
    IRIList blacklist;

    IRIList::const_iterator bit = base.begin();

    for(; bit != base.end(); ++bit)
    {
        IRI individual = *bit;
        if( blacklist.end() != std::find(blacklist.begin(), blacklist.end(), individual) )
        {
            LOG_DEBUG_S << "In blacklist: " << individual;
            continue;
        }

        IRIList::iterator uit = unique.begin();
        for(; uit != unique.end();)
        {
            IRI otherIndividual = *uit;
            if(individual != otherIndividual && isSameInstance(individual, otherIndividual) )
            {
                LOG_DEBUG_S << "Remove " << *uit << " since its an alias";
                unique.erase(uit);
                blacklist.push_back(otherIndividual);
            } else {
                ++uit;
            }
        }
    }

    return unique;
}

void KnowledgeBase::retract(const Axiom& a)
{
    mKernel->retract(const_cast<TDLAxiom*>(a.get()));
}

void KnowledgeBase::retractRelated(const owlapi::model::OWLAxiom::Ptr& a)
{
    ReferencedAxiomsMap::iterator it = mReferencedAxiomsMap.find(a);
    if(it != mReferencedAxiomsMap.end())
    {
        reasoner::factpp::Axiom::List& axioms = it->second;
        reasoner::factpp::Axiom::List::iterator ait = axioms.begin();
        for(; ait != axioms.end(); ++ait)
        {
            retract(*ait);
        }
    }
}

void KnowledgeBase::addReference(const owlapi::model::OWLAxiom::Ptr& modelAxiom, const reasoner::factpp::Axiom& kbAxiom)
{
    mReferencedAxiomsMap[modelAxiom].push_back(kbAxiom);
}

bool KnowledgeBase::assertAndAddRelation(const IRI& instance, const IRI& relation, const IRI& otherInstance)
{
    if( isRelatedTo(instance, relation, otherInstance) )
    {
        return true;
    } else {
        // seems to be unknown, or false so try add
        Axiom a = relatedTo(instance, relation, otherInstance);
        try {
            refresh();
            LOG_DEBUG_S << "'" << instance << "' '" << relation << "' '" << otherInstance << " is true -- added to db";
            return true;
        } catch(const std::exception& e)
        {
            // ok, that relation has already be added to the knowledge database
            retract(a);
            LOG_DEBUG_S << "'" << instance << "' '" << relation << "' '" << otherInstance << " is false";
            return false;
        }
    }
}

DataValue KnowledgeBase::getDataValue(const IRI& instance, const IRI& dataProperty)
{
    // Iterate over all axioms and filter the relevant one
    const AxiomVec& axioms = mKernel->getOntology().getAxioms();
    for(TDLAxiom* axiom : axioms)
    {
        TDLAxiomValueOf* valueAxiom = dynamic_cast<TDLAxiomValueOf*>(axiom);

        InstanceExpression instanceExpression = getInstance(instance);
        DataPropertyExpression dataPropertyExpression = getDataProperty(dataProperty);

        if(valueAxiom && valueAxiom->isUsed() && dataPropertyExpression.get() == valueAxiom->getAttribute() && instanceExpression.get() == valueAxiom->getIndividual())
        {
            return DataValue( valueAxiom->getValue());
        }
    }

    throw std::runtime_error("KnowledgeBase::getDataValue: instance " + instance.toQuotedString() + " has no value related via " + dataProperty.toQuotedString());

}

std::string KnowledgeBase::toString(representation::Type representation) const
{
    switch(representation)
    {
        case representation::LISP:
        {
            std::stringstream ss;
            TLISPOntologyPrinter printer(ss);

            TOntology& ontology = mKernel->getOntology();
            printer.visitOntology(ontology);

            return ss.str();
        }
        default:
            throw NotSupported("KnowledgeBase::toString: Representation is not supported");
    }

    //TOntology::iterator axiom = getOntology().begin();
    //for(; axiom != ontology.end(); ++axiom)
    //{
    //    axiom->accept(
    //}
}

ExplorationNode KnowledgeBase::getExplorationNode(const IRI& klass)
{
    ClassExpression e_klass = getClass(klass);
    ExplorationNode e( mKernel->buildCompletionTree(e_klass.get()));
    return e;
}

owlapi::model::IRISet KnowledgeBase::getRelatedObjectProperties(const IRI& instance, bool needInverse)
{
    owlapi::model::IRISet relatedProperties;
    InstanceExpression e_instance = getInstance(instance);
    bool isDataProperty = false;

    std::vector<const TNamedEntry*> names;
    mKernel->getRelatedRoles(e_instance.get(), names, isDataProperty, needInverse);
    for(const TNamedEntry* entry : names)
    {
        relatedProperties.insert( owlapi::model::IRI( entry->getName() ) );
    }
    return relatedProperties;
}

reasoner::factpp::ObjectPropertyExpressionList KnowledgeBase::getRelatedObjectPropertiesByKlass(const IRI& klass)
{
    ExplorationNode e_node = getExplorationNode(klass);
    ReasoningKernel::TCGRoleSet result;
    bool onlyDeterministicInfo = false;
    bool needIncoming = false;

    mKernel->getObjectRoles(e_node.get(), result, onlyDeterministicInfo, needIncoming);

    ObjectPropertyExpressionList relatedObjectProperties;
    ReasoningKernel::TCGRoleSet::const_iterator cit = result.begin();
    for(; cit != result.end(); ++cit)
    {
        TDLRoleExpression* role = const_cast<TDLRoleExpression*>(*cit);
        const ObjectPropertyExpression e_property(dynamic_cast<TDLObjectRoleExpression*>(role));
        relatedObjectProperties.push_back(e_property);
    }

    return relatedObjectProperties;
}

owlapi::model::IRISet KnowledgeBase::getRelatedDataProperties(const IRI& instance,
        bool needInverse)
{
    owlapi::model::IRISet dataProperties;

    for(const DataValueMap::value_type v : mValueOfAxioms)
    {
        if(v.first.first == instance)
        {
            dataProperties.insert(v.first.second);
        }
    }
    return dataProperties;

    // Factpp has the user API for requesting data property relations, but
    // it has not been implemented
    //
    //InstanceExpression e_instance = getInstance(instance);
    //bool isDataProperty = true;
    //std::vector<const TNamedEntry*> names;
    //mKernel->getRelatedRoles(e_instance.get(), names, isDataProperty, needInverse);

    //for(const TNamedEntry* entry : names)
    //{
    //    dataProperties.push_back( entry->getName() );
    //}
}

DataPropertyExpressionList KnowledgeBase::getRelatedDataPropertiesByKlass(const IRI& klass)
{
    ExplorationNode e_node = getExplorationNode(klass);
    ReasoningKernel::TCGRoleSet result;
    bool onlyDeterministicInfo = false;

    mKernel->getDataRoles(e_node.get(), result, onlyDeterministicInfo);

    DataPropertyExpressionList relatedDataProperties;
    ReasoningKernel::TCGRoleSet::const_iterator cit = result.begin();
    for(; cit != result.end(); ++cit)
    {
        TDLRoleExpression* role = const_cast<TDLRoleExpression*>(*cit);
        const DataPropertyExpression e_property(dynamic_cast<TDLDataRoleExpression*>(role));
        relatedDataProperties.push_back(e_property);
    }
    return relatedDataProperties;
}

void KnowledgeBase::cleanup()
{
    ReferencedAxiomsMap::iterator it = mReferencedAxiomsMap.begin();
    for(; it != mReferencedAxiomsMap.end(); ++it)
    {
        //owlapi::model::OWLAxiom::Ptr& axiom = it->first;
        if(it->first->isMarkedForRetraction())
        {
            retractRelated(it->first);
        }
    }

    // Cleanup cache as well
    mInstances.clear();
}

} // end namespace owlapi
