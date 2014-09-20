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

// this thing isn't thread safe, bla, bla, bla, but this is just a test :)
extern gchar *gist_json;
extern gchar *gist_content;
extern gint64 unix_utc;
extern guint url_count;
extern gchar *expected_urls[2];
extern gchar *expected_token;

// this is a poor man's mock of bluster_fetch_url :)
GString*
bluster_fetch_url(const gchar *url, const gchar *oauth_token, gboolean wants_json)
{
    g_assert_cmpstr(url, ==, expected_urls[url_count++]);
    if (expected_token == NULL)
        g_assert(oauth_token == NULL);
    else
        g_assert_cmpstr(oauth_token, ==, expected_token);
    gchar *str = wants_json ? gist_json : gist_content;
    if (str == NULL)
        return NULL;
    return g_string_new(str);
}


// this is a poor man's mock of g_date_time_new_now_utc :)
GDateTime*
g_date_time_new_now_utc(void)
{
    if (unix_utc == -1)
        unix_utc = 1234567890;
    GDateTime *rv = g_date_time_new_from_unix_utc(unix_utc);
    unix_utc = -1;
    return rv;
}

#include "../src/gist.c"
