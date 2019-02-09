#include <boost/test/unit_test.hpp>
#include "test_utils.hpp"

#include <owlapi/model/OWLOntology.hpp>
#include <owlapi/io/OWLOntologyIO.hpp>
#include <owlapi/model/OWLOntologyAsk.hpp>
#include <owlapi/model/OWLOntologyTell.hpp>

using namespace owlapi;
using namespace owlapi::model;

BOOST_AUTO_TEST_SUITE(ontology)

BOOST_AUTO_TEST_CASE(ask)
{
    OWLOntology::Ptr ontology = io::OWLOntologyIO::fromFile( getRootDir() + "/test/data/om-schema-v0.6.owl");
    OWLOntologyAsk ask(ontology);

    IRI resource("http://www.rock-robotics.org/2014/01/om-schema#Resource");
    IRI actor("http://www.rock-robotics.org/2014/01/om-schema#Actor");
    IRI sherpa("http://www.rock-robotics.org/2014/01/om-schema#Sherpa");
    IRI payload_camera("http://www.rock-robotics.org/2014/01/om-schema#PayloadCamera");

    std::vector<IRIList> subclasses = ask.allSubClassesOfWithDistance(resource );
    size_t i = 0;
    for(const IRIList& s : subclasses)
    {
        BOOST_TEST_MESSAGE("Subclasses: distance: " << i << " " << s);
        ++i;
    }

    BOOST_REQUIRE_MESSAGE(!ask.isDirectSubClassOf(sherpa, resource), "Direct subclass: " << sherpa << " of " << resource);
    BOOST_REQUIRE_MESSAGE(ask.isDirectSubClassOf(sherpa, actor), "Direct subclass: " << sherpa << " of " << actor);
    BOOST_REQUIRE_MESSAGE(!ask.isDirectSubClassOf(payload_camera, resource), "Not a direct subclass: " << payload_camera << " of " << resource);
}

BOOST_AUTO_TEST_CASE(load_restrictions)
{
    OWLOntology::Ptr ontology = io::OWLOntologyIO::fromFile( getRootDir() + "/test/data/om-schema-v0.6.owl");
    OWLOntologyAsk ask(ontology);


    {
        std::vector<IRI> klasses;
        IRI sherpa("http://www.rock-robotics.org/2014/01/om-schema#Sherpa");
        klasses.push_back(sherpa);
        std::vector<OWLCardinalityRestriction::Ptr> restrictions = ask.getCardinalityRestrictions(klasses);
        BOOST_REQUIRE(restrictions.size() > 0);

        std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
        for(; cit != restrictions.end(); ++cit)
        {
            BOOST_TEST_MESSAGE("Restriction: " << (*cit)->toString());
        }

        std::pair<OWLCardinalityRestriction::PtrList, bool> cachedResult =  ontology->getQueryCache().getCardinalityRestrictions(klasses, IRI(), OWLCardinalityRestriction::SUM_OP);
        BOOST_REQUIRE_MESSAGE(cachedResult.second && cachedResult.first ==
                restrictions, "Query results have been cached");
    }
    {
        std::vector<IRI> klasses;
        IRI sherpa("http://www.rock-robotics.org/2014/01/om-schema#Sherpa");
        IRI propertyHas("http://www.rock-robotics.org/2014/01/om-schema#has");
        klasses.push_back(sherpa);
        std::vector<OWLCardinalityRestriction::Ptr> restrictions = ask.getCardinalityRestrictions(klasses, propertyHas);
        BOOST_REQUIRE(restrictions.size() > 0);

        std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
        for(; cit != restrictions.end(); ++cit)
        {
            BOOST_TEST_MESSAGE("Restriction: " << (*cit)->toString());
        }
    }
    {
        IRI iri("http://www.rock-robotics.org/2014/01/om-schema#PayloadCamera");
        std::vector<OWLCardinalityRestriction::Ptr> restrictions = ask.getCardinalityRestrictions(iri);
        BOOST_CHECK_MESSAGE(restrictions.size() > 2, "PayloadCamera should inherit restrictions");

        std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
        for(; cit != restrictions.end(); ++cit)
        {
            BOOST_TEST_MESSAGE("Restrictions: " << (*cit)->toString());
        }
    }

    // Subsequent calls should benefit from cache
    BOOST_TEST_MESSAGE("Caching active");
    {
        std::vector<IRI> klasses;
        IRI sherpa("http://www.rock-robotics.org/2014/01/om-schema#Sherpa");
        klasses.push_back(sherpa);
        std::vector<OWLCardinalityRestriction::Ptr> restrictions = ask.getCardinalityRestrictions(klasses);
        BOOST_REQUIRE(restrictions.size() > 0);

        std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
        for(; cit != restrictions.end(); ++cit)
        {
            BOOST_TEST_MESSAGE("Restriction: " << (*cit)->toString());
        }
    }
    {
        std::vector<IRI> klasses;
        IRI sherpa("http://www.rock-robotics.org/2014/01/om-schema#Sherpa");
        IRI propertyHas("http://www.rock-robotics.org/2014/01/om-schema#has");
        klasses.push_back(sherpa);
        std::vector<OWLCardinalityRestriction::Ptr> restrictions = ask.getCardinalityRestrictions(klasses, propertyHas);
        BOOST_REQUIRE(restrictions.size() > 0);

        std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
        for(; cit != restrictions.end(); ++cit)
        {
            BOOST_TEST_MESSAGE("Restriction: " << (*cit)->toString());
        }
    }
    {
        IRI iri("http://www.rock-robotics.org/2014/01/om-schema#PayloadCamera");
        std::vector<OWLCardinalityRestriction::Ptr> restrictions = ask.getCardinalityRestrictions(iri);
        BOOST_CHECK_MESSAGE(restrictions.size() > 2, "PayloadCamera should inherit restrictions");

        std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = restrictions.begin();
        for(; cit != restrictions.end(); ++cit)
        {
            BOOST_TEST_MESSAGE("Restrictions: " << (*cit)->toString());
        }
    }
}

