# coding: utf-8

from fabric.api import env, run, task

from utils import configure, extract, fetch, make


@task
def install():
    fetch('http://ftp.gnome.org/pub/gnome/sources/json-glib/0.16/json-glib-%(json-glib)s.tar.xz')
    extract('json-glib-%(json-glib)s.tar.xz')
    configure('json-glib-%(json-glib)s', ['--prefix=%s' % env.prefix,
                                          '--disable-gcov',
                                          '--disable-introspection',
                                          'CC=clang'])
    make('json-glib-%(json-glib)s')
    make('json-glib-%(json-glib)s', 'install')
