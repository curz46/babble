#include <stdbool.h>

#include <jansson.h>

#include "entities.h"

#define PARSE(C_TYPE, JSON_TYPE, NAME) { \
    json_t* property = json_object_get(json, #NAME); \
    C_TYPE  value    = json_##JSON_TYPE##_value(property); \
    object.NAME      = value; }

#define PARSE_OBJECT(C_TYPE, NAME, PARSER) { \
    json_t* property = json_object_get(json, #NAME); \
    C_TYPE  value    = parse_##PARSER(property); \
    object.NAME      = value; }

#define PARSE_OBJECT_ARRAY(C_TYPE, NAME, PARSER) { \
    json_t* json_array = json_object_get(json, #NAME); \
    int length         = json_array_size(json_array); \
    C_TYPE* array      = (C_TYPE*) malloc(sizeof(C_TYPE) * length); \
    int index; \
    json_t* value; \
    json_array_foreach(json_array, index, value) { \
        array[index] = parse_##PARSER(value); \
    } \
    object.NAME = array; \
    object.num_##NAME = length; }

#define OVERWRITE_FIELDS \
    X(char*, string, id) \
    X(char*, string, type) \
    X(int, integer, allow) \
    X(int, integer, deny)

Overwrite parse_overwrite(json_t* json) {
    Overwrite object;
    #define X(C_TYPE, JSON_TYPE, NAME) PARSE(C_TYPE, JSON_TYPE, NAME)
    #define Y(C_TYPE, NAME, PARSER) PARSE_OBJECT_ARRAY(C_TYPE, NAME, PARSER)
    #define Z(C_TYPE, NAME, PARSER) PARSE_OBJECT(C_TYPE, NAME, PARSER)
    OVERWRITE_FIELDS
    #undef X
    #undef Y
    #undef Z
    return object;
}

#define USER_FIELDS \
    X(char*, string, id) \
    X(char*, string, username) \
    X(char*, string, discriminator) \
    X(char*, string, avatar) \
    X(bool, boolean, bot) \
    X(bool, boolean, system) \
    X(bool, boolean, mfa_enabled) \
    X(char*, string, locale) \
    X(bool, boolean, verified) \
    X(char*, string, email) \
    X(int, integer, flags) \
    X(int, integer, premium_type)

User parse_user(json_t* json) {
    User object;
    #define X(C_TYPE, JSON_TYPE, NAME) PARSE(C_TYPE, JSON_TYPE, NAME)
    #define Y(C_TYPE, NAME, PARSER) PARSE_OBJECT_ARRAY(C_TYPE, NAME, PARSER)
    #define Z(C_TYPE, NAME, PARSER) PARSE_OBJECT(C_TYPE, NAME, PARSER)
    USER_FIELDS
    #undef X
    #undef Y
    #undef Z
    return object;
}

// TODO: cba rn
Activity parse_activity(json_t* json) {
    Activity object;
    return object;
}

#define CLIENT_STATUS_FIELDS \
    X(char*, string, desktop) \
    X(char*, string, mobile) \
    X(char*, string, web)

ClientStatus parse_client_status(json_t* json) {
    ClientStatus object;
    #define X(C_TYPE, JSON_TYPE, NAME) PARSE(C_TYPE, JSON_TYPE, NAME)
    #define Y(C_TYPE, NAME, PARSER) PARSE_OBJECT_ARRAY(C_TYPE, NAME, PARSER)
    #define Z(C_TYPE, NAME, PARSER) PARSE_OBJECT(C_TYPE, NAME, PARSER)
    CLIENT_STATUS_FIELDS
    #undef X
    #undef Y
    #undef Z
    return object;
}

#define ROLE_FIELDS \
    X(char*, string, id) \
    X(char*, string, name) \
    X(int, integer, color) \
    X(bool, boolean, hoist) \
    X(int, integer, position) \
    X(int, integer, permissions) \
    X(bool, boolean, managed) \
    X(bool, boolean, mentionable)
 
Role parse_role(json_t* json) {
    Role object;
    #define X(C_TYPE, JSON_TYPE, NAME) PARSE(C_TYPE, JSON_TYPE, NAME)
    #define Y(C_TYPE, NAME, PARSER) PARSE_OBJECT_ARRAY(C_TYPE, NAME, PARSER)
    #define Z(C_TYPE, NAME, PARSER) PARSE_OBJECT(C_TYPE, NAME, PARSER)
    ROLE_FIELDS
    #undef X
    #undef Y
    #undef Z
    return object;
}

#define EMOJI_FIELDS \
    X(char*, string, id) \
    X(char*, string, name) \
    Y(Role, roles, role) \
    Z(User, user, user) \
    X(bool, boolean, require_colons) \
    X(bool, boolean, managed) \
    X(bool, boolean, animated)

Emoji parse_emoji(json_t* json) {
    Emoji object;
    #define X(C_TYPE, JSON_TYPE, NAME) PARSE(C_TYPE, JSON_TYPE, NAME)
    #define Y(C_TYPE, NAME, PARSER) PARSE_OBJECT_ARRAY(C_TYPE, NAME, PARSER)
    #define Z(C_TYPE, NAME, PARSER) PARSE_OBJECT(C_TYPE, NAME, PARSER)
    EMOJI_FIELDS
    #undef X
    #undef Y
    #undef Z
    return object;
}

char* parse_string(json_t* json) {
    return json_string_value(json);
}

#define MEMBER_FIELDS \
    Z(User, user, user) \
    X(char*, string, nick) \
    Y(char*, roles, string) \
    X(int, integer, joined_at) \
    X(int, integer, premium_since) \
    X(bool, boolean, deaf) \
    X(bool, boolean, mute)

Member parse_member(json_t* json) {
    Member object;
    #define X(C_TYPE, JSON_TYPE, NAME) PARSE(C_TYPE, JSON_TYPE, NAME)
    #define Y(C_TYPE, NAME, PARSER) PARSE_OBJECT_ARRAY(C_TYPE, NAME, PARSER)
    #define Z(C_TYPE, NAME, PARSER) PARSE_OBJECT(C_TYPE, NAME, PARSER)
    MEMBER_FIELDS
    #undef X
    #undef Y
    #undef Z
    return object;
}

#define CHANNEL_FIELDS \
    X(char*, string, id) \
    X(int, integer, type) \
    X(char*, string, guild_id) \
    X(int, integer, position) \
    Y(Overwrite, permission_overwrites, overwrite) \
    X(char*, string, name) \
    X(char*, string, topic) \
    X(bool, boolean, nsfw) \
    X(char*, string, last_message_id) \
    X(int, integer, bitrate) \
    X(int, integer, user_limit) \
    X(int, integer, rate_limit_per_user) \
    Y(User, recipients, user) \
    X(char*, string, icon) \
    X(char*, string, owner_id) \
    X(char*, string, application_id) \
    X(char*, string, parent_id) \
    X(int, integer, last_pin_timestamp)

Channel parse_channel(json_t* json) {
    Channel object;
    #define X(C_TYPE, JSON_TYPE, NAME) PARSE(C_TYPE, JSON_TYPE, NAME)
    #define Y(C_TYPE, NAME, PARSER) PARSE_OBJECT_ARRAY(C_TYPE, NAME, PARSER)
    #define Z(C_TYPE, NAME, PARSER) PARSE_OBJECT(C_TYPE, NAME, PARSER)
    CHANNEL_FIELDS
    #undef X
    #undef Y
    #undef Z
    return object;
}

#define PRESENCE_FIELDS \
    Z(User, user, user) \
    Y(Role, roles, role) \
    Z(Activity, game, activity) \
    X(char*, string, guild_id) \
    X(char*, string, status) \
    Y(Activity, activities, activity) \
    Z(ClientStatus, client_status, client_status) \
    X(int, integer, premium_since) \
    X(char*, string, nick)

Presence parse_presence(json_t* json) {
    Presence object;
    #define X(C_TYPE, JSON_TYPE, NAME) PARSE(C_TYPE, JSON_TYPE, NAME)
    #define Y(C_TYPE, NAME, PARSER) PARSE_OBJECT_ARRAY(C_TYPE, NAME, PARSER)
    #define Z(C_TYPE, NAME, PARSER) PARSE_OBJECT(C_TYPE, NAME, PARSER)
    PRESENCE_FIELDS
    #undef X
    #undef Y
    #undef Z
    return object;
}

#define GUILD_FIELDS \
    X(char*, string, id) \
    X(char*, string, name) \
    X(char*, string, icon) \
    X(char*, string, splash) \
    X(bool, boolean, owner) \
    X(char*, string, owner_id) \
    X(int, integer, permissions) \
    X(char*, string, region) \
    X(int, integer, afk_channel_id) \
    X(int, integer, afk_timeout) \
    X(bool, boolean, embed_enabled) \
    X(bool, boolean, embed_channel_id) \
    X(int, integer, verification_level) \
    X(int, integer, default_message_notifications) \
    X(int, integer, explicit_content_filter) \
    Y(Role, roles, role) \
    Y(Emoji, emojis, emoji) \
    Y(char*, features, string) \
    X(int, integer, mfa_level) \
    X(int, integer, application_id) \
    X(bool, boolean, widget_enabled) \
    X(int, integer, widget_channel_id) \
    X(int, integer, system_channel_id) \
    X(int, integer, joined_at) \
    X(bool, boolean, large) \
    X(bool, boolean, unavailable) \
    Y(Member, members, member) \
    Y(Channel, channels, channel) \
    Y(Presence, presences, presence) \
    X(int, integer, member_count) \
    X(int, integer, max_presences) \
    X(int, integer, max_members) \
    X(char*, string, vanity_url_code) \
    X(char*, string, description) \
    X(char*, string, banner) \
    X(int, integer, premium_tier) \
    X(int, integer, premium_subscription_count) \
    X(char*, string, preferred_locale)

Guild parse_guild(json_t* json) {
    Guild object;
    #define X(C_TYPE, JSON_TYPE, NAME) PARSE(C_TYPE, JSON_TYPE, NAME)
    #define Y(C_TYPE, NAME, PARSER) PARSE_OBJECT_ARRAY(C_TYPE, NAME, PARSER)
    #define Z(C_TYPE, NAME, PARSER) PARSE_OBJECT(C_TYPE, NAME, PARSER)
    GUILD_FIELDS
    #undef X
    #undef Y
    #undef Z
    return object;
}
