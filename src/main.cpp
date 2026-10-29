#include <iostream>

#include "httplib.h"

#include "guestbook/guestbook.hpp"
#include "guestbook/guestbook_routes.hpp"
#include "views/views.hpp"
#include "views/views_routes.hpp"

int main() {
  httplib::Server server;

  int views = loadViews();
  std::vector<GuestbookEntry> guestbook = loadGuestbook();
  registerViewsRoutes(server, views);
  registerGuestbookRoutes(server, guestbook);


  server.Get("/health",
             [](const httplib::Request &request, httplib::Response &response) {
               response.set_content("OK", "text/plain");
             });

  std::cout << "Blog backend started\n";
  std::cout << "Port: 8080\n";

  server.listen("0.0.0.0", 8080);

  return 0;
}