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

    holder.AddTag("ValueTag", 3);
    EXPECT_EQ(holder.GetTagValue<int>("ValueTag"), 3);
    EXPECT_TRUE(holder.GetTag("ValueTag").has_value());
    EXPECT_TRUE(holder.GetTag("ValueTag", 3).has_value());
    EXPECT_FALSE(holder.GetTag("ValueTag2", 3).has_value());
    EXPECT_FALSE(holder.GetTag("ValueTag", 4).has_value());

    holder.AddTag({"SampleTag1", "SampleTag2", "SampleTag3"});
    holder.AddTags("SampleTag4", "SampleTag5", "SampleTag6");
    for (const auto& tag : holder.GetTagNames())
    {
        std::cout << tag << " ";
    }
    std::cout << std::endl;
}