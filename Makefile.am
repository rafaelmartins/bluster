## Autotools settings

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	autogen.sh \
	.version \
	build-aux/git-version-gen \
	README.md \
	tests/fixtures/gist.json \
	tests/fixtures/gist-truncated.json

CLEANFILES =

BUILT_SOURCES = \
	.version

noinst_LTLIBRARIES = \
	librant.la

noinst_HEADERS = \
	src/requests.h \
	src/gist.h \
	tests/utils.h

bin_PROGRAMS = \
	rant

check_PROGRAMS = \
	tests/check_requests \
	tests/check_gist

TESTS = \
	$(check_PROGRAMS)


## Build rules: convenience library

librant_la_SOURCES = \
	src/requests.c \
	src/gist.c

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
	$(JSON_GLIB_CFLAGS) \
	librant.la


## Build rules: tests

tests_check_requests_SOURCES = \
	tests/check_requests.c \
	tests/mock_curl.c

tests_check_requests_CFLAGS = \
	$(BALDE_CFLAGS) \
	$(LIBCURL_CFLAGS) \
	$(JSON_GLIB_CFLAGS) \
	-I$(top_srcdir)/src

tests_check_requests_LDFLAGS = \
	-no-install

tests_check_requests_LDADD = \
	$(BALDE_LIBS) \
	$(LIBCURL_LIBS) \
	$(JSON_GLIB_LIBS)

tests_check_gist_SOURCES = \
	tests/check_gist.c \
	tests/mock_gist.c \
	tests/utils.c

tests_check_gist_CFLAGS = \
	$(BALDE_CFLAGS) \
	$(LIBCURL_CFLAGS) \
	$(JSON_GLIB_CFLAGS) \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/tests

tests_check_gist_LDFLAGS = \
	-no-install

tests_check_gist_LDADD = \
	$(BALDE_LIBS) \
	$(LIBCURL_LIBS) \
	$(JSON_GLIB_LIBS)


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
