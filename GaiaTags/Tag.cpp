#include "Tag.hpp"

#include <utility>

namespace Gaia::Tags
{
    /// Constructor which will initialize the name.
    Tag::Tag(std::string name) : Name(std::move(name))
    {}

    /// Check whether Name is equal to the given name.
    bool Tag::operator==(const std::string& name)
    {
        return NameEqual(name);
    }

    /// Check whether Name is equal to the given name.
    bool Tag::NameEqual(const std::string &name)
    {
        return Name == name;
    }

    /// Check whether Name is equal to the name of the given tag.
    bool Tag::NameEqual(const Tag &tag)
    {
        return Name == tag.Name;
    }
}