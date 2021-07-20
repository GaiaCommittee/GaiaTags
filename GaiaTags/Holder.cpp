#include "Holder.hpp"

#include <algorithm>

namespace Gaia::Tags
{
    /// Check whether this holder has the given tag or not.
    bool Holder::HasTag(const std::string &tag)
    {
        std::shared_lock lock(TagsMutex);
        return Tags.find(tag) != Tags.end();
    }

    /// Check whether this holder has all of the given tags.
    bool Holder::HasAllTags(const std::unordered_set<std::string> &tags)
    {
        if (tags.empty()) return true;
        std::shared_lock lock(TagsMutex);
        return std::all_of(tags.begin(), tags.end(),
                           [this](const auto& tag){return HasTag(tag);});
    }

    /// Check whether this holder has any of the given tags.
    bool Holder::HasAnyTags(const std::unordered_set<std::string> &tags)
    {
        if (tags.empty()) return false;
        std::shared_lock lock(TagsMutex);
        return std::any_of(tags.begin(), tags.end(),
                           [this](const auto& tag){return HasTag(tag);});
    }

    /// Add a tag into this holder.
    void Holder::AddTag(const std::string &tag)
    {
        std::unique_lock lock(TagsMutex);
        Tags.emplace(tag);
    }

    /// Add a list of tags into this holder.
    void Holder::AddTags(const std::unordered_set<std::string> &tags)
    {
        std::unique_lock lock(TagsMutex);
        for (const auto& tag : tags)
            Tags.emplace(tag);
    }

    /// Remove the given tag from this holder.
    void Holder::RemoveTag(const std::string &tag)
    {
        std::unique_lock lock(TagsMutex);
        Tags.erase(tag);
    }

    /// Remove all tags in the list from this holder.
    void Holder::RemoveTags(const std::unordered_set<std::string> &tags)
    {
        std::unique_lock lock(TagsMutex);
        for (const auto& tag : tags)
            Tags.erase(tag);
    }
}