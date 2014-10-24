## Autotools settings

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	autogen.sh \
	.version \
	build-aux/git-version-gen \
	README.md \
	src/templates/header.html \
	src/templates/footer.html \
	src/templates/list-header.html \
	src/templates/list-footer.html \
	src/templates/list-item.html \
	src/static/resources.xml \
	src/static/screen.css \
	tests/fixtures/gist.json \
	tests/fixtures/gist-truncated.json \
	tests/fixtures/gist-multiple-files.json \
	tests/fixtures/gist-multiple-files-mixed.json

CLEANFILES = \
	tests/utils.c

BUILT_SOURCES = \
	.version

noinst_LTLIBRARIES = \
	libbluster.la

noinst_HEADERS = \
	src/requests.h \
	src/gist.h \
	src/helpers.h \
	src/free.h \
	src/markdown.h \
	tests/utils.h

bin_PROGRAMS = \
	bluster

check_PROGRAMS = \
	tests/check_requests \
	tests/check_gist \
	tests/check_helpers \
	tests/check_markdown

TESTS = \
	$(check_PROGRAMS)


## Build rules: convenience library

libbluster_la_SOURCES = \
	src/requests.c \
	src/gist.c \
	src/helpers.c \
	src/free.c \
	src/markdown.c

libbluster_la_CFLAGS = \
	$(BALDE_CFLAGS) \
	$(LIBCURL_CFLAGS) \
	$(JSON_GLIB_CFLAGS) \
	-I$(top_srcdir)/src

libbluster_la_LIBADD = \
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

bluster_SOURCES = \
	src/main.c

nodist_bluster_SOURCES = \
	src/templates/header.c \
	src/templates/header.h \
	src/templates/footer.c \
	src/templates/footer.h \
	src/templates/list-header.c \
	src/templates/list-header.h \
	src/templates/list-footer.c \
	src/templates/list-footer.h \
	src/templates/list-item.c \
	src/templates/list-item.h \
	src/static/resources.c \
	src/static/resources.h

src/bluster-main.$(OBJEXT): $(nodist_bluster_SOURCES)

bluster_CFLAGS = \
	$(BALDE_CFLAGS) \
	$(LIBCURL_CFLAGS) \
	$(JSON_GLIB_CFLAGS) \
	-I$(top_builddir)/src \
	-I$(top_srcdir)/src

bluster_LDADD = \
	libbluster.la \
	$(BALDE_LIBS) \
	$(LIBCURL_LIBS) \
	$(JSON_GLIB_LIBS)


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
	libbluster.la \
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
	libbluster.la \
	$(BALDE_LIBS) \
	$(LIBCURL_LIBS) \
	$(JSON_GLIB_LIBS)

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
	libbluster.la \
	$(BALDE_LIBS) \
	$(LIBCURL_LIBS) \
	$(JSON_GLIB_LIBS)

tests_check_markdown_SOURCES = \
	tests/check_markdown.c

tests_check_markdown_CFLAGS = \
	$(BALDE_CFLAGS) \
	$(LIBCURL_CFLAGS) \
	$(JSON_GLIB_CFLAGS) \
	-I$(top_srcdir)/src

tests_check_markdown_LDFLAGS = \
	-no-install

tests_check_markdown_LDADD = \
	libbluster.la \
	$(BALDE_LIBS) \
	$(LIBCURL_LIBS) \
	$(JSON_GLIB_LIBS)


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
	rm -f $(distdir)/tests/utils.c
