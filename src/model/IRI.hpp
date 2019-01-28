#ifndef OWLAPI_MODEL_IRI_HPP
#define OWLAPI_MODEL_IRI_HPP

#include <stdexcept>
#include <vector>
#include <set>
#include <functional>
#include <owlapi/model/URI.hpp>
#include <boost/serialization/string.hpp>
#include <boost/functional/hash.hpp>

namespace owlapi {
namespace model {

class IRI;

typedef std::vector<IRI> IRIList;
typedef std::set<IRI> IRISet;

class IRI
{
protected:
    std::string mPrefix;
    std::string mRemainder;

    void setFromString(const std::string& s);

public:
    IRI();
    IRI(const char* s);
    IRI(const std::string& s);
    IRI(const std::string& prefix, const std::string& remainder);

    /**
     * Convert IRI to URI
     * \return URI
     */
    URI toURI() const;

    /**
     * Test if IRI is absolute
     * \return true upon success, false otherwise
     */
    bool isAbsolute() const;

    /**
     * \return IRI scheme, i.e http, urn
     * \throws std::invalid_argument if IRI does not have a scheme
     */
    std::string getScheme() const;

    /**
     * Get the prefix (namespace) of this IRI
     */
    std::string getPrefix() const { return mPrefix; }

    /**
     * Get the namespace / prefix of this IRI
     */
    std::string getNamespace() const { return getPrefix(); }

    /**
     * Get remainder
     */
    std::string getRemainder() const { return mRemainder; }

    /**
     * Resolve the IRI
     */
    IRI resolve(const std::string& relative_path) const;

    /**
     * To quoted string
     */
    std::string toQuotedString() const { return '<' + toString() + '>'; }

    /**
     * To string
     */
    std::string toString() const { return mPrefix + mRemainder; }

    /**
     * To string
     */
    static std::string toString(const IRIList& list, bool fragementsOnly = false);

    /**
     * To escape string, e.g. for usage as regular expression
     */
    std::string toEscapedString() const;

    /**
     * Get Fragement of IRI, i.e. everything past #
     */
    std::string getFragment() const;

    /**
     * Create IRI from single string
     * \return IRI
     */
    static IRI create(const std::string& iri_string);

    /**
     * Create IRI from prefix and suffix
     * \return IRI
     */
    static IRI create(const std::string& prefix, const std::string& suffix);

    /**
     * Create IRI from URI
     * \return IRI
     */
    static IRI create(const URI& uri) { return IRI::create(uri.toString()); }

    bool empty() const { return toString().empty(); }

    /**
     * Equals operator
     */
    bool operator==(const IRI& other) const;

    bool operator!=(const IRI& other) const { return !(*this == other); }

    bool operator<(const IRI& other) const { return toString() < other.toString(); }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & mPrefix;
        ar & mRemainder;
    }
};

/**
 * Stream an IRI
 */
std::ostream& operator<<(std::ostream& os, const owlapi::model::IRI& iri);

/**
 * Stream list of IRIs
 */
std::ostream& operator<<(std::ostream& os, const IRIList& iris);

/**
 * Stream set of IRIs
 */
std::ostream& operator<<(std::ostream& os, const IRISet& iris);

/**
 * Stream list of list of IRIs
 */
std::ostream& operator<<(std::ostream& os, const std::vector< IRIList >& iris);

} // end namespace model
} // end namespace owlapi

// Enable usage of IRI as key in unordered maps
namespace std {
template<>
struct hash<owlapi::model::IRI>
{
    size_t operator()(const owlapi::model::IRI& iri) const
    {
        return std::hash<std::string>()(iri.toString());
    }
};

template<>
struct hash< pair<owlapi::model::IRI, owlapi::model::IRI> >
{
    size_t operator()(const pair<owlapi::model::IRI, owlapi::model::IRI>& p) const
    {
        size_t seed = 0;
        boost::hash_combine(seed, p.first.toString());
        boost::hash_combine(seed, p.second.toString());
        return seed;
    }
};
} // end namespace std
#endif // OWLAPI_MODEL_IRI_HPP
