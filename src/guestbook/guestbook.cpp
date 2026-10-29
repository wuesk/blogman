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

    try {
        json data;
        file >> data;

        std::vector<GuestbookEntry> entries;

        for (const auto& item : data) {
            GuestbookEntry entry;

            entry.name = item.at("name").get<std::string>();
            entry.message = item.at("message").get<std::string>();
            entry.createdAt = item.at("createdAt");

            entries.push_back(entry);
        }

        return entries;
    } catch (const json::exception& error) {
        std::cerr << "Failed to load guestbook: " << error.what() << '\n';
        return {};
    }
}

bool saveGuestbook(const std::vector<GuestbookEntry>& entries)
{
    std::filesystem::create_directories(guestbookFilePath.parent_path());

    json data = json::array();

    for (const GuestbookEntry& entry : entries)
    {
        data.push_back({
            {"name", entry.name},
            {"message", entry.message},
            {"createdAt", entry.createdAt}
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
