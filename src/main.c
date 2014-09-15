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

#include <balde.h>
#include <glib.h>

#include "helpers.h"
#include "static/resources.h"
#include "templates/base.h"


balde_response_t*
main_view(balde_app_t *app, balde_request_t *request)
{
    rant_gist_ctx_t *ctx = rant_get_gist_ctx(app, "8803dc6042d662b603e4");
    if (ctx == NULL)
        return balde_abort(app, 404);
    balde_response_t* response = balde_make_response("");
    balde_response_set_tmpl_var(response, "content", ctx->content);
    balde_template_base(app, request, response);
    return response;
}


int
main(int argc, char **argv)
{
    balde_app_t *app = balde_app_init();
    app->user_data = NULL;
    balde_resources_load(app, resources_get_resource());
    balde_app_add_url_rule(app, "main", "/", BALDE_HTTP_GET, main_view);
    balde_app_run(app, argc, argv);
    balde_app_free(app);
    return 0;
}
