#include <csignal>
#include <filesystem>
#include <iostream>

#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "bot/Kulibot.hpp"
#include "commands/commands.hpp"

#include "utils/characters.hpp"
#include "utils/config.hpp"
#include "utils/inventory.hpp"

using namespace Kulike;
namespace fs = std::filesystem;

std::string raw1 = R"(
{
    "t": "INTERACTION_CREATE",
    "s": 4,
    "op": 0,
    "d": {
        "version": 1,
        "type": 2,
        "token":
        "aW50ZXJhY3Rpb246ODQ0Njg5Mzk5MTQ4NTc2Nzk5OlpYWnB1MkdKRnJkU29lQnVhcmxLSGpVYmxJejhBaGdCcE8xRVlWRlJNMkdmUkxTQXZFWlBKRjJLajU3SW5IanRJSHVab1hoZzBleUsyZDVYUGZiR0dvaGpJWHZCY3o3YklNQThJT21WZ2RaUld5VlFjemlCWVRWREtFOUtqMk9z",
        "member": {
            "user": {
                "username": "dnery",
                "public_flags": 128,
                "id": "232163710506893312",
                "discriminator": "0001",
                "avatar": "7abaed9bdb0f434a2dc40b88f89df75f"
            },
            "roles": [
                "232167108497244160"
            ],
            "premium_since": null,
            "permissions": "17179869183",
            "pending": false,
            "nick": "Daniel Nery",
            "mute": false,
            "joined_at": "2016-10-02T15:41:45.170000+00:00",
            "is_pending": false,
            "deaf": false,
            "avatar": null
        },
        "id": "844689399148576799",
        "guild_id": "232165233827774464",
        "data": {
            "options": [
                {
                    "type": 1,
                    "options": [
                        {
                            "value": "plus",
                            "type": 3,
                            "name": "operation"
                        },
                        {
                            "value": 56,
                            "type": 4,
                            "name": "value"
                        }
                    ],
                    "name": "gold"
                }
            ],
            "name": "boh",
            "id": "844688041711763507"
        },
        "channel_id": "637477278476927006",
        "application_id": "415326037035974668"
    }
}
)";

std::string raw2 = R"(
{
    "t": "INTERACTION_CREATE",
    "s": 4,
    "op": 0,
    "d": {
        "version": 1,
        "type": 2,
        "token":
        "aW50ZXJhY3Rpb246ODQ0NzAyNjMwNzk3NjM5NjgxOnhrbVRzRFVtcXptVGVmOGc0SzVrTE9KNVBJZmZvUVQ5eEdxN1ZkNHpzb3NEREhMdFpnZVREdGNnMlc3WTEwczZMNUtoYmNvUE44UElnY1I5UWxCY21Rc3NiT0hicG9hY0M1M0x3WFFXSVA4dWtvT3ZRSnpYeXBCd1JEck1lQ0hY",
        "member": {
            "user": {
                "username": "dnery",
                "public_flags": 128,
                "id": "232163710506893312",
                "discriminator": "0001",
                "avatar": "7abaed9bdb0f434a2dc40b88f89df75f"
            },
            "roles": [
                "232167108497244160"
            ],
            "premium_since": null,
            "permissions": "17179869183",
            "pending": false,
            "nick": "Daniel Nery",
            "mute": false,
            "joined_at": "2016-10-02T15:41:45.170000+00:00",
            "is_pending": false,
            "deaf": false,
            "avatar": null
        },
        "id": "844702630797639681",
        "guild_id": "232165233827774464",
        "data": {
            "name": "ping",
            "id": "844688042405265409"
        },
        "channel_id": "637477278476927006",
        "application_id": "415326037035974668"
    }
}
)";

