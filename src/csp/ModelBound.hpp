#ifndef OWLAPI_MODEL_CSP_MODEL_BOUND_HPP
#define OWLAPI_MODEL_CSP_MODEL_BOUND_HPP

#include <vector>
#include <map>
#include <owlapi/model/IRI.hpp>

namespace owlapi {
namespace csp {

struct ModelBound
{
    typedef std::vector<ModelBound> List;

    static uint32_t MAX_THRESHOLD;

    owlapi::model::IRI model;

    uint32_t min;
    uint32_t max;

    ModelBound();
    ModelBound(const owlapi::model::IRI& model, uint32_t min, uint32_t max);

    static uint32_t getMaxResourceCount(const List& boundList);
    std::string toString(size_t indent = 0) const;
    static std::string toString(const List& boundList, size_t indent = 0);

    /**
     * Substract the given ModelBound from the current object
     * \throws if model in other does not match to this
     */
    ModelBound substract(const ModelBound& other) const;

    /**
     * Substract list b from a
     * \param a List of model bounds (lval)
     * \param b List of model bounds (rval)
     * \param removeNegative Remove results with a negative result
     * \returns a - b
     */
    static ModelBound::List substract(const ModelBound::List& a, const ModelBound::List& b, bool removeNegative = false);

    /**
     * Decrement min and max values
     * \throws std::runtime_error if null values exist and cannot be further decremented
     */
    void decrement();
};

} // end namespace csp
} // end namespace owlapi
#endif // OWLAPI_MODEL_CSP_MODEL_BOUND_HPP
