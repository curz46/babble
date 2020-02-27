#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <uwsc.h>
#include <pthread.h>
#include <jansson.h>

#include "babble/err.h"
#include "babble/entities.h"
#include "babble/routes.h"

#include "gateway.h"
#include "payload.h"
#include "http.h"
#include "json.h"

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

void handle_ready(json_t* json) {
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

void handle_guild_create(json_t* json) {
    guild_t guild = parse_guild(json);
    for (int i = 0; i < guild.num_members; i++) {
        member_t member = guild.members[i];
        if (member.user.bot)
            printf("member_t: %s\n", member.user.username);
    }
}

void handle_message_create(json_t* json) {
    message_t message = parse_message(json);
    printf("Received message:\n");
    printf("Content=%s\n", message.content);
    printf("Author=%s\n", message.author.username);
    printf("Channel=%s\n", message.channel_id);

    if (strcmp(message.content, "bb!test") == 0) {
        message_t new_message = {0};
        new_message.channel_id = message.channel_id;
        new_message.content = "Hello, world!";
        message_t created_message = {0};
        printf("Responding...\n");
        int result = bbl_create_message(new_message, &created_message);

        if (result != ERR_OK) {
            printf("Failed to create message: result=%i\n", result);
            return;
        }

        printf("Created message:\n");
        printf("Content=%s\n", created_message.content);
        printf("Author=%s\n", created_message.author.username);
        printf("Channel=%s\n", created_message.channel_id);
    } else if (strcmp(message.content, "bb!edit") == 0) {
        int result;

        message_t new_message = {0};
        new_message.channel_id = message.channel_id;
        new_message.content = "Original content";
        message_t created = {0};
        result = bbl_create_message(new_message, &created);

        if (result != ERR_OK) {
            printf("Failed to create message.\n");
            return;
        }

        new_message.id = created.id;
        new_message.content = "Edited content";
        result = bbl_edit_message(new_message, NULL);

        if (result != ERR_OK) {
            printf("Failed to edit message.\n");
        }

        // TODO: new_message & created must have all their props freed
    }
}

// Is this necessarily READY? Need to double check...
void handle_dispatch(char* event, json_t* json) { // "READY"
    if (event == NULL) {
        printf("ERROR: DISPATCH event == null\n");
        return;
    }

    if (strcmp(event, "READY") == 0) {
        handle_ready(json);
    } else if (strcmp(event, "GUILD_CREATE") == 0) {
        handle_guild_create(json);
    } else if (strcmp(event, "MESSAGE_CREATE") == 0) { 
        handle_message_create(json);
    } else {
        printf("ERROR: Unrecognised DISPATCH event \"%s\"\n", event);
    }
}

void handle_heartbeat(char* event, json_t* json) {
    send_heartbeat();
}

void handle_hello(char* event, json_t* json) {
    int interval = json_integer_value( json_object_get(json, "heartbeat_interval") );

    context.heartbeat_interval = interval;
    context.sequence_number    = 0;

    send_heartbeat();
    pthread_t *thread;
    pthread_create(&thread, NULL, do_heartbeat, NULL);

    char* token = getenv("TOKEN");
    printf("Identifying with token: %s\n", token);
    json_t* identify_payload = make_identify(token);
    json_t* wrapped = wrap_payload(OP_IDENTIFY, identify_payload);
    char* data      = json_dumps(wrapped, 0);
    context.client->send(context.client, data, strlen(data), UWSC_OP_TEXT);
    printf("Sent identify\n");
}

void handle_heartbeat_ack(char* event, json_t* json) {
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
    char* event = json_string_value( json_object_get(json, "t") );
    
    // TODO: It appears that whenever this happens the JSON parser has failed.
    if (opcode == OP_DISPATCH && event == NULL) {
        //printf("event == null data:\n %s\n", data);
        printf("JSON parsing failed: \n");
        printf("json == null: %s\n", json == NULL ? "true" : "false");
        printf("data: \n%s\n", data);
        return;
    }

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
    handler(event, payload);
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

void gateway_onopen(struct uwsc_client* client) {}

void gateway_onmessage(struct uwsc_client* client,
                       void*   data,
                       size_t length,
                       bool   binary) {
    if (! binary) {
        char* content   = (char*) malloc(sizeof(char) * length + 1);
        memcpy(content, data, length);
        content[length] = '\0';
        handle(content);
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
