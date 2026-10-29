#include <chrono>
#include <iostream>

#include "guestbook.hpp"
#include "guestbook_routes.hpp"

#include "json.hpp"
#include <string>
#include <utility>

using json = nlohmann::json;

namespace {
    std::string currentUtcTimestamp()
    {
        const auto now = std::chrono::system_clock::now();
        const std::time_t time = std::chrono::system_clock::to_time_t(now);

        std::tm utc{};
        gmtime_r(&time, &utc);

        std::ostringstream output;
        output << std::put_time(&utc, "%Y-%m-%dT%H:%M:%SZ");

        return output.str();
    }
}

void registerGuestbookRoutes(httplib::Server &server,
                             std::vector<GuestbookEntry> &guestbook) {
  server.Get("/api/guestbook", [&guestbook](const httplib::Request &request,
                                            httplib::Response &response) {
    json data = json::array();

    for (const GuestbookEntry &entry : guestbook) {
      data.push_back({{"name", entry.name}, {"message", entry.message}, {"createdAt", entry.createdAt}});
    }

    response.set_content(data.dump(), "application/json");
  });

  server.Post("/api/guestbook", [&guestbook](const httplib::Request &request,
                                             httplib::Response &response) {
    try {
      json body = json::parse(request.body);

      if (!body.contains("name") || !body["name"].is_string() ||
          !body.contains("message") || !body["message"].is_string()) {
        response.status = 400;
        response.set_content("Invalid fields", "text/plain");
        return;
      }

      const std::string name = body["name"].get<std::string>();
      const std::string message = body["message"].get<std::string>();

      if (name.empty() || message.empty()) {
        response.status = 400;
        response.set_content("Name and message cannot be empty", "text/plain");
        return;
      }

      // Size in memory not character count!
      if (name.size() > 50 || message.size() > 500) {
        response.status = 400;
        response.set_content("Name or message is too long", "text/plain");
        return;
      }

      GuestbookEntry entry;
      entry.name = name;
      entry.message = message;
      entry.createdAt = currentUtcTimestamp();

      auto updatedGuestbook = guestbook;
      updatedGuestbook.push_back(entry);

      if (!saveGuestbook(updatedGuestbook)) {
        response.status = 500;
        response.set_content("Failed to save guestbook", "text/plain");
        return;
      }

      guestbook = std::move(updatedGuestbook);

      response.status = 201;
      response.set_content("Entry created", "text/plain");
    } catch (const json::exception &error) {
      std::cerr << "JSON error: " << error.what() << '\n';

      response.status = 400;
      response.set_content("Invalid request body", "text/plain");
    }
  });
}