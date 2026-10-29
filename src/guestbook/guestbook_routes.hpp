#pragma once

#include "guestbook/guestbook.hpp"
#include "httplib.h"

void registerGuestbookRoutes(httplib::Server& server, std::vector<GuestbookEntry>& guestbook);