std::string raw3 = R"(
{
    "t": "INTERACTION_CREATE",
    "s": 4,
    "op": 0,
    "d": {
        "version": 1,
        "type": 2,
        "token":
        "aW50ZXJhY3Rpb246ODQ0NzA3NTQ0NzE0MzEzNzg4OjZ0NHB5ZHdsR0IxQXJpcFgxRnlvNE1HS1dmYjJ5WVFoN2ZHZ0NxZ0tsTDQwcjNZQW0yS2dPb1BwbkVYQ1FWYlhFcmZUUERiZGdvTVhsTkJaQ1ZFYkJFZkZsb0thRzYwM2JzZkxrdWxsSlFhRHV3VjN4bWFrSWJJTFNEVG5uM1pK",
        "member": {
            "user": {
                "username": "dnery",
                "public_flags": 128,
                "id": "232163710506893312",
                "discriminator": "0001",
                "avatar": "7abaed9bdb0f434a2dc40b88f89df75f"
            },
            "roles": [
                "232167108497244160"
            ],
            "premium_since": null,
            "permissions": "17179869183",
            "pending": false,
            "nick": "Daniel Nery",
            "mute": false,
            "joined_at": "2016-10-02T15:41:45.170000+00:00",
            "is_pending": false,
            "deaf": false,
            "avatar": null
        },
        "id": "844707544714313788",
        "guild_id": "232165233827774464",
        "data": {
            "options": [
                {
                    "type": 1,
                    "name": "items"
                }
            ],
            "name": "boh",
            "id": "844688041711763507"
        },
        "channel_id": "637477278476927006",
        "application_id": "415326037035974668"
    }
}
)";

void print_data_opt(std::ostream& o, const dpp::command_data_option& cdo, int depth) {
    std::string spaces(depth, ' ');

    o << fmt::format("{}- Name: {}\n", spaces, cdo.name);
    o << fmt::format("{}  Type: {}\n", spaces, cdo.type);
    o << fmt::format("{}  Value: {}\n", spaces, cdo.value.index());
    o << fmt::format("{}  Options: ({})\n", spaces, cdo.options.size());

    for (auto& opt : cdo.options) {
        print_data_opt(o, opt, depth + 2);
    }
}

std::ostream& operator<<(std::ostream& o, const dpp::command_interaction& c) {
    o << fmt::format("== command_interaction {}\n", c.id);
    o << fmt::format("- Name: {}\n", c.name);
    o << fmt::format("  Options: ({})\n", c.options.size());

    for (auto& opt : c.options) {
        print_data_opt(o, opt, 2);
    }

    return o;
}

std::ostream& operator<<(std::ostream& o, const dpp::guild_member& m) {
    o << fmt::format("nick: {}\n", m.nickname);
    o << fmt::format("premium since: {}\n", m.premium_since);
    o << fmt::format("joined at: {}\n", m.joined_at);
    o << fmt::format("flags: {:8b}\n", m.flags);
    // o << fmt::format("roles: {}\n", m.roles);

    return o;
}

int main() {
    spdlog::set_level(spdlog::level::level_enum::debug);
    config::load("config.json");
    characters::load(config::get_string("chars_folder"));

    for (auto& key : characters::list()) {
        auto _char = characters::get(key);

        fmt::print("Char[{}] -> Name: {} | Img: {}\n", key, _char.name, _char.image);
    }

    // json j = json::parse(raw3);
    // dpp::interaction i;
    // j["d"].get_to(i);

    // fmt::print("Version: {}\n", i.version);
    // fmt::print("Type: {}\n", i.type);
    // fmt::print("Token: {}\n", i.token);
    // fmt::print("id: {}\n", i.id);
    // fmt::print("guild_id: {}\n", i.guild_id);
    // fmt::print("channel_id: {}\n", i.channel_id);
    // fmt::print("application_id: {}\n", i.application_id);
    // fmt::print("Member:\n");
    // std::cout << i.member << std::endl;

    // fmt::print("Data:\n");
    // auto opt = std::get<dpp::command_interaction>(i.data);
    // std::cout << opt << std::endl;
}
