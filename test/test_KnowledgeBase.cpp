#include <boost/test/unit_test.hpp>
#include "test_utils.hpp"

#include <boost/foreach.hpp>
#include <owlapi/KnowledgeBase.hpp>
#include <owlapi/vocabularies/OWL.hpp>

#include <factpp/Kernel.h>
#include <factpp/Actor.h>

using namespace owlapi;
using namespace owlapi::model;

BOOST_AUTO_TEST_SUITE(knowledge_base)

BOOST_AUTO_TEST_CASE(tell_and_ask)
{
    {
        KnowledgeBase kb;
        kb.setVerbose();

        IRI derived("Derived");
        IRI has("has");

        kb.subClassOf("Base", "Test");
        kb.subClassOf(derived, "Base");
        kb.objectProperty(has);
        kb.functionalProperty(has, KnowledgeBase::OBJECT);
        BOOST_REQUIRE_MESSAGE(kb.isFunctionalProperty(has), "Has is functional propery");

        IRI instance("instance");
        kb.instanceOf(instance, derived);

        kb.objectRangeOf(has, derived);
        kb.domainOf(has, derived, KnowledgeBase::OBJECT);

        reasoner::factpp::ObjectPropertyExpressionList list = kb.getRelatedObjectPropertiesByKlass(derived);
        kb.classify();
        BOOST_TEST_MESSAGE("Experimental evaluation of related object properties returned: " << list.size() << " properties");

        IRI numericValue("numericValue");
        kb.dataProperty(numericValue);

        owlapi::model::OWLLiteral::PtrList literals;
        OWLDataType doubleType( vocabulary::XSD::resolve("double") );
        literals.push_back(OWLLiteral::create("1.0",doubleType));
        literals.push_back(OWLLiteral::create("2.0",doubleType));
        literals.push_back(OWLLiteral::create("3.0",doubleType));

        reasoner::factpp::DataRange range = kb.dataOneOf(literals);
        kb.dataRangeOf(numericValue, range);

        reasoner::factpp::DataValue value = kb.dataValue("1.0", vocabulary::XSD::resolve("double").toString());
        reasoner::factpp::Axiom axiom = kb.valueOf(instance, numericValue, value);
        BOOST_REQUIRE_MESSAGE(kb.isConsistent(), "Using data range is valid");

        // Set invalid value
        kb.retract(axiom);
        reasoner::factpp::DataValue invalidValue = kb.dataValue("4.0",
                vocabulary::XSD::resolve("double").toString());
        kb.valueOf(instance, numericValue, invalidValue);
        BOOST_REQUIRE_MESSAGE(!kb.isConsistent(), "Using element outside of data range is invalid");

    }
}

BOOST_AUTO_TEST_CASE(class_hierarchy)
{
    KnowledgeBase kb;
    kb.setVerbose();
    kb.subClassOf("Derived", "Base");
    kb.subClassOf("DerivedDerived", "Derived");
    BOOST_REQUIRE_MESSAGE( kb.isSubClassOf("Derived", "Base"), "Derived is subclass of base");
    BOOST_REQUIRE_MESSAGE( kb.isSubClassOf("DerivedDerived", "Base"), "DerivedDerived is subclass of base");
    kb.instanceOf("Instance", "DerivedDerived");
    BOOST_REQUIRE_MESSAGE( kb.isInstanceOf("Instance", "DerivedDerived"), "instance of DerivedDerived");
    BOOST_REQUIRE_MESSAGE( kb.isInstanceOf("Instance", "Base"), "instance of Base");

    kb.instanceOf("A","Base");
    kb.instanceOf("B","Base");

    IRIList directInstances = kb.allInstancesOf("Base", true);
    BOOST_REQUIRE_MESSAGE(directInstances.size() == 2, "All direct instances of BASE: '" << directInstances.size() << "' expected 2");

    kb.disjoint("A","B", KnowledgeBase::INSTANCE );
    kb.relatedTo("A","sibling","B");
    reasoner::factpp::Axiom a = kb.relatedTo("A","sibling","B", false);

    BOOST_REQUIRE_THROW(kb.refresh(), std::exception);
    kb.retract(a);
    BOOST_REQUIRE_NO_THROW(kb.refresh()); //, "Kb inconsistent if inverses relation both apply" );

    BOOST_REQUIRE_MESSAGE( kb.isRelatedTo("A", "sibling", "B"), "A and B are related");
    BOOST_REQUIRE_THROW( !kb.isRelatedTo("A", "unknown", "B"), std::exception);

    IRIList klasses = kb.allClasses();
    BOOST_REQUIRE_MESSAGE( klasses.size() == 3, "Number of classes is '" << klasses.size() << "' expected 3");
}

