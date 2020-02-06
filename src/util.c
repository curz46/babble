#include "util.h"

#include <stddef.h>

char* str_concat(const char* str1, const char* str2) {
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

size_t curl_on_write(char* data, size_t size, size_t nmemb, char** content) {
    char* result;
    result = str_concat(*content, data);
    if (result == NULL) {
        printf("ERROR: Couldn't concat data to content.");
        exit(1);
    }
    *content = result;
    
    // libcurl expects this
    return size * nmemb;
}
