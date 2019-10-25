#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <uwsc.h>
#include <pthread.h>
#include <jansson.h>

#include "gateway.h"
#include "payload.h"

#define EV_LOOP EV_DEFAULT

const int GATEWAY_VERSION = 6;

const int OP_DISPATCH = 0; // "READY"
const int OP_HEARTBEAT = 1;
const int OP_IDENTIFY = 2;
const int OP_STATUS_UPDATE = 3;
const int OP_VOICE_STATUS_UPDATE = 4;
const int OP_RESUME = 6;
const int OP_RECONNECT = 7;
const int OP_REQUEST_GUILD_MEMBERS = 8;
const int OP_INVALID_SESSION = 9;
const int OP_HELLO = 10;
const int OP_HEARTBEAT_ACK = 11;

typedef struct {
    struct uwsc_client* client;

    int heartbeat_interval;
    
    int sequence_number;
    long last_heartbeat;

    char* session_id;
} client_context;

client_context context;

void send_heartbeat() {
    json_t* payload = make_heartbeat(context.sequence_number);
    json_t* wrapped = wrap_payload(OP_HEARTBEAT, payload);
    
    char* data      = json_dumps(wrapped, 0);
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    context.last_heartbeat = spec.tv_nsec / 1.0e6;

    context.client->send(context.client, data, strlen(data), UWSC_OP_TEXT);
    printf("Sent heartbeat\n");
}

void do_heartbeat(void* arg) {
    printf("Heartbeat starting...\n");
    while (1) {
        usleep(context.heartbeat_interval * 1000);
        send_heartbeat();
    }
}

// Is this necessarily READY? Need to double check...
void handle_dispatch(json_t* json) { // "READY"
    int version = json_integer_value( json_object_get(json, "v") );
    if (version != GATEWAY_VERSION) {
        printf("ERROR: Unsupported gateway version (%i)", version);
        exit(1);
    }

    json_t* user = json_object_get(json, "user");
    json_t* private_channels = json_object_get(json, "private_channels");
    json_t* guilds = json_object_get(json, "guilds");
    char* session_id = json_string_value( json_object_get(json, "session_id") );
    json_t* shard = json_object_get(json, "shard");

    context.session_id = session_id;

    printf("Received READY event with session id %s\n", session_id);
}

void handle_heartbeat(json_t* json) {
    send_heartbeat();
}

void handle_hello(json_t* json) {
    int interval = json_integer_value( json_object_get(json, "heartbeat_interval") );

    context.heartbeat_interval = interval;
    context.sequence_number    = 0;

    send_heartbeat();
    pthread_t *thread;
    pthread_create(&thread, NULL, do_heartbeat, NULL);

    char* token = getenv("TOKEN");
    printf("Identifying with token: %s\n", token);
    json_t* identify_payload = make_identify( getenv("TOKEN") );
    json_t* wrapped = wrap_payload(OP_IDENTIFY, identify_payload);
    char* data      = json_dumps(wrapped, 0);
    context.client->send(context.client, data, strlen(data), UWSC_OP_TEXT);
    printf("Sent identify\n");
}

void handle_heartbeat_ack(json_t* json) {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long now = round(spec.tv_nsec / 1.0e6);
    printf("Received HEARTBEAT_ACK: %ldms\n", now - context.last_heartbeat);
}

// array of handler functions
const void (*handlers[]) (json_t* json) = {
    handle_dispatch,  // 0
    handle_heartbeat, // 1
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    handle_hello, // 10,
    handle_heartbeat_ack // 11
};

void handle(char* data) {
    json_t* json = json_loads(data, 0, NULL);
    
    int opcode = json_integer_value( json_object_get(json, "op") );
    int sequence_number = json_integer_value( json_object_get("json", "s") );
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

    context.sequence_number = sequence_number;
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
