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

#include <glib.h>
#include <mkdio.h>
#include <string.h>

#include "markdown.h"


G_LOCK_DEFINE_STATIC(mkd);

gchar*
bluster_parse_markdown(const gchar *source)
{
    if (source == NULL)
        return NULL;
    gchar *rv = NULL;
    gchar *text;
    G_LOCK(mkd);
    MMIOT *doc = mkd_string(source, strlen(source), MKD_TABSTOP);
    if (doc == NULL)
        goto point0;
    mkd_compile(doc, MKD_TABSTOP);
    mkd_document(doc, &text);
    rv = g_strdup(text);
point0:
    mkd_cleanup(doc);
    G_UNLOCK(mkd);
    return rv;
}
