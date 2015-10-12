#include <boost/test/unit_test.hpp>
#include "test_utils.hpp"

#include <owlapi/model/OWLOntology.hpp>
#include <owlapi/io/OWLOntologyIO.hpp>
#include <owlapi/model/OWLOntologyAsk.hpp>
#include <owlapi/model/OWLOntologyTell.hpp>

using namespace owlapi;
using namespace owlapi::model;

BOOST_AUTO_TEST_SUITE(ontology)

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

    using namespace owlapi::model;
    {
        IRI actor = owlapi::vocabulary::OM::resolve("Sherpa");
        IRI property = owlapi::vocabulary::OM::resolve("maxVelocity");

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
        IRI actor = owlapi::vocabulary::OM::resolve("Sherpa");
        IRI property = owlapi::vocabulary::OM::resolve("probabilityOfFailure");

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
        IRI actor = owlapi::vocabulary::OM::resolve("Sherpa");
        IRI property = owlapi::vocabulary::OM::resolve("notExistingProperty");

        BOOST_REQUIRE_THROW(ask.getDataValue(actor, property), std::invalid_argument);
        BOOST_REQUIRE_THROW( ask.getDataPropertyDomain(property, true), std::invalid_argument);
    }
}

BOOST_AUTO_TEST_SUITE_END()
