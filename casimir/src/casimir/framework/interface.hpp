#ifndef CASIMIR_INTERFACE_HPP_
#define CASIMIR_INTERFACE_HPP_

#include <utility>

#include "../casimir.hpp"
#include "../utilities/uuid.hpp"
#include "../utilities/string_serializable.hpp"
#include "../utilities/string.hpp"
#include "contextual_object.hpp"
#include "indexable.hpp"

namespace Casimir::framework {
 
    class AbstractAllocator;
    
    /**
     * @brief An AbstractInterface (an interface) defines a specific computational capable hardware object. It can be
     * a CPU / a GPU, etc... It must have a dedicated memory (use the allocator to manage it)
     * @note The AbstractInterface class MUST not be deleted once allocated (not manually) as it is automatically done
     * when the context is released
     */
    class AbstractInterface : public utilities::StringSerializable, public ContextualObject, public IndexableObject {
        CASIMIR_DISABLE_COPY_MOVE(AbstractInterface)
    protected:
        /**
         * @brief Default protected constructor
         * @param ctx Because AbstractInterface is a ContextualObject each object are constructed with a given
         * context
         */
        CASIMIR_EXPORT AbstractInterface(CasimirContext ctx, utilities::Uuid uuid);
        
    public:
        /**
         * @brief Destructor of any AbstractInterface (will unregister the interface
         */
        CASIMIR_EXPORT virtual ~AbstractInterface();
        
        /**
         * @brief Retrieve the AbstractAllocator object linked to the current interface. It is used to manager memory
         * @return The linked AbstractAllocator*
         */
        virtual AbstractAllocator* allocator() const = 0;
        
        /**
         * @brief The readable-name of the interface
         * @return A utilities::String that hold the name of the interface
         */
        virtual utilities::String name() const = 0;
    };
    
}

#endif