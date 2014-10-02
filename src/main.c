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

#include <balde.h>
#include <glib.h>

#include "helpers.h"
#include "static/resources.h"
#include "templates/base.h"


G_LOCK_DEFINE_STATIC(template);

balde_response_t*
main_view(balde_app_t *app, balde_request_t *request)
{
    bluster_gist_ctx_t *ctx = bluster_get_gist_ctx(app);
    if (ctx == NULL)
        return balde_abort(app, 404);
    balde_response_t* response = balde_make_response("");
    balde_response_set_header(response, "x-powered-by", PACKAGE_STRING);
    balde_response_set_tmpl_var(response, "bluster_url", PACKAGE_URL);
    balde_response_set_tmpl_var(response, "content", ctx->content);
    G_LOCK(template);
    balde_template_base(app, request, response);
    G_UNLOCK(template);
    return response;
}


int
main(int argc, char **argv)
{
    balde_app_t *app = balde_app_init();
    balde_resources_load(app, resources_get_resource());
    balde_app_add_url_rule(app, "main", "/", BALDE_HTTP_GET, main_view);
    balde_app_set_config_from_envvar(app, "oauth_token", "BLUSTER_OAUTH_TOKEN", TRUE);
    balde_app_set_config_from_envvar(app, "gist_id", "BLUSTER_GIST_ID", FALSE);
    balde_app_set_config_from_envvar(app, "gist_ttl", "BLUSTER_GIST_TTL", TRUE);
    balde_app_run(app, argc, argv);
    balde_app_free(app);
    return 0;
}
