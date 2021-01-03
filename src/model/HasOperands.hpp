#ifndef OWLAPI_MODEL_HAS_OPERANDS_HPP
#define OWLAPI_MODEL_HAS_OPERANDS_HPP
namespace owlapi {
namespace model {

template<typename T>
class HasOperands
{
public:
    typedef shared_ptr<T> operand_t;
    typedef typename std::vector<operand_t> operands_t;

    HasOperands(const operands_t& operands)
        : mOperands(operands)
    {}

    virtual ~HasOperands() = default;

    const operands_t& getOperands() const { return mOperands; }

protected:
    operands_t mOperands;
};

} // end namespace model
} // end namespace owlapi
#endif // OWLAPI_MODEL_HAS_OPERANDS_HPP
