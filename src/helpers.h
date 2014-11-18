/*
 * bluster: A web app to publish rants written in markdown using GitHub Gists.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _BLUSTER_HELPERS_H
#define _BLUSTER_HELPERS_H

#include <balde.h>
#include "gist.h"

void bluster_before_request(balde_app_t *app, balde_request_t *request);
gchar* bluster_get_title(const gchar *content);
gchar* bluster_get_slug(const gchar *filename);

#endif
