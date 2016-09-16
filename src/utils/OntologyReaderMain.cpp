#include <iostream>
#include <owlapi/io/OWLOntologyIO.hpp>
#include <owlapi/model/OWLOntologyAsk.hpp>

using namespace owlapi::model;

void printUsage(int argc, char** argv)
{
        std::cout << "usage: " << argv[0] << " <ontology-filename>" << std::endl;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printUsage(argc, argv);
        return 0;
    }

    std::string filename(argv[1]);
    if(filename == "-h" || filename == "--help")
    {
        printUsage(argc, argv);
        return 0;
    }

    OWLOntology::Ptr ontology = owlapi::io::OWLOntologyIO::fromFile(filename);

    OWLOntologyAsk ask(ontology);
    std::cout << "All classes:  " << ask.allClasses();

    return 0;
}
