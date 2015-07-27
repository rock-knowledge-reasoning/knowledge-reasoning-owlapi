#include "ModelBound.hpp"
#include <sstream>
#include <limits>
#include <base/Logging.hpp>
#include <gecode/int.hh>

namespace owlapi {
namespace csp {

uint32_t ModelBound::MAX_THRESHOLD = 1E06;

ModelBound::ModelBound()
    : model()
    , min(0)
    , max(MAX_THRESHOLD)
{}

ModelBound::ModelBound(const owlapi::model::IRI& model, uint32_t min, uint32_t max)
    : model(model)
    , min(min)
    , max(std::min(max, MAX_THRESHOLD))
{}

uint32_t ModelBound::getMaxResourceCount(const List& boundList)
{
    ModelBound::List::const_iterator cit = boundList.begin();
    uint32_t max = 0;
    for(; cit != boundList.end(); ++cit)
    {
        const ModelBound& bound = *cit;
        if(bound.max > max)
        {
            // Limit to Gecode limit
            max = std::min(bound.max, MAX_THRESHOLD);
        }
    }
    return max;
}

std::string ModelBound::toString(size_t indent) const
{
    std::stringstream ss;
    ss << std::string(indent,' ');
    ss << "ModelBound: '" << model.toString() << "' ";
    ss << "(" << min << ", " << max << ")";
    return ss.str();
}

std::string ModelBound::toString(const List& boundList, size_t indent)
{
    std::stringstream ss;
    std::string hspace(indent,' ');
    ss << hspace;
    ss << "ModelBound::List" << std::endl;
    ModelBound::List::const_iterator cit = boundList.begin();
    for(; cit != boundList.end(); ++cit)
    {
        const ModelBound& bound = *cit;
        ss << hspace;
        ss << "    " << bound.toString() << std::endl;
    }
    return ss.str();
}

} // end namespace csp
} // end namespace owlapi
