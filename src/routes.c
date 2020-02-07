#include "routes.h"
#include "entities.h"
#include "http.h"

#include <stdbool.h>
#include <jansson.h>

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

int create_message(char* channel_id, Message message, Message* created) {
    char url[1000];
    format_route(url, CREATE_MESSAGE, channel_id);
    
    json_t* json = compose_message(message);
    json_t* response;

    char* keys[] = {"content", "nonce", "tts", "file", "embed", "payload_json"};
    int num_keys = sizeof(keys) / sizeof(keys[0]);
    enforce_keys(json, keys, num_keys);

    int result = http_post_json(url, json, &response);
    if (result == REQUEST_SUCCESS && created != NULL) {
        *created = parse_message(response);
    }

    return result;
}
