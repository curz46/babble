#include <stdbool.h>
#include <jansson.h>

#include "babble/err.h"
#include "babble/routes.h"
#include "babble/entities.h"

#include "http.h"
#include "json.h"

#define BASE "https://discordapp.com/api"
#define GATEWAY_GET "/gateway"

#define CREATE_MESSAGE "/channels/%s/messages"
#define EDIT_MESSAGE "/channels/%s/messages/%s"

#define format_route(URL, ROUTE, args...) \
    sprintf(URL, BASE ROUTE, args)

bool str_array_contains(char* value, char* array[], int length) {
    for (int i = 0; i < length; i++) {
        char* candidate = array[i];
        if (strcmp(value, candidate) == 0) return true;
    }
    return false;
}

void enforce_keys(json_t* json, char* keys[], int num_keys) {
    char *key;
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
