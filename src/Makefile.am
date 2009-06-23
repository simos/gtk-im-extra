## $Id: Makefile.am,v 1.16 2006/06/10 11:53:20 mariuslj Exp $
##
## Copyright (c) 2003 Noah Levitt
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
##

if OS_WIN32
no_undefined = -no-undefined
lintl = -lintl
endif

AM_CFLAGS = $(GTK_CFLAGS)

localedir = $(datadir)/locale
AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -DGTK_DISABLE_DEPRECATED=1

moduledir = $(libdir)/gtk-2.0/$(GTK_BINARY_VERSION)/immodules
module_LTLIBRARIES = im-yiddish-n.la \
                     im-yiddish-r.la \
                     im-tamil99.la \
                     im-latinplus.la \
                     im-devphon.la \
                     im-churchslavonic.la \
                     im-esperanto.la \
                     im-ipa-xsampa.la \
                     im-am-et.la \
                     im-ti-er.la \
                     im-ti-et.la \
                     im-thai.la \
                     im-linearb.la

im_yiddish_n_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined) $(lintl)
im_yiddish_n_la_SOURCES = imyiddish-n.c im-extra-intl.c im-extra-intl.h
im_yiddish_n_la_LIBADD = @GTK_LIBS@

im_yiddish_r_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined) $(lintl)
im_yiddish_r_la_SOURCES = imyiddish-r.c im-extra-intl.c im-extra-intl.h
im_yiddish_r_la_LIBADD = @GTK_LIBS@

im_tamil99_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined) $(lintl)
im_tamil99_la_SOURCES = imtamil99.c im-extra-intl.c im-extra-intl.h
im_tamil99_la_LIBADD = @GTK_LIBS@

im_latinplus_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined) $(lintl)
im_latinplus_la_SOURCES = imlatinplus.c im-extra-intl.c im-extra-intl.h
im_latinplus_la_LIBADD = @GTK_LIBS@

im_devphon_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined) $(lintl)
im_devphon_la_SOURCES = imdevphon.c im-extra-intl.c im-extra-intl.h
im_devphon_la_LIBADD = @GTK_LIBS@

im_churchslavonic_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined) $(lintl)
im_churchslavonic_la_SOURCES = imchurchslavonic.c im-extra-intl.c im-extra-intl.h
im_churchslavonic_la_LIBADD = @GTK_LIBS@

im_esperanto_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined) $(lintl)
im_esperanto_la_SOURCES = imesperanto.c im-extra-intl.c im-extra-intl.h
im_esperanto_la_LIBADD = @GTK_LIBS@

im_ipa_xsampa_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined) $(lintl)
im_ipa_xsampa_la_SOURCES = imipa-xsampa.c im-extra-intl.c im-extra-intl.h
im_ipa_xsampa_la_LIBADD = @GTK_LIBS@

im_am_et_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined)
im_am_et_la_SOURCES = gtkimcontextethio.c gtkimcontextethio.h imam-et.c
im_am_et_la_LIBADD = @GTK_LIBS@

im_ti_er_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined)
im_ti_er_la_SOURCES = gtkimcontextethio.c gtkimcontextethio.h imti-er.c
im_ti_er_la_LIBADD = @GTK_LIBS@

im_ti_et_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined)
im_ti_et_la_SOURCES = gtkimcontextethio.c gtkimcontextethio.h imti-et.c
im_ti_et_la_LIBADD = @GTK_LIBS@

im_thai_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined)
im_thai_la_SOURCES = 		\
	thai-charprop.c 	\
	thai-charprop.h 	\
	gtkimcontextthai.c 	\
	gtkimcontextthai.h	\
	imthai.c
im_thai_la_LIBADD = @GTK_LIBS@

im_linearb_la_LDFLAGS = -rpath $(moduledir) -avoid-version -module $(no_undefined) $(lintl)
im_linearb_la_SOURCES = imlinearb.c im-extra-intl.c im-extra-intl.h
im_linearb_la_LIBADD = @GTK_LIBS@

noinst_PROGRAMS = im-module-file
im_module_file_SOURCES = im-module-file.c
im_module_file_LDADD = @GTK_LIBS@

all-local: gtk.immodules

gtk.immodules: $(module_LTLIBRARIES)
	$(GTK_QUERY_IMMODULES) $(module_LTLIBRARIES) > gtk.immodules ;

install-data-hook: im-module-file
	GTK_PATH=$$GTK_PATH:$(DESTDIR)$(libdir)/gtk-2.0 $(GTK_QUERY_IMMODULES) > `./im-module-file` ;

MOSTLYCLEANFILES = gtk.immodules
