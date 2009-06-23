/*
 * Copyright (c) 2004 Alexander "Ajvol" Sigachev <ajvol@mail.ru>
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

#include <stdlib.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtkaccelgroup.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkimcontextsimple.h>

#include <gtk/gtkimcontext.h>
#include <gtk/gtkimmodule.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>

#include "im-extra-intl.h"

typedef struct _GtkComposeTable GtkComposeTable;

static guint16 esperanto_compose_seqs[] = {
  GDK_C,	0,			0,	0,	0,	0x0043,	/* LATIN CAPITAL LETTER C */
  GDK_C,	GDK_X,			0,	0,	0,	0x0108,	/* LATIN CAPITAL LETTER C WITH CIRCUMFLEX */
  GDK_C,	GDK_asciicircum,	0,	0,	0,	0x0108,	/* LATIN CAPITAL LETTER C WITH CIRCUMFLEX */
  GDK_C,	GDK_x,			0,	0,	0,	0x0108,	/* LATIN CAPITAL LETTER C WITH CIRCUMFLEX */
  GDK_G,	0,			0,	0,	0,	0x0047,	/* LATIN CAPITAL LETTER G */
  GDK_G,	GDK_X,			0,	0,	0,	0x011C,	/* LATIN CAPITAL LETTER G WITH CIRCUMFLEX */
  GDK_G,	GDK_asciicircum,	0,	0,	0,	0x011C,	/* LATIN CAPITAL LETTER G WITH CIRCUMFLEX */
  GDK_G,	GDK_x,			0,	0,	0,	0x011C,	/* LATIN CAPITAL LETTER G WITH CIRCUMFLEX */
  GDK_H,	0,			0,	0,	0,	0x0048,	/* LATIN CAPITAL LETTER H */
  GDK_H,	GDK_X,			0,	0,	0,	0x0124,	/* LATIN CAPITAL LETTER H WITH CIRCUMFLEX */
  GDK_H,	GDK_asciicircum,	0,	0,	0,	0x0124,	/* LATIN CAPITAL LETTER H WITH CIRCUMFLEX */
  GDK_H,	GDK_x,			0,	0,	0,	0x0124,	/* LATIN CAPITAL LETTER H WITH CIRCUMFLEX */
  GDK_J,	0,			0,	0,	0,	0x004A,	/* LATIN CAPITAL LETTER J */
  GDK_J,	GDK_X,			0,	0,	0,	0x0134,	/* LATIN CAPITAL LETTER J WITH CIRCUMFLEX */
  GDK_J,	GDK_asciicircum,	0,	0,	0,	0x0134,	/* LATIN CAPITAL LETTER J WITH CIRCUMFLEX */
  GDK_J,	GDK_x,			0,	0,	0,	0x0134,	/* LATIN CAPITAL LETTER J WITH CIRCUMFLEX */
  GDK_S,	0,			0,	0,	0,	0x0053,	/* LATIN CAPITAL LETTER S */
  GDK_S,	GDK_X,			0,	0,	0,	0x015C,	/* LATIN CAPITAL LETTER S WITH CIRCUMFLEX */
  GDK_S,	GDK_asciicircum,	0,	0,	0,	0x015C,	/* LATIN CAPITAL LETTER S WITH CIRCUMFLEX */
  GDK_S,	GDK_x,			0,	0,	0,	0x015C,	/* LATIN CAPITAL LETTER S WITH CIRCUMFLEX */
  GDK_U,	0,			0,	0,	0,	0x0055,	/* LATIN CAPITAL LETTER U */
  GDK_U,	GDK_X,			0,	0,	0,	0x016C,	/* LATIN CAPITAL LETTER U WITH BREVE */
  GDK_U,	GDK_asciicircum,	0,	0,	0,	0x016C,	/* LATIN CAPITAL LETTER U WITH BREVE */
  GDK_U,	GDK_x,			0,	0,	0,	0x016C,	/* LATIN CAPITAL LETTER U WITH BREVE */
  GDK_U,	GDK_asciitilde,		0,	0,	0,	0x016C,	/* LATIN CAPITAL LETTER U WITH BREVE */
  GDK_c,	0,			0,	0,	0,	0x0063,	/* LATIN SMALL LETTER C */
  GDK_c,	GDK_asciicircum,	0,	0,	0,	0x0109,	/* LATIN SMALL LETTER C WITH CIRCUMFLEX */
  GDK_c,	GDK_x,			0,	0,	0,	0x0109,	/* LATIN SMALL LETTER C WITH CIRCUMFLEX */
  GDK_g,	0,			0,	0,	0,	0x0067,	/* LATIN SMALL LETTER G */
  GDK_g,	GDK_asciicircum,	0,	0,	0,	0x011D,	/* LATIN SMALL LETTER G WITH CIRCUMFLEX */
  GDK_g,	GDK_x,			0,	0,	0,	0x011D,	/* LATIN SMALL LETTER G WITH CIRCUMFLEX */
  GDK_h,	0,			0,	0,	0,	0x0068,	/* LATIN SMALL LETTER H */
  GDK_h,	GDK_asciicircum,	0,	0,	0,	0x0125,	/* LATIN SMALL LETTER H WITH CIRCUMFLEX */
  GDK_h,	GDK_x,			0,	0,	0,	0x0125,	/* LATIN SMALL LETTER H WITH CIRCUMFLEX */
  GDK_j,	0,			0,	0,	0,	0x006A,	/* LATIN SMALL LETTER J */
  GDK_j,	GDK_asciicircum,	0,	0,	0,	0x0135,	/* LATIN SMALL LETTER J WITH CIRCUMFLEX */
  GDK_j,	GDK_x,			0,	0,	0,	0x0135,	/* LATIN SMALL LETTER J WITH CIRCUMFLEX */
  GDK_s,	0,			0,	0,	0,	0x0073,	/* LATIN SMALL LETTER S */
  GDK_s,	GDK_asciicircum,	0,	0,	0,	0x015D,	/* LATIN SMALL LETTER S WITH CIRCUMFLEX */
  GDK_s,	GDK_x,			0,	0,	0,	0x015D,	/* LATIN SMALL LETTER S WITH CIRCUMFLEX */
  GDK_u,	0,			0,	0,	0,	0x0075,	/* LATIN CAPITAL SMALL U */  
  GDK_u,	GDK_asciicircum,	0,	0,	0,	0x016D,	/* LATIN SMALL LETTER U WITH BREVE */
  GDK_u,	GDK_x,			0,	0,	0,	0x016D,	/* LATIN CAPITAL SMALL U WITH BREVE */
  GDK_u,	GDK_asciitilde,		0,	0,	0,	0x016D,	/* LATIN SMALL LETTER U WITH BREVE */  

};