BOOST_AUTO_TEST_CASE(property_hierarchy)
{
    using namespace reasoner::factpp;

    KnowledgeBase kb;
    kb.setVerbose();
    ObjectPropertyExpression oe = kb.objectProperty(vocabulary::OWL::topObjectProperty());
    kb.subPropertyOf("DerivedProperty", vocabulary::OWL::topObjectProperty());
}

BOOST_AUTO_TEST_CASE(equivalence)
{
    using namespace reasoner::factpp;

    KnowledgeBase kb;
    kb.setVerbose();
    kb.subClassOf("Base","Test");
    kb.subClassOf("OtherBase","Test");
    kb.equalClasses({ owlapi::model::IRI("Base"),owlapi::model::IRI("OtherBase")});

    BOOST_REQUIRE_MESSAGE(kb.isEquivalentClass("Base","OtherBase"), "Base and"
            " other base should be equivalent");

    kb.objectProperty("aProperty");
    kb.objectProperty("bProperty");
    kb.equalObjectProperties({ owlapi::model::IRI("aProperty"),
            owlapi::model::IRI("bProperty") });

    kb.getInstanceLazy("aInstance");
    kb.getInstanceLazy("bInstance");
    kb.relatedTo("aInstance", "aProperty", "bInstance");
    BOOST_REQUIRE_MESSAGE(kb.isRelatedTo("aInstance", "aProperty", "bInstance"),
            "a and b are related via aProperty");
    BOOST_REQUIRE_MESSAGE(kb.isRelatedTo("aInstance", "bProperty", "bInstance"),
            "a and b are related via bProperty - since aProperty is equivalent to bProperty");
}

BOOST_AUTO_TEST_CASE(disjointness)
{
    using namespace reasoner::factpp;

    KnowledgeBase kb;
    kb.setVerbose();
    kb.subClassOf("Base","Test");
    kb.subClassOf("OtherBase","Test");
    kb.disjointClasses({ owlapi::model::IRI("Base"),owlapi::model::IRI("OtherBase")});

    BOOST_REQUIRE_MESSAGE(kb.isDisjointClass("Base","OtherBase"), "Base and"
            " other base should be disjoint");
}

