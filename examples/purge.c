#include <babble/babble.h>

void on_message(message_t* message) {
    if (! str_startswith(message.content, "b!purge")) {
        return;
    }

    // By default, objects given by the library are only guaranteed to be
    // maintained in memory for the duration of the handler function's
    // execution. If the user wants to keep hold of a entity pointer, they
    // must express this:
    take_entity_ownership(message);

    char* parts[] = str_split(message.content, " ");
    int delete_count = str_to_int(parts[1]);

    // Should each message be converted to a struct automatically?
    // If so, isn't this inefficient?
    // What if we just need an array of message ids? (e.g. for bulk delete)
    message_t* fetched[delete_count];
    int num_fetched =
        bbl_fetch_messages(message.channel_id, delete_count, &fetched);
    bbl_bulk_delete(message.channel_id, &fetched[0], num_fetched);

    bbl_free_message(message);
    // No need to free 'fetched' message_ts, since the library still owns them
}

int main() {
    char* token = /* ... */;
    discord_client_t* client = bbl_create_client(token);
    bbl_set_active_client(client);

    // Event thread spawned here
    if (bbl_login() != BBL_OK) {
        printf("Login failed.\n");
        exit(1);
    }

    bbl_add_listener(MESSAGE_CREATE, on_message);

    // Return this to keep the process alive
    return bbl_keep_alive();
}
