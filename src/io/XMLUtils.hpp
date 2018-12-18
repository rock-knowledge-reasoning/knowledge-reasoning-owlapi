#ifndef OWLAPI_IO_XML_UTILS_HPP
#define OWLAPI_IO_XML_UTILS_HPP

#include <string>

namespace owlapi {
namespace io {

class XMLUtils
{
public:
   /**
     * Gets the index of the longest NCName that is the suffix of a character
     * sequence.
     *
     * \param s The character sequence.
     * \return The index of the longest suffix of the specified character
     *         sequence s that is an NCName, or -1 if the character
     *         sequence s does not have a suffix that is an NCName.
     */
    static size_t getNCNameSuffixIndex(const std::string& s);
};

} // end namespace io
} // end namespace owlapi
#endif // OWLAPI_IO_XML_UTILS_HPP
