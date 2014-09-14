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


void
test_foo(void)
{
    g_assert(TRUE);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/foo/foo", test_foo);
    return g_test_run();
}
