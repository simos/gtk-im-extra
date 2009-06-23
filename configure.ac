dnl $Id: configure.ac,v 1.4 2004/06/03 00:26:56 nlevitt Exp $
dnl
dnl Copyright (c) 2003 Noah Levitt 
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Lesser General Public
dnl License as published by the Free Software Foundation; either
dnl version 2.1 of the License, or (at your option) any later version.
dnl 
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Lesser General Public License for more details.
dnl 
dnl You should have received a copy of the GNU Lesser General Public
dnl License along with this library; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
dnl

AC_PREREQ([2.50])
AC_INIT([gtk-im-extra], [0.1.0])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(gtk-im-extra, 0.1.0)

# checks for progs
AC_PROG_CC
AC_PROG_LIBTOOL
AC_CHECK_PROGS([GTK_QUERY_IMMODULES], [gtk-query-immodules-2.0])

if test "x$GTK_QUERY_IMMODULES" = "x" ;
then
    AC_MSG_ERROR([gtk-query-immodules-2.0 not found.])
fi

# translation
GETTEXT_PACKAGE=gtk-im-extra
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", 
                   [Define the gettext package to be used])
ALL_LINGUAS=""
AM_GLIB_GNU_GETTEXT
AC_CHECK_FUNCS([bind_textdomain_codeset])

# check for win32
case "$host" in
  *-*-mingw*) os_win32="yes" ;; 
  *) os_win32="no" ;; 
esac

AM_CONDITIONAL(OS_WIN32, [test "$os_win32" = "yes"])

if test "x$os_win32" = "xyes"
then
    CFLAGS="$CFLAGS -mms-bitfields"
fi

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.2.0 pango >= 1.2.3)
GTK_BINARY_VERSION=`pkg-config --variable gtk_binary_version gtk+-2.0`

AC_SUBST([GETTEXT_PACKAGE])
AC_SUBST([GTK_CFLAGS])
AC_SUBST([GTK_LIBS])
AC_SUBST([GTK_BINARY_VERSION])

AC_OUTPUT([
Makefile
src/Makefile 
po/Makefile.in])
