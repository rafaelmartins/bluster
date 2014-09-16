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

#include "../src/helpers.h"
#include "../src/free.h"

gchar *content = NULL;
gchar *commit = NULL;
gint64 unix_utc = -1;
gboolean needs_reload = TRUE;


void
test_get_gist_ctx_reload(void)
{
    content = "bola";
    commit = "guda";
    unix_utc = -1;
    needs_reload = TRUE;

    balde_app_t *app = balde_app_init();

    rant_gist_ctx_t *ctx = rant_get_gist_ctx(app, "123456");
    g_assert(ctx != NULL);
    g_assert_cmpstr(ctx->content, ==, "bola");
    g_assert_cmpstr(ctx->commit, ==, "guda");
    g_assert(ctx->datetime != NULL);
    g_assert(ctx == app->user_data);

    rant_gist_ctx_free(ctx);
    balde_app_free(app);
}


void
test_get_gist_ctx_reload_with_old_ctx(void)
{
    content = "bola";
    commit = "guda";
    unix_utc = -1;
    needs_reload = TRUE;

    rant_gist_ctx_t *old_ctx = g_new(rant_gist_ctx_t, 1);
    old_ctx->content = g_strdup("chunda");
    old_ctx->commit = g_strdup("arcoiro");
    old_ctx->datetime = NULL;

    balde_app_t *app = balde_app_init();
    app->user_data = old_ctx;

    rant_gist_ctx_t *ctx = rant_get_gist_ctx(app, "123456");
    g_assert(ctx != NULL);
    g_assert_cmpstr(ctx->content, ==, "bola");
    g_assert_cmpstr(ctx->commit, ==, "guda");
    g_assert(ctx->datetime != NULL);
    g_assert(ctx == app->user_data);
    g_assert(ctx != old_ctx);

    rant_gist_ctx_free(ctx);
    balde_app_free(app);
}


void
test_get_gist_ctx_no_reload(void)
{
    content = "bola";
    commit = "guda";
    unix_utc = -1;
    needs_reload = FALSE;

    rant_gist_ctx_t *old_ctx = g_new(rant_gist_ctx_t, 1);
    old_ctx->content = g_strdup("chunda");
    old_ctx->commit = g_strdup("arcoiro");
    old_ctx->datetime = NULL;

    balde_app_t *app = balde_app_init();
    app->user_data = old_ctx;

    rant_gist_ctx_t *ctx = rant_get_gist_ctx(app, "123456");
    g_assert(ctx != NULL);
    g_assert_cmpstr(ctx->content, ==, "chunda");
    g_assert_cmpstr(ctx->commit, ==, "arcoiro");
    g_assert(ctx->datetime == NULL);
    g_assert(ctx == app->user_data);
    g_assert(ctx == old_ctx);

    rant_gist_ctx_free(ctx);
    balde_app_free(app);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/helpers/get_gist_ctx_reload", test_get_gist_ctx_reload);
    g_test_add_func("/helpers/get_gist_ctx_reload_with_old_ctx",
        test_get_gist_ctx_reload_with_old_ctx);
    g_test_add_func("/helpers/get_gist_ctx_no_reload",
        test_get_gist_ctx_no_reload);
    return g_test_run();
}
