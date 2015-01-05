# coding: utf-8

from fabric.api import env, prompt, run, task
from fabric.contrib.files import upload_template

from utils import autogen, configure, fetch_git, make


@task
def install():
    fetch_git('https://github.com/rafaelmartins/bluster.git', 'bluster')
    autogen('bluster')
    configure('bluster', ['--prefix=%s' % env.prefix,
                          'PKG_CONFIG_PATH=%s/lib/pkgconfig' % env.prefix,
                          'PATH=%s/bin:$PATH' % env.prefix,
                          'LDFLAGS="-Wl,--rpath -Wl,%s/lib -L%s/lib"' % (env.prefix,
                                                                         env.prefix),
                          'CFLAGS="-I%s/include"' % env.prefix,
                          'CC=clang'])
    make('bluster')
    make('bluster', 'install')


@task
def config():
    prompt('Github Gist ID (required):', 'gist_id', validate='[a-f0-9]+')
    prompt('Github Gist TTL:', 'gist_ttl', validate='[0-9]*')
    prompt('Github oAuth token (with gist scope):', 'oauth_token',
           validate='[a-f0-9]*')
    prompt('FastCGI threads to start, if any:', 'fastcgi_threads',
           validate='[0-9]*')
    upload_template('fabfile/templates/bluster.sh.j2', '%s/bluster.sh' % env.protected,
                    env, use_jinja=True, mode=0755)
