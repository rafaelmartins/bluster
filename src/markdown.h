/*
 * bluster: A web app to publish rants written in markdown using GitHub Gists.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _BLUSTER_MARKDOWN_H
#define _BLUSTER_MARKDOWN_H

#include <glib.h>

typedef struct {
    gchar *css;
    gchar *content;
} bluster_markdown_t;

bluster_markdown_t* bluster_parse_markdown(const gchar *source);

#endif
