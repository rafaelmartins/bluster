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

#include <glib.h>
#include <mkdio.h>
#include <string.h>

#include "markdown.h"


G_LOCK_DEFINE_STATIC(mkd);

bluster_markdown_t*
bluster_parse_markdown(const gchar *source)
{
    if (source == NULL)
        return NULL;
    bluster_markdown_t *rv = NULL;
    gchar *buffer;
    G_LOCK(mkd);
    MMIOT *doc = mkd_string(source, strlen(source), MKD_TABSTOP);
    if (doc == NULL)
        goto point0;
    mkd_compile(doc, MKD_TABSTOP);
    mkd_document(doc, &buffer);
    rv = g_new(bluster_markdown_t, 1);
    rv->content = g_strdup(buffer);
    mkd_css(doc, &buffer);
    rv->css = g_strdup(buffer);
point0:
    mkd_cleanup(doc);
    G_UNLOCK(mkd);
    return rv;
}
