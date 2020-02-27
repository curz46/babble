#include "babble/error.h"
#include "babble/routes.h"
#include "babble/entities.h"

#include "api.h"
#include "http.h"
#include "json.h"
#include "util.h"

#include <stdbool.h>
#include <jansson.h>
#include <string.h>

#define format_route(URL, ROUTE, args...) \
    sprintf(URL, BASE ROUTE, args)

void enforce_keys(json_t* json, char* keys[], int num_keys) {
    const char *key;
    void* tmp;
    json_t* value;

    json_object_foreach_safe(json, tmp, key, value) {
        if (! str_array_contains(key, keys, num_keys)) {
            json_object_del(json, key);
        }
    }
}

bbl_error_t bbl_create_message(message_t message, message_t* created) {
    // TODO: need a more consistent solution
    char url[1000];
    format_route(url, CREATE_MESSAGE, message.channel_id);
    
    json_t* json = compose_message(message);
    json_t* response;

    char* keys[] = {"content", "nonce", "tts", "file", "embed", "payload_json"};
    int num_keys = sizeof(keys) / sizeof(keys[0]);
    enforce_keys(json, keys, num_keys);

    int result = http_post_json(url, json, &response);
    if (result == ERR_OK && created != NULL) {
        *created = parse_message(response);
    }

    return result;
}

bbl_error_t bbl_edit_message(message_t message, message_t* edited) {
    char url[1000];
    format_route(url, EDIT_MESSAGE, message.channel_id, message.id);

    json_t* json = compose_message(message);
    json_t* response;

    char* keys[] = {"content", "embed", "flags"};
    int num_keys = sizeof(keys) / sizeof(keys[0]);
    enforce_keys(json, keys, num_keys);

    int result = http_patch_json(url, json, &response);
    if (result == ERR_OK && edited != NULL) {
        *edited = parse_message(response);
    }

    return result;
} 
