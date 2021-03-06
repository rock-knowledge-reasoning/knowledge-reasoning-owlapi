#include <boost/test/unit_test.hpp>
#include "test_utils.hpp"

#include <owlapi/OWLApi.hpp>
#include <owlapi/Vocabulary.hpp>
#include <boost/regex.hpp>
#include <unordered_map>

using namespace owlapi::model;

BOOST_AUTO_TEST_SUITE(owlapi)

BOOST_AUTO_TEST_CASE(iris)
{
    {
        IRI iri;
        BOOST_REQUIRE_THROW(iri.getScheme(), std::invalid_argument);

        BOOST_REQUIRE_MESSAGE(iri.getPrefix().empty(), "Prefix should be empty");
        BOOST_REQUIRE_MESSAGE(iri.getPrefix() == iri.getNamespace(), "Prefix should be same as namespace");
        BOOST_REQUIRE_MESSAGE(iri.getRemainder().empty(), "Remainder should be empty");
    }

    {
        std::string prefix = "http://www.rock-robotics.org/2001/09/om-schema#";
        std::string suffix = "test/";

        IRI iri(prefix, suffix);
        BOOST_REQUIRE_MESSAGE(iri.getScheme() == "http", "Scheme should be http");
        BOOST_REQUIRE_MESSAGE(iri.getPrefix() == iri.getNamespace(), "Prefix should be same as namespace");

        BOOST_REQUIRE_MESSAGE(iri.getPrefix() == prefix, "Prefix expected '" << prefix << "' got '" << iri.getPrefix() << "'");
        BOOST_REQUIRE_MESSAGE(iri.getRemainder() == suffix, "Remainder expected '" << suffix << "' got '" << iri.getRemainder() << "'");
        BOOST_REQUIRE_MESSAGE(iri.getRemainder() == suffix, "Remainder expected '" << suffix << "' got '" << iri.getRemainder() << "'");

    }
    {
        IRI iri("http://bla/1/2/3/","");
        BOOST_REQUIRE_MESSAGE(iri.isAbsolute(), "IRI '" << iri << " is absolute");
        IRI otherIri = iri.resolve("../../../2012/om-schema#bla");
        BOOST_TEST_MESSAGE("Resolve " << otherIri);

        URI uri("/www.example.org/index.php");
        BOOST_REQUIRE_MESSAGE(uri.isAbsolute(), "Uri is absolute " << uri.toString());
    }

    {
        std::string prefix = "http://www.rock-robotics.org/";
        std::string suffix = "test";

        IRI iri(prefix, suffix);
        BOOST_REQUIRE_MESSAGE(iri.getScheme() == "http", "Scheme should be http");
        BOOST_REQUIRE_MESSAGE(iri.getPrefix() == iri.getNamespace(), "Prefix should be same as namespace");

        BOOST_REQUIRE_MESSAGE(iri.getPrefix() == prefix, "Prefix expected '" << prefix << "' got '" << iri.getPrefix() << "'");
        BOOST_REQUIRE_MESSAGE(iri.getRemainder() == suffix, "Remainder expected '" << suffix << "' got '" << iri.getRemainder() << "'");
    }

    {
        IRI iri("http://www.w3.org/2002/07/owl#TransitiveProperty");
        URI uri = iri.toURI();
        BOOST_TEST_MESSAGE("URI should be same after toString 1: " << uri.toString());
        BOOST_TEST_MESSAGE("URI should be same after toString 2: " << uri.toString());
        BOOST_TEST_MESSAGE("URI should be same after toString 3: " << uri.toString());

        BOOST_REQUIRE_MESSAGE(iri.toString() == uri.toString(), "IRI should be same after uri conversion: " << iri.toString() << " vs. iri->uri->iri " << uri.toString() );
    }
    {
        URI relativeUri("Transitive");
        BOOST_REQUIRE_MESSAGE(relativeUri.toString() == "Transitive", "Relative uri remains the same " << relativeUri.toString());
        URI uri("http://www.w3.org/2002/07/owl#");
        BOOST_TEST_MESSAGE("Test base uri: " << uri.toString());

        URI resolvedUri = uri.resolve(relativeUri);

        BOOST_REQUIRE_MESSAGE(resolvedUri.toString() == "http://www.w3.org/2002/07/owl#Transitive", "Resolved uri via relative: '" << resolvedUri.toString() << "'");
    }

    {
        IRI iri("http://www.w3.org/2002/07/owl#");

        IRI resolvedIri = iri.resolve("TransitiveProperty");
        IRI expected("http://www.w3.org/2002/07/owl#TransitiveProperty");

        BOOST_REQUIRE_MESSAGE(resolvedIri == expected, "Resolved iri: " << resolvedIri << " vs. expected " << expected);

    }

    {
        IRI iri("http://www.w3.org/2002/07/owl#One+Two+Three[12]*");
        IRI iriFail("http://www.w3.org/2002/07/owl#One+Two+Three[]*");
        std::string escapedIri = iri.toEscapedString();

        boost::regex r(escapedIri);
        BOOST_REQUIRE_MESSAGE(regex_match(iri.toString(), r), "IRI correctly escaped -- positive test: regex is: " << r.str());
        BOOST_REQUIRE_MESSAGE(!regex_match(iriFail.toString(), r), "IRI correctly escaped -- negative test");
    }
}
BOOST_AUTO_TEST_CASE(iri_unordered_maps)
{
    owlapi::model::IRI iri("http://test/iri");
    owlapi::model::IRI otherIri("http://test/otherIri");
    size_t iriHashValue = std::hash<owlapi::model::IRI>()(iri);
    size_t otherIriHashValue = std::hash<owlapi::model::IRI>()(otherIri);
    BOOST_REQUIRE_MESSAGE(iriHashValue != otherIriHashValue, "Hash values differ");
    std::unordered_map<owlapi::model::IRI, size_t> iriMap = { { iri, 1 },
        {otherIri, 2 } };

    BOOST_REQUIRE_MESSAGE(iriMap[iri] == 1, "Iri retrieval: expected 1");
    BOOST_REQUIRE_MESSAGE(iriMap[otherIri] == 2, "Iri retrieval: expected 2");
}

