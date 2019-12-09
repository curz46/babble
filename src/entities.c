#include <stdbool.h>

#include <jansson.h>

#include "entities.h"

typedef enum {
    // Primitives
    BOOLEAN,
    INT,
    LONG,
    STRING,
    // Primitive Arrays,
    BOOLEAN_ARRAY,
    INT_ARRAY,
    LONG_ARRAY,
    STRING_ARRAY,
    // Objects
    USER,
    ROLE,
    EMOJI,
    ACTIVITY_TIMESTAMPS,
    ACTIVITY_PARTY,
    ACTIVITY_ASSETS,
    ACTIVITY_SECRETS,
    ACTIVITY,
    CLIENT_STATUS,
    PRESENCE,
    OVERWRITE,
    CHANNEL,
    MEMBER,
    VOICE_STATE,
    GUILD,
    INVITE,
    VOICE_REGION,
    WEBHOOK,
    AUDIT_LOG_CHANGE,
    AUDIT_LOG_ENTRY_INFO,
    AUDIT_LOG_ENTRY,
    ACCOUNT,
    INTEGRATION,
    AUDIT_LOG,
    // Object Arrays
    USER_ARRAY,
    ROLE_ARRAY,
    EMOJI_ARRAY,
    ACTIVITY_TIMESTAMPS_ARRAY,
    ACTIVITY_PARTY_ARRAY,
    ACTIVITY_ASSETS_ARRAY,
    ACTIVITY_SECRETS_ARRAY,
    ACTIVITY_ARRAY,
    CLIENT_STATUS_ARRAY,
    PRESENCE_ARRAY,
    OVERWRITE_ARRAY,
    CHANNEL_ARRAY,
    MEMBER_ARRAY,
    VOICE_STATE_ARRAY,
    GUILD_ARRAY,
    INVITE_ARRAY,
    VOICE_REGION_ARRAY,
    WEBHOOK_ARRAY,
    AUDIT_LOG_CHANGE_ARRAY,
    AUDIT_LOG_ENTRY_INFO_ARRAY,
    AUDIT_LOG_ENTRY_ARRAY,
    ACCOUNT_ARRAY,
    INTEGRATION_ARRAY,
    AUDIT_LOG_ARRAY
} AttributeType;

typedef struct {
    AttributeType type;
    char* name;
} Attribute;

int type_size(AttributeType type) {
    switch (type) {
        // Primitives
        case BOOLEAN: return sizeof(bool);
        case INT:     return sizeof(int);
        case LONG:    return sizeof(long);
        case STRING:  return sizeof(char*);
        // Primitive Arrays
        case BOOLEAN_ARRAY: return sizeof(void*) + sizeof(int);
        case INT_ARRAY:     return sizeof(void*) + sizeof(int);
        case LONG_ARRAY:    return sizeof(void*) + sizeof(int);
        case STRING_ARRAY:  return sizeof(void*) + sizeof(int);
        // Objects
        case USER:
        case ROLE:
        case EMOJI:
        case ACTIVITY_TIMESTAMPS:
        case ACTIVITY_PARTY:
        case ACTIVITY_ASSETS:
        case ACTIVITY_SECRETS:
        case ACTIVITY:
        case CLIENT_STATUS:
        case PRESENCE:
        case OVERWRITE:
        case CHANNEL:
        case MEMBER:
        case VOICE_STATE:
        case GUILD:
        case INVITE:
        case VOICE_REGION:
        case WEBHOOK:
        case AUDIT_LOG_CHANGE:
        case AUDIT_LOG_ENTRY_INFO:
        case AUDIT_LOG_ENTRY:
        case ACCOUNT:
        case INTEGRATION:
        case AUDIT_LOG:
            return sizeof(void*);
        // Arrays
        case USER_ARRAY:
        case ROLE_ARRAY:
        case EMOJI_ARRAY:
        case ACTIVITY_TIMESTAMPS_ARRAY:
        case ACTIVITY_PARTY_ARRAY:
        case ACTIVITY_ASSETS_ARRAY:
        case ACTIVITY_SECRETS_ARRAY:
        case ACTIVITY_ARRAY:
        case CLIENT_STATUS_ARRAY:
        case PRESENCE_ARRAY:
        case OVERWRITE_ARRAY:
        case CHANNEL_ARRAY:
        case MEMBER_ARRAY:
        case VOICE_STATE_ARRAY:
        case GUILD_ARRAY:
        case INVITE_ARRAY:
        case VOICE_REGION_ARRAY:
        case WEBHOOK_ARRAY:
        case AUDIT_LOG_CHANGE_ARRAY:
        case AUDIT_LOG_ENTRY_INFO_ARRAY:
        case AUDIT_LOG_ENTRY_ARRAY:
        case ACCOUNT_ARRAY:
        case INTEGRATION_ARRAY:
        case AUDIT_LOG_ARRAY:
            // pointer field + num_ field
            return sizeof(void*) + sizeof(int);
    }
    return -1;
}

