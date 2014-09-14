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
#include <curl/curl.h>

#include "../src/gist.h"
#include "utils.h"

gchar *gist_json = NULL;
gchar *gist_content = NULL;
gboolean send_json = TRUE;
gint64 unix_utc = -1;


void
test_fetch_gist(void)
{
    send_json = TRUE;
    gist_json = load_fixture("gist.json");
    rant_gist_ctx_t *ctx = rant_fetch_gist("123456");
    g_assert(ctx != NULL);
    g_assert_cmpstr(ctx->content, ==, "contents of gist");
    g_assert_cmpstr(ctx->commit, ==, "57a7f021a713b1c5a6a199b54cc514735d2d462f");
    g_assert(ctx->datetime != NULL);
    rant_gist_ctx_free(ctx);
    g_free(gist_json);
}


void
test_fetch_gist_truncated(void)
{
    send_json = TRUE;
    gist_json = load_fixture("gist-truncated.json");
    gist_content = "bola";
    rant_gist_ctx_t *ctx = rant_fetch_gist("123456");
    g_assert(ctx != NULL);
    g_assert_cmpstr(ctx->content, ==, "bola");
    g_assert_cmpstr(ctx->commit, ==, "57a7f021a713b1c5a6a199b54cc514735d2d461e");
    g_assert(ctx->datetime != NULL);
    rant_gist_ctx_free(ctx);
    g_free(gist_json);
}


void
test_fetch_gist_invalid(void)
{
    send_json = TRUE;
    gist_json = NULL;
    gist_content = NULL;
    g_assert(rant_fetch_gist("123456") == NULL);
}


void
test_fetch_gist_invalid_content(void)
{
    send_json = TRUE;
    gist_json = load_fixture("gist-truncated.json");
    gist_content = NULL;
    g_assert(rant_fetch_gist("123456") == NULL);
}


void
test_gist_ctx_needs_reload_true(void)
{
    unix_utc = 1234568250;
    rant_gist_ctx_t *ctx = g_new(rant_gist_ctx_t, 1);
    ctx->content = NULL;
    ctx->commit = NULL;
    ctx->datetime = g_date_time_new_from_unix_utc(1234567890);
    g_assert(rant_gist_ctx_needs_reload(ctx));
    rant_gist_ctx_free(ctx);
}


void
test_gist_ctx_needs_reload_false(void)
{
    unix_utc = 1234567899;
    rant_gist_ctx_t *ctx = g_new(rant_gist_ctx_t, 1);
    ctx->content = NULL;
    ctx->commit = NULL;
    ctx->datetime = g_date_time_new_from_unix_utc(1234567890);
    g_assert(!rant_gist_ctx_needs_reload(ctx));
    rant_gist_ctx_free(ctx);
}


void
test_gist_ctx_needs_reload_equals(void)
{
    unix_utc = 1234567890;
    rant_gist_ctx_t *ctx = g_new(rant_gist_ctx_t, 1);
    ctx->content = NULL;
    ctx->commit = NULL;
    ctx->datetime = g_date_time_new_from_unix_utc(1234567890);
    g_assert(!rant_gist_ctx_needs_reload(ctx));
    rant_gist_ctx_free(ctx);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/gist/fetch_gist", test_fetch_gist);
    g_test_add_func("/gist/fetch_gist_truncated", test_fetch_gist_truncated);
    g_test_add_func("/gist/fetch_gist_invalid", test_fetch_gist_invalid);
    g_test_add_func("/gist/fetch_gist_invalid_content",
        test_fetch_gist_invalid_content);
    g_test_add_func("/gist/ctx_needs_reload_true",
        test_gist_ctx_needs_reload_true);
    g_test_add_func("/gist/ctx_needs_reload_false",
        test_gist_ctx_needs_reload_false);
    g_test_add_func("/gist/ctx_needs_reload_equals",
        test_gist_ctx_needs_reload_equals);
    return g_test_run();
}
