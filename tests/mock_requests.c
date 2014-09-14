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

// this thing isn't thread safe, bla, bla, bla, but this is just a test :)
extern gchar *gist_json;
extern gchar *gist_content;
extern gboolean send_json;


// this is a poor man's mock of rant_fetch_url :)
GString*
rant_fetch_url(const gchar *url)
{
    gchar *str = send_json ? gist_json : gist_content;
    send_json = !send_json;
    if (str == NULL)
        return NULL;
    return g_string_new(str);
}

#include "../src/gist.c"
