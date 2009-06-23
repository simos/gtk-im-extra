/* $Id: im-extra-intl.c,v 1.2 2003/04/23 19:48:44 nlevitt Exp $ */
/*
 * Copyright (c) 2003 Noah Levitt 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef ENABLE_NLS

#include <libintl.h>
#include <gtk/gtk.h>

gchar *
im_extra_gettext (const gchar *str)
{
  static gboolean gucharmap_gettext_initialized = FALSE;

  g_printerr ("im_extra_gettext\n");

  if (!gucharmap_gettext_initialized)
    {
      bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
#ifdef HAVE_BIND_TEXTDOMAIN_CODESET
      bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
#endif
      gucharmap_gettext_initialized = TRUE;
    }

  return dgettext (GETTEXT_PACKAGE, str);
}

#endif /* #ifdef ENABLE_NLS */
