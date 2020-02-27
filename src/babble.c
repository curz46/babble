#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>
#include <jansson.h>
#include <uwsc.h>
#include <ev.h>

#include "babble/routes.h"

#include "api.h"
#include "gateway.h"
#include "util.h"

#define MAX_SEGMENTS 100 // Dynamic allocation too hard, just make it
                         // "large enough"

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    const CURL* curl = curl_easy_init();
    if (!curl) {
        printf("Failed to initialised curl!");
        exit(1);
    }

    char* data = "";

    curl_easy_setopt(curl, CURLOPT_URL, BASE GATEWAY_GET);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_on_write);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    const CURLcode result = curl_easy_perform(curl);

    if (result == CURLE_OK) {
        long response_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        printf("Response Code: %i\n", response_code);
    } else {
        printf("Error: %s\n", curl_easy_strerror(result));
    }

    printf("Final content: %s\n", data);

    curl_easy_cleanup(curl);
    //curl_global_cleanup();
    
    json_error_t error;
    json_t* root = json_loads(data, 0, &error);

    if (!root) {
        printf("ERROR: Failed to parse JSON from GATEWAY_GET response");
        exit(1);
    }

    char* url = json_string_value( json_object_get(root, "url") );

    printf("URL: %s\n", url);

    printf("Attempting WS connection...\n");
    struct uwsc_client* client = gateway_open_connection(url);
    // Blocking loop
    gateway_init_loop(client);
    free(client);

    printf("Exiting...\n");

    return 0;
}
