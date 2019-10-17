#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>

#include <routes.h>

size_t on_payload_received(void *ptr, size_t size, size_t nmemb, void *stream) {
	printf("Received: %s\n", ptr);
	return size * nmemb;
}

int main() {
	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if (!curl) {
		printf("Failed to initialised curl!");
		exit(1);
	}

	curl_easy_setopt(curl, CURLOPT_URL, BASE GATEWAY_GET);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, on_payload_received);
	res = curl_easy_perform(curl);
	
	if (res == CURLE_OK) {
		printf("OK!\n");
		
	} else {
		printf("Error: %s", curl_easy_strerror(res));
	}

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}

