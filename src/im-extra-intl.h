/* $Id: im-extra-intl.h,v 1.2 2003/04/23 19:48:44 nlevitt Exp $ */
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

#ifndef GUCHARMAP_INTL_H

#if HAVE_CONFIG_H
# include "config.h"
#endif

#undef _
#undef N_

#ifdef ENABLE_NLS

gchar * im_extra_gettext (const gchar *str);

# include <libintl.h>

# define _(String) gucharmap_gettext(String)

# ifdef gettext_noop
#  define N_(String) gettext_noop(String)
# else
#  define N_(String) (String)
# endif

#else /* NLS is disabled */

# define _(String) (String)
# define N_(String) (String)

# undef textdomain
# undef gettext
# undef dgettext
# undef dcgettext
# undef bindtextdomain

# define textdomain(String) (String)
# define gettext(String) (String)
# define dgettext(Domain,String) (String)
# define dcgettext(Domain,String,Type) (String)
# define bindtextdomain(Domain,Directory) (Domain) 

#endif

#endif /* #ifndef GUCHARMAP_INTL_H */
