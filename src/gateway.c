#include <uwsc.h>

struct uwsc_client* open_connection(char* url) {
    // I don't know what this does.
  	const struct ev_loop *loop = EV_DEFAULT;
  	const int PING_INTERVAL = 10; // seconds
	struct uwsc_client *client;
	client = uwsc_new(loop, url, PING_INTERVAL, NULL);
	return client;
}
