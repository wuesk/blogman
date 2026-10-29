#include "views_routes.hpp"
#include "views.hpp"

#include <string>

void registerViewsRoutes(httplib::Server& server, int& views)
{
  server.Get("/api/views", [&views](const httplib::Request &request,
                                    httplib::Response &response) {
    response.set_content(std::to_string(views), "text/plain");
  });

  server.Post("/api/views", [&views](const httplib::Request &request,
                                     httplib::Response &response) {
    const int newViews = views + 1;

    if (!saveViews(newViews)) {
      response.status = 500;
      response.set_content("Failed to save views", "text/plain");
      return;
    }

    views = newViews;

    response.set_content(std::to_string(views), "text/plain");
  });
}