#include "routes.h"
#include "entities.h"
#include "http.h"

int create_message(char* channel_id, Message message, Message* created) {
    char url[1000];
    format_route(url, CREATE_MESSAGE, channel_id);
    
    json_t* json = compose_message(message);
    json_t* response;

    int result = http_post_json(url, json, &response);
    if (result == REQUEST_SUCCESS && created != NULL) {
        *created = parse_message(response);
    }

    return result;
}
