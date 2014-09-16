/*
 * rant: A web app to publish single-page rants written in markdown using
 *       GitHub Gists.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _RANT_HELPERS_H
#define _RANT_HELPERS_H

#include <balde.h>
#include "gist.h"

rant_gist_ctx_t* rant_get_gist_ctx(balde_app_t *app, const gchar *gist_id);
gchar* rant_get_title(const gchar *content);
gchar* balde_tmpl_title(balde_app_t *app, balde_request_t *request, const gchar *content);

#endif
