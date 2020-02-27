#ifndef BABBLE_ENTITIES
#define BABBLE_ENTITIES

#include <stdbool.h>
#include <jansson.h>

typedef struct user_t {
    char* id;
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
} user_t;

typedef struct role_t {
    char* id;
    char* name;
    int color;
    bool hoist;
    int position;
    int permissions;
    bool managed;
    bool mentionable;
} role_t;

typedef struct emoji_t {
    char* id;
    char* name;
    role_t* roles;
    int num_roles;
    user_t user; //creator
    bool require_colons;
    bool managed;
    bool animated;
} emoji_t;

typedef struct activity_timestamps_t {
    int start;
    int end;
} activity_timestamps_t;

typedef struct activity_party_t {
    char* id;
    int* size;
    int num_size;
} activity_party_t;

typedef struct activity_assets_t {
    char* large_image;
    char* large_text;
    char* small_image;
    char* small_text;
} activity_assets_t;

typedef struct activity_secrets_t {
    char* join;
    char* spectate;
    char* match;
} activity_secrets_t;

typedef struct activity_t {
    char* name;
    int type;
    char* url;
    int created_at;
    activity_timestamps_t* timestamps;
    char* details;
    char* state;
    emoji_t emoji;
    activity_assets_t* assets;
    activity_secrets_t* secrets;
    bool instance;
    int flags;
} activity_t;

typedef struct client_status_t {
    char* desktop;
    char* mobile;
    char* web;
} client_status_t;

typedef struct presence_t {
    user_t user;
    role_t* roles;
    int num_roles;
    activity_t game;
    char* guild_id;
    char* status;
    activity_t* activities;
    int num_activities;
    client_status_t client_status;
    int premium_since;
    char* nick;
} presence_t;

typedef struct overwrite_t {
    char* id;
    char* type;
    int allow;
    int deny;
} overwrite_t;

typedef struct channel_t {
    char* id;
    int type;
    char* guild_id;
    int position;
    overwrite_t* permission_overwrites;
    int num_permission_overwrites;
    char* name;
    char* topic;
    bool nsfw;
    char* last_message_id;
    int bitrate;
    int user_limit;
    int rate_limit_per_user;
    user_t* recipients;
    int num_recipients;
    char* icon;
    char* owner_id;
    char* application_id;
    char* parent_id;
    int last_pin_timestamp;
} channel_t;

typedef struct member_t {
    user_t user;
    char* nick;
    char** roles;
    int num_roles;
    int joined_at;
    int premium_since;
    bool deaf;
    bool mute;
} member_t;

typedef struct voice_state_t {
    char* guild_id;
    char* channel_id;
    char* user_id;
    member_t* member;
    char* session_id;
    bool deaf;
    bool mute;
    bool self_deaf;
    bool self_mute;
    bool self_stream;
    bool suppress;
} voice_state_t;

typedef struct guild_t {
    char* id;
    char* name;
    char* icon;
    char* splash;
    bool owner;
    char* owner_id;
    int permissions;
    char* region;
    char* afk_channel_id;
    int afk_timeout;
    bool embed_enabled;
    char* embed_channel_id;
    int verification_level;
    int default_message_notifications;
    int explicit_content_filter;
    role_t* roles;
    int num_roles;
    emoji_t* emojis;
    int num_emojis;
    char** features;
    int num_features;
    int mfa_level;
    char* application_id;
    bool widget_enabled;
    char* widget_channel_id;
    char* system_channel_id;
    int joined_at;
    bool large;
    bool unavailable;
    int member_count;
    voice_state_t* voice_states;
    int num_voice_states;
    member_t* members;
    int num_members;
    channel_t* channels;
    int num_channels;
    presence_t* presences;
    int num_presences;
    int max_presences;
    int max_members;
    char* vanity_url_code;
    char* description;
    char* banner;
    int premium_tier;
    int premium_subscription_count;
    char* preferred_locale;
} guild_t;

typedef struct invite_t {
    char* code;
    guild_t* guild;
    channel_t* channel;
    user_t target_user;
    int target_user_type;
    int approximate_presence_count;
    int approximate_member_count;
} invite_t;

