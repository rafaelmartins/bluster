/*
 * rant: A web app to publish single-page rants written in markdown using
 *       GitHub Gists.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _RANT_GIST_H
#define _RANT_GIST_H

#include <glib.h>

typedef struct {
    gchar *content;
    gchar *commit;
    GDateTime *datetime;
} rant_gist_ctx_t;

rant_gist_ctx_t* rant_fetch_gist(const gchar *gist_id, const gchar *oauth_token);
gboolean rant_gist_ctx_needs_reload(rant_gist_ctx_t *ctx, gdouble ttl);

#endif
