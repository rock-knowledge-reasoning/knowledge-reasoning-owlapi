#include <iostream>

#include <owlapi/OWLApi.hpp>
#include <owlapi/io/OWLOntologyIO.hpp>

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cout << "usage: " << argv[0] << "-c <url>" << std::endl;
        std::cout << "    -c URL    canonize a url" << std::endl;
        return 0;
    }

    std::cout << owlapi::io::OWLOntologyIO::canonizeForOfflineUsage(argv[2]) << std::endl;
    return 0;
}
