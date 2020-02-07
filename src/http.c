#include "http.h"
#include "util.h"

#include <curl/curl.h>
#include <jansson.h>

const int REQUEST_SUCCESS = 100;
const int REQUEST_ERR_CURL_INIT  = 101;
const int REQUEST_ERR_CURL_FAIL  = 102;
const int REQUEST_ERR_HTTP_CODE  = 103;
const int REQUEST_ERR_JSON_PARSE = 104;

int http_post_json(char* url, json_t* json, json_t** response) {
    const CURL* curl = curl_easy_init();
    if (!curl) {
        return REQUEST_ERR_CURL_INIT;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_on_write);
    char* data = "";
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    // Add authorization headers

    struct curl_slist *slist = NULL;
    slist = curl_slist_append(slist, "");

    CURLcode result = curl_easy_perform(curl);
    if (result == CURLE_OK) {
        int response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        if (response_code / 100 != 2) {
            return REQUEST_ERR_HTTP_CODE;
        }
    } else {
        return REQUEST_ERR_CURL_FAIL;
    }

    // code==2xx
    if (response == NULL) {
        return REQUEST_SUCCESS;
    }

    json_error_t parse_error;
    json_t* parsed = json_loads(data, 0, &parse_error);

    //if (parse_error) {
    //    return REQUEST_ERR_JSON_PARSE;
    //}

    *response = parsed;
    return REQUEST_SUCCESS;
}
