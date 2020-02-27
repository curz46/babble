#ifndef BABBLE_HTTP
#define BABBLE_HTTP

#include <jansson.h>

bbl_error_t http_get(char* url, char** response);

bbl_error_t http_get_json(char* url, json_t** response);

bbl_error_t http_post(char* url, char* body, char** response);

bbl_error_t http_post_json(char* url, json_t* body, json_t** response);

bbl_error_t http_put(char* url, char* body, char** response);

bbl_error_t http_put_json(char* url, json_t* body, json_t** response);

bbl_error_t http_patch(char* url, char* body, char** response);

bbl_error_t http_patch_json(char* url, json_t* body, json_t** response);

// TODO: delete may have body, but is omitted in this impl
// only bother to add if discord needs it
bbl_error_t http_delete(char* url, char** response);

bbl_error_t http_delete_json(char* url, json_t** response);

#endif
