#include <iostream>
#include "httplib.h"
#include "views.hpp"
#include "guestbook.hpp"
#include "json.hpp"

using json = nlohmann::json;

int main()
{
    httplib::Server server;

    int views = loadViews();
    std::vector<GuestbookEntry> guestbook = loadGuestbook();

    server.Get("/health", [](const httplib::Request& request, httplib::Response& response)
    {
        response.set_content("OK", "text/plain");
    });

    server.Get("/api/views", [&views](const httplib::Request& request, httplib::Response& response)
    {
        response.set_content(std::to_string(views), "text/plain");
    });

    server.Post("/api/views", [&views](const httplib::Request& request, httplib::Response& response)
    {
        const int newViews = views + 1;

        if (!saveViews(newViews))
        {
            response.status = 500;
            response.set_content("Failed to save views", "text/plain");
            return;
        }

        views = newViews;
        
        response.set_content(std::to_string(views), "text/plain");
    });

    server.Get("/api/guestbook", [&guestbook](const httplib::Request& request, httplib::Response& response)
    {
        json data = json::array();

        for (const GuestbookEntry& entry : guestbook)
        {
            data.push_back({
                {"name", entry.name},
                {"message", entry.message}
            });
        }

        response.set_content(data.dump(), "application/json");
    });

    server.Post("/api/guestbook", [&guestbook](const httplib::Request& request, httplib::Response& response)
    {
        json body = json::parse(request.body);

        GuestbookEntry entry;
        entry.name = body.at("name").get<std::string>();
        entry.message = body.at("message").get<std::string>();

        guestbook.push_back(entry);

        if (!saveGuestbook(guestbook))
        {
            response.status = 500;
            response.set_content("Failed to save guestbook", "text/plain");
            return;
        }

        response.status = 201;
        response.set_content("Entry created", "text/plain");
    });

    std::cout << "Blog backend started\n";
    std::cout << "Port: 8080\n";

    server.listen("0.0.0.0", 8080);

    return 0;
}