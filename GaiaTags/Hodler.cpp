#include "Holder.hpp"

namespace Gaia::Tags
{
    /// Get the iterator of the tag with the given name and value from Tags.
    std::unordered_map<std::string, Tag>::iterator Holder::FindTag(const std::string &name)
    {
        std::shared_lock lock(TagsMutex);
        auto finder = Tags.find(name);
        return finder;
    }

    /// Add a tag to this holder.
    void Holder::AddTag(const Tag &tag)
    {
        auto finder = FindTag(tag.Name);

        std::unique_lock lock(TagsMutex);
        if (finder == Tags.end())
        {
            Tags.emplace(tag.Name, tag);
            TagNames.insert(tag.Name);
        }
        else
        {
            finder->second.Value = tag.Value;
        }
    }

    /// Construct a tag and add it to this holder.
    void Holder::AddTag(const std::string &name)
    {
        auto finder = FindTag(name);

        if (finder == Tags.end())
        {
            std::unique_lock lock(TagsMutex);
            Tags.emplace(name, Tag(name));
            TagNames.insert(name);
        }
    }

    /// Try to remove the tag with the given name.
    bool Holder::RemoveTag(const std::string &name)
    {
        auto finder = FindTag(name);
        if (finder != Tags.end())
        {
            std::unique_lock lock(TagsMutex);
            Tags.erase(finder);
            TagNames.erase(name);
            return true;
        }
        return false;
    }

    /// Check whether this holder have the tag with the given name or not.
    bool Holder::HasTag(const std::string &name)
    {
        return FindTag(name) != Tags.end();
    }

    /// Try to get the tag with the given name.
    std::optional<Tag> Holder::GetTag(const std::string &name)
    {
        auto finder = FindTag(name);

        std::shared_lock lock(TagsMutex);
        if (finder != Tags.end())
        {
            return {finder->second};
        }
        return std::nullopt;
    }

    /// Get the names of all tags that this holder has.
    const std::unordered_set<std::string> &Holder::GetTagNames()
    {
        return TagNames;
    }

    /// Construct multiple tags and add them to this holder.
    void Holder::AddTag(const std::initializer_list<std::string> names)
    {
        for (const auto& name : names)
        {
            AddTag(name);
        }
    }
}