#ifndef BABBLE_UTIL_H
#define BABBLE_UTIL_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Concatenate the two given strings by allocating a new string in memory
 * and calling strcat.
 */
char* str_concat(const char* str1, const char* str2);

/**
 * Duplicate the given string by allocating a new string in memory and copying
 * the content.
 */
char* str_copy(const char* src);

/**
 * Determine if 'array' of size 'length' contains 'value'.
 */
bool str_array_contains(char* value, char* array[], int length);

/**
 * Append the string content in 'data' to the string at the pointer given by
 * 'content'.
 */
size_t curl_on_write(char* data, size_t size, size_t nmemb, char** content);

#endif
