#ifndef BABBLE_PAYLOAD_H
#define BABBLE_PAYLOAD_H

json_t* make_heartbeat(int sequence_number);

json_t* make_identify(char* token);

json_t* wrap_payload(int opcode, json_t* payload);

#endif
