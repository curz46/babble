#include "routes.h"
#include "entities.h"

int create_message(char* channel_id, Message* message) {
    char* url    = format_route(CREATE_MESSAGE, channel_id);
    json_t* json = compose_message(*message);
    return do_post(url, json);
}
