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

TEST(StlTest, HandlesMap)
{
    std::map<std::string, int> a, b;
    Context ctx;

    a["abc"] = 1;
    a["def"] = 2;

    Serialize(ctx, a);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, b));

    EXPECT_EQ(a, b);
}

TEST(StlTest, HandlesUnorderedMap)
{
    std::unordered_map<std::string, int> a, b;
    Context ctx;

    a["abc"] = 1;
    a["def"] = 2;

    Serialize(ctx, a);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, b));

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
            ::Serialize(context, name, quality);
        }

        bool Deserialize(Context& context)
        {
            return ::Deserialize(context, name, quality);
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
            ::Serialize(context, x, y);
        }

        bool Deserialize(Context& context)
        {
            return ::Deserialize(context, x, y);
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
            ::Serialize(context, position, inventory);
        }

        bool Deserialize(Context& context)
        {
            return ::Deserialize(context, position, inventory);
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
    Serialize(ctx, players_a);
    ctx.cursor = 0;
    EXPECT_TRUE(Deserialize(ctx, players_b));

    EXPECT_EQ(players_a, players_b);
}
