#ifndef OWLAPI_MODEL_OWL_DATA_RANGE_HPP
#define OWLAPI_MODEL_OWL_DATA_RANGE_HPP

#include <vector>
#include <map>
#include "OWLPropertyRange.hpp"

namespace owlapi {
namespace model {

class OWLDataRange : public OWLPropertyRange//, SWRLPredicate
{
public:
    typedef shared_ptr<OWLDataRange> Ptr;
    typedef std::vector<Ptr> PtrList;

    enum Type { COMPLEMENT_OF,
        INTERSECTION_OF,
        ONE_OF,
        UNION_OF,
        DATATYPE,
        DATATYPE_RESTRICTION
    };

    static std::map<Type,std::string> TypeTxt;


    OWLDataRange(Type type = DATATYPE);
    virtual ~OWLDataRange();

    virtual bool isDataRange() const { return true; }

    bool isDatatype() const { return mDataRangeType == DATATYPE; }

    bool isTopDatatype() const { return mTop; }

    /**
     * Get the data range type
     */
    OWLDataRange::Type getDataRangeType() const { return mDataRangeType; }

private:
    void markAsTopDatatype() { mTop = true; }

private:
    OWLDataRange::Type mDataRangeType;
    bool mTop;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_OWL_DATA_RANGE_HPP
