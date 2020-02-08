#ifndef BABBLE_HTTP
#define BABBLE_HTTP

#include <jansson.h>

extern const int REQUEST_SUCCESS;
extern const int REQUEST_ERR_CURL_INIT;
extern const int REQUEST_ERR_CURL_FAIL;
extern const int REQUEST_ERR_HTTP_CODE;
extern const int REQUEST_ERR_JSON_PARSE;

int http_get(char* url, char** response);

int http_get_json(char* url, json_t** response);

int http_post(char* url, char* body, char** response);

int http_post_json(char* url, json_t* body, json_t** response);

int http_put(char* url, char* body, char** response);

int http_put_json(char* url, json_t* body, json_t** response);

int http_patch(char* url, char* body, char** response);

int http_patch_json(char* url, json_t* body, json_t** response);

// TODO: delete may have body, but is omitted in this impl
// only bother to add if discord needs it
int http_delete(char* url, char** response);

int http_delete_json(char* url, json_t** response);

#endif