BOOST_AUTO_TEST_CASE(punning)
{
    OWLOntology::Ptr ontology = io::OWLOntologyIO::fromFile( getRootDir() + "/test/data/om-schema-v0.9.owl");
    OWLOntologyAsk ask(ontology);
    OWLOntologyTell tell(ontology);

    owlapi::vocabulary::Custom OM("http://www.rock-robotics.org/2014/01/om-schema#");

    using namespace owlapi::model;
    {
        IRI actor = OM.resolve("Sherpa");
        IRI property = OM.resolve("maxVelocity");

        {
            // Data retrieval
            OWLLiteral::Ptr value = ask.getDataValue(actor, property);
            BOOST_REQUIRE_MESSAGE(value, "Predefined data value of " << actor.toString() << " for " << property.toString() << " is found: " << value->getDouble());
        }

        {
            // Data setting
            OWLLiteral::Ptr value = OWLLiteral::create("7.3", owlapi::vocabulary::XSD::resolve("double"));
            tell.valueOf(actor, property, value);
            ontology->refresh();

            value = ask.getDataValue(actor, property);
            BOOST_REQUIRE_MESSAGE(value->getDouble() == 7.3, "Updated data value of " << actor.toString() << " for " << property.toString() << " is found: " << value->getDouble());
        }
    }
    {
        IRI actor = OM.resolve("Sherpa");
        IRI property = OM.resolve("probabilityOfFailure");

        OWLLiteral::Ptr value = ask.getDataValue(actor, property);

        BOOST_REQUIRE_MESSAGE(value, "Data value of " << actor << " for " << property << " is found: " << value->getDouble());

        {
            using namespace owlapi::vocabulary;
            IRIList domains = ask.getDataPropertyDomain(property, true);

            BOOST_REQUIRE_MESSAGE(domains.size() == 1, "Domains for property for '" << property << "' should not be empty "
                    << " was " << domains);
        }

        {
            IRIList domains = ask.getDataPropertyDomain(property, false);
            BOOST_REQUIRE_MESSAGE(!domains.empty(), "Domains for property '" << property << "' should not be empty"
                    << " was " << domains);
        }

        {
            IRIList properties = ask.getDataPropertiesForDomain(actor);
            BOOST_REQUIRE_MESSAGE(!properties.empty(), "Data properties for '" << actor << "' should not be empty, was " << properties);
        }
    }
    {
        IRI actor = OM.resolve("Sherpa");
        IRI property = OM.resolve("notExistingProperty");

        BOOST_REQUIRE_THROW(ask.getDataValue(actor, property), std::invalid_argument);
        BOOST_REQUIRE_THROW( ask.getDataPropertyDomain(property, true), std::invalid_argument);
    }

    {
        // Try setting of value for parent class via punning
        // Should be inherited to subclasses (and punning instances thereof)
        IRI actor = OM.resolve("Actor");
        IRI property = OM.resolve("probabilityOfFailure");

        OWLLiteral::Ptr defaultProbabilityOfFailure = OWLLiteral::create("0.5", owlapi::vocabulary::XSD::resolve("double"));
        tell.instanceOf(actor, actor);
        tell.valueOf(actor, property, defaultProbabilityOfFailure);

        OWLLiteral::Ptr value = ask.getDataValue(actor, property);
        BOOST_REQUIRE_MESSAGE(value->getDouble() == 0.5, "Data value of " << actor << " for " << property << " is found: " << value->getDouble());

        IRI arobot = OM.resolve("ARobot");
        tell.subClassOf(arobot, actor);
        tell.instanceOf(arobot, arobot);
        value = ask.getDataValue(arobot, property);

        BOOST_REQUIRE_MESSAGE(value->getDouble() == 0.5, "Data value of " << actor << " for " << property << " is found: " << value->getDouble());

        OWLLiteral::Ptr probabilityOfFailure = OWLLiteral::create("0.8", owlapi::vocabulary::XSD::resolve("double"));
        tell.valueOf(arobot, property, probabilityOfFailure);
        value = ask.getDataValue(arobot, property);
        BOOST_REQUIRE_MESSAGE(value->getDouble() == 0.8, "Data value of " << actor << " for " << property << " is found: " << value->getDouble());
    }
}