BOOST_AUTO_TEST_CASE(data_value)
{
    KnowledgeBase kb;

    {
        reasoner::factpp::DataValue dataValueString = kb.dataValue("foo","string");
        BOOST_REQUIRE_THROW( dataValueString.toDouble(), std::runtime_error);
    }

    kb.subClassOf("Robot","TOP");
    kb.instanceOf("CREX","Robot");
    kb.instanceOf("Sherpa","Robot");
    kb.dataProperty("hasWeight");

    {
        reasoner::factpp::DataValue dataValue = kb.dataValue("100","int");
        kb.valueOf("CREX","hasWeight", dataValue);

        BOOST_REQUIRE_MESSAGE( dataValue.toDouble() == 100.0, "Conversion to double");
        reasoner::factpp::DataValue dbValue = kb.getDataValue("CREX","hasWeight");
        BOOST_REQUIRE_MESSAGE(dbValue.toDouble() == 100.0, "CREX hasWeight: db value expected: 100.0 was " << dbValue.toDouble());
    }

    {
        reasoner::factpp::DataValue dataValue = kb.dataValue("50","int");
        kb.valueOf("Sherpa","hasWeight", dataValue);

        BOOST_REQUIRE_MESSAGE( dataValue.toDouble() == 50.0, "Conversion to double");
        {
            reasoner::factpp::DataValue dbValue = kb.getDataValue("Sherpa","hasWeight");
            BOOST_REQUIRE_MESSAGE(dbValue.toDouble() == 50.0, "Sherpa hasWeight: db value expected: 50.0 was " << dbValue.toDouble());
        }

        {
            reasoner::factpp::DataValue dbValue = kb.getDataValue("CREX","hasWeight");
            BOOST_REQUIRE_MESSAGE(dbValue.toDouble() == 100.0, "CREX hasWeight: db value expected: 100.0 was " << dbValue.toDouble());
        }
    }

    // getNeighbours etc. do not work for data, thus implemented an alternative way
    // to retrieve information about data value from this representation
    //
    //kb.subClassOf("Sensor","TOP");
    //kb.instanceOf("Camera","Sensor");
    //kb.objectProperty("hasPart");
    //kb.relatedTo("CREX","hasPart", "Camera");
    //const KnowledgeExplorer::TCGNode* node = kb.getReasoningKernel()->buildCompletionTree(klass.get());
    //
    //ClassExpression klass = kb.getClassLazy("Robot");
    //InstanceExpression instance = kb.getInstance("CREX");
    //
    //{
    //    KnowledgeExplorer::TCGNodeVec result;
    //    kb.getReasoningKernel()->getNeighbours ( node, dataRole.get(), result );
    //    BOOST_TEST_MESSAGE("RESULT SIZE: " << result.size());
    //}

    //{
    //    ReasoningKernel::NamesVector vector;
    //    bool data = true;
    //    bool needInverse = false;
    //    kb.getReasoningKernel()->getRelatedRoles (instance.get(), vector, data, needInverse);
    //    BOOST_TEST_MESSAGE("RESULT SIZE: " << vector.size());
    //    // std::vector<const TNamedEntry*>
    //    BOOST_FOREACH(const TNamedEntry* entry, vector)
    //    {
    //        BOOST_TEST_MESSAGE("Entry " << entry->getName() << " id: " << entry->getId());
    //    }
    //}

    //{
    //    ReasoningKernel::TCGRoleSet result;
    //    bool onlyDet = false;
    //    kb.getReasoningKernel()->getDataRoles (node, result, onlyDet );
    //    BOOST_TEST_MESSAGE("ROLE SET SIZE: " << result.size());
    //}
    {
        reasoner::factpp::DataValue dataValue = kb.getDataValue("CREX","hasWeight");
        BOOST_TEST_MESSAGE( "DataValue: " << dataValue.getType() << " - " << dataValue.getValue());
    }
}

BOOST_AUTO_TEST_CASE(data_properties)
{
    {
        KnowledgeBase kb;
        kb.setVerbose();

        IRI derived("Derived");
        IRI has("has");
        IRI geometricProperty("geometricProperty");
        IRI minLength("minLength");
        IRI maxLength("maxLength");

        kb.subClassOf("Base", "Test");
        kb.subClassOf(derived, "Base");

        kb.objectProperty(has);

        kb.dataProperty(geometricProperty);

        kb.dataProperty(minLength);
        kb.subClassOf(minLength, geometricProperty);

        kb.dataProperty(maxLength);
        kb.subClassOf(maxLength, geometricProperty);

        kb.functionalProperty(minLength, KnowledgeBase::DATA);
        kb.functionalProperty(maxLength, KnowledgeBase::DATA);

        IRI instance("instance");
        IRI otherInstance("otherInstance");
        kb.instanceOf(instance, derived);
        kb.instanceOf(otherInstance, derived);
        kb.relatedTo(instance, has, otherInstance);

        reasoner::factpp::DataValue minValue = kb.dataValue("1.0", vocabulary::XSD::resolve("double").toString());
        reasoner::factpp::DataValue maxValue = kb.dataValue("1.0", vocabulary::XSD::resolve("double").toString());
        reasoner::factpp::Axiom axiomMinLength = kb.valueOf(instance, minLength, minValue);
        reasoner::factpp::Axiom axiomMaxLength = kb.valueOf(instance, maxLength, maxValue);

        owlapi::model::IRISet list = kb.getRelatedObjectProperties(instance);
        BOOST_CHECK_MESSAGE(list.size() == 1, "Found '" << list.size()
                << "' related object properties: " << list);

        owlapi::model::IRISet dlist = kb.getRelatedDataProperties(instance);
        BOOST_REQUIRE_MESSAGE(dlist.size() == 2, "Found '" << dlist.size()
                << "' related data properties "
                << dlist);
    }
}

