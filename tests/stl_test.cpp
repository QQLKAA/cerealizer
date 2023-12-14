#include <gtest/gtest.h>

#include <cerealizer.h>

using namespace Cerealizer;

TEST(StlTest, HandlesString)
{
    Context ctx;

    std::string a = "xyz", b;

    ctx.Serialize(a);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(b));

    EXPECT_EQ(a, b);
}

TEST(StlTest, HandlesEmptyString)
{
    Context ctx;

    std::string a = "", b = "asdf";

    ctx.Serialize(a);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(b));

    EXPECT_EQ(a, b);
}

TEST(StlTest, HandlesInvalidString)
{
    Context ctx;

    std::string b;

    ctx.Serialize<uint32_t>(10);
    ctx.Serialize('a');
    ctx.Serialize('b');
    ctx.Serialize('c');
    
    ctx.Rewind();

    EXPECT_FALSE(ctx.Deserialize(b));
}

TEST(StlTest, HandlesVector)
{
    Context ctx;

    std::vector<uint32_t> a = { 1, 2, 3 }, b;

    ctx.Serialize(a);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(b));

    EXPECT_EQ(a, b);
}

TEST(StlTest, HandlesMap)
{
    Context ctx;

    std::map<std::string, int> a, b;

    a["abc"] = 1;
    a["def"] = 2;

    ctx.Serialize(a);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(b));

    EXPECT_EQ(a, b);
}

TEST(StlTest, HandlesUnorderedMap)
{
    Context ctx;

    std::unordered_map<std::string, int> a, b;

    a["abc"] = 1;
    a["def"] = 2;

    ctx.Serialize(a);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(b));

    EXPECT_EQ(a, b);
}

TEST(StlTest, HandlesComposition)
{
    struct Item
    {
        std::string name;
        int quality;

        void Serialize(Context& context) const
        {
            context.Serialize(name, quality);
        }

        bool Deserialize(Context& context)
        {
            return context.Deserialize(name, quality);
        }
        
        bool operator==(const Item& other) const
        {
            return name == other.name && quality == other.quality;
        }
    };

    struct Vector2i
    {
        int32_t x, y;

        void Serialize(Context& context) const
        {
            context.Serialize(x, y);
        }

        bool Deserialize(Context& context)
        {
            return context.Deserialize(x, y);
        }

        bool operator==(const Vector2i& other) const
        {
            return x == other.x && y == other.y;
        }
    };

    struct Player
    {
        Vector2i position;
        std::vector<std::pair<Item, uint8_t>> inventory;

        void Serialize(Context& context) const
        {
            context.Serialize(position, inventory);
        }

        bool Deserialize(Context& context)
        {
            return context.Deserialize(position, inventory);
        }

        bool operator==(const Player& other) const
        {
            return position == other.position && inventory == other.inventory;
        }
    };

    std::unordered_map<std::string, Player> players_a, players_b;
    players_a["player a"] = Player {
        .position = Vector2i { 150, -300 },
        .inventory = {
            { Item { .name = "Sword", .quality = 10 }, 1 },
            { Item { .name = "Coin", .quality = 1 }, 50 },
        },
    };
    players_a["player b"] = Player {
        .position = Vector2i { 5, -3 },
        .inventory = {
            { Item { .name = "Axe", .quality = 8 }, 1 },
            { Item { .name = "Coin", .quality = 1 }, 27 },
        },
    };

    Context ctx;
    ctx.Serialize(players_a);
    ctx.Rewind();
    EXPECT_TRUE(ctx.Deserialize(players_b));

    EXPECT_EQ(players_a, players_b);
}
