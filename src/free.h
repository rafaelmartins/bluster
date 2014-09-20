/*
 * bluster: A web app to publish single-page texts written in markdown using
 *          GitHub Gists.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _BLUSTER_FREE_H
#define _BLUSTER_FREE_H

#include "gist.h"

void bluster_gist_ctx_free(bluster_gist_ctx_t *ctx);

#endif
