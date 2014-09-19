/*
 * rant: A web app to publish single-page rants written in markdown using
 *       GitHub Gists.
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


rant_gist_ctx_t*
rant_fetch_gist(const gchar *gist_id, const gchar *oauth_token)
{
    if (gist_id == NULL)
        return NULL;

    gchar *url = g_strdup_printf("https://api.github.com/gists/%s", gist_id);
    GString *response = rant_fetch_url(url, oauth_token, TRUE);
    g_free(url);

    if (response == NULL)
        return NULL;

    rant_gist_ctx_t *ctx = NULL;

    JsonParser *parser = json_parser_new();

    if (!json_parser_load_from_data(parser, response->str, response->len, NULL))
        goto point1;

    JsonNode *root = json_parser_get_root(parser);
    if (JSON_NODE_TYPE(root) != JSON_NODE_OBJECT)
        goto point1;

    JsonReader *reader = json_reader_new(root);

    if (!json_reader_read_member(reader, "files"))
        goto point2;
    if (!json_reader_read_element(reader, 0))
        goto point2;
    if (!json_reader_read_member(reader, "truncated"))
        goto point2;

    gchar *content;
    gboolean truncated = json_reader_get_boolean_value(reader);
    json_reader_end_element(reader);
    if (truncated) {  // fetch from raw_url
        if (!json_reader_read_member(reader, "raw_url"))
            goto point2;
        GString *raw = rant_fetch_url(json_reader_get_string_value(reader),
            oauth_token, FALSE);
        if (raw == NULL)
            goto point2;
        content = g_string_free(raw, FALSE);
    }
    else {
        if (!json_reader_read_member(reader, "content"))
            goto point2;
        content = g_strdup(json_reader_get_string_value(reader));
    }

    json_reader_end_element(reader);
    json_reader_end_element(reader);
    json_reader_end_element(reader);
    json_reader_end_element(reader);

    if (!json_reader_read_member(reader, "history"))
        goto point3;
    if (!json_reader_read_element(reader, 0))
        goto point3;
    if (!json_reader_read_member(reader, "version"))
        goto point3;

    ctx = g_new(rant_gist_ctx_t, 1);
    ctx->content = g_strdup(content);
    ctx->commit = g_strdup(json_reader_get_string_value(reader));
    ctx->datetime = g_date_time_new_now_utc();

point3:
    g_free(content);
point2:
    g_object_unref(reader);
point1:
    g_object_unref(parser);
    g_string_free(response, TRUE);
    return ctx;
}


gboolean
rant_gist_ctx_needs_reload(rant_gist_ctx_t *ctx, gdouble ttl)
{
    if (ctx == NULL)
        return TRUE;
    GDateTime *now = g_date_time_new_now_utc();
    GTimeSpan diff = g_date_time_difference(now, ctx->datetime);
    g_date_time_unref(now);
    return diff >= (G_TIME_SPAN_MINUTE * ttl);
}
