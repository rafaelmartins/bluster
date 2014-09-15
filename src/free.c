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

#include "free.h"
#include "gist.h"


void
rant_gist_ctx_free(rant_gist_ctx_t *ctx)
{
    if (ctx == NULL)
        return;
    g_free(ctx->content);
    g_free(ctx->commit);
    if (ctx->datetime != NULL)
        g_date_time_unref(ctx->datetime);
    g_free(ctx);
}
