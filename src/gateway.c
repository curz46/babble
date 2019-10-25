#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <uwsc.h>
#include <pthread.h>
#include <jansson.h>

#include "gateway.h"
#include "payload.h"

#define EV_LOOP EV_DEFAULT

typedef struct {
    struct uwsc_client* client;   
    int heartbeat_interval;
    int sequence_number;
} client_context;

client_context context;

void send_heartbeat() {
    printf("Making heartbeat\n");
    json_t* payload = make_heartbeat(context.sequence_number);
    printf("Made payload...\n");
    json_t* wrapped = wrap_payload(1, payload);
    printf("Wrapped payload...\n");
    char* data      = json_dumps(wrapped, 0);
    printf("Dumped data...\n");
    printf("Sending heartbeat...\n");
    context.client->send(context.client, data, strlen(data), UWSC_OP_TEXT);
    printf("Sent heartbeat\n");
}

void do_heartbeat(void* arg) {
    printf("Heartbeat starting...\n");
    while (1) {
        printf("usleep: %i\n", context.heartbeat_interval);
        usleep(context.heartbeat_interval * 1000);
        send_heartbeat();
    }
}

void handle_hello(json_t* json) {
    int interval = json_integer_value( json_object_get(json, "heartbeat_interval") );
    printf("handle_hello: interval = %i\n", interval);

    context.heartbeat_interval = interval;
    context.sequence_number    = 0;

    send_heartbeat();
    pthread_t *thread;
    pthread_create(&thread, NULL, do_heartbeat, NULL);

    char* token = getenv("TOKEN");
    printf("Identifying with token: %s\n", token);
    json_t* identify_payload = make_identify( getenv("TOKEN") );
    json_t* wrapped = wrap_payload(2, identify_payload);
    char* data      = json_dumps(wrapped, 0);
    printf("Sending identify: %s\n", data);
    context.client->send(context.client, data, strlen(data), UWSC_OP_TEXT);
    printf("Sent identify\n");
}

// array of handler functions
const void (*handlers[]) (json_t* json) = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    handle_hello // 10
};

void handle(char* data) {
    json_t* json = json_loads(data, 0, NULL);
    
    int opcode = json_integer_value( json_object_get(json, "op") );
    json_t* payload = json_object_get(json, "d");
    
    int length = sizeof(handlers) / sizeof(handlers[0]);
    if (opcode < 0 || opcode >= length) {
        printf("ERROR: Unrecognised opcode: %i\n", opcode);
        return;
    }
    void (*handler) () = handlers[opcode];
    if (handler == NULL) {
        printf("ERROR: Unrecognised opcode: %i\n", opcode);
        return;
    }

    handler(payload);
}

struct uwsc_client* gateway_open_connection(char* url) {
    const int PING_INTERVAL = 10; // seconds
    struct uwsc_client* client;
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
                       void*   data,
                       size_t length,
                       bool   binary) {
    printf("onmessage:\n");
    printf("binary: %i\n", binary);
    if (binary) {
        // ...
    } else {
        printf("string: %s\n", data);
        handle(data);
    }
}   

void gateway_init_loop(struct uwsc_client* client) {
    context.client = client;
    
    client->onopen    = gateway_onopen;
    client->onmessage = gateway_onmessage;
    
    // ??
    struct ev_signal signal_watcher;
    ev_signal_init(&signal_watcher, NULL, SIGINT);
    ev_signal_start(EV_LOOP, &signal_watcher);

    ev_run(EV_LOOP, 0);
}
