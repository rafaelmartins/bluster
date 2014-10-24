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

#include <glib.h>

#include "../src/gist.h"

// this thing isn't thread safe, bla, bla, bla, but this is just a test :)
extern gchar *name;
extern gchar *title;
extern gchar *content;
extern gchar *parsed_content;
extern gchar *slug;
extern gchar *headline;
extern gchar *commit;
extern gint64 unix_utc;
extern gchar *expected_token;
extern gboolean needs_reload;
extern gdouble expected_ttl;


// this is a poor man's mock of bluster_fetch_gist :)
bluster_gist_ctx_t*
bluster_fetch_gist(const gchar *gist_id, const gchar *oauth_token)
{
    if (expected_token == NULL)
        g_assert(oauth_token == NULL);
    else
        g_assert_cmpstr(oauth_token, ==, expected_token);
    bluster_gist_ctx_t *ctx = g_new(bluster_gist_ctx_t, 1);
    ctx->files = NULL;
    ctx->headline = g_strdup(headline);
    bluster_gist_file_t *f = g_new(bluster_gist_file_t, 1);
    f->name = g_strdup(name);
    f->title = g_strdup(title);
    f->content = g_strdup(content);
    f->parsed_content = g_strdup(parsed_content);
    f->slug = g_strdup(slug);
    ctx->files = g_slist_append(ctx->files, f);
    ctx->commit = g_strdup(commit);
    if (unix_utc == -1)
        unix_utc = 1234567890;
    ctx->datetime = g_date_time_new_from_unix_utc(unix_utc);
    unix_utc = -1;
    return ctx;
}


// this is a poor man's mock of bluster_gist_ctx_needs_reload :)
gboolean
bluster_gist_ctx_needs_reload(bluster_gist_ctx_t *ctx, gdouble ttl)
{
    g_assert_cmpfloat(ttl, ==, expected_ttl);
    return needs_reload;
}

#include "../src/helpers.c"
