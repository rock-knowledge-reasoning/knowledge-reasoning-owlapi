#ifndef OWLAPI_MODEL_CHANGE_APPLIED_HPP
#define OWLAPI_MODEL_CHANGE_APPLIED_HPP

namespace owlapi {
namespace model {

enum ChangeApplied { /// No operation carried out, i.e. change had no effect
                     CHANGE_NO_OPERATION,
                     /// change has been applied successfully
                     CHANGE_SUCCESS,
                     /// change has not been applied
                     CHANGE_FAILED
                    };

} // end namespace model
} // end namespace owlapi

#endif // OWLAPI_MODEL_CHANGE_APPLIED_HPP

