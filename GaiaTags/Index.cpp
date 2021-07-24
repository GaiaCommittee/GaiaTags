#include "Index.hpp"
#include <tbb/tbb.h>

namespace Gaia::Tags
{
    /// Add a Holder into this layer.
    void Index::AddHolder(Holder *holder)
    {
        std::shared_lock lock(HoldersMutex);
        Holders.emplace(holder);
    }

    /// Add holders into this layer.
    void Index::AddHolders(const std::unordered_set<Holder *> &holders)
    {
        std::unique_lock lock(HoldersMutex);
        for (auto* holder : holders)
        {
            Holders.emplace(holder);
        }
    }

    /// Remove the given holder from this layer.
    void Index::RemoveHolder(Holder *holder)
    {
        std::unique_lock lock(HoldersMutex);
        Holders.erase(holder);
    }

    /// Remove holders from this layer.
    void Index::RemoveHolders(const std::unordered_set<Holder *> &holders)
    {
        std::unique_lock lock(HoldersMutex);
        for (auto* holder : holders)
        {
            Holders.erase(holder);
        }
    }

    /// Found all holders that has the given tag.
    std::unordered_set<Holder *> Index::Filter(const std::string &tag)
    {
        tbb::concurrent_queue<Holder *> filtered_holders;

        std::shared_lock lock(HoldersMutex);
        tbb::parallel_for_each(Holders, [&filtered_holders, &tag](Holder* holder)
        {
           if (holder->HasTag(tag))
               filtered_holders.emplace(holder);
        });
        lock.unlock();

        std::unordered_set<Holder*> result;
        while (!filtered_holders.empty())
        {
            Holder* holder;
            filtered_holders.try_pop(holder);
            result.emplace(holder);
        }
        return result;
    }

    /// Found all holders that satisfy the condition.
    std::unordered_set<Holder *>
    Index::Filter(const std::unordered_set<std::string> &with_tags, const std::unordered_set<std::string> &without_tags)
    {
        tbb::concurrent_queue<Holder *> filtered_holders;

        std::shared_lock lock(HoldersMutex);
        tbb::parallel_for_each(Holders, [&filtered_holders, &with_tags, &without_tags]
        (Holder* holder)
        {
            if (holder->HasAllTags(with_tags) && !holder->HasAnyTags(without_tags))
                filtered_holders.emplace(holder);
        });
        lock.unlock();

        std::unordered_set<Holder*> result;
        while (!filtered_holders.empty())
        {
            Holder* holder;
            filtered_holders.try_pop(holder);
            result.emplace(holder);
        }
        return result;
    }

    /// Filter the holders with the given condition lambda function.
    std::unordered_set<Holder *> Index::Filter(const std::function<bool(Holder *)> &condition)
    {
        if (!condition) return {};

        tbb::concurrent_queue<Holder *> filtered_holders;

        std::shared_lock lock(HoldersMutex);
        tbb::parallel_for_each(Holders, [&filtered_holders, &condition]
                (Holder* holder)
        {
            if (condition(holder))
                filtered_holders.emplace(holder);
        });
        lock.unlock();

        std::unordered_set<Holder*> result;
        while (!filtered_holders.empty())
        {
            Holder* holder;
            filtered_holders.try_pop(holder);
            result.emplace(holder);
        }
        return result;
    }

    /// Copy constructor.
    Index::Index(const Index &target) : Holders(target.Holders)
    {}

    /// Move constructor.
    Index::Index(Index &&target) noexcept : Holders(std::move(target.Holders))
    {}

    /// Clear all holders.
    void Index::ClearHolders()
    {
        std::unique_lock lock(HoldersMutex);
        Holders.clear();
    }
}