BOOST_AUTO_TEST_CASE(property_expressions)
{
    OWLObjectPropertyExpression::Ptr oProperty( new OWLObjectProperty("http://www.w3.org/2002/07/custom#has"));
    OWLInverseObjectProperty inverseProperty(oProperty);
    OWLObjectPropertyExpression::Ptr inverse = inverseProperty.getInverse();
    BOOST_REQUIRE_MESSAGE( inverse == oProperty, "Inverse should be the same: " << inverse << " vs." << oProperty);
}

BOOST_AUTO_TEST_CASE(class_expressions)
{
    using namespace owlapi::model;
    OWLClass::Ptr baseClass = make_shared<OWLClass>("baseClass");

    IRI has("has");
    OWLObjectProperty::Ptr oPropertyPtr = make_shared<OWLObjectProperty>(has);
    uint32_t cardinality = 10;
    OWLObjectCardinalityRestriction::Ptr oe = make_shared<OWLObjectExactCardinality>(oPropertyPtr, cardinality, baseClass);

    BOOST_REQUIRE( oe->getCardinality() == cardinality );
    BOOST_REQUIRE( oe->getCardinalityRestrictionType() == OWLCardinalityRestriction::EXACT );
    BOOST_REQUIRE( oe->isQualified() );
    BOOST_REQUIRE( oe->getFiller() == baseClass);

    // ClassRestriction
    IRI name("individual");
    OWLIndividual::Ptr individualPtr(new OWLNamedIndividual( name ));
    OWLClassAssertionAxiom ce(individualPtr, dynamic_pointer_cast<OWLClassExpression>(oe));
    BOOST_REQUIRE( ce.getIndividual() == individualPtr);
    BOOST_REQUIRE( ce.getClassExpression() == oe );
}

