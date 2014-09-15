/*
 * rant: A web app to publish single-page rants written in markdown using
 *       GitHub Gists.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _RANT_FREE_H
#define _RANT_FREE_H

#include "gist.h"

void rant_gist_ctx_free(rant_gist_ctx_t *ctx);

#endif
