## Autotools settings

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	autogen.sh \
	.version \
	build-aux/git-version-gen \
	README.md \
	src/templates/base.html \
	src/static/resources.xml \
	src/static/screen.css \
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
	src/helpers.h \
	src/free.h \
	tests/utils.h

bin_PROGRAMS = \
	rant

check_PROGRAMS = \
	tests/check_requests \
	tests/check_gist \
	tests/check_helpers

TESTS = \
	$(check_PROGRAMS)


## Build rules: convenience library

librant_la_SOURCES = \
	src/requests.c \
	src/gist.c \
	src/helpers.c \
	src/free.c

librant_la_CFLAGS = \
	$(BALDE_CFLAGS) \
	$(LIBCURL_CFLAGS) \
	$(JSON_GLIB_CFLAGS) \
	-I$(top_srcdir)/src

librant_la_LIBADD = \
	$(BALDE_LIBS) \
	$(LIBCURL_LIBS) \
	$(JSON_GLIB_CFLAGS)

src/templates/%.c: src/templates/%.html
	$(AM_V_GEN)$(BALDE_TEMPLATE_GEN) $< $@

src/templates/%.h: src/templates/%.html
	$(AM_V_GEN)$(BALDE_TEMPLATE_GEN) $< $@

src/static/resources.c: src/static/resources.xml $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir $(top_srcdir)/src/static $(top_srcdir)/src/static/resources.xml)
	$(AM_V_GEN)$(GLIB_COMPILE_RESOURCES) --generate --sourcedir $(top_srcdir)/src/static --target $@ $<

src/static/resources.h: src/static/resources.xml $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir $(top_srcdir)/src/static $(top_srcdir)/src/static/resources.xml)
	$(AM_V_GEN)$(GLIB_COMPILE_RESOURCES) --generate --sourcedir $(top_srcdir)/src/static --target $@ $<


## Build rules: binary

rant_SOURCES = \
	src/main.c

nodist_rant_SOURCES = \
	src/templates/base.c \
	src/templates/base.h \
	src/static/resources.c \
	src/static/resources.h

src/rant-main.$(OBJEXT): $(nodist_rant_SOURCES)

rant_CFLAGS = \
	$(BALDE_CFLAGS) \
	$(LIBCURL_CFLAGS) \
	$(JSON_GLIB_CFLAGS) \
	-I$(top_builddir)/src \
	-I$(top_srcdir)/src

rant_LDADD = \
	$(BALDE_LIBS) \
	$(LIBCURL_LIBS) \
	$(JSON_GLIB_LIBS) \
	librant.la


## Build rules: tests

tests_check_requests_SOURCES = \
	tests/check_requests.c \
	tests/mock_requests.c

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
	$(JSON_GLIB_LIBS) \
	librant.la

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
	$(JSON_GLIB_LIBS) \
	librant.la

tests_check_helpers_SOURCES = \
	tests/check_helpers.c \
	tests/mock_helpers.c

tests_check_helpers_CFLAGS = \
	$(BALDE_CFLAGS) \
	$(LIBCURL_CFLAGS) \
	$(JSON_GLIB_CFLAGS) \
	-I$(top_srcdir)/src

tests_check_helpers_LDFLAGS = \
	-no-install

tests_check_helpers_LDADD = \
	$(BALDE_LIBS) \
	$(LIBCURL_LIBS) \
	$(JSON_GLIB_LIBS) \
	librant.la


## Helpers: Local clean

clean-local:
	rm -f src/{templates,static}/*.{c,h}


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
