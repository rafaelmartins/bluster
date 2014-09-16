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
#include <curl/curl.h>

// this thing isn't thread safe, bla, bla, bla, but this is just a test :)
extern CURLcode return_value;


// this is a poor man's mock of curl_easy_perform :)
CURLcode
curl_easy_perform(CURL *easy_handle)
{
    g_assert(easy_handle != NULL);
    return return_value;
}

#include "../src/requests.c"
