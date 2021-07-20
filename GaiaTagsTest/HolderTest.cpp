#include <gtest/gtest.h>
#include "../GaiaTags/GaiaTags.hpp"

using namespace Gaia::Tags;

TEST(HolderTest, Basic)
{
    Holder holder;
    holder.AddTag("SampleTag");
    EXPECT_TRUE(holder.HasTag("SampleTag"));
    EXPECT_FALSE(holder.HasTag("SampleTest2"));
    holder.RemoveTag("SampleTag3");
    EXPECT_TRUE(holder.HasTag("SampleTag"));
    holder.RemoveTag("SampleTag");
    EXPECT_FALSE(holder.HasTag("SampleTag"));

    holder.AddTags({"SampleTag1", "SampleTag2", "SampleTag3"});
    for (const auto& tag : holder.GetTags())
    {
        std::cout << tag << " ";
    }
    std::cout << std::endl;
}