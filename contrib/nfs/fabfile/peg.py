# coding: utf-8

from fabric.api import env, run, task

from utils import extract, fetch, make


@task
def install():
    fetch('http://piumarta.com/software/peg/peg-%(peg)s.tar.gz')
    extract('peg-%(peg)s.tar.gz')
    make('peg-%(peg)s', 'CC=clang')
    make('peg-%(peg)s', 'PREFIX=%s install' % env.prefix)
