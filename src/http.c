#include "http.h"
#include "util.h"

#include <curl/curl.h>
#include <jansson.h>

const int REQUEST_SUCCESS = 100;
const int REQUEST_ERR_CURL_INIT  = 101;
const int REQUEST_ERR_CURL_FAIL  = 102;
const int REQUEST_ERR_HTTP_CODE  = 103;
const int REQUEST_ERR_JSON_PARSE = 104;

void set_writefunc(const CURL* curl, char** response) {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_on_write);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
}

struct curl_slist* make_and_set_headers(const CURL* curl) {
    struct curl_slist *headers = NULL;

    // Authorization
    char* token = getenv("TOKEN"); // TODO: Get from client
    char* auth_header[1000];
    sprintf(auth_header, "Authorization: Bot %s", token);
    headers = curl_slist_append(headers, auth_header);
    // Content-Type
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Set headers
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
}

int http_get(char* url, char** response) {
    const CURL* curl = curl_easy_init();
    if (!curl) {
        return REQUEST_ERR_CURL_INIT;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);

    char* data = "";
    set_writefunc(curl, &data);
    struct curl_slist* headers = make_and_set_headers(curl);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

    CURLcode result = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    if (result != CURLE_OK) {
        return REQUEST_ERR_CURL_FAIL;
    }

    int response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code / 100 != 2) {
        return REQUEST_ERR_HTTP_CODE;
    }

    // response_code == 2xx
    if (response != NULL) {
        *response = data;
    }

    return REQUEST_SUCCESS;
}

int http_get_json(char* url, json_t** response) {
    char* body = "";
    int result = http_get(url, &body);

    if (response != NULL) {
        json_error_t err;
        json_t* parsed = json_loads(received_body, 0, &err);

        if (parsed == NULL) {
            return REQUEST_ERR_JSON_PARSE;
        }

        *response = parsed;
    }

    return result;
}

CURL* http_post_prepare(char* url, char* body, char** response) {
    const CURL* curl = curl_easy_init();
    if (!curl) {
        return REQUEST_ERR_CURL_INIT;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);

    char* data = "";
    set_writefunc(curl, &data);
    struct curl_slist* headers = make_and_set_headers(curl);

    // Set POST data
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

    return curl;
}

int http_post_perform(CURL* curl, char* url, char* body, char** response) {
    CURLcode result = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    if (result != CURLE_OK) {
        return REQUEST_ERR_CURL_FAIL;
    }

    int response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code / 100 != 2) {
        return REQUEST_ERR_HTTP_CODE;
    }

    // response_code == 2xx
    if (response != NULL) {
        *response = data;
    }

    return REQUEST_SUCCESS;
}

int handle_json(int result, char* received_body, json_t** response) {
    if (response != NULL) {
        json_error_t err;
        json_t* parsed = json_loads(received_body, 0, &err);

        if (parsed == NULL) {
            return REQUEST_ERR_JSON_PARSE;
        }

        *response = parsed;
    }

    return result;
}

#define DEFINE_POST_METHOD(NAME, METHOD_NAME) \
    int http_##NAME(char* url, char* body, char** response) {             \
        CURL* curl = http_post_prepare(url, body, response);              \
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, #METHOD_NAME);      \
        return http_post_perform(curl, url, body, response);              \
    }                                                                     \
                                                                          \
    int http_##NAME##_json(char* url, json_t* body, json_t** response) {  \
        char* dumped_body   = json_dumps(body, 0);                        \
        char* received_body = "";                                         \
                                                                          \
        CURL* curl = http_post_prepare(url, dumped_body, &received_body); \
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, #METHOD_NAME);      \
        int result = http_post_perform(curl, url, dumped_body, &received_body); \
        free(dumped_body);                                                \
                                                                          \
        return handle_json(result, received_body, response);            \
    }

DEFINE_POST_METHOD(post, POST)
DEFINE_POST_METHOD(put, PUT)
DEFINE_POST_METHOD(patch, PATCH)
