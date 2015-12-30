#include <boost/test/unit_test.hpp>
#include <owlapi/OWLApi.hpp>
#include <owlapi/io/OWLOntologyIO.hpp>
#include <owlapi/io/RedlandWriter.hpp>
#include "test_utils.hpp"

using namespace owlapi::model;
using namespace owlapi::io;

// NOTE:
// whan running test either do:
//     export BOOST_TEST_CATCH_SYSTEM_ERRORS="no"
// or use argument: --catch_system_errors=no
//
// otherwise (if the pkgconfig search fails) you will get an error on failed
// child process
// see http://stackoverflow.com/questions/5325202/boostunit-test-case-fails-because-a-child-process-exits-with-nonzero

BOOST_AUTO_TEST_SUITE(io)

BOOST_AUTO_TEST_CASE(redland)
{
    {
        OWLOntology::Ptr ontology = OWLOntology::fromFile( getRootDir() + "/test/data/om-schema-v0.9.owl");

        owlapi::io::RedlandWriter writer;
        writer.setFormat("rdfxml");
        writer.write("/tmp/test_file-rdfxml.owl", ontology);

        writer.setFormat("ntriples");
        writer.write("/tmp/test_file-ntriples.owl", ontology);
    }

    {
        OWLOntology::Ptr ontology = OWLOntology::fromFile("/tmp/test_file-rdfxml.owl" );
        OWLAxiom::PtrList axioms = ontology->getAxioms();
        BOOST_REQUIRE(!axioms.empty());
        IRI origin = axioms.back()->getOrigin();
        BOOST_REQUIRE_MESSAGE(origin != IRI(), "Origin of axiom is: " << origin);

    }
    {
        OWLOntology::Ptr ontology = OWLOntology::fromFile("/tmp/test_file-ntriples.owl" );
    }
}

BOOST_AUTO_TEST_CASE(canonize)
{
    IRI iri("http://www.rock-robotics.org/2014/09/om-schema");
    std::string canonizedName = owlapi::io::OWLOntologyIO::canonizeForOfflineUsage(iri);
    BOOST_TEST_MESSAGE("Test name: " << canonizedName);
}

BOOST_AUTO_TEST_CASE(retrieve)
{
    IRI iri("http://www.w3.org/2002/07/owl");
    BOOST_TEST_MESSAGE("Find iri: '" << iri << " -- if the following test fails: give the following "
            " options when running the test: --catch_system_errors=no");
    std::string fileName;
    BOOST_REQUIRE_NO_THROW(fileName = owlapi::io::OWLOntologyIO::retrieve(iri))
    BOOST_TEST_MESSAGE("Retrieved iri '" << iri << "', absolute path: " << fileName);
}

BOOST_AUTO_TEST_CASE(create_with_builtin)
{
    OWLOntology::Ptr ontology = owlapi::make_shared<OWLOntology>();
    IRI iri("http://www.w3.org/2002/07/owl");
    ontology->addDirectImportsDocument(iri);

    BOOST_REQUIRE_THROW(OWLOntologyIO::load(ontology), std::invalid_argument);
    BOOST_REQUIRE_NO_THROW(ontology = OWLOntologyIO::load(ontology, "http://test/owlapi/create_with_builtin") );
    BOOST_REQUIRE_MESSAGE(ontology->getAxioms().empty(), "Ontology is empty -- after importing only builtin vocabularies");
}


BOOST_AUTO_TEST_SUITE_END()
