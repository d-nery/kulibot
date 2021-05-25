#include <dpp/nlohmann/json.hpp>
#include <gtest/gtest.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/spdlog.h>

#include "utils/characters.hpp"

class CharTest : public testing::Test {
  protected:
    std::ostringstream _oss;

    void SetUp() override {
        Kulike::characters::load("testdata/chars");

        auto ostream_logger = spdlog::get("gtest_logger");
        if (!ostream_logger) {
            auto ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(_oss);
            ostream_logger = std::make_shared<spdlog::logger>("gtest_logger", ostream_sink);
            ostream_logger->set_pattern("%v");
            ostream_logger->set_level(spdlog::level::warn);
        }

        spdlog::set_default_logger(ostream_logger);
    }

    void TearDown() override {
        // Kulike::config::reset();
        _oss.str("");
    }
};

TEST_F(CharTest, LoadsCharactersCorrrectly) {
    int got = Kulike::characters::list().size();
    int expected = 2;
    ASSERT_EQ(got, expected);
}

TEST_F(CharTest, ParsesCharacterCorrrectly) {
    auto c = Kulike::characters::get("char1");

    ASSERT_EQ(c.name, "Character Name");
    ASSERT_EQ(c.race, "Human");
    EXPECT_EQ(c.classes.size(), 1);
    ASSERT_EQ(c.classes[0], "Hunter Ranger (10)");
    ASSERT_EQ(c.background, "Background");
    ASSERT_EQ(c.alignment, "Chaotic Good");
    ASSERT_EQ(c.image, "img.png");
    ASSERT_EQ(c.stats["str"], 10);
    ASSERT_EQ(c.stats["dex"], 11);
    ASSERT_EQ(c.stats["con"], 12);
    ASSERT_EQ(c.stats["int"], 13);
    ASSERT_EQ(c.stats["wis"], 14);
    ASSERT_EQ(c.stats["cha"], 15);
    EXPECT_EQ(c.proficiencies.size(), 2);
    ASSERT_EQ(c.proficiencies[0], "arcana");
    ASSERT_EQ(c.proficiencies[1], "dex");
    ASSERT_EQ(c.max_hp, 100);
    ASSERT_EQ(c.ac, 10);
    ASSERT_EQ(c.prof, 2);
    ASSERT_EQ(c.owner, 1);
}

TEST_F(CharTest, GetsModifierCorrrectly) {
    auto c = Kulike::characters::get("char1");

    ASSERT_EQ(c.modifier("wrong"), 0);
    ASSERT_EQ(c.modifier("str"), 0);
    ASSERT_EQ(c.modifier("dex"), 2);
    ASSERT_EQ(c.modifier("cha"), 2);
    ASSERT_EQ(c.modifier("arcana"), 3);
    ASSERT_EQ(c.modifier("deception"), 2);

    auto c2 = Kulike::characters::get("char2");
    ASSERT_EQ(c2.modifier("arcana"), 2); // Jack of All trades gets half prof
}

TEST_F(CharTest, RollsCorrrectly) {
    auto c = Kulike::characters::get("char1");

    ASSERT_EQ(c.roll("wrong"), 0);

    uint32_t rolls = 0;
    for (int i = 0; i < 10000; i++) {
        rolls += c.roll("str");
    }

    double avg = rolls / 10000.0;

    // The average roll must be 10.5, check if its near
    ASSERT_TRUE(10 < avg && avg < 11) << "Avg: " << avg;
}
