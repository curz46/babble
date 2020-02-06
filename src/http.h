#ifndef BABBLE_HTTP
#define BABBLE_HTTP

#include <jansson.h>

const int REQUEST_SUCCESS = 100;
const int REQUEST_ERR_CURL_INIT  = 101;
const int REQUEST_ERR_CURL_FAIL  = 102;
const int REQUEST_ERR_HTTP_CODE  = 103;
const int REQUEST_ERR_JSON_PARSE = 104;

int http_post_json(char* url, json_t* json, json_t** response);

#endif
