#include <gtest/gtest.h>

#include <cerealizer.h>

using namespace Cerealizer;

TEST(GeneralTest, CopyingBufferBetweenContexts)
{
    Context ctxA, ctxB;
    std::string a = "abc", b;

    ctxA.Serialize(a);
    
    ctxB.SetBuffer(ctxA.GetBuffer());

    EXPECT_TRUE(ctxB.Deserialize(b));
    EXPECT_EQ(a, b);
}

TEST(GeneralTest, InitializingWithBuffer)
{
    Context ctxA;
    std::string a = "abc", b;

    ctxA.Serialize(a);
    
    Context ctxB(ctxA.GetBuffer());

    EXPECT_TRUE(ctxB.Deserialize(b));
    EXPECT_EQ(a, b);
}

TEST(GeneralTest, ReadingAfterSettingBuffer)
{
    Context ctxA;
    std::string a = "asdf", b;
    ctxA.Serialize(a);

    Context ctxB;
    ctxB.Serialize<uint8_t>(5);

    ctxB.SetBuffer(ctxA.GetBuffer());

    EXPECT_TRUE(ctxB.Deserialize(b));
    EXPECT_EQ(a, b);
}

TEST(GeneralTest, RewindAllowsReadingTwice)
{
    Context ctxA;

    std::string a = "abc", b, c;
    ctxA.Serialize(a);

    Context ctxB(ctxA.GetBuffer());
    
    EXPECT_TRUE(ctxB.Deserialize(b));
    EXPECT_EQ(a, b);

    ctxB.Rewind();

    EXPECT_TRUE(ctxB.Deserialize(c));
    EXPECT_EQ(a, c);
}

TEST(GeneralTest, DeserializationWithoutRewindShouldFail)
{
    Context ctx;

    uint32_t a = 18, b;

    ctx.Serialize(a);
    EXPECT_FALSE(ctx.Deserialize(b));
}

TEST(GeneralTest, DeserializationAfterClearShouldFail)
{
    Context ctx;

    uint32_t a = 5, b;

    ctx.Serialize(a);
    ctx.ClearBuffer();
    EXPECT_FALSE(ctx.Deserialize(b));
}

TEST(GeneralTest, ClearingShouldResetBuffer)
{
    Context ctx;

    uint32_t a = 5, b;

    ctx.Serialize(std::string("abcdefghij"));
    ctx.ClearBuffer();
    ctx.Serialize(a);
    ctx.Rewind();
    
    EXPECT_TRUE(ctx.Deserialize(b));
    EXPECT_EQ(a, b);
}
