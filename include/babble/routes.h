#ifndef BABBLE_ROUTES
#define BABBLE_ROUTES

#include "error.h"
#include "entities.h"

bbl_error_t bbl_create_message(message_t message, message_t* created);

bbl_error_t bbl_edit_message(message_t message, message_t* edited);

#endif
