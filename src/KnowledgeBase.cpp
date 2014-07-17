#include "KnowledgeBase.hpp"

#include <factpp/Kernel.h>
#include <factpp/Actor.h>
#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>
#include <factpp/tOntologyPrinterLISP.h>

namespace owl_om {

std::map<KnowledgeBase::PropertyType, std::string> KnowledgeBase::PropertyTypeTxt = boost::assign::map_list_of
    ( KnowledgeBase::UNKNOWN_PROPERTY_TYPE, "UNKNOWN PROPERTY TYPE")
    ( KnowledgeBase::OBJECT, "OBJECT")
    ( KnowledgeBase::DATA, "DATA");

std::map<KnowledgeBase::EntityType, std::string> KnowledgeBase::EntityTypeTxt = boost::assign::map_list_of
    ( KnowledgeBase::UNKNOWN_ENTITY_TYPE, "UNKNOWN ENTITY TYPE")
    ( KnowledgeBase::CLASS, "CLASS")
    ( KnowledgeBase::INSTANCE, "INSTANCE")
    ( KnowledgeBase::OBJECT_PROPERTY, "OBJECT_RELATION")
    ( KnowledgeBase::DATA_PROPERTY, "DATA_RELATION");


Axiom::Axiom(TDLAxiom* axiom)
    : mAxiom(axiom)
{
}

ClassExpression::ClassExpression(TDLConceptExpression* expression)
    : mExpression(expression)
{}

InstanceExpression::InstanceExpression(TDLIndividualExpression* expression)
    : mExpression(expression)
{}

ObjectPropertyExpression::ObjectPropertyExpression(TDLObjectRoleExpression* expression)
    : mExpression(expression)
{}

DataPropertyExpression::DataPropertyExpression(TDLDataRoleExpression* expression)
    : mExpression(expression)
{}

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
        throw std::invalid_argument("Class '" + klass.toString() + "' does not exist");
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
        throw std::invalid_argument("Instance '" + instance.toString() + "' does not exist");
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
        throw std::invalid_argument("ObjectProperty '" + property.toString() + "' does not exist");
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

DataPropertyExpression KnowledgeBase::getDataProperty(const IRI& property) const
{
    IRIDataPropertyExpressionMap::const_iterator cit = mDataProperties.find(property);
    if(cit == mDataProperties.end())
    {
        throw std::invalid_argument("DataProperty '" + property.toString() + "' does not exist");
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
    mKernel->setTopBottomRoleNames ( "http://www.w3.org/2002/07/owl#topObjectProperty", \
        "http://www.w3.org/2002/07/owl#bottomObjectProperty", \
        "http://www.w3.org/2002/07/owl#topDataProperty", \
        "http://www.w3.org/2002/07/owl#bottomDataProperty");
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
            throw std::invalid_argument("Functional property can only be created for object or data property");
    }
}

Axiom KnowledgeBase::inverseFunctionalProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectPropertyLazy(property);
    return Axiom( mKernel->setInverseFunctional(e_property.get()) );
}

Axiom KnowledgeBase::reflexiveProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectPropertyLazy(property);
    return Axiom( mKernel->setReflexive(e_property.get()) );
}

Axiom KnowledgeBase::irreflexiveProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectPropertyLazy(property);
    return Axiom( mKernel->setIrreflexive(e_property.get()) );
}

Axiom KnowledgeBase::symmetricProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectPropertyLazy(property);
    return Axiom( mKernel->setSymmetric(e_property.get()) );
}

Axiom KnowledgeBase::asymmetricProperty(const IRI& property)
{
    ObjectPropertyExpression e_property = getObjectPropertyLazy(property);
    return Axiom( mKernel->setAsymmetric(e_property.get()) );
}

Axiom KnowledgeBase::subclassOf(const IRI& subclass, const IRI& parentClass)
{
    ClassExpression ce = getClassLazy(subclass);
    return subclassOf( ce, parentClass );
}

Axiom KnowledgeBase::subclassOf(const ClassExpression& subclass, const IRI& parentClass)
{
    ClassExpression e_parentClass = getClassLazy(parentClass);
    return Axiom( mKernel->impliesConcepts(subclass.get(), e_parentClass.get()) );
}

Axiom KnowledgeBase::subclassOf(const IRI& subclass, const ClassExpression& parentClass)
{
    ClassExpression e_subclass = getClassLazy(subclass);
    return Axiom( mKernel->impliesConcepts(e_subclass.get(), parentClass.get()) );
}

