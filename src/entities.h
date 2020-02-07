#include <stdbool.h>

#include <jansson.h>

typedef struct {
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
} User;

typedef struct {
    char* id;
    char* name;
    int color;
    bool hoist;
    int position;
    int permissions;
    bool managed;
    bool mentionable;
} Role;

typedef struct {
    char* id;
    char* name;
    Role* roles;
    int num_roles;
    User user; //creator
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
    Emoji emoji;
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
    User user;
    Role* roles;
    int num_roles;
    Activity game;
    long guild_id;
    char* status;
    Activity* activities;
    int num_activities;
    ClientStatus client_status;
    long premium_since;
    char* nick;
} Presence;

typedef struct {
    char* id;
    char* type;
    int allow;
    int deny;
} Overwrite;

typedef struct {
    char* id;
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
    User user;
    char* nick;
    char** roles;
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
    User target_user;
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
    char* id;
    int type;
    long guild_id;
    long channel_id;
    User user;
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
    char* id;
    char* type;
    char* role_name;
} AuditLogEntryInfo;

typedef struct {
    char* target_id;
    AuditLogChange* changes;
    int num_changes;
    long user_id;
    char* id;
    int action_type;
} AuditLogEntry;

typedef struct {
    char* id;
    char* name;
} Account;

typedef struct {
    char* id;
    char* name;
    char* type;
    bool enabled;
    bool syncing;
    long role_id;
    int expire_behavior;
    int expire_grace_period;
    User user;
    Account account;
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

typedef struct {
    char* id;
    char* filename;
    int size;
    char* url;
    char* proxy_url;
    int height;
    int width;
} Attachment;

typedef struct {
    char* text;
    char* icon_url;
    char* proxy_icon_url;
} EmbedFooter;

typedef struct {
    char* url;
    char* proxy_url;
    int height;
    int width;
} EmbedImage;

typedef struct {
    char* url;
    char* proxy_url;
    int height;
    int width;
} EmbedThumbnail;

typedef struct {
    char* url;
    int height;
    int width;
} EmbedVideo;

typedef struct {
    char* name;
    char* url;
} EmbedProvider;

typedef struct {
    char* name;
    char* url;
    char* icon_url;
    char* proxy_icon_url;
} EmbedAuthor;

typedef struct {
    char* name;
    char* value;
    bool is_inline;
} EmbedField;

typedef struct {
    char* title;
    char* type;
    char* description;
    char* url;
    long timestamp;
    int color;
    EmbedFooter footer;
    EmbedImage image;
    EmbedThumbnail thumbnail;
    EmbedVideo video;
    EmbedProvider provider;
    EmbedAuthor author;
    EmbedField* fields;
    int num_fields;
} Embed;

typedef struct {
    char* id;
    char* guild_id;
    int type;
    char* name;
} ChannelMention;

typedef struct {
    int count;
    bool me;
    Emoji emoji;
} Reaction;

typedef struct {
    int ype;
    char* party_id;
} MessageActivity;

typedef struct {
    char* id;
    char* cover_image;
    char* description;
    char* icon;
    char* name;
} MessageApplication;

typedef struct {
    char* message_id;
    char* channel_Id;
    char* guild_id;
} MessageReference;

typedef struct {
    char* id;
    char* channel_id;
    char* guild_id;
    User author;
    Member member;
    char* content;
    long timestamp;
    long edited_timestamp;
    bool tts;
    bool mention_everyone;
    User* mentions;
    int num_mentions;
    Role* mention_roles;
    int num_mention_roles;
    ChannelMention* mention_channels;
    int num_mention_channels;
    Attachment* attachments;
    int num_attachments;
    Embed* embeds;
    int num_embeds;
    Reaction* reactions;
    int num_reactions;
    int nonce;
    bool pinned;
    char* webhook_id;
    int type;
    MessageActivity activity;
    MessageApplication application;
    MessageReference message_reference;
    int flags;
} Message;

///////////////////////////////////////////////////////////////////////////////

Role parse_role(json_t* json);
Emoji parse_emoji(json_t* json);
char* parse_feature(json_t* json);
Member parse_member(json_t* json);
Channel parse_channel(json_t* json);
Presence parse_presence(json_t* json);
Guild parse_guild(json_t* json);
json_t* compose_role(Role object);
json_t* compose_emoji(Emoji object);
json_t* compose_feature(char* object);
json_t* compose_member(Member object);
json_t* compose_channel(Channel object);
json_t* compose_presence(Presence object);
json_t* compose_guild(Guild object);

// message
Attachment parse_attachment(json_t* json);
json_t* compose_attachment(Attachment object);

EmbedFooter parse_embed_footer(json_t* json);
EmbedImage parse_embed_image(json_t* json);
EmbedThumbnail parse_embed_thumbnail(json_t* json);
EmbedVideo parse_embed_video(json_t* json);
EmbedProvider parse_embed_provider(json_t* json);
EmbedAuthor parse_embed_author(json_t* json);
EmbedField parse_embed_field(json_t* json);
Embed parse_embed(json_t* json);
json_t* compose_embed_footer(EmbedFooter object);
json_t* compose_embed_image(EmbedImage object);
json_t* compose_embed_thumbnail(EmbedThumbnail object);
json_t* compose_embed_video(EmbedVideo object);
json_t* compose_embed_provider(EmbedProvider object);
json_t* compose_embed_author(EmbedAuthor object);
json_t* compose_embed_field(EmbedField object);
json_t* compose_embed(Embed object);

ChannelMention parse_channel_mention(json_t* json);
json_t* compose_channel_mention(ChannelMention object);

Reaction parse_reaction(json_t* json);
json_t* compose_reaction(Reaction object);

MessageActivity parse_message_activity(json_t* json);
MessageApplication parse_message_application(json_t* json);
MessageReference parse_message_reference(json_t* json);
json_t* compose_message_activity(MessageActivity object);
json_t* compose_message_application(MessageApplication object);
json_t* compose_message_reference(MessageReference object);

Message parse_message(json_t* json);
json_t* compose_message(Message object);
