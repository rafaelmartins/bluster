/*
 * bluster: A web app to publish rants written in markdown using GitHub Gists.
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
#include <curl/curl.h>

#include "helpers.h"
#include "free.h"
#include "static/resources.h"
#include "templates/base.h"
#include "templates/list.h"


static balde_response_t*
create_response(bluster_gist_ctx_t *ctx)
{
    balde_response_t* response = balde_make_response("");
    balde_response_set_header(response, "x-powered-by", PACKAGE_STRING);
    balde_response_set_tmpl_var(response, "bluster_url", PACKAGE_URL);

    gchar *gist_datetime = g_date_time_format(ctx->datetime,
        "%Y-%m-%d %H:%M:%S GMT");
    balde_response_set_tmpl_var(response, "gist_datetime", gist_datetime);
    g_free(gist_datetime);

    return response;
}


static balde_response_t*
main_view(balde_app_t *app, balde_request_t *request)
{
    bluster_gist_ctx_t *ctx = balde_app_get_user_data(app);
    if (ctx == NULL)
        return balde_abort(app, 404);

    const gchar *slug = balde_request_get_view_arg(request, "slug");

    if ((slug != NULL) ^ (ctx->files != NULL && ctx->files->next == NULL)) {
        bluster_gist_file_t *file = NULL;
        if (ctx->files != NULL && ctx->files->next == NULL) {
            file = ctx->files->data;
        }
        else {
            for (GSList *tmp = ctx->files; tmp != NULL; tmp = tmp->next) {
                bluster_gist_file_t *tmp_file = tmp->data;
                if (g_strcmp0(tmp_file->slug, slug) == 0) {
                    file = tmp_file;
                    break;
                }
            }
        }
        if (file == NULL)
            return balde_abort(app, 404);
        balde_response_t* response = create_response(ctx);
        balde_response_set_tmpl_var(response, "css",
            file->parsed_content->css == NULL ? "" : file->parsed_content->css);
        gchar *content;
        gchar *title;
        if (file->title != NULL) {
            title = file->title;
            content = g_strdup(file->parsed_content->content);
        }
        else {
            title = ctx->headline;
            content = g_strdup_printf("<h1>%s</h1>\n%s", ctx->headline,
                file->parsed_content->content);
        }
        balde_response_set_tmpl_var(response, "title", title);
        balde_response_set_tmpl_var(response, "content", content);
        balde_template_base(app, request, response);
        g_free(content);
        return response;
    }
    if (slug == NULL) {
        balde_response_t* response = create_response(ctx);
        balde_response_set_tmpl_var(response, "title", ctx->headline);
        GString *list = g_string_new("");
        for (GSList *tmp = ctx->files; tmp != NULL; tmp = tmp->next) {
            bluster_gist_file_t *tmp_file = tmp->data;
            balde_response_set_tmpl_var(response, "slug", tmp_file->slug);
            balde_response_set_tmpl_var(response, "link_title", tmp_file->title);
            g_string_append(list, balde_str_template_list(app, request, response));
        }
        balde_response_set_tmpl_var(response, "list", list->str);
        g_string_free(list, TRUE);
        balde_template_base(app, request, response);
        return response;
    }
    return balde_abort(app, 404);
}


int
main(int argc, char **argv)
{
    curl_global_init(CURL_GLOBAL_ALL);
    balde_app_t *app = balde_app_init();
    balde_resources_load(app, resources_get_resource());
    balde_app_set_user_data_destroy_func(app, (GDestroyNotify) bluster_gist_ctx_free);
    balde_app_add_before_request(app, bluster_before_request);
    balde_app_add_url_rule(app, "main", "/", BALDE_HTTP_GET, main_view);
    balde_app_add_url_rule(app, "content", "/<slug>/", BALDE_HTTP_GET, main_view);
    balde_app_set_config_from_envvar(app, "oauth_token", "BLUSTER_OAUTH_TOKEN", TRUE);
    balde_app_set_config_from_envvar(app, "gist_id", "BLUSTER_GIST_ID", FALSE);
    balde_app_set_config_from_envvar(app, "gist_ttl", "BLUSTER_GIST_TTL", TRUE);
    balde_app_run(app, argc, argv);
    balde_app_free(app);
    curl_global_cleanup();
    return 0;
}
