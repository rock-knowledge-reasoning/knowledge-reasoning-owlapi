#include <boost/test/unit_test.hpp>
#include <owlapi/model/OWLFacetRestriction.hpp>
#include <owlapi/Vocabulary.hpp>

using namespace owlapi::model;

BOOST_AUTO_TEST_SUITE(facet)

BOOST_AUTO_TEST_CASE(is_compliant)
{
    {
        OWLFacetRestriction facetRestriction(OWLFacet::getFacet(owlapi::vocabulary::XSD::minInclusive()),
                OWLLiteral::doubleValue(1.2));
        BOOST_REQUIRE_MESSAGE( facetRestriction.isCompliant("1.2"), "Facet (>=1.2) is compliant with 1.2");
        BOOST_REQUIRE_MESSAGE(!facetRestriction.isCompliant("1.1"), "Facet (>=1.2) should not be compliant with 1.1");
    }
    {
        OWLFacetRestriction facetRestriction(OWLFacet::getFacet(owlapi::vocabulary::XSD::minExclusive()),
                OWLLiteral::doubleValue(1.2));
        BOOST_REQUIRE_MESSAGE( facetRestriction.isCompliant("1.3"), "Facet (>=1.2) is compliant with 1.3");
        BOOST_REQUIRE_MESSAGE(!facetRestriction.isCompliant("1.2"), "Facet (>=1.2) should not be compliant with 1.2");
    }


    {
        OWLFacetRestriction facetRestriction(OWLFacet::getFacet(owlapi::vocabulary::XSD::maxInclusive()),
                OWLLiteral::doubleValue(1.2));
        BOOST_REQUIRE_MESSAGE( facetRestriction.isCompliant("1.2"), "Facet (<=1.2) is compliant with 1.2");
        BOOST_REQUIRE_MESSAGE(!facetRestriction.isCompliant("1.3"), "Facet (<=1.2) should not be compliant with 1.3");
    }
    {
        OWLFacetRestriction facetRestriction(OWLFacet::getFacet(owlapi::vocabulary::XSD::maxExclusive()),
                OWLLiteral::doubleValue(1.2));
        BOOST_REQUIRE_MESSAGE( facetRestriction.isCompliant("1.1"), "Facet (<=1.2) is compliant with 1.1");
        BOOST_REQUIRE_MESSAGE(!facetRestriction.isCompliant("1.2"), "Facet (<=1.2) should not be compliant with 1.2");
    }
}

BOOST_AUTO_TEST_SUITE_END()
