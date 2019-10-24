#include <stdio.h>
#include <stdlib.h>

#include <uwsc.h>

#define EV_LOOP EV_DEFAULT

struct uwsc_client* gateway_open_connection(char* url) {
    const int PING_INTERVAL = 10; // seconds
    struct uwsc_client *client;
    client = uwsc_new(EV_LOOP, url, PING_INTERVAL, NULL);

    if (!client) {
        fprintf(stderr, "Failed to establish gateway connection. Exiting...\n");
        exit(1);
    }

    return client;
}

void gateway_onopen(struct uwsc_client* client) {
    printf("onopen\n");
}

void gateway_onmessage(struct uwsc_client* client,
                       void   *data,
                       size_t length,
                       bool   binary) {
    printf("onmessage:\n");
    printf("binary: %i\n", binary);
    if (binary) {
        // ...
    } else {
        printf("string: %s\n", data);
    }
}   

void gateway_init_loop(struct uwsc_client* client) {
    client->onopen    = gateway_onopen;
    client->onmessage = gateway_onmessage;

    // ??
    struct ev_signal signal_watcher;
    ev_signal_init(&signal_watcher, NULL, SIGINT);
    ev_signal_start(EV_LOOP, &signal_watcher);

    ev_run(EV_LOOP, 0);
}
