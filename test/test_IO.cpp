#include <boost/test/unit_test.hpp>
#include <owlapi/OWLApi.hpp>
#include <owlapi/io/OWLOntologyIO.hpp>
#include <owlapi/io/RedlandWriter.hpp>
#include <owlapi/io/RedlandReader.hpp>
#include "test_utils.hpp"

#define BOOST_TEST_IGNORE_NON_ZERO_CHILD_CODE

using namespace owlapi::model;
using namespace owlapi::io;

// NOTE:
// when running test either do:
//     export BOOST_TEST_CATCH_SYSTEM_ERRORS="no"
// or use argument: --catch_system_errors=no
//
// otherwise (if the pkgconfig search fails) you will get an error on failed
// child process
// see http://stackoverflow.com/questions/5325202/boostunit-test-case-fails-because-a-child-process-exits-with-nonzero

BOOST_AUTO_TEST_SUITE(io)

BOOST_AUTO_TEST_CASE(redland_writer)
{
    {
        OWLOntology::Ptr ontology = OWLOntology::fromFile( getRootDir() + "/test/data/om-schema-v0.9.owl");

        owlapi::io::RedlandWriter writer;
        writer.setFormat("rdfxml");
        writer.write("/tmp/test_file-rdfxml" + FormatSuffixes[RDFXML], ontology);

        writer.setFormat("turtle");
        writer.write("/tmp/test_file-turtle" + FormatSuffixes[TURTLE], ontology);
    }

    {
        OWLOntology::Ptr ontology =
            OWLOntology::fromFile("/tmp/test_file-rdfxml"
                    + FormatSuffixes[RDFXML] );
        OWLAxiom::PtrList axioms = ontology->getAxioms();
        BOOST_REQUIRE(!axioms.empty());
        IRI origin = axioms.back()->getOrigin();
        BOOST_REQUIRE_MESSAGE(origin != IRI(), "Origin of axiom is: " << origin);

    }
    {
        OWLOntology::Ptr ontology =
            OWLOntology::fromFile("/tmp/test_file-turtle" +
                    FormatSuffixes[TURTLE] );
        OWLAxiom::PtrList axioms = ontology->getAxioms();
        BOOST_REQUIRE(!axioms.empty());
        IRI origin = axioms.back()->getOrigin();
        BOOST_REQUIRE_MESSAGE(origin != IRI(), "Origin of axiom is: " << origin);
    }
}

BOOST_AUTO_TEST_CASE(redland_reader)
{
    {
        owlapi::io::RedlandReader reader;
        reader.setFormat("turtle");
        reader.read( getRootDir() + "test/data/test-turtle-value_types.ttl");

        librdf_model* model = reader.getModel();
        BOOST_REQUIRE_MESSAGE(librdf_model_size(model) == 15,
                "Redland Reader has read model in turtle syntax");
    }
}

BOOST_AUTO_TEST_CASE(formats)
{
    using namespace owlapi::io;
    for(int i = RDFXML; i < END_FORMAT; ++i)
    {
        Format format = static_cast<Format>(i);
        // not support for import
        if(format == TRIG || format == JSON)
        {
            continue;
        }

        std::string suffix = FormatSuffixes[format];
        std::string baseFile = getRootDir() + "/test/data/om-schema-v0.9.owl";
        std::string outFile = "/tmp/test-owlapi-io-format" + suffix;
        std::string cmd = "rapper " + baseFile
            + " -o " + FormatTxt[format] + " > " + outFile;
        BOOST_REQUIRE_MESSAGE( system(cmd.c_str()) == 0, "Failed to create test file: " <<
                outFile << ": command: '" + cmd + "'");

        OWLOntology::Ptr ontology;
        BOOST_TEST_MESSAGE("Testing: format " << FormatTxt[format] << ": " << outFile);
        try {
            ontology = OWLOntology::fromFile( outFile );
        } catch(const std::exception& e)
        {
            BOOST_REQUIRE_MESSAGE(false, e.what());
        }

        std::string filename = "/tmp/test-owlapi-io-format-export" + suffix;
        OWLOntologyIO::write(filename,
                ontology,
                static_cast<Format>(i));

        BOOST_REQUIRE_NO_THROW(ontology = OWLOntology::fromFile( filename) );
        BOOST_REQUIRE(!ontology->getAxioms().empty());
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
    BOOST_REQUIRE_NO_THROW(fileName = owlapi::io::OWLOntologyIO::retrieve(iri));
    BOOST_TEST_MESSAGE("Retrieved iri '" << iri << "', absolute path: " << fileName);
}

BOOST_AUTO_TEST_CASE(create_with_builtin)
{
    OWLOntology::Ptr ontology = owlapi::make_shared<OWLOntology>();
    IRI iri("http://www.w3.org/2002/07/owl");
    ontology->addDirectImportsDocument(iri);

    BOOST_REQUIRE_THROW(OWLOntologyIO::load(ontology), std::invalid_argument);
    BOOST_REQUIRE_NO_THROW(ontology = OWLOntologyIO::loadNew(ontology, "http://test/owlapi/create_with_builtin") );
    BOOST_REQUIRE_MESSAGE(ontology->getAxioms().empty(), "Ontology is empty -- after importing only builtin vocabularies");
}

BOOST_AUTO_TEST_CASE(create_from_unknown)
{
    OWLOntology::Ptr ontology = owlapi::make_shared<OWLOntology>();
    IRI iri("http://rock-robotics.org/2019/01/owlapi/test");
    std::string canonizedName = owlapi::io::OWLOntologyIO::canonizeForOfflineUsage(iri);
    ontology->addDirectImportsDocument(iri);

    IRI ontologyIRI("http://test/owlapi/create_from_unknown");
    BOOST_REQUIRE_THROW( OWLOntologyIO::loadNew(ontology, ontologyIRI),
            owlapi::io::OWLOntologyNotFound );
}

// This test case requires
// export BOOST_TEST_CATCH_SYSTEM_ERRORS="no"
// since otherwise the underlying failing system call
// of 'wget' will fail the whole test
BOOST_AUTO_TEST_CASE(create_with_custom)
{
    OWLOntology::Ptr ontology = owlapi::make_shared<OWLOntology>();
    {
        IRI iri("http://www.rock-robotics.org/2019/01/owlapi/test/create-with-custom");
        std::string canonizedName = owlapi::io::OWLOntologyIO::canonizeForOfflineUsage(iri);
        ontology->addDirectImportsDocument(iri);
    }

    BOOST_TEST_MESSAGE("Check if the custom model can be loaded -- if that fails, then you have to check whether"
            " you really used RDF/XML and not OWL/XML(!) -- sopranos serializer segfaults otherwise");
    IRI ontologyIRI("http://test/owlapi/create_with_custom");
    ontology = OWLOntologyIO::loadNew(ontology, ontologyIRI);
    BOOST_REQUIRE_MESSAGE(ontology->getIRI()  == ontologyIRI, "Ontology has assigned iri: " << ontologyIRI);
    BOOST_REQUIRE_MESSAGE(!ontology->getAxioms().empty(), "Ontology is not empty after importing custom vocabularies");

    OWLOntologyTell tell(ontology);
    tell.namedIndividual("http://test-foo/Individual");
    OWLOntologyIO::write("/tmp/owlapi-test-io-create_with_custom-ontology.owl", ontology);
}


BOOST_AUTO_TEST_SUITE_END()
