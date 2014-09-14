## Autotools settings

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	autogen.sh \
	.version \
	build-aux/git-version-gen \
	README.md

CLEANFILES =

BUILT_SOURCES = \
	.version

noinst_LTLIBRARIES = \
	librant.la

bin_PROGRAMS = \
	rant

check_PROGRAMS = \
	tests/check_foo

TESTS = \
	$(check_PROGRAMS)


## Build rules: convenience library

librant_la_SOURCES = \
	src/lib.c

librant_la_CFLAGS = \
	$(BALDE_CFLAGS) \
	$(LIBCURL_CFLAGS) \
	$(JSON_GLIB_CFLAGS) \
	-I$(top_srcdir)/src

librant_la_LIBADD = \
	$(BALDE_LIBS) \
	$(LIBCURL_LIBS) \
	$(JSON_GLIB_CFLAGS)


## Build rules: binary

rant_SOURCES = \
	src/main.c

rant_CFLAGS = \
	$(BALDE_CFLAGS) \
	$(LIBCURL_CFLAGS) \
	$(JSON_GLIB_CFLAGS) \
	-I$(top_srcdir)/src

rant_LDADD = \
	$(BALDE_LIBS) \
	$(LIBCURL_LIBS) \
	$(JSON_GLIB_CFLAGS)


## Build rules: tests

tests_check_foo_SOURCES = \
	tests/check_foo.c

tests_check_foo_CFLAGS = \
	$(BALDE_CFLAGS) \
	$(LIBCURL_CFLAGS) \
	$(JSON_GLIB_CFLAGS) \
	-I$(top_srcdir)/src

tests_check_foo_LDFLAGS = \
	-no-install

tests_check_foo_LDADD = \
	$(BALDE_LIBS) \
	$(LIBCURL_LIBS) \
	$(JSON_GLIB_CFLAGS)


## Helpers: Valgrind runners

if USE_VALGRIND
valgrind: all
	$(MAKE) check TESTS_ENVIRONMENT=" \
		G_SLICE=always-malloc \
		G_DEBUG=gc-friendly \
		$(LIBTOOL) \
			--mode=execute \
			$(VALGRIND) \
				--tool=memcheck \
				--leak-check=full \
				--leak-resolution=high \
				--num-callers=20 \
				--show-possibly-lost=no"

valgrind-ci: all clean-local
	$(MAKE) check TESTS_ENVIRONMENT=" \
		G_SLICE=always-malloc \
		G_DEBUG=gc-friendly \
		$(LIBTOOL) \
			--mode=execute \
			$(VALGRIND) \
				--tool=memcheck \
				--xml=yes \
				--xml-file=valgrind-%p.xml \
				--leak-check=full \
				--leak-resolution=high \
				--num-callers=20 \
				--show-possibly-lost=no"
endif


# Helpers: Git version

.version:
	echo $(VERSION) > $@-t && mv $@-t $@

dist-hook:
	echo $(VERSION) > $(distdir)/.tarball-version
