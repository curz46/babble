#ifndef BABBLE_HTTP
#define BABBLE_HTTP

#include <jansson.h>

extern const int REQUEST_SUCCESS;
extern const int REQUEST_ERR_CURL_INIT;
extern const int REQUEST_ERR_CURL_FAIL;
extern const int REQUEST_ERR_HTTP_CODE;
extern const int REQUEST_ERR_JSON_PARSE;

int http_post_json(char* url, json_t* json, json_t** response);

#endif
