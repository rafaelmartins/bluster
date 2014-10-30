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

#include "../src/markdown.h"


void
test_parse_markdown(void)
{
    bluster_markdown_t *mkd = bluster_parse_markdown("### Foo ###\n");
    g_assert(mkd != NULL);
    g_assert_cmpstr(mkd->content, ==, "<h3>Foo</h3>");
    g_assert(mkd->css == NULL);
    bluster_markdown_free(mkd);
}


void
test_parse_markdown_with_css(void)
{
    bluster_markdown_t *mkd = bluster_parse_markdown(
        "### Foo ###\n"
        "\n"
        "<style type=\"text/css\">\n"
        "  h1 { color: red; }\n"
        "</style>");
    g_assert(mkd != NULL);
    g_assert_cmpstr(mkd->content, ==, "<h3>Foo</h3>\n\n");
    g_assert_cmpstr(mkd->css, ==,
        "<style type=\"text/css\">\n"
        "  h1 { color: red; }\n"
        "</style>\n");
    bluster_markdown_free(mkd);
}


void
test_parse_markdown_with_null(void)
{
    bluster_markdown_t *mkd = bluster_parse_markdown(NULL);
    g_assert(mkd == NULL);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/markdown/parse", test_parse_markdown);
    g_test_add_func("/markdown/parse_with_css", test_parse_markdown_with_css);
    g_test_add_func("/markdown/parse_with_null",
        test_parse_markdown_with_null);
    return g_test_run();
}
