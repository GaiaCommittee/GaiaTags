#pragma once

#include <unordered_set>
#include <shared_mutex>
#include <functional>
#include "Holder.hpp"

namespace Gaia::Tags
{
    /**
     * @brief Layer is a container that associate many tag holders. A tag holder can be added into
     *        many layers, but no one of these layers will manage the memory of this holder.
     */
    class Layer
    {
    protected:
        /// Mutex for holders set.
        std::shared_mutex HoldersMutex;
        /// Holders in this layer.
        std::unordered_set<Holder*> Holders;

    public:
        /// Add a Holder into this layer.
        void AddHolder(Holder* holder);

        /// Add holders into this layer.
        void AddHolders(const std::unordered_set<Holder *> &holders);

        /// Remove the given holder from this layer.
        void RemoveHolder(Holder* holder);

        /// Remove holders from this layer.
        void RemoveHolders(const std::unordered_set<Holder *> &holders);

        /// Found all holders that has the given tag.
        std::unordered_set<Holder*> Filter(const std::string& tag);
        /**
         * @brief Found all holders that satisfy the condition.
         * @param with_tags Tags that desired holders should have.
         * @param without_tags Tags that desired holders should not have.
         * @return Holders that satisfy the condition.
         */
        std::unordered_set<Holder*> Filter(const std::unordered_set<std::string>& with_tags,
                                           const std::unordered_set<std::string>& without_tags);
        /**
         * @brief Filter the holders with the given condition lambda function.
         * @param condition Condition used to found desired holders.
         * @return Holders that satisfy the condition.
         * @details Returns an empty set if the condition is empty.
         */
        std::unordered_set<Holder*> Filter(const std::function<bool(Holder*)>& condition);
    };
}