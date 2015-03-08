# coding: utf-8

from fabric.api import env, run, task

env.hosts = ['ssh.phx.nearlyfreespeech.net']

env.home = '/home/private'
env.build = '%s/build' % env.home
env.prefix = '%s/prefix' % env.home
env.protected = '/home/protected'

env.versions = {
    'discount': '2.1.6',
    'fcgi': '2.4.1-SNAP-0910052249',
    'json-glib': '0.16.2',
    'peg': '0.1.15',
}


import balde
import bluster
import discount
import fcgi
import json_glib
import peg
import utils


@task
def install():
    utils.clean()
    peg.install()
    fcgi.install()
    json_glib.install()
    discount.install()
    balde.install()
    bluster.install()
    utils.clean()


@task
def config():
    bluster.config()


@task
def clean():
    utils.clean()
