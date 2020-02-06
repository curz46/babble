#include "routes.h"
#include "entities.h"
#include "http.h"

int create_message(char* channel_id, Message* message, Message* created) {
    char* url    = format_route(CREATE_MESSAGE, channel_id);
    json_t* json = compose_message(*message);
    json_t* response;

    int result = http_post_json(url, json, &response);
    if (result == REQUEST_SUCCESS && created != NULL) {
        *created = parse_message(response);
    }

    return result;
}
