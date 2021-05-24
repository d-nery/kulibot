#include <dpp/nlohmann/json.hpp>
#include <gtest/gtest.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/spdlog.h>

#include "utils/inventory.hpp"

namespace Kulike {
namespace inventory {

bool operator==(const Item i1, const Item i2) {
    return i1.name == i2.name && i1.description == i2.description && i1.amount == i2.amount;
}

} // namespace inventory
} // namespace Kulike

class InventoryTest : public testing::Test {
  protected:
    std::ostringstream _oss;

    void SetUp() override {
        Kulike::inventory::load("testdata/inventory.json");

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
        Kulike::inventory::reset();
        _oss.str("");
    }
};

TEST_F(InventoryTest, LoadsCorrectly) {
    ASSERT_EQ(Kulike::inventory::get_categories(), (std::vector<std::string>{"category1", "category2"}));
    ASSERT_EQ(Kulike::inventory::get_items("category1"),
              (std::vector<Kulike::inventory::Item>{{"item1", "desc1", 0}, {"item2", "desc2", 1}}));
    ASSERT_EQ(Kulike::inventory::get_items("category2"), (std::vector<Kulike::inventory::Item>{{"item3", "desc3", 2}}));
}

TEST_F(InventoryTest, ErrorOnInsertExisting) {
    Kulike::inventory::add_item("category1", {"item1", "desc1", 0});

    std::string test = _oss.str();
    ASSERT_EQ(test, "Item already exists!\n");
}
