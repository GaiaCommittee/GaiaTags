#include <gtest/gtest.h>
#include "../GaiaTags/GaiaTags.hpp"

using namespace Gaia::Tags;

TEST(IndexTest, Basic)
{
    Index sample_index;
    Holder sample_holder1, sample_holder2, sample_holder3;
    sample_holder1.AddTags("SampleTag1", "SampleTag2");
    sample_holder2.AddTags("SampleTag2", "SampleTag3");
    sample_holder3.AddTags("SampleTag1", "SampleTag2", "SampleTag3");
    sample_index.Register(&sample_holder1);
    sample_index.Register(&sample_holder2);
    sample_index.Register(&sample_holder3);

    auto result1 = sample_index.Filter("SampleTag1");
    EXPECT_EQ(result1.count(&sample_holder1), 1);
    EXPECT_EQ(result1.count(&sample_holder3), 1);

    auto result2 = sample_index.Filter("SampleTag2");
    EXPECT_EQ(result2.count(&sample_holder1), 1);
    EXPECT_EQ(result2.count(&sample_holder2), 1);
    EXPECT_EQ(result2.count(&sample_holder3), 1);

    auto result3 = sample_index.Filter("SampleTag3");
    EXPECT_EQ(result2.count(&sample_holder2), 1);
    sample_holder2.RemoveTag("SampleTag3");
    auto result4 = sample_index.Filter("SampleTag3");
    EXPECT_EQ(result4.count(&sample_holder2), 0);
}