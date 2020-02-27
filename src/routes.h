#ifndef BABBLE_ROUTES
#define BABBLE_ROUTES

#include "entities.h"

#define BASE "https://discordapp.com/api"
#define GATEWAY_GET "/gateway"

#define CREATE_MESSAGE "/channels/%s/messages"
#define EDIT_MESSAGE "/channels/%s/messages/%s"

#define format_route(URL, ROUTE, args...) \
    sprintf(URL, BASE ROUTE, args)

int create_message(message_t message, message_t* created);
int edit_message(message_t message, message_t* edited);

#endif
