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

#include <balde.h>
#include <glib.h>

#include "gist.h"
#include "helpers.h"
#include "free.h"


G_LOCK_DEFINE_STATIC(ctx);

bluster_gist_ctx_t*
bluster_get_gist_ctx(balde_app_t *app)
{
    const gchar *gist_id = balde_app_get_config(app, "gist_id");
    const gchar *gist_ttl = balde_app_get_config(app, "gist_ttl");
    const gchar *oauth_token = balde_app_get_config(app, "oauth_token");
    gdouble ttl = 5;
    if (gist_ttl != NULL) {
        ttl = g_ascii_strtod(gist_ttl, NULL);
        if (ttl == 0)
            balde_abort_set_error_with_description(app, 500,
                "Invalid value of gist TTL");
    }
    G_LOCK(ctx);
    bluster_gist_ctx_t *ctx = (bluster_gist_ctx_t*) app->user_data;
    if (bluster_gist_ctx_needs_reload(ctx, ttl)) {
        bluster_gist_ctx_t *new_ctx = bluster_fetch_gist(gist_id, oauth_token);
        bluster_gist_ctx_free(ctx);
        app->user_data = new_ctx;
    }
    G_UNLOCK(ctx);
    return (bluster_gist_ctx_t*) app->user_data;
}


gchar*
bluster_get_title(const gchar *content)
{
    gchar *content_ = g_strdup(content);
    gchar *tmp = content_;
    guint offset = 0;
    gboolean start = TRUE;
    gboolean hash_title = FALSE;
    gboolean my_break = FALSE;
    gchar ruler = 0;
    gchar *rv = NULL;
    for (guint i = 0; content[i] != '\0'; i++) {
        switch (content[i]) {
            case '#':
                if (start)
                    hash_title = TRUE;
            case ' ':
                if (start) {
                    tmp++;
                    offset++;
                }
                break;
            case '\n':
            case '\r':
                ruler = content[i + 1];
                if (ruler == '\n' || ruler == '\r')
                    ruler = content[i + 2];
                rv = g_strndup(tmp, i - offset);
                my_break = TRUE;
            default:
                start = FALSE;
        }
        if (my_break)
            break;
    }
    if (rv != NULL) {
        if (!hash_title && (ruler == '\0' || (ruler != '=' && ruler != '-'))) {
            g_free(rv);
            rv = NULL;
        }
    }
    else if (hash_title || ruler != '\0')
        rv = g_strdup(tmp);
    g_free(content_);
    return rv;
}


gchar*
bluster_get_slug(const gchar *filename)
{
    GRegex *re_slug = g_regex_new("([0-9]+[_-])?(.*)\\.(md|mkd|markdown)",
        0, 0, NULL);
    GMatchInfo *info;
    gchar *rv = NULL;
    if (g_regex_match(re_slug, filename, 0, &info)) {
        rv = g_match_info_fetch(info, 2);
    }
    g_match_info_free(info);
    g_regex_unref(re_slug);
    return rv;
}
