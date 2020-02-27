#ifndef BABBLE_JSON_H
#define BABBLE_JSON_H

role_t* parse_role(json_t* json);
emoji_t* parse_emoji(json_t* json);
char* parse_feature(json_t* json);
member_t* parse_member(json_t* json);
channel_t* parse_channel(json_t* json);
presence_t* parse_presence(json_t* json);
guild_t* parse_guild(json_t* json);
json_t* compose_role(const role_t* object);
json_t* compose_emoji(const emoji_t* object);
json_t* compose_feature(const char* string);
json_t* compose_member(const member_t* object);
json_t* compose_channel(const channel_t* object);
json_t* compose_presence(const presence_t* object);
json_t* compose_guild(const guild_t* object);

// message
attachment_t* parse_attachment(json_t* json);
json_t* compose_attachment(const attachment_t* object);

embed_footer_t* parse_embed_footer(json_t* json);
embed_image_t* parse_embed_image(json_t* json);
embed_thumbnail_t* parse_embed_thumbnail(json_t* json);
embed_video_t* parse_embed_video(json_t* json);
embed_provider_t* parse_embed_provider(json_t* json);
embed_author_t* parse_embed_author(json_t* json);
embed_field_t* parse_embed_field(json_t* json);
embed_t* parse_embed(json_t* json);
json_t* compose_embed_footer(const embed_footer_t* object);
json_t* compose_embed_image(const embed_image_t* object);
json_t* compose_embed_thumbnail(const embed_thumbnail_t* object);
json_t* compose_embed_video(const embed_video_t* object);
json_t* compose_embed_provider(const embed_provider_t* object);
json_t* compose_embed_author(const embed_author_t* object);
json_t* compose_embed_field(const embed_field_t* object);
json_t* compose_embed(const embed_t* object);

channel_mention_t* parse_channel_mention(json_t* json);
json_t* compose_channel_mention(const channel_mention_t* object);

reaction_t* parse_reaction(json_t* json);
json_t* compose_reaction(const reaction_t* object);

message_activity_t* parse_message_activity(json_t* json);
message_application_t* parse_message_application(json_t* json);
message_reference_t* parse_message_reference(json_t* json);
json_t* compose_message_activity(const message_activity_t* object);
json_t* compose_message_application(const message_application_t* object);
json_t* compose_message_reference(const message_reference_t* object);

message_t* parse_message(json_t* json);
json_t* compose_message(const message_t* object);

#endif
