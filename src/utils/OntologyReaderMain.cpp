#include <iostream>
#include <owlapi/io/OWLOntologyIO.hpp>
#include <owlapi/model/OWLOntologyAsk.hpp>
#include <boost/program_options.hpp>
#include "../io/RedlandWriter.hpp"

using namespace owlapi::model;

int main(int argc, char** argv)
{
    namespace po = boost::program_options;

    bool serialize_imports = false;
    bool show_formats = false;

    po::options_description description("This utility allows to read an ontology"
            "(from owl/rdfxml) and will list containing classes if no further "
            "options are provided.\n"
            "It also allows conversion into another (rdflib supported)"
            "representation.\n"
            "Usage");
    description.add_options()
        ("help,h","describe arguments")
        ("ontology,o", po::value<std::string>(), "Ontology file name")
        ("format,f", po::value<std::string>(), "Output format")
        ("serialize_imports,i", po::bool_switch(&serialize_imports), "Serialize also imports")
        ("outfile", po::value<std::string>(), "Output file")
        ("show_formats,s", po::bool_switch(&show_formats), "Show formats")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    owlapi::io::RedlandWriter writer;

    if(vm.count("help"))
    {
        std::cout << description << std::endl;
        exit(0);
    }

    if(show_formats)
    {
        std::cout << "Supported target formats:" << std::endl;
        for(const std::string& format : writer.getSupportedFormats())
        {
            std::cout << "    " << format << std::endl;
        }
        exit(0);
    }

    if(!vm.count("ontology"))
    {
        std::cout << description << std::endl;
        exit(1);
    }

    std::string filename = vm["ontology"].as<std::string>();
    OWLOntology::Ptr ontology = owlapi::io::OWLOntologyIO::fromFile(filename);

    if( !vm.count("format") )
    {
        OWLOntologyAsk ask(ontology);
        std::cout << "All classes:  " << ask.allClasses();
    } else {
        std::string outfile;
        if(!vm.count("outfile"))
        {
            outfile = "/tmp/ontology.owl";
        } else {
            outfile = vm["outfile"].as<std::string>();
        }
        std::string format = vm["format"].as<std::string>();
        try {
            writer.setFormat(format);
            writer.write(outfile, ontology, serialize_imports);
        } catch(const std::invalid_argument& e)
        {
            std::cout << e.what() << std::endl;
            std::cout << "Supported target formats:" << std::endl;
            for(const std::string& format : writer.getSupportedFormats())
            {
                std::cout << "    " << format << std::endl;
            }
        }
        std::cout << "(Re-)Formatted output in " << outfile << std::endl;
    }

    return 0;
}
