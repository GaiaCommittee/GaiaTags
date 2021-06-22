#include "Index.hpp"

namespace Gaia::Tags
{
    /// Get the holders set of TagToHoldersMap with the given tag name.
    std::unordered_set<Holder *>& Index::GetCategory(const std::string& name)
    {
        auto category_finder = TagToHoldersMap.find(name);
        if (category_finder == TagToHoldersMap.end())
        {
            category_finder = TagToHoldersMap.insert(TagToHoldersMap.end(), std::make_pair(name, std::unordered_set<Holder*>()));
        }
        return category_finder->second;
    }

    /// Clear the whole index.
    void Index::Clear()
    {
        // Attention, dead lock may happens here.
        std::unique_lock tag_to_holders_lock(TagToHoldersMapMutex);
        std::unique_lock holder_to_tags_lock(HolderToTagsMapMutex);

        TagToHoldersMap.clear();
        HolderToTagsMap.clear();
    }

    /// Clear all holders with the given tag name in the index.
    void Index::Clear(const std::string &tag_name)
    {
        std::unique_lock tag_to_holders_lock(TagToHoldersMapMutex);
        auto category_finder = TagToHoldersMap.find(tag_name);
        if (category_finder != TagToHoldersMap.end())
        {
            // Attention, dead lock may happens here.
            std::unique_lock holder_to_tags_lock(HolderToTagsMapMutex);

            auto& category = category_finder->second;

            for (auto* holder : category)
            {
                HolderToTagsMap.erase(holder);
            }

            TagToHoldersMap.erase(category_finder);
        }
    }

    /// Register a holder into the index.
    void Index::Register(Holder *holder)
    {
        if (!holder) return;

        auto tag_names = holder->GetTagNames();

        std::unique_lock holder_to_tags_lock(HolderToTagsMapMutex);
        auto holder_finder = HolderToTagsMap.find(holder);
        if (holder_finder != HolderToTagsMap.end())
        {
            holder_finder->second = tag_names;
        }
        holder_to_tags_lock.unlock();

        std::unique_lock tag_to_holders_lock(TagToHoldersMapMutex);
        for (const auto& tag_name : tag_names)
        {
            GetCategory(tag_name).insert(holder);
        }
        tag_to_holders_lock.unlock();
    }

    /// Unregister a holder from the index.
    void Index::Unregister(Holder *holder)
    {
        if (!holder) return;

        std::unordered_set<std::string> tag_names;

        std::shared_lock holder_to_tags_lock(HolderToTagsMapMutex);
        auto holder_finder = HolderToTagsMap.find(holder);
        if (holder_finder != HolderToTagsMap.end())
        {
            tag_names = holder_finder->second;
        }
        holder_to_tags_lock.unlock();

        std::unique_lock tag_to_holders_lock(TagToHoldersMapMutex);
        for (const auto& tag_name : tag_names)
        {
            auto category_finder = TagToHoldersMap.find(tag_name);
            if (category_finder != TagToHoldersMap.end())
            {
                category_finder->second.erase(holder);
            }
            if (category_finder->second.empty())
            {
                TagToHoldersMap.erase(category_finder);
            }
        }
        tag_to_holders_lock.unlock();

        HolderToTagsMap.erase(holder_finder);
    }

    /// Get all holders with the given name.
    std::unordered_set<Holder*> Index::Filter(const std::string &name)
    {
        std::unordered_set<Holder*> result;

        std::unique_lock lock(TagToHoldersMapMutex);
        auto category_finder = TagToHoldersMap.find(name);
        if (category_finder != TagToHoldersMap.end())
        {
            auto& category = category_finder->second;
            for (auto* holder : category)
            {
                if (holder->HasTag(name))
                {
                    result.insert(holder);
                }
                else
                {
                    category.erase(holder);
                }
            }
        }
        return result;
    }
}