ClassExpression KnowledgeBase::intersectionOf(const IRI& klass, const IRI& otherKlass)
{
    IRIList klasses;
    klasses.push_back(klass);
    klasses.push_back(otherKlass);
    return intersectionOf(klasses);
}

ClassExpression KnowledgeBase::intersectionOf(const IRIList& klasses)
{
    IRIList::const_iterator cit = klasses.begin();
    getExpressionManager()->newArgList();
    for(; cit != klasses.end(); ++cit)
    {
        ClassExpression e_class = getClassLazy(*cit);
        getExpressionManager()->addArg(e_class.get());
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
            throw std::invalid_argument("Alias for entity type '" + EntityTypeTxt[type] + "' not supported");
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
        LOG_WARN("PROCESS DIFFERENT");
        TDLAxiom* axiom = mKernel->processDifferent();
        return Axiom(axiom);
    }

    throw std::runtime_error("owl_om::KnowledgeBase::disjoint requires either list of classes or instances");

}

Axiom KnowledgeBase::instanceOf(const IRI& individual, const IRI& klass)
{
    ClassExpression e_class = getClass(klass);
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

Axiom KnowledgeBase::inverseOf(const IRI& base, const IRI& inverse)
{
    ObjectPropertyExpression e_role = getObjectPropertyLazy(base);
    ObjectPropertyExpression e_inverse = getObjectPropertyLazy(inverse);

    return Axiom( mKernel->setInverseRoles(e_role.get(), e_inverse.get()) );
}

ClassExpression KnowledgeBase::oneOf(const IRIList& instanceList)
{
    IRIList::const_iterator cit = instanceList.begin();
    for(; cit != instanceList.end(); ++cit)
    {
        IRI instance = *cit;
        InstanceExpression e_instance = getInstanceLazy(instance);
        getExpressionManager()->newArgList();
        getExpressionManager()->addArg(e_instance.get());
    }
    return ClassExpression( getExpressionManager()->OneOf() );
}

ClassExpression KnowledgeBase::objectPropertyRestriction(restriction::Type type, const IRI& relationProperty, const IRI& klassOrInstance, int cardinality)
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

    throw std::runtime_error("owl_om::KnowledgeBase::objectPropertyRestriction: Unknown restriction type");
}

bool KnowledgeBase::isSubclassOf(const IRI& subclass, const IRI& parentClass)
{
    ClassExpression e_class = getClass(subclass);
    return isSubclassOf( e_class, parentClass );
}

