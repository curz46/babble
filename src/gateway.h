#pragma once

#include <uwsc.h>

struct client_context;

struct uwsc_client* open_connection(char* url);
