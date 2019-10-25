#include <jansson.h>

#include "payload.h"

json_t* make_heartbeat(int sequence_number) {
    return json_integer(sequence_number);
}

json_t* make_identify(char* token) {
    json_t* payload = json_object();
    json_object_set(payload, "token", json_string(token));

    json_t* properties = json_object();
    json_object_set(properties, "$os", json_string("linux"));
    json_object_set(properties, "$browser", json_string("babble"));
    json_object_set(properties, "$device", json_string("babble"));

    json_object_set(payload, "properties", properties);
    return payload;
}

json_t* wrap_payload(int opcode, json_t* payload) {
    json_t* data = json_object();
    json_object_set(data, "op", json_integer(opcode));
    json_object_set(data, "d", payload);
    return data;   
}
