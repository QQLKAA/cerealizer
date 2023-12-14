#include <gtest/gtest.h>

#include <cerealizer.h>

using namespace Cerealizer;

TEST(PrimitivesTest, HandlesUint8) 
{
    Context ctx;
    uint8_t x = 0x12, y = 0;

    Serialize(ctx, x);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesUint16) 
{
    Context ctx;
    uint16_t x = 0x1234, y = 0;

    Serialize(ctx, x);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesUint32) 
{
    Context ctx;
    uint32_t x = 0x12345678, y = 0;

    Serialize(ctx, x);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, y));

    EXPECT_EQ(x, y);
}


TEST(PrimitivesTest, HandlesUint64) 
{
    Context ctx;
    uint64_t x = 0x1234567890123456, y = 0;

    Serialize(ctx, x);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesInt8) 
{
    Context ctx;
    uint8_t x = -125, y = 0;

    Serialize(ctx, x);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesInt16) 
{
    Context ctx;
    uint16_t x = -1024, y = 0;

    Serialize(ctx, x);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesInt32) 
{
    Context ctx;
    int32_t x = -1048576, y = 0;

    Serialize(ctx, x);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesInt64) 
{
    Context ctx;
    int64_t x = -1099511627776, y = 0;

    Serialize(ctx, x);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesBoolean) 
{
    Context ctx;
    bool x = true, y = 0;

    Serialize(ctx, x);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesFloat)
{
    Context ctx;
    float x = 12.34f, y;

    Serialize(ctx, x);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesDouble)
{
    Context ctx;
    double x = 12.34, y;
    
    Serialize(ctx, x);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesMultipleValues)
{
    Context ctx;

    int32_t x1 = 1234567, y1; 
    int8_t x2 = 110, y2; 
    int16_t x3 = 12345, y3;

    Serialize(ctx, x1);
    Serialize(ctx, x2);
    Serialize(ctx, x3);
    ASSERT_EQ(ctx.cursor, sizeof(int32_t) + sizeof(int8_t) + sizeof(int16_t));

    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, y1));
    EXPECT_TRUE(Deserialize(ctx, y2));
    EXPECT_TRUE(Deserialize(ctx, y3));
    ASSERT_EQ(ctx.cursor, sizeof(int32_t) + sizeof(int8_t) + sizeof(int16_t));

    ASSERT_EQ(x1, y1);
    ASSERT_EQ(x2, y2);
    ASSERT_EQ(x3, y3);
}
