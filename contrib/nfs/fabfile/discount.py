# coding: utf-8

from fabric.api import env, run, task

from utils import configure, extract, fetch, make


@task
def install():
    fetch('http://www.pell.portland.or.us/~orc/Code/discount/discount-%(discount)s.tar.bz2')
    extract('discount-%(discount)s.tar.bz2')
    configure('discount-%(discount)s', ['--prefix=%s' % env.prefix,
                                        '--libdir=%s/lib' % env.prefix,
                                        '--mandir=%s/man' % env.prefix,
                                        '--shared',
                                        '--enable-all-features'],
              'configure.sh')
    run('sed -i .bkp -e "/ldconfig/d" %s/%s/librarian.sh' %
        (env.build, 'discount-%(discount)s' % env.versions))
    make('discount-%(discount)s')
    make('discount-%(discount)s', 'install')
