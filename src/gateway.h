#pragma once

#include <uwsc.h>

extern const int OP_DISPATCH;
extern const int OP_HEARTBEAT;
extern const int OP_IDENTIFY;
extern const int OP_STATUS_UPDATE;
extern const int OP_VOICE_STATUS_UPDATE;
extern const int OP_RESUME;
extern const int OP_RECONNECT;
extern const int OP_REQUEST_GUILD_MEMBERS;
extern const int OP_INVALID_SESSION;
extern const int OP_HELLO;
extern const int OP_HEARTBEAT_ACK;

struct client_context;

struct uwsc_client* open_connection(char* url);
