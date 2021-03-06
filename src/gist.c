/*
 * bluster: A web app to publish rants written in markdown using GitHub Gists.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <glib.h>
#include <json-glib/json-glib.h>

#include "requests.h"
#include "gist.h"
#include "helpers.h"
#include "markdown.h"


bluster_gist_ctx_t*
bluster_fetch_gist(const gchar *gist_id, const gchar *oauth_token)
{
    if (gist_id == NULL)
        return NULL;

    gchar *url = g_strdup_printf("https://api.github.com/gists/%s", gist_id);
    GString *response = bluster_fetch_url(url, oauth_token, TRUE);
    g_free(url);

    if (response == NULL)
        return NULL;

    bluster_gist_ctx_t *ctx = NULL;

    JsonParser *parser = json_parser_new();

    if (!json_parser_load_from_data(parser, response->str, response->len, NULL))
        goto point1;

    JsonNode *root = json_parser_get_root(parser);
    if (JSON_NODE_TYPE(root) != JSON_NODE_OBJECT)
        goto point1;

    JsonReader *reader = json_reader_new(root);

    if (!json_reader_read_member(reader, "files"))
        goto point2;

    gchar **files = json_reader_list_members(reader);
    GSList *file_list = NULL;
    for (guint i = 0; files[i] != NULL; i++) {
        gchar *content = NULL;
        if (!json_reader_read_member(reader, files[i]))
            goto point3;

        if (!json_reader_read_member(reader, "language"))
            goto point3;

        const gchar *language = NULL;
        if (!json_reader_get_null_value(reader))
            language = json_reader_get_string_value(reader);
        json_reader_end_member(reader);

        if (language == NULL || g_strcmp0(language, "Markdown") != 0) {
            json_reader_end_member(reader);
            continue;
        }

        if (!json_reader_read_member(reader, "truncated"))
            goto point3;

        gboolean truncated = json_reader_get_boolean_value(reader);
        json_reader_end_element(reader);
        if (truncated) {  // fetch from raw_url
            if (!json_reader_read_member(reader, "raw_url"))
                goto point3;
            const gchar *raw_url = NULL;
            if (!json_reader_get_null_value(reader))
                raw_url = json_reader_get_string_value(reader);
            GString *raw = bluster_fetch_url(raw_url, oauth_token, FALSE);
            if (raw == NULL)
                goto point3;
            content = g_string_free(raw, FALSE);
        }
        else {
            if (!json_reader_read_member(reader, "content"))
                goto point3;
            const gchar *tmp_content = NULL;
            if (!json_reader_get_null_value(reader))
                tmp_content = json_reader_get_string_value(reader);
            content = g_strdup(tmp_content);
        }

        json_reader_end_member(reader);
        json_reader_end_member(reader);

        gchar *slug = bluster_get_slug(files[i]);
        if (slug == NULL)
            continue;

        gchar *title = bluster_get_title(content);

        bluster_gist_file_t *file = g_new(bluster_gist_file_t, 1);
        file->name = g_strdup(files[i]);
        file->slug = slug;
        file->title = title;
        file->content = content;
        file->parsed_content = bluster_parse_markdown(content);
        file_list = g_slist_append(file_list, file);
    }

    json_reader_end_member(reader);
    json_reader_end_member(reader);

    if (!json_reader_read_member(reader, "description"))
        goto point3;

    const gchar *tmp_headline = NULL;
    if (!json_reader_get_null_value(reader))
        tmp_headline = json_reader_get_string_value(reader);
    gchar *headline = g_strdup(tmp_headline);
    json_reader_end_element(reader);

    if (!json_reader_read_member(reader, "history"))
        goto point3;
    if (!json_reader_read_element(reader, 0))
        goto point3;
    if (!json_reader_read_member(reader, "version"))
        goto point3;

    const gchar *tmp_commit = NULL;
    if (!json_reader_get_null_value(reader))
        tmp_commit = json_reader_get_string_value(reader);
    gchar *commit = g_strdup(tmp_commit);

    ctx = g_new(bluster_gist_ctx_t, 1);
    ctx->headline = headline;
    ctx->files = file_list;
    ctx->commit = commit;
    ctx->datetime = g_date_time_new_now_utc();

point3:
    g_strfreev(files);
point2:
    g_object_unref(reader);
point1:
    g_object_unref(parser);
    g_string_free(response, TRUE);
    return ctx;
}


gboolean
bluster_gist_ctx_needs_reload(bluster_gist_ctx_t *ctx, gdouble ttl)
{
    if (ctx == NULL)
        return TRUE;
    GDateTime *now = g_date_time_new_now_utc();
    GTimeSpan diff = g_date_time_difference(now, ctx->datetime);
    g_date_time_unref(now);
    return diff >= (G_TIME_SPAN_MINUTE * ttl);
}
