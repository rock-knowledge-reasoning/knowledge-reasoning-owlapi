#include <iostream>
#include <owlapi/io/OWLOntologyReader.hpp>
#include <owlapi/io/OWLOntologyIO.hpp>

using namespace owlapi::model;

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "usage: " << argv[0] << " <ontology-filename>" << std::endl;
        return 0;
    }

    std::string filename(argv[1]);
    owlapi::io::OWLOntologyReader reader;
    OWLOntology::Ptr ontology = reader.fromFile(filename);

    std::string outFilename("/tmp/ontology-out.owl");
    owlapi::io::OWLOntologyIO::write(outFilename, ontology);

    std::cout << "Written output to '" << outFilename << "'" << std::endl;
    return 0;
}