BOOST_AUTO_TEST_CASE(data_restriction_equivalence)
{
    KnowledgeBase kb;
    kb.setVerbose();

    IRI item("klass_item");
    IRI item2m("klass_item_2m");
    IRI hasLength("hasLength");
    kb.getClassLazy(item);
    kb.instanceOf(item, item);

    reasoner::factpp::ClassExpression item2mClass = kb.getClassLazy(item2m);
    kb.instanceOf(item2m, item2m);

    // Class requires instances query, otherwise
    // facpp++ show memory error
    kb.refresh();

    IRIList instancesOfItem2m = kb.allInstancesOf(item2m);
    BOOST_TEST_MESSAGE("All instances: " << instancesOfItem2m);

    reasoner::factpp::DataPropertyExpression hasLengthProperty = kb.dataProperty(hasLength);
    reasoner::factpp::DataTypeName dataTypeName = kb.dataType(vocabulary::XSD::resolve("double").toString());
    //
    //kb.getReasoningKernel()->setDRange(hasLengthProperty.get(), dataTypeName.get());

    reasoner::factpp::DataValue minValue = kb.dataValue("1.0", vocabulary::XSD::resolve("double").toString());
    reasoner::factpp::DataValue maxValue = kb.dataValue("2.0", vocabulary::XSD::resolve("double").toString());

    const TDLFacetExpression* minFacetExpression =
        kb.getExpressionManager()->FacetMinInclusive(minValue.get());
    const TDLFacetExpression* maxFacetExpression =
        kb.getExpressionManager()->FacetMaxExclusive(maxValue.get());
    BOOST_REQUIRE_MESSAGE(kb.isConsistent(), "KB remains consistent");

    TDLDataTypeRestriction* dataTypeRestriction = kb.getExpressionManager()->RestrictedType(dataTypeName.get(),
        minFacetExpression);
    dataTypeRestriction = kb.getExpressionManager()->RestrictedType(dataTypeRestriction, maxFacetExpression);
    BOOST_REQUIRE_MESSAGE(kb.isConsistent(), "KB remains consistent");

    reasoner::factpp::DataValue value = kb.dataValue("1.5", vocabulary::XSD::resolve("double").toString());
    TDLConceptExpression* aConcept = kb.getExpressionManager()->Exists(hasLengthProperty.get(),
            dataTypeRestriction);
    //TDLConceptExpression* aConcept = kb.getExpressionManager()->Value(hasLengthProperty.get(),
    //        value.get());
    reasoner::factpp::ClassExpression lengthRestriction(aConcept);
    kb.equals(item2mClass, lengthRestriction);

    BOOST_REQUIRE_MESSAGE(kb.isConsistent(), "KB remains consistent");

    // Now check if the item_0 with the property in the corresponding range
    // is automatically set as subclass of Item_2m
    IRI item_0("Instance_item_0");
    kb.instanceOf(item_0, item);
    kb.valueOf(item_0, hasLength, value);

    IRI item_1("Instance_item_1");
    kb.instanceOf(item_1, item);
    kb.valueOf(item_1, hasLength, minValue);
    kb.classify();

    IRI item_2("Instance_item_2");
    kb.instanceOf(item_2, item);
    kb.valueOf(item_2, hasLength, maxValue);
    kb.classify();

    IRIList instancesOfItem_2m = kb.allInstancesOf(item2m);
    BOOST_REQUIRE_MESSAGE(kb.isInstanceOf(item_0, item2m), "Automatically added parent class for item_0");
    BOOST_REQUIRE_MESSAGE(kb.isInstanceOf(item_1, item2m), "Automatically added parent class for item_1");
    BOOST_REQUIRE_MESSAGE(!kb.isInstanceOf(item_2, item2m), "Not adding parent class for item_2");


    BOOST_REQUIRE_MESSAGE(std::find(instancesOfItem_2m.begin(), instancesOfItem_2m.end(), item_0) != instancesOfItem_2m.end(), "Automatically resolved item in range");
    BOOST_REQUIRE_MESSAGE(std::find(instancesOfItem_2m.begin(), instancesOfItem_2m.end(), item_1) != instancesOfItem_2m.end(), "Automatically resolved item in range");
    BOOST_REQUIRE_MESSAGE(std::find(instancesOfItem_2m.begin(), instancesOfItem_2m.end(), item_2) == instancesOfItem_2m.end(), "Automatically ignored item outside range");


    {
        IRI item2x2m("class_item_2x2m");
        reasoner::factpp::ClassExpression item2x2mClass = kb.getClassLazy(item2x2m);
        kb.instanceOf(item2x2m,item2x2m);

        IRI hasHeight("hasHeight");
        reasoner::factpp::DataPropertyExpression hasHeightProperty = kb.dataProperty(hasHeight);
        reasoner::factpp::DataTypeName dataTypeName = kb.dataType("double");
        TDLConceptExpression* hConcept = kb.getExpressionManager()->Exists(hasHeightProperty.get(),
                dataTypeRestriction);
        reasoner::factpp::ClassExpression heightRestriction(hConcept);

        reasoner::factpp::ClassExpression heightAndLengthRestriction = kb.intersectionOf(heightRestriction, lengthRestriction);

        kb.equals(item2x2mClass, heightAndLengthRestriction);

        IRI item_3("Instance_item_3");
        kb.instanceOf(item_3, item);
        kb.valueOf(item_3, hasLength, minValue);
        kb.valueOf(item_3, hasHeight, minValue);
        kb.classify();

        BOOST_REQUIRE_MESSAGE(kb.isInstanceOf(item_3, item2x2m), "Automatically added parent class for item_3");

        IRI item_4("Instance_item_4");
        kb.instanceOf(item_4, item);
        kb.valueOf(item_4, hasLength, minValue);
        kb.valueOf(item_4, hasHeight, maxValue);
        kb.classify();

        BOOST_REQUIRE_MESSAGE(!kb.isInstanceOf(item_4, item2x2m), "Not classifying class as item_2x2m for item_4");
    }
}

