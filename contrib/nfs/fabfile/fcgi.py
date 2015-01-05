# coding: utf-8

from fabric.api import env, run, task

from utils import configure, extract, fetch, make


@task
def install():
    fetch('http://www.fastcgi.com/dist/fcgi-%(fcgi)s.tar.gz')
    extract('fcgi-%(fcgi)s.tar.gz')
    configure('fcgi-%(fcgi)s', ['--prefix=%s' % env.prefix])
    make('fcgi-%(fcgi)s')
    make('fcgi-%(fcgi)s', 'install')
