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

#include "helpers.h"
#include "free.h"
#include "static/resources.h"
#include "templates/header.h"
#include "templates/footer.h"
#include "templates/list-header.h"
#include "templates/list-footer.h"
#include "templates/list-item.h"


static balde_response_t*
create_response(bluster_gist_ctx_t *ctx)
{
    balde_response_t* response = balde_make_response("");
    balde_response_set_header(response, "x-powered-by", PACKAGE_STRING);
    balde_response_set_tmpl_var(response, "bluster_url", PACKAGE_URL);

    gchar *gist_datetime = g_date_time_format(ctx->datetime,
        "%Y-%m-%d %H:%I:%S GMT");
    balde_response_set_tmpl_var(response, "gist_datetime", gist_datetime);
    g_free(gist_datetime);

    return response;
}


static balde_response_t*
main_view(balde_app_t *app, balde_request_t *request)
{
    bluster_gist_ctx_t *ctx = bluster_get_gist_ctx(app);
    if (ctx == NULL)
        return balde_abort(app, 404);

    const gchar *slug = balde_request_get_view_arg(request, "slug");

    if (slug != NULL ^ (ctx->files != NULL && ctx->files->next == NULL)) {
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
        balde_response_set_tmpl_var(response, "title", file->title);
        balde_response_set_tmpl_var(response, "css",
            file->parsed_content->css == NULL ? "" : file->parsed_content->css);
        balde_template_header(app, request, response);
        balde_response_append_body(response, file->parsed_content->content);
        balde_template_footer(app, request, response);
        return response;
    }
    if (slug == NULL) {
        balde_response_t* response = create_response(ctx);
        balde_response_set_tmpl_var(response, "css", "");
        balde_response_set_tmpl_var(response, "title", ctx->headline);
        balde_template_header(app, request, response);
        balde_template_list_header(app, request, response);
        for (GSList *tmp = ctx->files; tmp != NULL; tmp = tmp->next) {
            bluster_gist_file_t *tmp_file = tmp->data;
            balde_response_set_tmpl_var(response, "slug", tmp_file->slug);
            balde_response_set_tmpl_var(response, "link_title", tmp_file->title);
            balde_template_list_item(app, request, response);
        }
        balde_template_list_footer(app, request, response);
        balde_template_footer(app, request, response);
        return response;
    }
    return balde_abort(app, 404);
}


int
main(int argc, char **argv)
{
    balde_app_t *app = balde_app_init();
    balde_resources_load(app, resources_get_resource());
    balde_app_add_url_rule(app, "main", "/", BALDE_HTTP_GET, main_view);
    balde_app_add_url_rule(app, "content", "/<slug>/", BALDE_HTTP_GET, main_view);
    balde_app_set_config_from_envvar(app, "oauth_token", "BLUSTER_OAUTH_TOKEN", TRUE);
    balde_app_set_config_from_envvar(app, "gist_id", "BLUSTER_GIST_ID", FALSE);
    balde_app_set_config_from_envvar(app, "gist_ttl", "BLUSTER_GIST_TTL", TRUE);
    balde_app_run(app, argc, argv);
    bluster_gist_ctx_free(app->user_data);
    balde_app_free(app);
    return 0;
}
