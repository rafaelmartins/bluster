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
#include <curl/curl.h>

#include "requests.h"


size_t
bluster_curl_write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    GString **str = (GString**) userp;
    *str = g_string_append_len(*str, contents, size * nmemb);
    return size * nmemb;
}


GString*
bluster_fetch_url(const gchar *url, const gchar *oauth_token, gboolean wants_json)
{
    struct curl_slist *headers = NULL;

    if (oauth_token != NULL) {
        gchar *oauth_token_header = g_strdup_printf("Authorization: token %s",
            oauth_token);
        headers = curl_slist_append(headers, oauth_token_header);
        g_free(oauth_token_header);
    }

    if (wants_json)
        headers = curl_slist_append(headers, "Accept: application/vnd.github.v3+json");

    CURL *hnd = curl_easy_init();

    GString *rv = g_string_new("");

    curl_easy_setopt(hnd, CURLOPT_URL, url);
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT,
        PACKAGE_NAME "/" PACKAGE_VERSION " (See " PACKAGE_URL " for info)");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, bluster_curl_write_callback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *) &rv);

    if(curl_easy_perform(hnd) != CURLE_OK) {
        g_string_free(rv, TRUE);
        rv = NULL;
    }

    curl_easy_cleanup(hnd);
    curl_slist_free_all(headers);
    return rv;
}
