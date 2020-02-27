#ifndef BABBLE_ROUTES_H
#define BABBLE_ROUTES_H

#include "error.h"
#include "entities.h"

bbl_error_t bbl_create_message(char* channel_id, message_t* message, message_t** created);

bbl_error_t bbl_edit_message(message_t* message, message_t** edited);

#endif
