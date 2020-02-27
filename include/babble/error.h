#ifndef BABBLE_ERRNO
#define BABBLE_ERRNO

typedef enum bbl_error_t {
    ERR_OK,
    ERR_REQUEST_CURL_INIT,
    ERR_REQUEST_CURL_FAIL,
    ERR_REQUEST_HTTP_CODE,
    ERR_REQUEST_JSON_PARSE
} bbl_error_t;

char* bbl_error_message(bbl_error_t error);

#endif
