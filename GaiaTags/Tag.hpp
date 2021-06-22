#pragma once

#include <string>
#include <optional>
#include <any>
#include <type_traits>

namespace Gaia::Tags
{
    /**
     * @brief Tag represents a kind of feature.
     * @details
     *  TagToHoldersMap can be located and filtered according to tags.
     */
    class Tag
    {
    public:
        /**
         * @brief Constructor
         * @param name The name of the tag.
         * @details
         *  The name of a tag can not be changed after it is constructed.
         */
        explicit Tag(std::string name);
        /**
         * @brief Constructor with a initial value.
         * @param name The name of the tag.
         * @param value The value of the tag.
         * @details
         *  The name of a tag can not be changed after it is constructed.
         *  The value of a tag will be converted to std::any.
         */
        template<typename ValueType>
        explicit Tag(std::string name, ValueType value) : Name(std::move(name)), Value(value)
        {}
        /// Copy constructor.
        Tag(const Tag&) = default;
        /// Move constructor.
        Tag(Tag&&) noexcept = default;

        /// Name of the tag.
        const std::string Name;

        /// Optional value.
        std::any Value;

        /// Check whether Name is equal to the given name.
        bool operator==(const std::string& name);
        /// Check whether Name is equal to the given name.
        bool NameEqual(const std::string& name);
        /// Check whether Name is equal to the name of the given tag.
        bool NameEqual(const Tag& tag);

        /// Check whether Value is equal to the given value or not.
        template <typename ValueType>
        bool ValueEqual(ValueType value) noexcept(false)
        {
            try
            {
                if (Value.has_value() && value == std::any_cast<ValueType>(Value))
                {
                    return true;
                }
            }catch (std::bad_any_cast&)
            {}

            return false;
        }
        /// Check whether Value is equal to the given value or not.
        template <typename ValueType>
        bool ValueEqual(const Tag& tag) noexcept(false)
        {
            try
            {
                if (Value.has_value() && tag.Value.has_value() &&
                    std::any_cast<ValueType>(tag.Value) == std::any_cast<ValueType>(Value))
                {
                    return true;
                }
            }catch (std::bad_any_cast&)
            {}

            return false;
        }

        /**
         * @brief Check whether both of Name and Value are equal to those of the given tag.
         * @param target The tag to compare.
         * @retval true Both of Name and Value are equal to those of the target, or Name is equal to the
         *              Name of target and both of itself and target do not have a value.
         */
        template<typename ValueType>
        bool TagEqual(const Tag& target)
        {
            if (Name == target.Name)
            {
                if (Value.has_value() && target.Value.has_value())
                {
                    if (Value.type() == target.Value.type())
                    {
                        try
                        {
                            if (std::any_cast<ValueType>(Value) == std::any_cast<ValueType>(target.Value))
                            {
                                return true;
                            }
                        }catch (std::bad_any_cast&)
                        {}
                    }
                }
                if (!Value.has_value() && !target.Value.has_value())
                {
                    return true;
                }
            }

            return false;
        }
    };
}