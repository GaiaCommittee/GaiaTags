#pragma once

#include "Holder.hpp"

#include <unordered_map>
#include <unordered_set>
#include <shared_mutex>

namespace Gaia::Tags
{
    class Index
    {
    private:
        /// Mutex for TagToHoldersMap.
        std::shared_mutex TagToHoldersMapMutex;
        /// Cached tag-holder map for boosting query process.
        std::unordered_map<std::string, std::unordered_set<Holder*>> TagToHoldersMap;
        /// Mutex for HolderToTagsMap.
        std::shared_mutex HolderToTagsMapMutex;
        /// Cached holder-tags map for boosting query process especially for removing a holder.
        std::unordered_map<Holder*, std::unordered_set<std::string>> HolderToTagsMap;

    protected:
        /// Get the holders set of TagToHoldersMap with the given tag name.
        std::unordered_set<Holder*>& GetCategory(const std::string& name);

    public:
        /// Clear the whole index.
        void Clear();
        /// Clear all holders with the given tag name in the index.
        void Clear(const std::string& tag_name);
        /// Register a holder into the index.
        void Register(Holder* holder);
        /// Unregister a holder from the index.
        void Unregister(Holder* holder);

        /// Get all holders with the given name.
        std::unordered_set<Holder*> Filter(const std::string& name);
        /**
         * @brief Get all holders with the given name and value.
         * @tparam ValueType The type for tag value to try to convert into.
         * @param name The name of the tag.
         * @param value The value of the tag.
         * @return Set of holders that meet the requirement.
         */
        template<typename ValueType>
        std::unordered_set<Holder*> Filter(const std::string& name, ValueType value)
        {
            std::unordered_set<Holder*> result;

            auto category_finder = TagToHoldersMap.find(name);
            if (category_finder != TagToHoldersMap.end())
            {
                auto& category = category_finder->second;
                for (auto* holder : category)
                {
                    if (holder->HasTag<ValueType>(name, value))
                    {
                        result.insert(holder);
                    }
                }
            }

            return result;
        }
    };
}
