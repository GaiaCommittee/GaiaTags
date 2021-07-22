#pragma once

#include <unordered_set>
#include <string>
#include <initializer_list>
#include <shared_mutex>

namespace Gaia::Tags
{
    /**
     * @brief Interface for classes that can have tags and found according to tags.
     */
    class Holder
    {
    private:
        /// Mutex for tags set.
        std::shared_mutex TagsMutex;
        /// Tags that owned by this holder.
        std::unordered_set<std::string> Tags;

    public:
        // Make destructor virtual.
        virtual ~Holder() = default;

        /// Copy constructor.
        Holder(const Holder& target);
        /// Move constructor.
        Holder(Holder&& target) noexcept;

        /// Get all tags of this holder.
        [[nodiscard]] inline const decltype(Tags)& GetTags() const noexcept
        {
            return Tags;
        }

        /**
         * @brief Check whether this holder has the given tag or not.
         * @param tag Tag to check.
         * @retval True This holder has the given tag.
         * @retval False This holder does not have the given tag.
         */
        bool HasTag(const std::string& tag);
        /**
         * @brief Check whether this holder has all of the given tags.
         * @param tags Tags list to check.
         * @retval true This holder has all of the given tags.
         * @retval false This holder doest not have one or more of the given tags.
         * @details Returns true if the given tags is empty.
         */
        bool HasAllTags(const std::unordered_set<std::string>& tags);
        /**
         * @brief Check whether this holder has any of the given tags.
         * @param tags Tags list to check.
         * @retval true This holder has ore or more of the given tags.
         * @retval false This holder doest not have any of the given tags.
         * @details Returns false if the given tags is empty.
         */
        bool HasAnyTags(const std::unordered_set<std::string>& tags);

        /// Add a tag into this holder.
        void AddTag(const std::string& tag);
        /// Add a list of tags into this holder.
        void AddTags(const std::unordered_set<std::string>& tags);

        /// Remove the given tag from this holder.
        void RemoveTag(const std::string& tag);
        /// Remove all tags in the list from this holder.
        void RemoveTags(const std::unordered_set<std::string>& tags);
    };
}