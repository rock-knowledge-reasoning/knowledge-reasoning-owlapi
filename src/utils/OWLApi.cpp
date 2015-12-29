#include <iostream>

#include <owlapi/OWLApi.hpp>
#include <owlapi/io/OWLOntologyIO.hpp>
#include <owlapi/io/OWLOntologyReader.hpp>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("canonize,c", po::value<std::string>(), "canonize a url")
        ("list,l", "list known ontologies");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help") || argc == 1)
    {
        std::cout << "usage: " << argv[0] << " [options]" << std::endl;
        std::cout << desc << std::endl;
        return 0;
    }

    if(vm.count("canonize"))
    {
        std::cout << owlapi::io::OWLOntologyIO::canonizeForOfflineUsage(argv[2]) << std::endl;
        return 0;
    }

    if(vm.count("list"))
    {
        boost::filesystem::path ontologyPath = owlapi::io::OWLOntologyIO::getOntologyPath();
        boost::filesystem::directory_iterator dirIt(ontologyPath);
        boost::filesystem::directory_iterator endDirIt;
        for(; dirIt != endDirIt; ++dirIt)
        {
            if(boost::filesystem::is_regular_file(dirIt->path()))
            {
                std::string filename = dirIt->path().string();
                owlapi::io::OWLOntologyReader reader;
                owlapi::model::OWLOntology::Ptr ontology = reader.open(filename);
                reader.loadDeclarationsAndImports(ontology, true);
                std::cout << "ontology: " << ontology->getIRI() << ", file: " << filename << std::endl;
            }
        }
        return 0;
    }

    return 0;
}
