#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>
#include <jansson.h>
#include <uwsc.h>
#include <ev.h>

#include "routes.h"
#include "gateway.h"

#define MAX_SEGMENTS 100 // Dynamic allocation too hard, just make it
                         // "large enough"

char* concat(const char* str1, const char* str2) {
    // Allow 1 byte for the NULL terminator
    char* result = malloc(1 + strlen(str1) + strlen(str2));
    if (result != NULL) {
        result[0] = '\0';
        strcat(result, str1);
        strcat(result, str2);
        return result;
    } else {
        return NULL;
    }
}

size_t on_receive(char* data, size_t size, size_t nmemb, char** content) {
    char* result;
    result = concat(*content, data);
    if (result == NULL) {
        printf("ERROR: Couldn't concat data to content.");
        exit(1);
    }
    *content = result;
    
    // libcurl expects this
    return size * nmemb;
}

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    const CURL* curl = curl_easy_init();
    if (!curl) {
        printf("Failed to initialised curl!");
        exit(1);
    }

    char* data = "";

    curl_easy_setopt(curl, CURLOPT_URL, BASE GATEWAY_GET);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, on_receive);
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
    curl_global_cleanup();
    
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
