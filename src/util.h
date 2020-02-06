#ifndef BABBLE_UTIL
#define BABBLE_UTIL

#include <stddef.h>

char* str_concat(const char* str1, const char* str2);

size_t curl_on_write(char* data, size_t size, size_t nmemb, char** content);

#endif
