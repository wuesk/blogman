#pragma once

#include <string>
#include <vector>

struct GuestbookEntry
{
    std::string name;
    std::string message;
    std::string createdAt;
};

std::vector<GuestbookEntry> loadGuestbook();
bool saveGuestbook(const std::vector<GuestbookEntry>& entries);