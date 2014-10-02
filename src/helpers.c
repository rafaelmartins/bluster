/*
 * bluster: A web app to publish single-page texts written in markdown using
 *          GitHub Gists.
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
    gchar *rv;
    for (guint i = 0; content[i] != '\0'; i++) {
        switch (content[i]) {
            case ' ':
            case '#':
                if (start) {
                    tmp++;
                    offset++;
                }
                break;
            case '\n':
            case '\r':
                rv = g_strndup(tmp, i - offset);
                g_free(content_);
                return rv;
            default:
                start = FALSE;
        }
    }
    rv = g_strdup(tmp);
    g_free(content_);
    return rv;
}


gchar*
balde_tmpl_title(balde_app_t *app, balde_request_t *request, const gchar *content)
{
    return bluster_get_title(content);
}
