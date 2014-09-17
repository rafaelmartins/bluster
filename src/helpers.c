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

#include <balde.h>
#include <glib.h>

#include "gist.h"
#include "helpers.h"
#include "free.h"


rant_gist_ctx_t*
rant_get_gist_ctx(balde_app_t *app)
{
    const gchar *gist_id = balde_app_get_config(app, "gist_id");
    const gchar *oauth_token = balde_app_get_config(app, "oauth_token");
    rant_gist_ctx_t *ctx = (rant_gist_ctx_t*) app->user_data;
    if (rant_gist_ctx_needs_reload(ctx)) {
        rant_gist_ctx_t *new_ctx = rant_fetch_gist(gist_id, oauth_token);
        rant_gist_ctx_free(ctx);
        app->user_data = new_ctx;
    }
    return (rant_gist_ctx_t*) app->user_data;
}


gchar*
rant_get_title(const gchar *content)
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
    return rant_get_title(content);
}