static void esperanto_class_init         (GtkIMContextSimpleClass  *class);
static void esperanto_init               (GtkIMContextSimple       *im_context_simple);

static GType type_esperanto = 0;

static const GtkIMContextInfo esperanto_info = 
{
  "esperanto",      /* ID */
  N_("Esperanto"), /* Human readable name */
  GETTEXT_PACKAGE,       /* Translation domain */
  LOCALEDIR,             /* Dir for bindtextdomain */
  "",                    /* Languages for which this module is the default */  
};

static void
esperanto_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (GtkIMContextSimpleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) esperanto_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (GtkIMContextSimple),
    0,              /* n_preallocs */
    (GInstanceInitFunc) esperanto_init,
  };
  type_esperanto = 
    g_type_module_register_type (module,
                                 GTK_TYPE_IM_CONTEXT_SIMPLE,
                                 "GtkIMContextSimpleEsperanto",
                                 &object_info, 0);
}

static void
esperanto_class_init (GtkIMContextSimpleClass *class)
{

}

void 
im_module_exit ()
{

}

static void
esperanto_init (GtkIMContextSimple *im_context_simple)
{
  gtk_im_context_simple_add_table (im_context_simple,
				   esperanto_compose_seqs,
				   4,
				   G_N_ELEMENTS (esperanto_compose_seqs) / 6);
}


static const GtkIMContextInfo *info_list[] = 
{
  &esperanto_info,
};


void
im_module_init (GTypeModule *module)
{
  esperanto_register_type (module);
}


void 
im_module_list (const GtkIMContextInfo ***contexts, gint *n_contexts)
{
  *contexts = info_list;
  *n_contexts = G_N_ELEMENTS (info_list);
}


GtkIMContextSimple *
im_module_create (const gchar *context_id)
{
  if (strcmp (context_id, "esperanto") == 0)
    return GTK_IM_CONTEXT_SIMPLE (g_object_new (type_esperanto, NULL));
  else
    return NULL;
}
