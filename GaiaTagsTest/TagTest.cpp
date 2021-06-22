#include <gtest/gtest.h>
#include "../GaiaTags/GaiaTags.hpp"

using namespace Gaia::Tags;

TEST(TagTest, Equal)
{
    Tag sample_tag1("Sample1");
    EXPECT_EQ(sample_tag1.Name, "Sample1");
    EXPECT_TRUE(sample_tag1 == "Sample1");
    Tag sample_tag1_2("Sample1");
    EXPECT_TRUE(sample_tag1.NameEqual(sample_tag1_2));
    Tag sample_tag2("Sample2", 3);
    EXPECT_TRUE(sample_tag2.ValueEqual(3));
    Tag sample_tag2_1("Sample2", 3);
    EXPECT_TRUE(sample_tag2.TagEqual<int>(sample_tag2_1));
    Tag sample_tag2_2("Sample2", 4);
    EXPECT_TRUE(sample_tag2.NameEqual(sample_tag2_2));
    Tag sample_tag3_1("Sample3", 4);
    EXPECT_FALSE(sample_tag2.ValueEqual<int>(sample_tag3_1));
    EXPECT_FALSE(sample_tag2.TagEqual<int>(sample_tag2_2));
    Tag sample_tag3_2("Sample3", 5);
    EXPECT_FALSE(sample_tag2.TagEqual<int>(sample_tag3_2));
}