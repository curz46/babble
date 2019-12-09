#include <stdbool.h>

#include <jansson.h>

typedef struct {
    long id;
    char* username;
    char* discriminator;
    char* avatar;
    bool bot;
    bool system;
    bool mfa_enabled;
    char* locale;
    bool verified;
    char* email;
    int flags;
    int premium_type;
} User;

typedef struct {
    long id;
    char* name;
    int color;
    bool hoist;
    int position;
    int permissions;
    bool managed;
    bool mentionable;
} Role;

typedef struct {
    long id;
    char* name;
    Role* roles;
    int num_roles;
    User* user; //creator
    bool require_colons;
    bool managed;
    bool animated;
} Emoji;

typedef struct {
    int start;
    int end;
} ActivityTimestamps;

typedef struct {
    char* id;
    int* size;
    int num_size;
} ActivityParty;

typedef struct {
    char* large_image;
    char* large_text;
    char* small_image;
    char* small_text;
} ActivityAssets;

typedef struct {
    char* join;
    char* spectate;
    char* match;
} ActivitySecrets;

typedef struct {
    char* name;
    int type;
    char* url;
    int created_at;
    ActivityTimestamps* timestamps;
    char* details;
    char* state;
    Emoji* emoji;
    ActivityAssets* assets;
    ActivitySecrets* secrets;
    bool instance;
    int flags;
} Activity;

typedef struct {
    char* desktop;
    char* mobile;
    char* web;
} ClientStatus;

typedef struct {
    User* user;
    Role* roles;
    int num_roles;
    Activity* game;
    long guild_id;
    char* status;
    Activity* activities;
    int num_activities;
    ClientStatus* client_status;
    long premium_since;
    char* nick;
} Presence;

typedef struct {
    long id;
    char* type;
    int allow;
    int deny;
} Overwrite;

typedef struct {
    long id;
    int type;
    long guild_id;
    int position;
    Overwrite* permission_overwrites;
    int num_permission_overwrites;
    char* name;
    char* topic;
    bool nsfw;
    long last_message_id;
    int bitrate;
    int user_limit;
    int rate_limit_per_user;
    User* recipients;
    int num_recipients;
    char* icon;
    char* owner_id;
    long application_id;
    long parent_id;
    long last_pin_timestamp;
} Channel;

typedef struct {
    User* user;
    char* nick;
    long* roles;
    int num_roles;
    long joined_at;
    long premium_since;
    bool deaf;
    bool mute;
} Member;

typedef struct {
    long guild_id;
    long channel_id;
    long user_id;
    Member* member;
    char* session_id;
    bool deaf;
    bool mute;
    bool self_deaf;
    bool self_mute;
    bool self_stream;
    bool suppress;
} VoiceState;

typedef struct {
    char* id;      // offset=0 bytes
    char* name;   // offset=8 bytes
    char* icon;   // offset=16 bytes
    char* splash;
    bool owner;
    char* owner_id;
    int permissions;
    char* region;
    long afk_channel_id;
    long afk_timeout;
    bool embed_enabled;
    long embed_channel_id;
    int verification_level;
    int default_message_notifications;
    int explicit_content_filter;
    Role* roles;
    int num_roles;
    Emoji* emojis;
    int num_emojis;
    char** features;
    int num_features;
    int mfa_level;
    long application_id;
    bool widget_enabled;
    long widget_channel_id;
    long system_channel_id;
    long joined_at;
    bool large;
    bool unavailable;
    int member_count;
    VoiceState* voice_states;
    int num_voice_states;
    Member* members;
    int num_members;
    Channel* channels;
    int num_channels;
    Presence* presences;
    int num_presences;
    int max_presences;
    int max_members;
    char* vanity_url_code;
    char* description;
    char* banner;
    int premium_tier;
    int premium_subscription_count;
    char* preferred_locale;
} Guild;

typedef struct {
    char* code;
    Guild* guild;
    Channel* channel;
    User* target_user;
    int target_user_type;
    int approximate_presence_count;
    int approximate_member_count;
} Invite;

typedef struct {
    char* id;
    char* name;
    bool vip;
    bool optimal;
    bool deprecated;
    bool custom;
} VoiceRegion;

typedef struct {
    long id;
    int type;
    long guild_id;
    long channel_id;
    User* user;
    char* name;
    char* avatar;
    char* token;
} Webhook;

typedef struct {
    void* new_value;
    void* old_value;
    char* key;
} AuditLogChange;

typedef struct {
    char* delete_member_days;
    char* members_removed;
    long channel_id;
    long message_id;
    char* count;
    long id;
    char* type;
    char* role_name;
} AuditLogEntryInfo;

typedef struct {
    char* target_id;
    AuditLogChange* changes;
    int num_changes;
    long user_id;
    long id;
    int action_type;
} AuditLogEntry;

typedef struct {
    char* id;
    char* name;
} Account;

typedef struct {
    long id;
    char* name;
    char* type;
    bool enabled;
    bool syncing;
    long role_id;
    int expire_behavior;
    int expire_grace_period;
    User* user;
    Account* account;
    long synced_at;
} Integration;

typedef struct {
    Webhook* webhooks;
    int num_webhooks;
    User* users;
    int num_users;
    AuditLogEntry* entries;
    int num_entries;
    Integration* integrations;
    int num_integrations;
} AuditLog;

Guild* parse_guild(json_t* json);