typedef struct voice_region_t {
    char* id;
    char* name;
    bool vip;
    bool optimal;
    bool deprecated;
    bool custom;
} voice_region_t;

typedef struct webhook_t {
    char* id;
    int type;
    char* guild_id;
    char* channel_id;
    user_t user;
    char* name;
    char* avatar;
    char* token;
} webhook_t;

typedef struct audit_log_change_t {
    void* new_value;
    void* old_value;
    char* key;
} audit_log_change_t;

typedef struct audit_log_entry_info_t {
    char* delete_member_days;
    char* members_removed;
    char* channel_id;
    char* message_id;
    char* count;
    char* id;
    char* type;
    char* role_name;
} audit_log_entry_info_t;

typedef struct audit_log_entry_t {
    char* target_id;
    audit_log_change_t* changes;
    int num_changes;
    char* user_id;
    char* id;
    int action_type;
} audit_log_entry_t;

typedef struct account_t {
    char* id;
    char* name;
} account_t;

typedef struct integration_t {
    char* id;
    char* name;
    char* type;
    bool enabled;
    bool syncing;
    char* role_id;
    int expire_behavior;
    int expire_grace_period;
    user_t user;
    account_t account;
    int synced_at;
} integration_t;

typedef struct audit_log_t {
    webhook_t* webhooks;
    int num_webhooks;
    user_t* users;
    int num_users;
    audit_log_entry_t* entries;
    int num_entries;
    integration_t* integrations;
    int num_integrations;
} audit_log_t;

typedef struct attachment_t {
    char* id;
    char* filename;
    int size;
    char* url;
    char* proxy_url;
    int height;
    int width;
} attachment_t;

typedef struct embed_footer_t {
    char* text;
    char* icon_url;
    char* proxy_icon_url;
} embed_footer_t;

typedef struct embed_image_t {
    char* url;
    char* proxy_url;
    int height;
    int width;
} embed_image_t;

typedef struct embed_thumbnail_t {
    char* url;
    char* proxy_url;
    int height;
    int width;
} embed_thumbnail_t;

typedef struct embed_video_t {
    char* url;
    int height;
    int width;
} embed_video_t;

typedef struct embed_provider_t {
    char* name;
    char* url;
} embed_provider_t;

typedef struct embed_author_t {
    char* name;
    char* url;
    char* icon_url;
    char* proxy_icon_url;
} embed_author_t;

typedef struct embed_field_t {
    char* name;
    char* value;
    bool is_inline;
} embed_field_t;

typedef struct embed_t {
    char* title;
    char* type;
    char* description;
    char* url;
    int timestamp;
    int color;
    embed_footer_t footer;
    embed_image_t image;
    embed_thumbnail_t thumbnail;
    embed_video_t video;
    embed_provider_t provider;
    embed_author_t author;
    embed_field_t* fields;
    int num_fields;
} embed_t;

typedef struct channel_mention_t {
    char* id;
    char* guild_id;
    int type;
    char* name;
} channel_mention_t;

typedef struct reaction_t {
    int count;
    bool me;
    emoji_t emoji;
} reaction_t;

typedef struct message_activity_t {
    int ype;
    char* party_id;
} message_activity_t;

typedef struct message_application_t {
    char* id;
    char* cover_image;
    char* description;
    char* icon;
    char* name;
} message_application_t;

typedef struct message_reference_t {
    char* message_id;
    char* channel_Id;
    char* guild_id;
} message_reference_t;

typedef struct message_t {
    char* id;
    char* channel_id;
    char* guild_id;
    user_t author;
    member_t member;
    char* content;
    int timestamp;
    int edited_timestamp;
    bool tts;
    bool mention_everyone;
    user_t* mentions;
    int num_mentions;
    role_t* mention_roles;
    int num_mention_roles;
    channel_mention_t* mention_channels;
    int num_mention_channels;
    attachment_t* attachments;
    int num_attachments;
    embed_t* embeds;
    int num_embeds;
    reaction_t* reactions;
    int num_reactions;
    int nonce;
    bool pinned;
    char* webhook_id;
    int type;
    message_activity_t activity;
    message_application_t application;
    message_reference_t message_reference;
    int flags;
} message_t;

#endif
