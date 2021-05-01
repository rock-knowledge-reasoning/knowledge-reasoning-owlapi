#include <boost/test/unit_test.hpp>
#include <owlapi/model/IRI.hpp>
#include "test_utils.hpp"

using namespace owlapi::model;

BOOST_AUTO_TEST_SUITE(iri)

BOOST_AUTO_TEST_CASE(is_valid)
{
    std::vector<std::string> iris =
    { "http://www.rock-robotics.org/01/2014/om-schema#test_a" };

    for(const std::string& test_iri : iris)
    {
        BOOST_REQUIRE_MESSAGE(IRI::isValid(test_iri), "Is IRI " << test_iri);
    }

    std::vector<std::string> no_iris =
    { "_a*_b" };

    for(const std::string& test_no_iri : no_iris)
    {
        BOOST_REQUIRE_MESSAGE(!IRI::isValid(test_no_iri), "Is not an IRI " << test_no_iri);
    }
}


BOOST_AUTO_TEST_SUITE_END()