BOOST_AUTO_TEST_CASE(retract)
{
    OWLOntology::Ptr ontology = make_shared<OWLOntology>();
    OWLOntologyAsk ask(ontology);
    OWLOntologyTell tell(ontology);

    IRI klass("http://my-classes#class-0");
    IRI instance("http://my-classes/#class-0-instance-0");

    tell.klass(klass);
    OWLClassAssertionAxiom::Ptr axiom = tell.instanceOf(instance, klass);

    BOOST_REQUIRE_MESSAGE(ask.isInstanceOf(instance, klass), "Instance " << instance << " of " << klass);
    BOOST_REQUIRE_MESSAGE( ask.allInstancesOf(klass).size() == 1, "One instance of klass type " << klass);

    tell.removeIndividual(instance);

    BOOST_REQUIRE_THROW(ask.isInstanceOf(instance, klass), std::invalid_argument);
    BOOST_REQUIRE_MESSAGE( ask.allInstancesOf(klass).empty(), "No instances remaining of klass " << klass);
}

BOOST_AUTO_TEST_CASE(equivalence)
{
    OWLOntology::Ptr ontology = make_shared<OWLOntology>();
    OWLOntologyAsk ask(ontology);
    OWLOntologyTell tell(ontology);

    IRI klass0("http://my-classes#class-0");
    IRI klass1("http://my-classes/#class-1");
    IRI subclass("http://my-classes/#subclass-0");

    tell.klass(klass0);
    tell.klass(klass1);
    tell.klass(subclass);
    tell.subClassOf(subclass, klass0);

    BOOST_REQUIRE_MESSAGE( ask.isDirectSubClassOf(subclass, klass0), "Subclass"
            " is subclass of original class");
    BOOST_REQUIRE_MESSAGE( !ask.isDirectSubClassOf(subclass, klass1), "Subclass"
            " is not subclass of other class");

    tell.equalClasses({klass0, klass1});
    BOOST_REQUIRE_MESSAGE( ask.isDirectSubClassOf(subclass, klass1), "Subclass"
            " is subclass of equivalent class");
}

