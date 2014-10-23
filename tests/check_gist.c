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
#include <curl/curl.h>

#include "../src/gist.h"
#include "../src/helpers.h"
#include "../src/free.h"
#include "utils.h"

gchar *gist_json = NULL;
gchar *gist_content = NULL;
gint64 unix_utc = -1;
guint url_count = 0;
gchar *expected_urls[2] = {NULL, NULL};
gchar *expected_token = NULL;


void
test_fetch_gist(void)
{
    gist_json = load_fixture("gist.json");
    expected_urls[0] = "https://api.github.com/gists/123456";
    expected_urls[1] = NULL;
    url_count = 0;
    expected_token = NULL;
    bluster_gist_ctx_t *ctx = bluster_fetch_gist("123456", NULL);
    g_assert(ctx != NULL);
    g_assert(ctx->files != NULL);
    g_assert_cmpstr(ctx->headline, ==, "description of gist");
    bluster_gist_file_t *file = ctx->files->data;
    g_assert_cmpstr(file->name, ==, "ring.erl");
    g_assert_cmpstr(file->content, ==, "contents of gist");
    g_assert(ctx->files->next == NULL);
    g_assert_cmpstr(ctx->commit, ==, "57a7f021a713b1c5a6a199b54cc514735d2d462f");
    g_assert(ctx->datetime != NULL);
    bluster_gist_ctx_free(ctx);
    g_free(gist_json);
}


void
test_fetch_gist_truncated(void)
{
    gist_json = load_fixture("gist-truncated.json");
    gist_content = "bola";
    expected_urls[0] = "https://api.github.com/gists/123456";
    expected_urls[1] = "https://gist.githubusercontent.com/raw/365370/"
        "8c4d2d43d178df44f4c03a7f2ac0ff512853564e/ring.erl";
    url_count = 0;
    expected_token = "asdfgdfhfgj";
    bluster_gist_ctx_t *ctx = bluster_fetch_gist("123456", "asdfgdfhfgj");
    g_assert(ctx != NULL);
    g_assert(ctx->files != NULL);
    g_assert_cmpstr(ctx->headline, ==, "description of gist");
    bluster_gist_file_t *file = ctx->files->data;
    g_assert_cmpstr(file->name, ==, "ring.erl");
    g_assert_cmpstr(file->content, ==, "bola");
    g_assert(ctx->files->next == NULL);
    g_assert_cmpstr(ctx->commit, ==, "57a7f021a713b1c5a6a199b54cc514735d2d461e");
    g_assert(ctx->datetime != NULL);
    bluster_gist_ctx_free(ctx);
    g_free(gist_json);
}


void
test_fetch_gist_invalid(void)
{
    gist_json = NULL;
    gist_content = NULL;
    expected_urls[0] = "https://api.github.com/gists/123456";
    expected_urls[1] = NULL;
    url_count = 0;
    expected_token = NULL;
    g_assert(bluster_fetch_gist("123456", NULL) == NULL);
}


void
test_fetch_gist_invalid_content(void)
{
    gist_json = load_fixture("gist-truncated.json");
    gist_content = NULL;
    expected_urls[0] = "https://api.github.com/gists/123456";
    expected_urls[1] = "https://gist.githubusercontent.com/raw/365370/"
        "8c4d2d43d178df44f4c03a7f2ac0ff512853564e/ring.erl";
    url_count = 0;
    expected_token = "asdfgdfhfgj";
    g_assert(bluster_fetch_gist("123456", "asdfgdfhfgj") == NULL);
    g_free(gist_json);
}


void
test_fetch_gist_multiple_files(void)
{
    gist_json = load_fixture("gist-multiple-files.json");
    expected_urls[0] = "https://api.github.com/gists/123456";
    expected_urls[1] = NULL;
    url_count = 0;
    expected_token = NULL;
    bluster_gist_ctx_t *ctx = bluster_fetch_gist("123456", NULL);
    g_assert(ctx != NULL);
    g_assert(ctx->files != NULL);
    g_assert_cmpstr(ctx->headline, ==, "description of gist");
    bluster_gist_file_t *file = ctx->files->data;
    g_assert_cmpstr(file->name, ==, "ring.erl");
    g_assert_cmpstr(file->content, ==, "contents of gist");

    g_assert(ctx->files->next != NULL);
    file = ctx->files->next->data;
    g_assert_cmpstr(file->name, ==, "sing.erl");
    g_assert_cmpstr(file->content, ==, "contents of hist");

    g_assert(ctx->files->next->next != NULL);
    file = ctx->files->next->next->data;
    g_assert_cmpstr(file->name, ==, "ting.erl");
    g_assert_cmpstr(file->content, ==, "contents of iist");

    g_assert(ctx->files->next->next->next == NULL);

    g_assert_cmpstr(ctx->commit, ==, "57a7f021a713b1c5a6a199b54cc514735d2d462f");
    g_assert(ctx->datetime != NULL);
    bluster_gist_ctx_free(ctx);
    g_free(gist_json);
}


void
test_gist_ctx_needs_reload_true(void)
{
    unix_utc = 1234568250;
    bluster_gist_ctx_t *ctx = g_new(bluster_gist_ctx_t, 1);
    ctx->headline = NULL;
    ctx->files = NULL;
    ctx->commit = NULL;
    ctx->datetime = g_date_time_new_from_unix_utc(1234567890);
    g_assert(bluster_gist_ctx_needs_reload(ctx, 5));
    bluster_gist_ctx_free(ctx);
}


void
test_gist_ctx_needs_reload_false(void)
{
    unix_utc = 1234567899;
    bluster_gist_ctx_t *ctx = g_new(bluster_gist_ctx_t, 1);
    ctx->headline = NULL;
    ctx->files = NULL;
    ctx->commit = NULL;
    ctx->datetime = g_date_time_new_from_unix_utc(1234567890);
    g_assert(!bluster_gist_ctx_needs_reload(ctx, 5));
    bluster_gist_ctx_free(ctx);
}


void
test_gist_ctx_needs_reload_equals(void)
{
    unix_utc = 1234567890;
    bluster_gist_ctx_t *ctx = g_new(bluster_gist_ctx_t, 1);
    ctx->headline = NULL;
    ctx->files = NULL;
    ctx->commit = NULL;
    ctx->datetime = g_date_time_new_from_unix_utc(1234567890);
    g_assert(!bluster_gist_ctx_needs_reload(ctx, 5));
    bluster_gist_ctx_free(ctx);
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
    g_test_add_func("/gist/fetch_gist_multiple_files",
        test_fetch_gist_multiple_files);
    g_test_add_func("/gist/ctx_needs_reload_true",
        test_gist_ctx_needs_reload_true);
    g_test_add_func("/gist/ctx_needs_reload_false",
        test_gist_ctx_needs_reload_false);
    g_test_add_func("/gist/ctx_needs_reload_equals",
        test_gist_ctx_needs_reload_equals);
    return g_test_run();
}
