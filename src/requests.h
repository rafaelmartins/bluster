/*
 * bluster: A web app to publish single-page texts written in markdown using
 *          GitHub Gists.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _BLUSTER_REQUESTS_H
#define _BLUSTER_REQUESTS_H

size_t bluster_curl_write_callback(void *contents, size_t size, size_t nmemb, void *userp);
GString* bluster_fetch_url(const gchar *url, const gchar *oauth_token, gboolean wants_json);

#endif