BOOST_AUTO_TEST_CASE(cardinality_restrictions)
{
    using namespace owlapi::model;
    OWLClass::Ptr fork = make_shared<OWLClass>("fork");
    OWLClass::Ptr spoon = make_shared<OWLClass>("spoon");
    OWLClass::Ptr knife = make_shared<OWLClass>("knife");

    IRI has("has");
    OWLObjectProperty::Ptr oPropertyPtr = make_shared<OWLObjectProperty>(has);
    BOOST_REQUIRE_MESSAGE( dynamic_pointer_cast<OWLObjectPropertyExpression>(oPropertyPtr),
            "Object property is an object property expression");

    {
        uint32_t cardinality = 2;
        OWLCardinalityRestriction::Ptr forkRestriction0 = OWLObjectCardinalityRestriction::createInstance(
                oPropertyPtr, cardinality, fork, OWLCardinalityRestriction::EXACT);

        BOOST_REQUIRE_MESSAGE(forkRestriction0, "Restriction created");
        BOOST_REQUIRE_MESSAGE(forkRestriction0->getProperty(), "Restriction has property");

        OWLCardinalityRestriction::Ptr forkRestriction1 = OWLObjectCardinalityRestriction::createInstance(
                oPropertyPtr, cardinality, fork, OWLCardinalityRestriction::EXACT);
        BOOST_REQUIRE_MESSAGE(forkRestriction1, "Restriction created");
        BOOST_REQUIRE_MESSAGE(forkRestriction1->getProperty(), "Restriction has property");

        BOOST_REQUIRE_MESSAGE(dynamic_pointer_cast<OWLCardinalityRestriction>(forkRestriction1)->getProperty(), "Property is available");

        BOOST_REQUIRE_MESSAGE(forkRestriction0->isOverlapping(dynamic_pointer_cast<OWLCardinalityRestriction>(forkRestriction1)),
                "Restrictions overlap");

        OWLCardinalityRestriction::Ptr restriction =
            OWLCardinalityRestrictionOps::intersection(forkRestriction0, forkRestriction1);

        BOOST_REQUIRE_MESSAGE(restriction, "Merging exact restrictions of same cardinality");
    }

    {
        uint32_t cardinality0 = 2;
        uint32_t cardinality1 = 4;
        OWLCardinalityRestriction::Ptr forkRestriction0 = OWLObjectCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality0, fork, OWLCardinalityRestriction::EXACT);

        OWLCardinalityRestriction::Ptr forkRestriction1 = OWLObjectCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality1, fork, OWLCardinalityRestriction::EXACT);

        BOOST_REQUIRE_THROW(OWLCardinalityRestrictionOps::intersection(forkRestriction0, forkRestriction1), std::invalid_argument);
    }
    {
        uint32_t cardinality0 = 2;
        uint32_t cardinality1 = 4;
        OWLCardinalityRestriction::Ptr forkRestriction = OWLObjectCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality0, fork, OWLCardinalityRestriction::EXACT);

        OWLCardinalityRestriction::Ptr spoonRestriction = OWLObjectCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality1, spoon, OWLCardinalityRestriction::EXACT);

        BOOST_REQUIRE_NO_THROW(OWLCardinalityRestrictionOps::intersection(forkRestriction, spoonRestriction));
        OWLCardinalityRestriction::Ptr restriction = OWLCardinalityRestrictionOps::intersection(forkRestriction, spoonRestriction);
        BOOST_REQUIRE_MESSAGE(!restriction, "Non overlapping restrictions return null pointer");
    }
    {
        uint32_t cardinality0 = 2;
        uint32_t cardinality1 = 4;
        OWLCardinalityRestriction::Ptr forkRestriction0 = OWLObjectCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality0, fork, OWLCardinalityRestriction::MIN);

        OWLCardinalityRestriction::Ptr forkRestriction1 = OWLObjectCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality1, fork, OWLCardinalityRestriction::MIN);

        OWLCardinalityRestriction::Ptr restriction = OWLCardinalityRestrictionOps::intersection(forkRestriction0, forkRestriction1);
        BOOST_REQUIRE_MESSAGE(restriction->getCardinality() == cardinality1, "Min cardinality takes max");
    }
    {
        uint32_t cardinality0 = 2;
        uint32_t cardinality1 = 4;
        OWLCardinalityRestriction::Ptr forkRestriction0 = OWLObjectCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality0, fork, OWLCardinalityRestriction::MAX);

        OWLCardinalityRestriction::Ptr forkRestriction1 = OWLObjectCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality1, fork, OWLCardinalityRestriction::MAX);

        OWLCardinalityRestriction::Ptr restriction = OWLCardinalityRestrictionOps::intersection(forkRestriction0, forkRestriction1);
        BOOST_REQUIRE_MESSAGE(restriction->getCardinality() == cardinality0, "Max cardinality takes min");
    }
    {
        uint32_t cardinality0 = 2;
        uint32_t cardinality1 = 4;
        OWLCardinalityRestriction::Ptr forkRestriction0 = OWLObjectCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality0, fork, OWLCardinalityRestriction::MIN);

        OWLCardinalityRestriction::Ptr forkRestriction1 = OWLObjectCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality1, fork, OWLCardinalityRestriction::MAX);

        OWLCardinalityRestriction::Ptr restriction = OWLCardinalityRestrictionOps::intersection(forkRestriction0, forkRestriction1);
        BOOST_REQUIRE_MESSAGE(!restriction, "Min max cardinality returns null pointer if valid");
    }
    {
        uint32_t cardinality0 = 2;
        uint32_t cardinality1 = 4;
        OWLCardinalityRestriction::Ptr forkRestriction0 = OWLObjectCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality0, fork, OWLCardinalityRestriction::MAX);

        OWLCardinalityRestriction::Ptr forkRestriction1 = OWLObjectCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality1, fork, OWLCardinalityRestriction::MIN);

        BOOST_REQUIRE_THROW(OWLCardinalityRestrictionOps::intersection(forkRestriction0, forkRestriction1), std::invalid_argument);
    }

    {
        std::vector<OWLCardinalityRestriction::Ptr> restrictionsA;
        std::vector<OWLCardinalityRestriction::Ptr> restrictionsB;
        {
            OWLCardinalityRestriction::Ptr forkRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, fork, OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr spoonRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, spoon, OWLCardinalityRestriction::EXACT);
            restrictionsA.push_back(forkRestriction);
            restrictionsA.push_back(spoonRestriction);
        }
        {
            OWLCardinalityRestriction::Ptr forkRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, fork, OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr spoonRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, spoon, OWLCardinalityRestriction::EXACT);
            restrictionsB.push_back(forkRestriction);
            restrictionsB.push_back(spoonRestriction);
        }

        {
            std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = OWLCardinalityRestrictionOps::intersection(restrictionsA, restrictionsB);
            BOOST_REQUIRE_MESSAGE(cardinalityRestrictions.size() == 2, "Intersected sets should reduce to size 2");
        }

        {
            std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = OWLCardinalityRestrictionOps::intersection(restrictionsB, restrictionsA);
            BOOST_REQUIRE_MESSAGE(cardinalityRestrictions.size() == 2, "Intersected sets (reverse) should reduce to size 2");
        }
    }

    {
        std::vector<OWLCardinalityRestriction::Ptr> restrictionsA;
        std::vector<OWLCardinalityRestriction::Ptr> restrictionsB;
        {
            OWLCardinalityRestriction::Ptr forkRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, fork, OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr spoonRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, spoon, OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr knifeRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 3, knife, OWLCardinalityRestriction::EXACT);
            restrictionsA.push_back(forkRestriction);
            restrictionsA.push_back(spoonRestriction);
            restrictionsA.push_back(knifeRestriction);
        }
        {
            OWLCardinalityRestriction::Ptr forkRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, fork, OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr spoonRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, spoon, OWLCardinalityRestriction::EXACT);
            restrictionsB.push_back(forkRestriction);
            restrictionsB.push_back(spoonRestriction);
        }

        {
            std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = OWLCardinalityRestrictionOps::intersection(restrictionsA, restrictionsB);
            BOOST_REQUIRE_MESSAGE(cardinalityRestrictions.size() == 3, "Intersected sets should reduce to size 3");
        }

        {
            std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = OWLCardinalityRestrictionOps::intersection(restrictionsB, restrictionsA);
            BOOST_REQUIRE_MESSAGE(cardinalityRestrictions.size() == 3, "Intersected sets (reverse) should reduce to size 3");
        }
    }

    // Check join
    {
        {
            std::vector<OWLCardinalityRestriction::Ptr> restrictionsA;
            std::vector<OWLCardinalityRestriction::Ptr> restrictionsB;

            {
                OWLCardinalityRestriction::Ptr forkRestriction = OWLObjectCardinalityRestriction::getInstance(
                        oPropertyPtr, 1, fork, OWLCardinalityRestriction::EXACT);

                restrictionsA.push_back(forkRestriction);
                restrictionsB.push_back(forkRestriction->clone());

                std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = OWLCardinalityRestrictionOps::join(restrictionsA, restrictionsB);
                BOOST_REQUIRE_MESSAGE(cardinalityRestrictions.size() == 1, "Joined set expected to reduce to size 1, size was " << cardinalityRestrictions.size());
            }
        }

        std::vector<OWLCardinalityRestriction::Ptr> restrictionsA;
        std::vector<OWLCardinalityRestriction::Ptr> restrictionsB;

        {
            OWLCardinalityRestriction::Ptr forkRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, fork, OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr spoonRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, spoon, OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr knifeRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 3, knife, OWLCardinalityRestriction::EXACT);
            restrictionsA.push_back(forkRestriction);
            restrictionsA.push_back(spoonRestriction);
            restrictionsA.push_back(knifeRestriction);
        }
        {
            OWLCardinalityRestriction::Ptr forkRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, fork, OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr spoonRestriction = OWLObjectCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, spoon, OWLCardinalityRestriction::EXACT);
            restrictionsB.push_back(forkRestriction);
            restrictionsB.push_back(spoonRestriction);
        }

        {
            std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = OWLCardinalityRestrictionOps::join(restrictionsA, restrictionsB);
            BOOST_REQUIRE_MESSAGE(cardinalityRestrictions.size() == 3, "Joined sets should reduce to size 3");

            std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = cardinalityRestrictions.begin();
            for(;cit != cardinalityRestrictions.end(); ++cit)
            {
                OWLObjectCardinalityRestriction::Ptr restriction = dynamic_pointer_cast<OWLObjectCardinalityRestriction>(*cit);
                if(restriction->getFiller() == fork)
                {
                    BOOST_REQUIRE_MESSAGE(restriction->getCardinality() == 2, "Joined restrictions for fork should have cardinality 2");
                } else if(restriction->getFiller() == spoon)
                {
                    BOOST_REQUIRE_MESSAGE(restriction->getCardinality() == 2, "Joined restrictions for spoon should have cardinality 2");
                } else if(restriction->getFiller() == knife)
                {
                    BOOST_REQUIRE_MESSAGE(restriction->getCardinality() == 3, "Joined restrictions for knife should have cardinality 3");
                }
            }
        }

        {
            std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = OWLCardinalityRestrictionOps::join(restrictionsB, restrictionsA);
            BOOST_REQUIRE_MESSAGE(cardinalityRestrictions.size() == 3, "Joined sets (reverse) should reduce to size 3");

            std::vector<OWLCardinalityRestriction::Ptr>::const_iterator cit = cardinalityRestrictions.begin();
            for(;cit != cardinalityRestrictions.end(); ++cit)
            {
                OWLObjectCardinalityRestriction::Ptr restriction = dynamic_pointer_cast<OWLObjectCardinalityRestriction>(*cit);
                if(restriction->getFiller() == fork)
                {
                    BOOST_REQUIRE_MESSAGE(restriction->getCardinality() == 2, "Joined restrictions for fork should have cardinality 2");
                } else if(restriction->getFiller() == spoon)
                {
                    BOOST_REQUIRE_MESSAGE(restriction->getCardinality() == 2, "Joined restrictions for spoon should have cardinality 2");
                } else if(restriction->getFiller() == knife)
                {
                    BOOST_REQUIRE_MESSAGE(restriction->getCardinality() == 3, "Joined restrictions for knife should have cardinality 3");
                }
            }
        }

        {

        }

    }
}

BOOST_AUTO_TEST_SUITE_END()
