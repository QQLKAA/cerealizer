#include <gtest/gtest.h>

#include <cerealizer.h>

using namespace Cerealizer;

TEST(PrimitivesTest, HandlesUint8) 
{
    Context ctx;
    uint8_t x = 0x12, y = 0;

    ctx.Serialize(x);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesUint16) 
{
    Context ctx;
    uint16_t x = 0x1234, y = 0;

    ctx.Serialize(x);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesUint32) 
{
    Context ctx;
    uint32_t x = 0x12345678, y = 0;

    ctx.Serialize(x);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesUint64) 
{
    Context ctx;
    uint64_t x = 0x1234567890123456, y = 0;

    ctx.Serialize(x);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y));

    EXPECT_EQ(x, y);
}
TEST(PrimitivesTest, HandlesInt8) 
{
    Context ctx;
    int8_t x = -120, y = 0;

    ctx.Serialize(x);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesInt16) 
{
    Context ctx;
    int16_t x = -30210, y = 0;

    ctx.Serialize(x);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesInt32) 
{
    Context ctx;
    int32_t x = -2147483648, y = 0;

    ctx.Serialize(x);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesInt64) 
{
    Context ctx;
    int64_t x = -92233720368547758, y = 0;

    ctx.Serialize(x);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesBoolean) 
{
    Context ctx;
    bool x = true, y = 0;

    ctx.Serialize(x);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesFloat) 
{
    Context ctx;
    float x = 12.34f, y = 0;

    ctx.Serialize(x);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesDouble) 
{
    Context ctx;
    double x = 12.34, y = 0;

    ctx.Serialize(x);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y));

    EXPECT_EQ(x, y);
}

TEST(PrimitivesTest, HandlesMultipleValues)
{
    Context ctx;

    int32_t x1 = 1234567, y1; 
    int8_t x2 = 110, y2; 
    int16_t x3 = 12345, y3;

    ctx.Serialize(x1);
    ctx.Serialize(x2);
    ctx.Serialize(x3);

    ctx.Rewind();

    EXPECT_TRUE(ctx.Deserialize(y1));
    EXPECT_TRUE(ctx.Deserialize(y2));
    EXPECT_TRUE(ctx.Deserialize(y3));
 
    ASSERT_EQ(x1, y1);
    ASSERT_EQ(x2, y2);
    ASSERT_EQ(x3, y3);
}

TEST(PrimitivesTest, HandlesVariadicTemplateSerialization)
{
    Context ctx;

    uint8_t x1 = 10, x2 = 15, y1, y2;

    ctx.Serialize(x1, x2);

    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y1));
    EXPECT_TRUE(ctx.Deserialize(y2));

    EXPECT_EQ(x1, y1);
    EXPECT_EQ(x2, y2);
}

TEST(PrimitivesTest, HandlesVariadicTemplateDeserialization)
{
    Context ctx;

    uint8_t x1 = 10, x2 = 15, y1, y2;

    ctx.Serialize(x1);
    ctx.Serialize(x2);

    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(y1, y2));

    EXPECT_EQ(x1, y1);
    EXPECT_EQ(x2, y2);
}

TEST(PrimitivesTest, HandlesVariadicTemplate)
{
    Context ctx;

    uint32_t x1 = 5, y1;
    std::string x2 = "abc", y2;

    ctx.Serialize(x1, x2);
    ctx.Rewind();
    ctx.Deserialize(y1, y2);

    EXPECT_EQ(x1, y1);
    EXPECT_EQ(x2, y2);
}
