#include <boost/test/unit_test.hpp>
#include "test_utils.hpp"

#include <owlapi/OWLApi.hpp>
#include <owlapi/Vocabulary.hpp>
#include <boost/regex.hpp>

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
    OWLClass baseClass("baseClass");

    IRI has("has");
    OWLObjectProperty::Ptr oPropertyPtr(new OWLObjectProperty(has));
    uint32_t cardinality = 10;
    OWLCardinalityRestriction::Ptr oe(new OWLObjectExactCardinality(oPropertyPtr, cardinality, baseClass.getIRI()) );

    BOOST_REQUIRE( oe->getCardinality() == cardinality );
    BOOST_REQUIRE( oe->getCardinalityRestrictionType() == OWLCardinalityRestriction::EXACT );
    BOOST_REQUIRE( oe->isQualified() );
    BOOST_REQUIRE( oe->getQualification() == baseClass.getIRI() );

    // ClassRestriction
    IRI name("individual");
    OWLIndividual::Ptr individualPtr(new OWLNamedIndividual( name ));
    OWLClassAssertionAxiom ce(individualPtr, oe);
    BOOST_REQUIRE( ce.getIndividual() == individualPtr);
    BOOST_REQUIRE( ce.getClassExpression() == oe );
}

BOOST_AUTO_TEST_CASE(cardinality_restrictions)
{
    using namespace owlapi::model;
    OWLClass fork("fork");
    OWLClass spoon("spoon");
    OWLClass knife("knife");

    IRI has("has");
    OWLObjectProperty::Ptr oPropertyPtr(new OWLObjectProperty(has));

    {
        uint32_t cardinality = 2;
        OWLCardinalityRestriction::Ptr forkRestriction0 = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality, fork.getIRI(), OWLCardinalityRestriction::EXACT);

        OWLCardinalityRestriction::Ptr forkRestriction1 = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality, fork.getIRI(), OWLCardinalityRestriction::EXACT);

        OWLCardinalityRestriction::Ptr restriction = 
            OWLCardinalityRestriction::merge(forkRestriction0, forkRestriction1);

        BOOST_REQUIRE_MESSAGE(restriction, "Merging exact restrictions of same cardinality");
    }

    {
        uint32_t cardinality0 = 2;
        uint32_t cardinality1 = 4;
        OWLCardinalityRestriction::Ptr forkRestriction0 = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality0, fork.getIRI(), OWLCardinalityRestriction::EXACT);

        OWLCardinalityRestriction::Ptr forkRestriction1 = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality1, fork.getIRI(), OWLCardinalityRestriction::EXACT);

        BOOST_REQUIRE_THROW(OWLCardinalityRestriction::merge(forkRestriction0, forkRestriction1), std::invalid_argument);
    }
    {
        uint32_t cardinality0 = 2;
        uint32_t cardinality1 = 4;
        OWLCardinalityRestriction::Ptr forkRestriction = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality0, fork.getIRI(), OWLCardinalityRestriction::EXACT);

        OWLCardinalityRestriction::Ptr spoonRestriction = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality1, spoon.getIRI(), OWLCardinalityRestriction::EXACT);

        BOOST_REQUIRE_NO_THROW(OWLCardinalityRestriction::merge(forkRestriction, spoonRestriction));
        OWLCardinalityRestriction::Ptr restriction = OWLCardinalityRestriction::merge(forkRestriction, spoonRestriction);
        BOOST_REQUIRE_MESSAGE(!restriction, "Non overlapping restrictions return null pointer");
    }
    {
        uint32_t cardinality0 = 2;
        uint32_t cardinality1 = 4;
        OWLCardinalityRestriction::Ptr forkRestriction0 = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality0, fork.getIRI(), OWLCardinalityRestriction::MIN);

        OWLCardinalityRestriction::Ptr forkRestriction1 = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality1, fork.getIRI(), OWLCardinalityRestriction::MIN);

        OWLCardinalityRestriction::Ptr restriction = OWLCardinalityRestriction::merge(forkRestriction0, forkRestriction1);
        BOOST_REQUIRE_MESSAGE(restriction->getCardinality() == cardinality1, "Min cardinality takes max");
    }
    {
        uint32_t cardinality0 = 2;
        uint32_t cardinality1 = 4;
        OWLCardinalityRestriction::Ptr forkRestriction0 = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality0, fork.getIRI(), OWLCardinalityRestriction::MAX);

        OWLCardinalityRestriction::Ptr forkRestriction1 = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality1, fork.getIRI(), OWLCardinalityRestriction::MAX);

        OWLCardinalityRestriction::Ptr restriction = OWLCardinalityRestriction::merge(forkRestriction0, forkRestriction1);
        BOOST_REQUIRE_MESSAGE(restriction->getCardinality() == cardinality0, "Max cardinality takes min");
    }
    {
        uint32_t cardinality0 = 2;
        uint32_t cardinality1 = 4;
        OWLCardinalityRestriction::Ptr forkRestriction0 = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality0, fork.getIRI(), OWLCardinalityRestriction::MIN);

        OWLCardinalityRestriction::Ptr forkRestriction1 = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality1, fork.getIRI(), OWLCardinalityRestriction::MAX);

        OWLCardinalityRestriction::Ptr restriction = OWLCardinalityRestriction::merge(forkRestriction0, forkRestriction1);
        BOOST_REQUIRE_MESSAGE(!restriction, "Min max cardinality returns null pointer if valid");
    }
    {
        uint32_t cardinality0 = 2;
        uint32_t cardinality1 = 4;
        OWLCardinalityRestriction::Ptr forkRestriction0 = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality0, fork.getIRI(), OWLCardinalityRestriction::MAX);

        OWLCardinalityRestriction::Ptr forkRestriction1 = OWLCardinalityRestriction::getInstance(
                oPropertyPtr, cardinality1, fork.getIRI(), OWLCardinalityRestriction::MIN);

        BOOST_REQUIRE_THROW(OWLCardinalityRestriction::merge(forkRestriction0, forkRestriction1), std::invalid_argument);
    }

    {
        std::vector<OWLCardinalityRestriction::Ptr> restrictionsA;
        std::vector<OWLCardinalityRestriction::Ptr> restrictionsB;
        {
            OWLCardinalityRestriction::Ptr forkRestriction = OWLCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, fork.getIRI(), OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr spoonRestriction = OWLCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, spoon.getIRI(), OWLCardinalityRestriction::EXACT);
            restrictionsA.push_back(forkRestriction);
            restrictionsA.push_back(spoonRestriction);
        }
        {
            OWLCardinalityRestriction::Ptr forkRestriction = OWLCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, fork.getIRI(), OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr spoonRestriction = OWLCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, spoon.getIRI(), OWLCardinalityRestriction::EXACT);
            restrictionsB.push_back(forkRestriction);
            restrictionsB.push_back(spoonRestriction);
        }

        {
            std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = OWLCardinalityRestriction::merge(restrictionsA, restrictionsB);
            BOOST_REQUIRE_MESSAGE(cardinalityRestrictions.size() == 2, "Merge sets should reduce to size 2");
        }
    
        {
            std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = OWLCardinalityRestriction::merge(restrictionsB, restrictionsA);
            BOOST_REQUIRE_MESSAGE(cardinalityRestrictions.size() == 2, "Merge sets (reverse) should reduce to size 2");
        }
    }

    {
        std::vector<OWLCardinalityRestriction::Ptr> restrictionsA;
        std::vector<OWLCardinalityRestriction::Ptr> restrictionsB;
        {
            OWLCardinalityRestriction::Ptr forkRestriction = OWLCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, fork.getIRI(), OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr spoonRestriction = OWLCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, spoon.getIRI(), OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr knifeRestriction = OWLCardinalityRestriction::getInstance(
                    oPropertyPtr, 3, knife.getIRI(), OWLCardinalityRestriction::EXACT);
            restrictionsA.push_back(forkRestriction);
            restrictionsA.push_back(spoonRestriction);
            restrictionsA.push_back(knifeRestriction);
        }
        {
            OWLCardinalityRestriction::Ptr forkRestriction = OWLCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, fork.getIRI(), OWLCardinalityRestriction::EXACT);
            OWLCardinalityRestriction::Ptr spoonRestriction = OWLCardinalityRestriction::getInstance(
                    oPropertyPtr, 1, spoon.getIRI(), OWLCardinalityRestriction::EXACT);
            restrictionsB.push_back(forkRestriction);
            restrictionsB.push_back(spoonRestriction);
        }
    
        {
            std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = OWLCardinalityRestriction::merge(restrictionsA, restrictionsB);
            BOOST_REQUIRE_MESSAGE(cardinalityRestrictions.size() == 3, "Merge sets should reduce to size 3");
        }

        {
            std::vector<OWLCardinalityRestriction::Ptr> cardinalityRestrictions = OWLCardinalityRestriction::merge(restrictionsB, restrictionsA);
            BOOST_REQUIRE_MESSAGE(cardinalityRestrictions.size() == 3, "Merge sets (reverse) should reduce to size 3");
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()
