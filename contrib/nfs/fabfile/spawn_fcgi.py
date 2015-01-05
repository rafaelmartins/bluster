# coding: utf-8

from fabric.api import env, run, task

from utils import configure, extract, fetch, make


@task
def install():
    fetch('http://www.lighttpd.net/download/spawn-fcgi-%(spawn-fcgi)s.tar.xz')
    extract('spawn-fcgi-%(spawn-fcgi)s.tar.xz')
    configure('spawn-fcgi-%(spawn-fcgi)s', ['--prefix=%s' % env.prefix,
                                            'CC=clang'])
    make('spawn-fcgi-%(spawn-fcgi)s')
    make('spawn-fcgi-%(spawn-fcgi)s', 'install')