BOOST_AUTO_TEST_CASE(ranges)
{
    OWLOntology::Ptr ontology = make_shared<OWLOntology>();
    OWLOntologyAsk ask(ontology);
    OWLOntologyTell tell(ontology);

    IRI klass("http://my-classes#class");
    IRI instance("http://my-classes/#instance");
    IRI property("http://my-classes/#property");
    IRI subproperty0("http://my-classes/#subproperty0");
    IRI subproperty1("http://my-classes/#subproperty1");

    tell.klass(klass);
    tell.instanceOf(instance, klass);
    tell.dataProperty(property);
    tell.subPropertyOf(subproperty0, property);
    tell.subPropertyOf(subproperty1, property);

    tell.dataPropertyRangeOf(property, vocabulary::XSD::resolve("double"));
    tell.dataPropertyRangeOf(subproperty0, vocabulary::XSD::resolve("double"));
    tell.dataPropertyRangeOf(subproperty1, vocabulary::XSD::resolve("double"));

    OWLLiteral::Ptr value = OWLLiteral::create("7.3", owlapi::vocabulary::XSD::resolve("double"));
    BOOST_REQUIRE_NO_THROW( tell.valueOf(instance, property, value) );
    BOOST_REQUIRE_NO_THROW( tell.valueOf(instance, subproperty0, value) );
    BOOST_REQUIRE_NO_THROW( tell.valueOf(instance, subproperty1, value) );
}

BOOST_AUTO_TEST_CASE(turtle_value_types)
{
    std::string filename =  getRootDir() +
        "test/data/test-turtle-value_types.ttl";

    OWLOntology::Ptr ontology = io::OWLOntologyIO::fromFile( filename );
    OWLOntologyAsk ask(ontology);

    owlapi::vocabulary::Custom custom("http://www.rock-robotics.org/test/turtle/value_types#");

    {
        OWLLiteral::Ptr literal = ask.getDataValue( custom.resolve("AClass"),
                custom.resolve("datapropertyA"));
        BOOST_REQUIRE_MESSAGE(literal->getDouble() == 1.0, "Get datapropertyA from TTL");
    }
}

BOOST_AUTO_TEST_CASE(restrictions_monolithic)
{
    OWLOntology::Ptr ontology = OWLOntology::fromFile( getRootDir() +
            "/test/data/test-restrictions.ttl");

    owlapi::vocabulary::Custom vocab("http://www.rock-robotics.org/test/turtle/restrictions#");

    OWLOntologyAsk ask(ontology);
    OWLCardinalityRestriction::PtrList restrictions;
    restrictions = ask.getCardinalityRestrictions(vocab.resolve("RobotA"), vocab.resolve("has"));
    BOOST_REQUIRE_MESSAGE( restrictions.size() == 4,
                "RobotA has 4 restrictions, found: " << OWLCardinalityRestriction::toString(restrictions, 4));

    restrictions = ask.getCardinalityRestrictions(vocab.resolve("RobotB"), vocab.resolve("has"));
    BOOST_REQUIRE_MESSAGE( restrictions.size() == 5,
                "RobotB has 5 restrictions, found: " << OWLCardinalityRestriction::toString(restrictions, 4));

    restrictions = ask.getCardinalityRestrictions(vocab.resolve("RobotC"), vocab.resolve("has"));
    BOOST_REQUIRE_MESSAGE( restrictions.size() == 6,
                "RobotB has 6 restrictions, found: " << OWLCardinalityRestriction::toString(restrictions, 4));
}

BOOST_AUTO_TEST_CASE(restrictions_modular)
{
    OWLOntology::Ptr ontology = OWLOntology::fromFile( getRootDir() +
            "/test/data/test-restrictions-modular-c.ttl");

    owlapi::vocabulary::Custom vocab("http://www.rock-robotics.org/test/turtle/restrictions-modular#");

    OWLOntologyAsk ask(ontology);
    OWLCardinalityRestriction::PtrList restrictions;
    restrictions = ask.getCardinalityRestrictions(vocab.resolve("RobotA"), vocab.resolve("has"));
    BOOST_REQUIRE_MESSAGE( restrictions.size() == 4,
                "RobotA has 4 restrictions, found: " << OWLCardinalityRestriction::toString(restrictions, 4));

    restrictions = ask.getCardinalityRestrictions(vocab.resolve("RobotB"), vocab.resolve("has"));
    BOOST_REQUIRE_MESSAGE( restrictions.size() == 5,
                "RobotB has 5 restrictions, found: " << OWLCardinalityRestriction::toString(restrictions, 4));

    restrictions = ask.getCardinalityRestrictions(vocab.resolve("RobotC"), vocab.resolve("has"));
    BOOST_REQUIRE_MESSAGE( restrictions.size() == 6,
                "RobotB has 6 restrictions, found: " << OWLCardinalityRestriction::toString(restrictions, 4));
}

BOOST_AUTO_TEST_SUITE_END()
