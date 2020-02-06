#ifndef BABBLE_ROUTES
#define BABBLE_ROUTES

#define BASE "https://discordapp.com/api"
#define GATEWAY_GET "/gateway"

#define CREATE_MESSAGE "/channels/%s/messages"

#define format_route(ROUTE, args...) \
    BASE sprintf(ROUTE, args)

#endif
