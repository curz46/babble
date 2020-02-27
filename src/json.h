#ifndef BABBLE_JSON_H
#define BABBLE_JSON_H

role_t parse_role(json_t* json);
emoji_t parse_emoji(json_t* json);
char* parse_feature(json_t* json);
member_t parse_member(json_t* json);
channel_t parse_channel(json_t* json);
presence_t parse_presence(json_t* json);
guild_t parse_guild(json_t* json);
json_t* compose_role(role_t object);
json_t* compose_emoji(emoji_t object);
json_t* compose_feature(char* object);
json_t* compose_member(member_t object);
json_t* compose_channel(channel_t object);
json_t* compose_presence(presence_t object);
json_t* compose_guild(guild_t object);

// message
attachment_t parse_attachment(json_t* json);
json_t* compose_attachment(attachment_t object);

embed_footer_t parse_embed_footer(json_t* json);
embed_image_t parse_embed_image(json_t* json);
embed_thumbnail_t parse_embed_thumbnail(json_t* json);
embed_video_t parse_embed_video(json_t* json);
embed_provider_t parse_embed_provider(json_t* json);
embed_author_t parse_embed_author(json_t* json);
embed_field_t parse_embed_field(json_t* json);
embed_t parse_embed(json_t* json);
json_t* compose_embed_footer(embed_footer_t object);
json_t* compose_embed_image(embed_image_t object);
json_t* compose_embed_thumbnail(embed_thumbnail_t object);
json_t* compose_embed_video(embed_video_t object);
json_t* compose_embed_provider(embed_provider_t object);
json_t* compose_embed_author(embed_author_t object);
json_t* compose_embed_field(embed_field_t object);
json_t* compose_embed(embed_t object);

channel_mention_t parse_channel_mention(json_t* json);
json_t* compose_channel_mention(channel_mention_t object);

reaction_t parse_reaction(json_t* json);
json_t* compose_reaction(reaction_t object);

message_activity_t parse_message_activity(json_t* json);
message_application_t parse_message_application(json_t* json);
message_reference_t parse_message_reference(json_t* json);
json_t* compose_message_activity(message_activity_t object);
json_t* compose_message_application(message_application_t object);
json_t* compose_message_reference(message_reference_t object);

message_t parse_message(json_t* json);
json_t* compose_message(message_t object);

#endif