bool KnowledgeBase::isSubclassOf(const ClassExpression& subclass, const IRI& parentClass)
{
    ClassExpression e_class = getClass(parentClass);
    return mKernel->isSubsumedBy(subclass.get(), e_class.get());
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

bool KnowledgeBase::isSameInstance(const IRI& instance, const IRI& otherInstance)
{
    InstanceExpression e_instance = getInstance(instance);
    InstanceExpression e_otherInstance = getInstance(otherInstance);
    return mKernel->isSameIndividuals(e_instance.get(), e_otherInstance.get());
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

IRIList KnowledgeBase::allSubclassesOf(const IRI& klass, bool direct)
{
    ClassExpression e_class = getClass(klass);
    IRIList subclasses;

    Actor actor;
    actor.needConcepts();
    mKernel->getSubConcepts(e_class.get(), direct, actor);
    const char** result = actor.getElements1D();
    for(size_t i = 0; result[i] != NULL; ++i)
    {
        LOG_DEBUG_S << "subconcept of '" << klass << "': " << result[i];
        subclasses.push_back( IRI::create(result[i]) );
    }
    delete[] result;
    return subclasses;
}

IRIList KnowledgeBase::allAncestorsOf(const IRI& klass, bool direct)
{
    ClassExpression e_class = getClass(klass);
    IRIList superclasses;

    Actor actor;
    actor.needConcepts();
    mKernel->getSupConcepts(e_class.get(), direct, actor);
    const char** result = actor.getElements1D();
    for(size_t i = 0; result[i] != NULL; ++i)
    {
        LOG_DEBUG_S << "super concept of '" << klass << "': " << result[i];
        superclasses.push_back( IRI::create(result[i]) );
    }
    delete[] result;
    return superclasses;
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

    const char** result = actor.getElements1D();
    for(size_t i = 0; result[i] != NULL; ++i)
    {
        // Fact does seem to fail at extracting direct instances
        IRI instanceName = IRI::create(result[i]);
        if(direct && ! (typeOf(instanceName) == klass) )
        {
            LOG_DEBUG_S << "instance '" << instanceName << "' not a direct instance of '" << klass << "'";
            continue;
        }
        LOG_DEBUG_S << "instance of '" << klass << "': " << instanceName;
        instances.push_back(instanceName);

    }
    delete[] result;
    return instances;
}

IRIList KnowledgeBase::allRelatedInstances(const IRI& individual, const IRI& relationProperty)
{
    InstanceExpression e_instance = getInstance(individual);
    ObjectPropertyExpression e_relation = getObjectProperty(relationProperty);

    ReasoningKernel::IndividualSet relatedIndividuals;
    mKernel->getRoleFillers(e_instance.get(), e_relation.get(), relatedIndividuals);

    IRIList individuals;
    ReasoningKernel::IndividualSet::const_iterator cit = relatedIndividuals.begin();
    for(; cit != relatedIndividuals.end(); ++cit)
    {
        const TNamedEntry* entry = *cit;
        LOG_INFO_S << "'" << individual << "' " << relationProperty << " '" << entry->getName() << "'";
        individuals.push_back( IRI::create( entry->getName() ) );
    }
    return individuals;
}

IRI KnowledgeBase::relatedInstance(const IRI& individual, const IRI& relationProperty)
{
    IRIList instances = allRelatedInstances(individual, relationProperty);
    if(instances.empty())
    {
        throw std::invalid_argument("KnowledgeBase::relatedInstance: no instance related to '" + individual.toString() + "' via property '" + relationProperty.toString() + "'");
    }
    return instances.front();
}

IRIList KnowledgeBase::allInverseRelatedInstances(const IRI& individual, const IRI& relationProperty)
{
    InstanceExpression e_instance = getInstance(individual);
    ObjectPropertyExpression e_relation = getObjectProperty(relationProperty);
    TDLObjectRoleExpression* f_inverse = mKernel->getExpressionManager()->Inverse(e_relation.get());

    ReasoningKernel::IndividualSet relatedIndividuals;
    mKernel->getRoleFillers(e_instance.get(), f_inverse, relatedIndividuals);

    IRIList individuals;
    ReasoningKernel::IndividualSet::const_iterator cit = relatedIndividuals.begin();
    for(; cit != relatedIndividuals.end(); ++cit)
    {
        const TNamedEntry* entry = *cit;
        LOG_INFO_S << "'" << individual << "' -" << relationProperty << " '" << entry->getName() << "'";
        individuals.push_back( IRI::create( entry->getName() ) );
    }
    return individuals;
}

IRIList KnowledgeBase::allObjectProperties() const
{
    IRIList properties;
    IRIObjectPropertyExpressionMap::const_iterator cit = mObjectProperties.begin();
    for(; cit != mObjectProperties.end(); ++cit)
    {
        properties.push_back(cit->first);
    }
    return properties;
}

IRIList KnowledgeBase::allDataProperties() const
{
    IRIList properties;
    IRIDataPropertyExpressionMap::const_iterator cit = mDataProperties.begin();
    for(; cit != mDataProperties.end(); ++cit)
    {
        properties.push_back(cit->first);
    }
    return properties;
}

IRIList KnowledgeBase::typesOf(const IRI& instance, bool direct) const
{
    InstanceExpression e_instance = getInstance(instance);

    IRIList klasses;

    Actor actor;
    actor.needConcepts();
    mKernel->getTypes(e_instance.get(), direct, actor);
    const char** result = actor.getElements1D();
    for(size_t i = 0; result[i] != NULL; ++i)
    {
        LOG_INFO_S << "instance " << instance << " of type '" << result[i] << "'";
        klasses.push_back( IRI::create(result[i]) );
    }
    delete[] result;
    return klasses;
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

    IRIList alias;

    Actor actor;
    actor.needIndividuals();
    mKernel->getSameAs(e_instance.get(), actor);
    const char** result = actor.getElements1D();
    for(size_t i = 0; result[i] != NULL; ++i)
    {
        LOG_INFO_S << "instance or alias of " << aliasOrInstance << " -> '" << result[i] << "'";
        alias.push_back( IRI::create(result[i]) );
    }
    delete[] result;
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

void KnowledgeBase::retract(Axiom& a)
{
    mKernel->retract(a.get());
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
            throw NotSupported("Representation is not supported");
    }

    //TOntology::iterator axiom = getOntology().begin();
    //for(; axiom != ontology.end(); ++axiom)
    //{
    //    axiom->accept(
    //}
}

} // namespace owl_om