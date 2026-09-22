#include "guestbook.hpp"
#include "json.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace {
    const std::filesystem::path guestbookFilePath = "data/guestbook.json";
}

std::vector<GuestbookEntry> loadGuestbook()
{
    std::ifstream file(guestbookFilePath);

    if (!file.is_open())
    {
        return {};
    }

    if (file.peek() == std::ifstream::traits_type::eof())
    {
        return {};
    }

    // An existing but empty file is not valid JSON. Treat it like a new
    // guestbook so a partially initialized data directory cannot crash the
    // server during startup.
    json data = json::parse(file, nullptr, false);

    if (data.is_discarded() || !data.is_array())
    {
        std::cerr << "Warning: ignoring invalid guestbook data in "
                  << guestbookFilePath << '\n';
        return {};
    }

    std::vector<GuestbookEntry> entries;

    for (const auto& item : data)
    {
        GuestbookEntry entry;
        entry.name = item.at("name").get<std::string>();
        entry.message = item.at("message").get<std::string>();

        entries.push_back(entry);
    }

    return entries;
}

bool saveGuestbook(const std::vector<GuestbookEntry>& entries)
{
    std::filesystem::create_directories(guestbookFilePath.parent_path());

    json data = json::array();

    for (const GuestbookEntry& entry : entries)
    {
        data.push_back({
            {"name", entry.name},
            {"message", entry.message}
        });
    }

    std::ofstream file(guestbookFilePath);

    if (!file.is_open())
    {
        return false;
    }

    file << data.dump(4);

    return true;
}
