#include <gtest/gtest.h>

#include <cerealizer.h>

using namespace Cerealizer;

TEST(StlTest, HandlesString)
{
    std::string a = "xyz", b;
    Context ctx;

    Serialize(ctx, a);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, b));

    EXPECT_EQ(a, b);
}

TEST(StlTest, HandlesEmptyString)
{
    std::string a = "", b = "asdf";
    Context ctx;

    Serialize(ctx, a);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, b));

    EXPECT_EQ(a, b);
}

TEST(StlTest, HandlesInvalidString)
{
    std::string b;
    Context ctx;

    Serialize<uint32_t>(ctx, 10);
    Serialize<char>(ctx, 'a');
    Serialize<char>(ctx, 'b');
    Serialize<char>(ctx, 'c');
    ctx.cursor = 0;

    EXPECT_FALSE(Deserialize(ctx, b));
}

TEST(StlTest, HandlesVector)
{
    std::vector<uint32_t> a = { 1, 2, 3 }, b;
    Context ctx;

    Serialize(ctx, a);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, b));

    EXPECT_EQ(a, b);
}
