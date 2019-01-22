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

        reasoner::factpp::ObjectPropertyExpressionList list = kb.getRelatedObjectProperties(derived);
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

BOOST_AUTO_TEST_SUITE_END()
