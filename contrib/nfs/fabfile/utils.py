# coding: utf-8

from fabric.api import cd, env, run
from fabric.contrib.files import exists


def clean():
    run('rm -rf %s' % env.build)


def autogen(path):
    with cd('%s/%s' % (env.build, path % env.versions)):
        run('./autogen.sh')


def fetch(url):
    run('wget -c -P %s %s' % (env.build, url % env.versions))


def fetch_git(url, dest):
    if exists("%s/%s" % (env.build, dest)):
        with cd('%s/%s' % (env.build, dest)):
            run('git pull')
    else:
        run('git clone %s %s/%s' % (url, env.build, dest))


def extract(path):
    if path.endswith('.tar.gz') or path.endswith('.tar.bz2') \
       or path.endswith('.tar.xz'):
        run('tar -xvf %s/%s -C %s' % (env.build, path % env.versions, env.build))


def configure(path, options, script='configure'):
    with cd('%s/%s' % (env.build, path % env.versions)):
        run('./%s %s' % (script, ' '.join(options)))


def make(path, args=''):
    run('gmake -C %s/%s %s' % (env.build, path % env.versions, args))
