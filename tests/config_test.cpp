#include <dpp/nlohmann/json.hpp>
#include <gtest/gtest.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/spdlog.h>

#include "utils/config.hpp"

class ConfigTest : public testing::Test {
  protected:
    std::ostringstream _oss;

    void SetUp() override {
        Kulike::config::load("testdata/config.json");

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

TEST_F(ConfigTest, GetsItemCorrrectly) {
    std::string got = Kulike::config::get_string("token");
    std::string expected = "ABCDEFGH";
    ASSERT_EQ(got, expected);
}

TEST_F(ConfigTest, GetsNestedItemCorrrectly) {
    std::string got = Kulike::config::get_string("nested/key1");
    std::string expected = "val1";
    ASSERT_EQ(got, expected);
}

TEST_F(ConfigTest, ErrorsOnWrongKey) {
    std::string got = Kulike::config::get_string("invalid_key");

    std::string error = _oss.str();
    ASSERT_TRUE(error.find("Error getting value") != std::string::npos);
}
