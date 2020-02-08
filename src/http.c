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
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
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

int handle_json(int result, char* received_body, json_t** response) {
    if (response != NULL) {
        printf("%s\n", received_body);

        json_error_t err;
        json_t* parsed = json_loads(received_body, 0, &err);

        if (parsed == NULL) {
            printf("ERROR: Parsing failed: %s\n", err.text);
            return REQUEST_ERR_JSON_PARSE;
        }

        *response = parsed;
    }

    return result;
}

int do_http_get(char* method, char* url, char** response) {
    const CURL* curl = curl_easy_init();
    if (!curl) {
        return REQUEST_ERR_CURL_INIT;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);

    char* data = "";
    set_writefunc(curl, &data);
    struct curl_slist* headers = make_and_set_headers(curl);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);

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

int do_http_get_json(char* method, char* url, json_t** response) {
    char* body = "";
    int result = do_http_get(method, url, &body);

    return handle_json(result, body, response);
}

int do_http_post(char* method, char* url, char* body, char** response) {
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

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);

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

int do_http_post_json(char* method, char* url, json_t* body, json_t** response) {
    char* dumped_body   = json_dumps(body, 0);
    char* received_body = "";

    int result = do_http_post(method, url, dumped_body, &received_body);
    free(dumped_body);

    return handle_json(result, received_body, response);
}

int http_get(char* url, char** response) {
    return do_http_get("GET", url, response);
}

int http_get_json(char* url, json_t** response) {
    return do_http_get_json("GET", url, response);
}

int http_delete(char* url, char** response) {
    return do_http_get("DELETE", url, response);
}

int http_delete_json(char* url, json_t** response) {
    return do_http_get("DELETE", url, response);
}

int http_post(char* url, char* body, char** response) {
    return do_http_post("POST", url, body, response);
}

int http_post_json(char* url, json_t* body, json_t** response) {
    return do_http_post_json("POST", url, body, response);
}

int http_put(char* url, char* body, char** response) {
    return do_http_post("PUT", url, body, response);
}

int http_put_json(char* url, json_t* body, json_t** response) {
    return do_http_post_json("PUT", url, body, response);
}

int http_patch(char* url, char* body, char** response) {
    return do_http_post("PATCH", url, body, response);
}

int http_patch_json(char* url, json_t* body, json_t** response) {
    return do_http_post_json("PATCH", url, body, response);
}
