#ifndef BABBLE_ROUTES
#define BABBLE_ROUTES

#define BASE "https://discordapp.com/api"
#define GATEWAY_GET "/gateway"

#define CREATE_MESSAGE "/channels/%s/messages"

#define format_route(URL, ROUTE, args...) \
    sprintf(URL, BASE ROUTE, args)

#endif