#define HANDLE(TYPE, C_TYPE, JSON_TYPE)                    \
    TYPE: ; {                                              \
        C_TYPE value = json_##JSON_TYPE##_value(property); \
        *((C_TYPE*) field) = value; }                      \
        break;

#define HANDLE_ARRAY(TYPE, C_TYPE, JSON_TYPE)                      \
    TYPE: ; {                                                      \
        int length = json_array_size(property);                    \
        C_TYPE* array = (C_TYPE*) malloc(sizeof(C_TYPE) * length); \
        for (int i = 0; i < length; i++) {                         \
            json_t* element = json_array_get(property, i);         \
            C_TYPE value = json_##JSON_TYPE##_value(element);      \
            array[i] = value;                                      \
        }                                                          \
        *((C_TYPE*) field) = array;                                \
        *((C_TYPE*) (field + sizeof(void*))) = length; }           \
        break;

void read_field(json_t* json, AttributeType type, char* name, void* field) {
    json_t* property = json_object_get(json, name);
    switch (type) {
        //HANDLE(BOOLEAN, bool, boolean)
        case BOOLEAN: ; {
            bool value = json_boolean_value(property);
            *((bool*) field) = value;
            printf("Boolean: %i\n", value);
        }
        HANDLE(INT, int, integer)
        HANDLE(LONG, long, long)
        case STRING: ; {
            if (json_is_null(property) || !json_is_string(property)) break;
            char* value = json_string_value(property);
            *((char**) field) = value;
            break;
        }
    }
}

Attribute GUILD_FIELDS[] = {
    {STRING, "id"},
    {STRING, "name"},
    {STRING, "icon"},
    {STRING, "splash"},
    {BOOLEAN, "owner"},
    {STRING, "owner_id"},
    {INT, "permissions"},
    {STRING, "region"},
    {LONG, "afk_channel_id"},
    {LONG, "afk_timeout"},
    {BOOLEAN, "embed_enabled"},
    {BOOLEAN, "embed_channel_id"},
    {INT, "verification_level"},
    {INT, "default_message_notifications"},
    {INT, "explicit_content_filter"},
    {ROLE_ARRAY, "roles"},
    {EMOJI_ARRAY, "emojis"},
    {STRING_ARRAY, "features"},
    {INT, "mfa_level"},
    {LONG, "application_id"},
    {BOOLEAN, "widget_enabled"},
    {LONG, "widget_channel_id"},
    {LONG, "system_channel_id"},
    {LONG, "joined_at"},
    {BOOLEAN, "large"},
    {BOOLEAN, "unavailable"},
    {INT, "member_count"},
    {VOICE_STATE_ARRAY, "voice_states"},
    {MEMBER_ARRAY, "members"},
    {CHANNEL_ARRAY, "channels"},
    {PRESENCE_ARRAY, "presences"},
    {INT, "max_presences"},
    {INT, "max_members"},
    {STRING, "vanity_url_code"},
    {STRING, "description"},
    {STRING, "banner"},
    {INT, "premium_tier"},
    {INT, "premium_subscription_count"},
    {STRING, "preferrred_locale"}
};

Guild* parse_guild(json_t* json) {
    Guild* guild = (Guild*) malloc(sizeof(Guild));

    int num_fields = sizeof(GUILD_FIELDS) / sizeof(GUILD_FIELDS[0]);
    char* root      = (char*) guild;
    int offset     = 0;
    for (int i = 0; i < num_fields; i++) {
        Attribute attr = GUILD_FIELDS[i];
        void* field = (void*) (root + offset);
        read_field(json, attr.type, attr.name, field);
        offset += type_size(attr.type);
    }

    return guild;
}
