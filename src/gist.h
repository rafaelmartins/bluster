/*
 * bluster: A web app to publish single-page texts written in markdown using
 *          GitHub Gists.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _BLUSTER_GIST_H
#define _BLUSTER_GIST_H

#include <glib.h>

typedef struct {
    gchar *content;
    gchar *commit;
    GDateTime *datetime;
} bluster_gist_ctx_t;

bluster_gist_ctx_t* bluster_fetch_gist(const gchar *gist_id, const gchar *oauth_token);
gboolean bluster_gist_ctx_needs_reload(bluster_gist_ctx_t *ctx, gdouble ttl);

#endif
