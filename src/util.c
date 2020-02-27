#include "util.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

char* str_copy(const char* src) {
    int size   = strlen(src);
    char* dest = (char*) malloc((size+1) * sizeof(char));
    strcpy(dest, src);
    return dest;
}

bool str_array_contains(char* value, char* array[], int length) {
    for (int i = 0; i < length; i++) {
        char* candidate = array[i];
        if (strcmp(value, candidate) == 0) return true;
    }
    return false;
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