BOOST_AUTO_TEST_CASE(object_restriction_equivalence)
{
    KnowledgeBase kb;
    kb.setVerbose();

    IRI item("item");
    reasoner::factpp::ClassExpression itemClass = kb.getClassLazy(item);
    kb.instanceOf(item, item);

    IRI component("component");
    reasoner::factpp::ClassExpression componentClass = kb.getClassLazy(component);
    kb.instanceOf(component, component);

    IRI item_with_components("item_with_components");
    reasoner::factpp::ClassExpression item_with_componentsClass =
        kb.getClassLazy(item_with_components);
    kb.instanceOf(item_with_components, item_with_components);

    IRI hasComponent("hasComponent");
    reasoner::factpp::ObjectPropertyExpression hasComponentProperty = kb.objectProperty(hasComponent);

    IRI wheel_0("wheel_0");
    IRI wheel_1("wheel_1");
    IRI wheel_2("wheel_2");
    IRI wheel_3("wheel_3");
    kb.instanceOf(wheel_0, component);
    kb.instanceOf(wheel_1, component);
    kb.instanceOf(wheel_2, component);
    kb.instanceOf(wheel_3, component);

    // Class requires instances query, otherwise
    // facpp++ show memory error
    kb.refresh();

    // TDLConceptExpression* Exists ( const TDLObjectRoleExpression* R, const TDLConceptExpression* C )
    // TDLConceptExpression* MinCardinality ( unsigned int n, const TDLObjectRoleExpression* R, const TDLConceptExpression* C )
    TDLConceptExpression* minConcept =
        kb.getExpressionManager()->MinCardinality(0, hasComponentProperty.get(), componentClass.get());

    TDLConceptExpression* maxConcept =
        kb.getExpressionManager()->MaxCardinality(3, hasComponentProperty.get(), componentClass.get());

    reasoner::factpp::ClassExpression minConceptExpr(minConcept);
    reasoner::factpp::ClassExpression maxConceptExpr(maxConcept);
    std::vector<reasoner::factpp::ClassExpression> klasses = { minConceptExpr, maxConceptExpr };
    reasoner::factpp::ClassExpression intersection = kb.intersectionOf(klasses);

    kb.equals(item_with_componentsClass, intersection);
    BOOST_REQUIRE_MESSAGE(kb.isConsistent(), "KB remains consistent");

    // Now check if the item_0 with the property in the corresponding range
    // is automatically set as subclass of Item_2m

    // In order to handle the open world assumption https://mailman.stanford.edu/pipermail/protege-owl/2011-February/016047.html
    // we need to restrict item_0 a bit further, here limiting to a maximum of
    // three instances - otherwise it would not be guaranteed that cardinality is maximum 3
    IRI id("oneOf_wheels");
    reasoner::factpp::ClassExpression oneOfWheels = kb.objectOneOf(id, { wheel_0, wheel_1,
            wheel_2 });
    TDLConceptExpression* restrictValuesConcept =
        kb.getExpressionManager()->Forall(hasComponentProperty.get(),
                oneOfWheels.get());
    kb.getReasoningKernel()->impliesConcepts(itemClass.get()
            , restrictValuesConcept);

    IRI item_0("item_0");
    reasoner::factpp::ClassExpression item0Class = kb.getClassLazy(item_0);
    kb.getReasoningKernel()->impliesConcepts(item0Class.get()
            , restrictValuesConcept);
    kb.instanceOf(item_0, item_0);
    kb.relatedTo(item_0, hasComponent, wheel_0);
    BOOST_REQUIRE_MESSAGE(kb.isConsistent(), "KB remains consistent");

    IRI item_1("item_1");
    reasoner::factpp::ClassExpression item1Class = kb.getClassLazy(item_1);
    kb.getReasoningKernel()->impliesConcepts(item1Class.get()
            , restrictValuesConcept);
    kb.instanceOf(item_1, item_1);
    kb.relatedTo(item_1, hasComponent, wheel_0);
    kb.relatedTo(item_1, hasComponent, wheel_1);
    kb.relatedTo(item_1, hasComponent, wheel_2);
    BOOST_REQUIRE_MESSAGE(kb.isConsistent(), "KB remains consistent");


    IRI item_2("item_2");
    reasoner::factpp::ClassExpression item2Class = kb.getClassLazy(item_2);
    reasoner::factpp::ClassExpression oneOf4Wheels = kb.objectOneOf(IRI("oneOf_4wheels"), { wheel_0, wheel_1, wheel_2, wheel_3 });
    TDLConceptExpression* restrictValuesOf4Concept =
        kb.getExpressionManager()->Forall(hasComponentProperty.get(),
                oneOf4Wheels.get());
    kb.getReasoningKernel()->impliesConcepts(itemClass.get()
            , restrictValuesOf4Concept);
    kb.instanceOf(item_2, item_2);
    // The actual relation do not affect the reasoner
    //kb.relatedTo(item_2, hasComponent, wheel_0);
    //kb.relatedTo(item_2, hasComponent, wheel_1);
    //kb.relatedTo(item_2, hasComponent, wheel_2);
    //kb.relatedTo(item_2, hasComponent, wheel_3);
    BOOST_REQUIRE_MESSAGE(kb.isConsistent(), "KB remains consistent");

    IRIList instancesOfItemWithComponents = kb.allInstancesOf(item_with_components);
    BOOST_TEST_MESSAGE("Instances" << instancesOfItemWithComponents);
    BOOST_REQUIRE_MESSAGE(kb.isInstanceOf(item_0, item_with_components), "Automatically added parent class for item_0");
    BOOST_REQUIRE_MESSAGE(kb.isInstanceOf(item_1, item_with_components), "Automatically added parent class for item_1");
    BOOST_REQUIRE_MESSAGE(!kb.isInstanceOf(item_2, item_with_components), "Not adding parent class for item_2");
}

BOOST_AUTO_TEST_SUITE_END()
