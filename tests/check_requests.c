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
#include <curl/curl.h>

#include "../src/requests.h"

CURLcode return_value = CURLE_OK;


void
test_fetch_url(void)
{
    GString *str = bluster_fetch_url("http://google.com", NULL, TRUE);
    g_assert(str != NULL);
    g_assert_cmpstr(str->str, ==, "");
    g_string_free(str, TRUE);
}


void
test_fetch_url_nok(void)
{
    return_value = CURLE_HTTP_NOT_FOUND;
    g_assert(bluster_fetch_url("http://google.com", NULL, TRUE) == NULL);
}


void
test_curl_write_callback(void)
{
    GString *str = g_string_new("");
    g_assert_cmpint(bluster_curl_write_callback("guda", sizeof(char), 4, &str), ==, 4);
    g_assert_cmpint(bluster_curl_write_callback("bola", sizeof(char), 4, &str), ==, 4);
    g_assert_cmpint(str->len, ==, 8);
    g_assert_cmpstr(str->str, ==, "gudabola");
    g_string_free(str, TRUE);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/requests/fetch_url", test_fetch_url);
    g_test_add_func("/requests/fetch_url_nok", test_fetch_url_nok);
    g_test_add_func("/requests/curl_write_callback", test_curl_write_callback);
    return g_test_run();
}
