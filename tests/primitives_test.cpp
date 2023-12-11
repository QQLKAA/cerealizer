#include <gtest/gtest.h>

#include <cerealizer.h>

using namespace Cerealizer;

TEST(PrimitevesTest, HandlesUint32) 
{
    Context ctx;
    uint32_t x = 0x12345678, y = 0;

    Serialize(ctx, x);
    ctx.cursor = 0;
    Deserialize(ctx, y);

    EXPECT_EQ(x, y);
}
