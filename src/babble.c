#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>

#include "routes.h"
#include "gateway.h"

#define MAX_SEGMENTS 100 // Dynamic allocation too hard, just make it
						 // "large enough"

// struct response_data {
// 	int num_segments;
// 	char* segments[MAX_SEGMENTS];
// };

char* concat(const char* str1, const char* str2) {
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

size_t on_receive(char* data, size_t size, size_t nmemb, char** content) {
	printf("Received: %s\n", data);
	char* result;
	result = concat(*content, data);
	if (result == NULL) {
		fprintf(stderr, "Couldn't concat data to content.");
		exit(1);
	}
	*content = result;

	// *content = malloc( sizeof(*result) );
	// strcpy(content, result);

	printf("New content: %s\n", result);
	// libcurl expects this
	return size * nmemb;

	// size_t new_len = s->len + size*nmemb;
	// s->ptr = realloc(s->ptr, new_len+1);

	// if (s->ptr == NULL) {
 	// 	fprintf(stderr, "realloc() failed\n");
	// 	exit(1);
	// }

	// memcpy(s->ptr + s->len, ptr, size*nmemb);
	// s->ptr[new_len] = '\0';
	// s->len = new_len;

	// printf("on_receive: %s", s->ptr);

	// return size * nmemb;
}

int main() {
	CURL *curl;
	CURLcode result;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if (!curl) {
		printf("Failed to initialised curl!");
		exit(1);
	}

	// struct response_data *data = malloc( sizeof(*data) );
	// data->num_segments = 0;

	char* data = "";

	curl_easy_setopt(curl, CURLOPT_URL, BASE GATEWAY_GET);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, on_receive);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

	result = curl_easy_perform(curl);
	printf("perform Done!\n");

	if (result == CURLE_OK) {
		long response_code = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
		printf("Response Code: %i\n", response_code);
	} else {
		printf("Error: %s\n", curl_easy_strerror(result));
	}

	// int i;
	// for (i = 0; i < data->num_segments; i++) {
	// 	printf("%i\n", i);
	// 	printf("Response: %s\n", data->segments[i]);
	// }

	printf("Final content: %s\n", data);

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}

