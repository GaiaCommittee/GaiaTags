#pragma once

#include "Tag.hpp"

#include <unordered_map>
#include <unordered_set>
#include <shared_mutex>
#include <functional>
#include <optional>

namespace Gaia::Tags
{
    /**
     * @brief Holder is the base class to provide tags management and query ability.
     */
    class Holder
    {
    private:
        /// Mutex for Tags and TagNames.
        std::shared_mutex TagsMutex;
        /// Tags are indexed and stored according to their names to boost the query process.
        std::unordered_map<std::string, Tag> Tags;
        /// Names of all tags.
        std::unordered_set<std::string> TagNames;

        /**
         * @brief Get the iterator of the tag with the given name and value from Tags.
         * @tparam ValueType The type for tag value to try to convert into.
         * @param name The name of the tag to find.
         * @return The iterator to the wanted tag or end iterator of Tags.
         */
        decltype(Tags)::iterator FindTag(const std::string& name);

        /**
         * @brief Get the iterator of the tag with the given name and value from Tags.
         * @tparam ValueType The type for tag value to try to convert into.
         * @param name The name of the tag to find.
         * @param value The expected value of the tag to find.
         * @return The iterator to the wanted tag or end iterator of Tags.
         */
        template<typename ValueType>
        decltype(Tags)::iterator FindTag(const std::string& name, ValueType value)
        {
            auto finder = FindTag(name);

            std::shared_lock lock(TagsMutex);
            if (finder != Tags.end() && finder->second.Value.has_value())
            {
                try
                {
                    if (std::any_cast<ValueType>(finder->second.Value) == value)
                    {
                        return finder;
                    }
                }catch (std::bad_any_cast&) {}
            }
            return Tags.end();
        }

    public:
        /**
         * @brief Construct a tag and add it to this holder.
         * @param tag The tag to add.
         * @attention Old tag with the same name will be replaced.
         */
        void AddTag(const Tag& tag);

        /**
         * @brief Construct a tag and add it to this holder.
         * @param name The name of the tag to add.
         * @attention Old tag with the same name will be replaced.
         */
        void AddTag(const std::string& name);
        /**
         * @brief Construct multiple tags and add them to this holder.
         * @param names The names of the tags to add.
         * @attention Old tag with the same name will be replaced.
         */
        void AddTag(const std::initializer_list<std::string> names);

        /**
         * @brief Construct multiple tags and add them to this holder.
         * @param names The names of the tags to add.
         * @attention Old tag with the same name will be replaced.
         */
        template <typename... NameTypes>
        void AddTags(NameTypes... names)
        {
            AddTag(std::initializer_list<std::string>{names...});
        }

        /**
         * @brief Construct a tag and add it to this holder.
         * @param name The name of the tag to add.
         * @param value The value of the tag to add.
         * @attention Old tag with the same name will be replaced.
         */
        template<typename ValueType>
        void AddTag(const std::string& name, ValueType value)
        {
            AddTag(Tag(name, value));
        }

        /**
         * @brief Try to remove the tag with the given name.
         * @param name The name of the tag to remove.
         * @retval true Target tag has been removed.
         * @retval false The tag with the given name can not be found.
         */
        bool RemoveTag(const std::string& name);
        /**
         * @brief Try to remove the tag with the given name and value.
         * @tparam ValueType The type for tag value to try to convert into.
         * @param name The name of the tag to remove.
         * @param value The expected value of the tag to remove.
         * @retval true Target tag has been removed.
         * @retval false The tag with the given name and value can not be found,
         *               or a tag with the value has been found but its value can not be converted
         *               into ValueType.
         */
        template<typename ValueType>
        bool RemoveTag(const std::string& name, ValueType value)
        {
            auto finder = FindTag(name, value);

            if (finder != Tags.end())
            {
                std::unique_lock lock(TagsMutex);
                Tags.erase(finder);
                return true;
            }
            return false;
        }

        /// Check whether this holder have the tag with the given name or not.
        bool HasTag(const std::string& name);

        /// Check whether this holder have the tag with the given name and value or not.
        template<typename ValueType>
        bool HasTag(const std::string& name, ValueType value)
        {
            return FindTag(name, value) != Tags.end();
        }

        /**
         * @brief Try to get the tag with the given name.
         * @param name The name of the tag to get.
         * @return Optional value of Tag, std::nullopt if it is not found.
         */
        std::optional<Tag> GetTag(const std::string& name);

        /**
         * @brief Try to get the tag with the given name and value.
         * @tparam ValueType The type for tag value to try to convert into.
         * @param name The name of the tag to get.
         * @param value The expected value of the tag to get.
         * @return Optional value of tag value, std::nullopt if the tag of the given name is not found,
         *         or fail to convert its value into ValueType.
         */
        template<typename ValueType>
        std::optional<Tag> GetTag(const std::string& name, ValueType value)
        {
            auto tag = GetTag(name);

            std::shared_lock lock(TagsMutex);
            if (tag && tag->Value.has_value())
            {
                try
                {
                    if (value == std::any_cast<ValueType>(tag->Value))
                    {
                        return tag;
                    }
                }catch (std::bad_any_cast&) {}
            }
            return std::nullopt;
        }

        /**
         * @brief Try to get the value of the tag as ValueType.
         * @tparam ValueType The type for tag value to try to convert into.
         * @param name The name of the tag to get.
         * @return Optional value of tag value, std::nullopt if the tag of the given name is not found,
         *         or fail to convert its value into ValueType.
         */
        template<typename ValueType>
        std::optional<ValueType> GetTagValue(const std::string& name)
        {
            auto tag = GetTag(name);

            std::shared_lock lock(TagsMutex);
            if (tag && tag->Value.has_value())
            {
                try
                {
                    return std::any_cast<ValueType>(tag->Value);
                }catch (std::bad_any_cast&) {}
            }

            return std::nullopt;
        }

        /// Get the names of all tags that this holder has.
        const std::unordered_set<std::string>& GetTagNames();
    };
}
