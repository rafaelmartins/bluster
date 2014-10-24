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

#include "../src/helpers.h"
#include "../src/free.h"

gchar *name = NULL;
gchar *title = NULL;
gchar *content = NULL;
gchar *parsed_content = NULL;
gchar *slug = NULL;
gchar *headline = NULL;
gchar *commit = NULL;
gint64 unix_utc = -1;
gchar *expected_token = NULL;
gboolean needs_reload = TRUE;
gdouble expected_ttl = 5.0;


void
test_get_gist_ctx_reload(void)
{
    name = "bola.txt";
    title = "my title";
    content = "bolac";
    parsed_content = "parsed content";
    slug = "bola";
    headline = "head";
    commit = "guda";
    unix_utc = -1;
    needs_reload = TRUE;
    expected_ttl = 5.0;

    balde_app_t *app = balde_app_init();

    bluster_gist_ctx_t *ctx = bluster_get_gist_ctx(app);
    g_assert(ctx != NULL);
    g_assert(ctx->files != NULL);
    bluster_gist_file_t *file = ctx->files->data;
    g_assert_cmpstr(file->name, ==, "bola.txt");
    g_assert_cmpstr(file->content, ==, "bolac");
    g_assert_cmpstr(file->slug, ==, "bola");
    g_assert(ctx->files->next == NULL);
    g_assert_cmpstr(ctx->commit, ==, "guda");
    g_assert(ctx->datetime != NULL);
    g_assert(ctx == app->user_data);

    bluster_gist_ctx_free(ctx);
    balde_app_free(app);
}


void
test_get_gist_ctx_reload_with_old_ctx(void)
{
    name = "bola.txt";
    title = "my title";
    content = "bolac";
    parsed_content = "<p>parsed content</p>";
    slug = "bola";
    headline = "head";
    commit = "guda";
    unix_utc = -1;
    needs_reload = TRUE;
    expected_ttl = 10.0;

    bluster_gist_ctx_t *old_ctx = g_new(bluster_gist_ctx_t, 1);
    old_ctx->files = NULL;
    old_ctx->headline = NULL;

    bluster_gist_file_t *f = g_new(bluster_gist_file_t, 1);
    f->name = g_strdup("bola.txt");
    f->title = g_strdup("my old title");
    f->content = g_strdup("chunda");
    f->parsed_content = g_strdup("<p>chunda</p>");
    f->slug = g_strdup("bola");

    old_ctx->files = g_slist_append(old_ctx->files, f);
    old_ctx->commit = g_strdup("arcoiro");
    old_ctx->datetime = NULL;

    balde_app_t *app = balde_app_init();
    balde_app_set_config(app, "gist_ttl", "10");
    app->user_data = old_ctx;

    bluster_gist_ctx_t *ctx = bluster_get_gist_ctx(app);
    g_assert(ctx != NULL);
    g_assert(ctx->files != NULL);
    g_assert_cmpstr(ctx->headline, ==, "head");
    bluster_gist_file_t *file = ctx->files->data;
    g_assert_cmpstr(file->name, ==, "bola.txt");
    g_assert_cmpstr(file->title, ==, "my title");
    g_assert_cmpstr(file->content, ==, "bolac");
    g_assert_cmpstr(file->parsed_content, ==, "<p>parsed content</p>");
    g_assert_cmpstr(file->slug, ==, "bola");
    g_assert(ctx->files->next == NULL);
    g_assert_cmpstr(ctx->commit, ==, "guda");
    g_assert(ctx->datetime != NULL);
    g_assert(ctx == app->user_data);
    g_assert(ctx != old_ctx);

    bluster_gist_ctx_free(ctx);
    balde_app_free(app);
}


void
test_get_gist_ctx_no_reload(void)
{
    name = "bola.txt";
    title = "my title";
    content = "bolac";
    parsed_content = "<p>parsed content</p>";
    slug = "bola";
    headline = "head";
    commit = "guda";
    unix_utc = -1;
    needs_reload = FALSE;
    expected_ttl = 5.0;

    bluster_gist_ctx_t *old_ctx = g_new(bluster_gist_ctx_t, 1);
    old_ctx->files = NULL;
    old_ctx->headline = g_strdup("head");

    bluster_gist_file_t *f = g_new(bluster_gist_file_t, 1);
    f->name = g_strdup("bola.txt");
    f->title = g_strdup("my old title");
    f->content = g_strdup("chunda");
    f->parsed_content = g_strdup("<p>chunda</p>");
    f->slug = g_strdup("bola");

    old_ctx->files = g_slist_append(old_ctx->files, f);
    old_ctx->commit = g_strdup("arcoiro");
    old_ctx->datetime = NULL;

    balde_app_t *app = balde_app_init();
    app->user_data = old_ctx;

    bluster_gist_ctx_t *ctx = bluster_get_gist_ctx(app);
    g_assert(ctx != NULL);
    g_assert(ctx->files != NULL);
    g_assert_cmpstr(ctx->headline, ==, "head");
    bluster_gist_file_t *file = ctx->files->data;
    g_assert_cmpstr(file->name, ==, "bola.txt");
    g_assert_cmpstr(file->title, ==, "my old title");
    g_assert_cmpstr(file->content, ==, "chunda");
    g_assert_cmpstr(file->parsed_content, ==, "<p>chunda</p>");
    g_assert_cmpstr(file->slug, ==, "bola");
    g_assert(ctx->files->next == NULL);
    g_assert_cmpstr(ctx->commit, ==, "arcoiro");
    g_assert(ctx->datetime == NULL);
    g_assert(ctx == app->user_data);
    g_assert(ctx == old_ctx);

    bluster_gist_ctx_free(ctx);
    balde_app_free(app);
}


