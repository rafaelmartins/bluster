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
rant_get_gist_ctx(balde_app_t *app, const gchar *gist_id)
{
    rant_gist_ctx_t *ctx = (rant_gist_ctx_t*) app->user_data;
    if (rant_gist_ctx_needs_reload(ctx)) {
        rant_gist_ctx_t *new_ctx = rant_fetch_gist(gist_id);
        rant_gist_ctx_free(ctx);
        app->user_data = new_ctx;
    }
    return (rant_gist_ctx_t*) app->user_data;
}


gchar*
rant_get_title(const gchar *content)
{
    for (guint i = 0; content[i] != '\0'; i++)
        if (content[i] == '\n' || content[i] == '\r')
            return g_strndup(content, i);
    return g_strdup(content);
}


gchar*
balde_tmpl_title(balde_app_t *app, balde_request_t *request, const gchar *content)
{
    return rant_get_title(content);
}
