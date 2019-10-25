#include <jansson.h>

#include "payload.h"

json_t* make_heartbeat(int sequence_number) {
    return json_integer(sequence_number);
}

json_t* wrap_payload(int opcode, json_t* payload) {
    json_t* data = json_object();
    json_object_set(data, "op", json_integer(opcode));
    json_object_set(data, "d", payload);
    return data;   
}
