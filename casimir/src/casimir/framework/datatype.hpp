#ifndef CASIMIR_DATATYPE_HPP_
#define CASIMIR_DATATYPE_HPP_

#include <memory>
#include <utility>

#include "../casimir.hpp"
#include "../core/context.hpp"
#include "../utilities/uuid.hpp"
#include "contextual_object.hpp"

namespace Casimir::framework {

    struct __DataTypeHandle;
    class DataTypeBuilder;
    class ComplexDataTypeBuilder;
    
    enum class EDataType : ubyte {
        None      = 0b0000,
        Float     = 0b0001,
        Double    = 0b0010,
        Int32     = 0b0011,
        Int64     = 0b0100,
        Structure = 0b1000
    };
    
    /**
     * @brief The DataType class is a immutable object that stored information relative to a given type.
     *
     * It can be either a complex type built from multiple base-type where each sub-type is characterise by an Uuid or
     * a BaseType.
     *
     * Once create this object cannot and may not be modified
     */
    class DataType : public ContextualObject {
        friend class DataTypeBuilder;
        friend class ComplexDataTypeBuilder;
    private:
        std::shared_ptr<__DataTypeHandle> m_handle;
        
        /**
         * @brief Private constructor for the DataType
         * @param ctx The context attached to the current DataType
         * @param handle An opaque handle to the background data stored by this object
         */
        CASIMIR_EXPORT DataType(CasimirContext ctx, std::shared_ptr<__DataTypeHandle> handle);
    
    public:
        /**
         * @brief The class Parameters defines the sub-parameters hold by an DataType object
         */
        struct Parameters {
            /**
             * @brief The offset hold the distance between the start of the data chunk and the current parameters we are considering
             */
            cuint offset;
            
            /**
             * @brief The size is the length (in byte) of the data required by the current Parameters
             */
            cuint size;
            
            /**
             * @brief The type of the current sub-object. Notice that it cannot be another Structure to prevent recurrence issue
             */
            EDataType dtype;
        };
        
        /**
         * @brief Destructor of the DataType object
         */
        CASIMIR_EXPORT ~DataType();
        
        /**
         * @brief Return the size of the overall object
         * @return The size of the object
         */
        CASIMIR_EXPORT cuint sizeOf() const;
        
        /**
         * @brief Return the type of the object
         *
         * There's two kind of type.
         * We have complex type in which case the output of this function will be EDataType::Structure. This structure
         * hold multiple simple object
         * We have base type that doesn't have any sub-object
         *
         * @return The type of the current instance
         */
        CASIMIR_EXPORT EDataType dtype() const;
        
        /**
         * @brief Detect if a given parameters (saved by key) is valid or not
         * @param key The name of the parameters we are considering
         * @return Whether or not the parameters exists in the current object
         */
        CASIMIR_EXPORT bool has(const utilities::Uuid& key) const;
        
        /**
         * @brief Return an utilities::Optional holding the Parameters linked to the given `key` if such object exists otherwise
         * return utilities::Optional::empty()
         * @param key The name of the parameters we are considering
         * @return The parameters linked to the given key if exists otherwise empty
         */
        CASIMIR_EXPORT utilities::Optional<Parameters> at(const utilities::Uuid& key) const;
        
        /**
         * @brief Return the Parameters linked to the given key
         * @throw Exception if no parameters is linked to the given jey
         * @param key The name of the parameters we are considering
         * @return The Parameters linked to the given key
         */
        inline Parameters operator[](const utilities::Uuid& key) const {
            return at(key).get();
        }
 
        /**
         * @brief Detect if the current type is complex or not
         * @return Whether the current object hold sub-object
         */
        inline bool isComplexType() const {
            return dtype() == EDataType::Structure;
        }
    };
    
    /**
     * @brief Builder to create a basic data type object
     */
    class DataTypeBuilder {
    private:
        std::shared_ptr<__DataTypeHandle> m_handle;
        std::shared_ptr<bool> m_builderDisabled;

    public:
        /**
         * @brief Default constructor
         */
        CASIMIR_EXPORT DataTypeBuilder();
        
        /**
         * @brief Set the data type of the object. Notice that it cannot be either EDataType::None or EDataType::Structure
         * as these types are non-base type
         * @param dtype the EDataType of the object we want to create
         * @throw utilities::Exception if the Builder is no longer active (if the object has been created or if we have already called
         * the DataTypeBuilder::asStructure())
         * @return A self-reference
         */
        CASIMIR_EXPORT DataTypeBuilder& setType(EDataType dtype);
        
        /**
         * @brief Convert the current type to a structure object (complex object that hold sub-object)
         * @throw utilities::Exception if the Builder is no longer active (if the object has been created or if we have already called
         * the DataTypeBuilder::asStructure())
         * @return The ComplexData builder
         */
        CASIMIR_EXPORT ComplexDataTypeBuilder asStructure();
        
        /**
         * @brief Create the DataType basic object based on the current configuration
         * @throw utilities::Exception if the Builder is no longer active (if the object has been created or if we have already called
         * the DataTypeBuilder::asStructure())
         * @throw utilities::Exception if the datatype hold by the current builder isn't compatible with a simple object
         * @param ctx The context used to instantiate the datatype
         * @return The new DataType object
         */
        CASIMIR_EXPORT DataType create(CasimirContext ctx);
    };
    
    class ComplexDataTypeBuilder {
        friend class DataTypeBuilder;
    private:
        std::shared_ptr<__DataTypeHandle> m_handle;
        std::shared_ptr<bool> m_builderDisabled;
        std::shared_ptr<cuint> m_currentOffset;
    
        /**
         * @brief Default private constructor of the ComplexDataTypeBuilder
         * @param handle an opaque handle
         */
        CASIMIR_EXPORT explicit ComplexDataTypeBuilder(std::shared_ptr<__DataTypeHandle> handle);
        
    public:
        /**
         * @brief Register a new sub-type to the current builder instance
         * @throw utilities::Exception if the ComplexDataTypeBuilder is no longer active (an object has already been created)
         * @throw utilities::Exception if the type is invalid (EDataType::None or EDataType::Structure) or if the uuid already
         * exists as a parameters
         * @param uuid The name of the parameters we want to create
         * @param dtype The type of the parameters we want to create
         * @return A self-reference
         */
        CASIMIR_EXPORT ComplexDataTypeBuilder& registerSubType(const utilities::Uuid& uuid, EDataType dtype);
        
        /**
         * @brief Create a new instance of DataType (based on the given context) with the given configuration
         * @param ctx the context of the new instance of DataType
         * @return The new instance of DataType
         */
        CASIMIR_EXPORT DataType create(CasimirContext ctx);
    };
    
}

#endif