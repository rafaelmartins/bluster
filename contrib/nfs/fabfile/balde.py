# coding: utf-8

from fabric.api import env, run, task

from utils import autogen, configure, fetch_git, make


@task
def install():
    fetch_git('https://github.com/balde/balde.git', 'balde')
    autogen('balde')
    configure('balde', ['--prefix=%s' % env.prefix,
                        'PATH=%s/bin:$PATH' % env.prefix,
                        'LDFLAGS="-Wl,--rpath -Wl,%s/lib -L%s/lib"' % (env.prefix,
                                                                       env.prefix),
                        'CFLAGS="-I%s/include"' % env.prefix,
                        'CC=clang'])
    make('balde')
    make('balde', 'install')