void
test_get_title(void)
{
    gchar *title = bluster_get_title("asdqwe\nzxcvbn");
    g_assert_cmpstr(title, ==, "asdqwe");
    g_free(title);
    title = bluster_get_title("poilkj\rzxc");
    g_assert_cmpstr(title, ==, "poilkj");
    g_free(title);
    title = bluster_get_title("poilkjzxc");
    g_assert_cmpstr(title, ==, "poilkjzxc");
    g_free(title);
    title = bluster_get_title("# poilkj\nzxc");
    g_assert_cmpstr(title, ==, "poilkj");
    g_free(title);
    title = bluster_get_title("#  poilkj\nzxc");
    g_assert_cmpstr(title, ==, "poilkj");
    g_free(title);
    title = bluster_get_title("## poilkj\nzxc");
    g_assert_cmpstr(title, ==, "poilkj");
    g_free(title);
    title = bluster_get_title("##  poilkj\nzxc");
    g_assert_cmpstr(title, ==, "poilkj");
    g_free(title);
    title = bluster_get_title("# poilkjzxc");
    g_assert_cmpstr(title, ==, "poilkjzxc");
    g_free(title);
    title = bluster_get_title("#  poilkjzxc");
    g_assert_cmpstr(title, ==, "poilkjzxc");
    g_free(title);
    title = bluster_get_title("## poilkjzxc");
    g_assert_cmpstr(title, ==, "poilkjzxc");
    g_free(title);
    title = bluster_get_title("##  poilkjzxc");
    g_assert_cmpstr(title, ==, "poilkjzxc");
    g_free(title);
    title = bluster_get_title(" # poilkj\nzxc");
    g_assert_cmpstr(title, ==, "poilkj");
    g_free(title);
    title = bluster_get_title(" #  poilkj\nzxc");
    g_assert_cmpstr(title, ==, "poilkj");
    g_free(title);
    title = bluster_get_title(" ## poilkj\nzxc");
    g_assert_cmpstr(title, ==, "poilkj");
    g_free(title);
    title = bluster_get_title(" ##  poilkj\nzxc");
    g_assert_cmpstr(title, ==, "poilkj");
    g_free(title);
    title = bluster_get_title(" # poilkjzxc");
    g_assert_cmpstr(title, ==, "poilkjzxc");
    g_free(title);
    title = bluster_get_title(" #  poilkjzxc");
    g_assert_cmpstr(title, ==, "poilkjzxc");
    g_free(title);
    title = bluster_get_title(" ## poilkjzxc");
    g_assert_cmpstr(title, ==, "poilkjzxc");
    g_free(title);
    title = bluster_get_title(" ##  poilkjzxc");
    g_assert_cmpstr(title, ==, "poilkjzxc");
    g_free(title);
}


void
test_get_slug(void)
{
    gchar *slug = bluster_get_slug("001_bola.txt");
    g_assert_cmpstr(slug, ==, "bola");
    g_free(slug);
    slug = bluster_get_slug("001-bola.txt");
    g_assert_cmpstr(slug, ==, "bola");
    g_free(slug);
    slug = bluster_get_slug("0_bola.md");
    g_assert_cmpstr(slug, ==, "bola");
    g_free(slug);
    slug = bluster_get_slug("01_chunda.txt");
    g_assert_cmpstr(slug, ==, "chunda");
    g_free(slug);
    slug = bluster_get_slug("001_bola.mkd");
    g_assert_cmpstr(slug, ==, "bola");
    g_free(slug);
    slug = bluster_get_slug("001_bola.markdown");
    g_assert_cmpstr(slug, ==, "bola");
    g_free(slug);
    slug = bluster_get_slug("bola.txt");
    g_assert(slug == NULL);
    slug = bluster_get_slug("bola.md");
    g_assert(slug == NULL);
    slug = bluster_get_slug("bola.mkd");
    g_assert(slug == NULL);
    slug = bluster_get_slug("bola.markdown");
    g_assert(slug == NULL);
    slug = bluster_get_slug("001_bola");
    g_assert(slug == NULL);
    slug = bluster_get_slug("001-bola");
    g_assert(slug == NULL);
    slug = bluster_get_slug("0_bola");
    g_assert(slug == NULL);
    slug = bluster_get_slug("01_chunda");
    g_assert(slug == NULL);
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
    g_test_add_func("/helpers/get_title", test_get_title);
    g_test_add_func("/helpers/get_slug", test_get_slug);
    return g_test_run();
}
