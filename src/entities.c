#include <stdbool.h>

#include <jansson.h>

#include "entities.h"

/**
 * PARSE expands to a block of code that:
 * 1. Gets the value of some property NAME of json_t* Object variable "json"
 * 2. Assigns the value for struct "object" on member NAME.
 */
#define PARSE(C_TYPE, JSON_TYPE, NAME) { \
    json_t* property = json_object_get(json, #NAME); \
    C_TYPE  value    = json_##JSON_TYPE##_value(property); \
    object.NAME      = value; }

/**
 * PARSE_OBJECT is similar to PARSE, but it allows some arbitrary parse
 * function to interpret the json_t* field. The parse function should be in
 * form parse_{name}, where {name} should be given by the argument PARSER.
 * The name given is not the full function name such that the function
 * arguments can also be utilised by some composition function.
 */
#define PARSE_OBJECT(C_TYPE, NAME, PARSER) { \
    json_t* property = json_object_get(json, #NAME); \
    C_TYPE  value    = parse_##PARSER(property); \
    object.NAME      = value; }

/**
 * PARSE_OBJECT_ARRAY is similar to PARSE_OBJECT, but rather than expecting
 * a singular Object it expects an array of Objects.
 */
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

/**
 * The reverse of PARSE. It should:
 * 1. Extract the value of a member from some struct "object".
 * 2. Use json_object_set to assign the value to a property of a json_t*
 *    object "json".
 */
#define COMPOSE(C_TYPE, JSON_TYPE, NAME) { \
    C_TYPE value    = object.NAME; \
    json_t* wrapped = json_##JSON_TYPE(value); \
    json_object_set(json, #NAME, property); } \

/**
 * The reverse of PARSE_OBJECT. It should:
 * 1. Extract the Object value of a member from some struct "object".
 * 2. Transform the Object value into a json_t* object.
 * 3. Use json_object_set to assign the transformed value to a property of
 *    json_t* object "json".
 */
#define COMPOSE_OBJECT(C_TYPE, NAME, COMPOSER) { \
    C_TYPE value        = object.NAME; \
    json_t* transformed = compose_##COMPOSER(value); \
    json_object_set(json, #NAME, transformed); }

/**
 * The reverse of PARSE_OBJECT_ARRAY. It should:
 * 1. Loop through the Object array on struct "object" and generate a json_t*
 *    array.
 * 2. For each member, use compose_#COMPOSER to create a json_t* value.
 * 2. Use json_object_set to assign the Object array to json_t* object "json".
 */
#define COMPOSE_OBJECT_ARRAY(C_TYPE, NAME, COMPOSER) { \
    json_t* array = json_array(); \
    int length = object.num_#NAME; \
    for (int i = 0; i < length; i++) { \
        C_TYPE value        = object.NAME[i]; \
        json_t* transformed = compose_#COMPOSER(value); \
        json_array_append_new(array, transformed); \
    } \
    json_object_set(json, #NAME, array); }
    
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

ChannelMention parse_channel_mention(json_t* json) {
    ChannelMention object;
    // TODO
    return object;
}

MessageActivity parse_message_activity(json_t* json) {
    MessageActivity object;
    // TODO
    return object;
}

MessageApplication parse_message_application(json_t* json) {
    MessageApplication object;
    // TODO
    return object;
}

MessageReference parse_message_reference(json_t* json) {
    MessageReference object;
    // TODO
    return object;
}

Embed parse_embed(json_t* json) {
    Embed object;
    // TODO
    return object;
}

Attachment parse_attachment(json_t* json) {
    Attachment object;
    // TODO
    return object;
}

Reaction parse_reaction(json_t* json) {
    Reaction object;
    // TODO
    return object;
}

#define MESSAGE_FIELDS \
    X(char*, string, id) \
    X(char*, string, channel_id) \
    X(char*, string, guild_id) \
    Z(User, author, user) \
    Z(Member, member, member) \
    X(char*, string, content) \
    X(long, integer, timestamp) \
    X(long, integer, edited_timestamp) \
    X(bool, boolean, tts) \
    X(bool, boolean, mention_everyone) \
    Y(User, mentions, user) \
    Y(Role, mention_roles, role) \
    Y(ChannelMention, mention_channels, channel_mention) \
    Y(Attachment, attachments, attachment) \
    Y(Embed, embeds, embed) \
    Y(Reaction, reactions, reaction) \
    X(int, integer, nonce) \
    X(bool, boolean, pinned) \
    X(char*, string, webhook_id) \
    X(int, integer, type) \
    Z(MessageActivity, activity, message_activity) \
    Z(MessageApplication, application, message_application) \
    Z(MessageReference, message_reference, message_reference) \
    X(int, integer, flags)

Message parse_message(json_t* json) {
    Message object;
    #define X(C_TYPE, JSON_TYPE, NAME) PARSE(C_TYPE, JSON_TYPE, NAME)
    #define Y(C_TYPE, NAME, PARSER) PARSE_OBJECT_ARRAY(C_TYPE, NAME, PARSER)
    #define Z(C_TYPE, NAME, PARSER) PARSE_OBJECT(C_TYPE, NAME, PARSER)
    MESSAGE_FIELDS
    #undef X
    #undef Y
    #undef Z
    return object;
}


