/* GTK - The GIMP Toolkit
 * Copyright (C) 2000 Red Hat Software
 * Copyright (C) 2000 SuSE Linux Ltd
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Original author: Owen Taylor <otaylor@redhat.com>
 * 
 * Modified for Tigrigna - Daniel Yacob <locales@geez.org>
 *
 */

#include <string.h>

#include <gtk/gtkimmodule.h>
#include "im-extra-intl.h"
#include "gtkimcontextethio.h"


GType type_ti_et_translit = 0;

static void ti_et_class_init (GtkIMContextEthiopicClass *class);
static void ti_et_init (GtkIMContextEthiopic *im_context);

static guint16 ti_et_compose_seqs[] = {
  /* do punctuation and numerals here */

  NUM_PUNCT
  '?',  0,  0, 0, 0, 0, '?',
  '?',  '?',  0, 0, 0, 0, 0x1367,
  '?',  '?','?', 0, 0, 0, '?',     /* circle end */
  'A',  0,  0,  0,  0, 0, 0x12A0,
  'A','A',  0,  0,  0, 0, 0x12D0,
  SYLW('B', 0x1260)
  SYLW('C', 0x1328)
  SYLW('D', 0x12f8)
  'E',  0,  0,  0,  0, 0, 0x12A4,
  'E','E',  0,  0,  0, 0, 0x12D4,
  SYLW1('F', 0x1348)
  'F', 'Y',   0,  0,  0, 0, 0x135A,
  'F', 'Y', 'A',  0,  0, 0, 0x135A,
  'F', 'Y', 'a',  0,  0, 0, 0x135A,
  SYLW2('F', 0x1348)
  SYL('G', 0x1318)
  SYLW('H', 0x1210)
  'I',  0,  0,  0,  0, 0, 0x12A5,
  'I','A',  0,  0,  0, 0, 0x12A3,
  'I','E',  0,  0,  0, 0, 0x12A4,
  'I','I',  0,  0,  0, 0, 0x12D5,
  'I','I','E',  0,  0, 0, 0x12D4,
  'I','I','a',  0,  0, 0, 0x12D3,
  'I','I','e',  0,  0, 0, 0x12D0,
  'I','I','e','e',  0, 0, 0x12D4,
  'I','I','i',  0,  0, 0, 0x12D2,
  'I','I','o',  0,  0, 0, 0x12D6,
  'I','I','u',  0,  0, 0, 0x12D1,
  'I','O',  0,  0,  0, 0, 0x12A6,
  'I','U',  0,  0,  0, 0, 0x12A1,
  'I','W',  0,  0,  0, 0, 0x12A7,
  'I','a',  0,  0,  0, 0, 0x12A3,
  'I','e',  0,  0,  0, 0, 0x12A0,
  'I','e','e',  0,  0, 0, 0x12A4,
  'I','i',  0,  0,  0, 0, 0x12A2,
  'I','o',  0,  0,  0, 0, 0x12A6,
  'I','u',  0,  0,  0, 0, 0x12A1,
  SYLWW('K', 0x12b8)
  SYLW('L', 0x1208)
  SYLW1('M', 0x1218)
  'M', 'Y',   0,  0,  0, 0, 0x1359,
  'M', 'Y', 'A',  0,  0, 0, 0x1359,
  'M', 'Y', 'a',  0,  0, 0, 0x1359,
  SYLW2('M', 0x1218)
  SYLW('N', 0x1298)
  'O',  0,  0,  0,  0, 0, 0x12A6,
  'O','O',  0,  0,  0, 0, 0x12D6,
  SYLW('P', 0x1330)
  SYLWW('Q', 0x1250) 
  SYLW1('R', 0x1228)
  'R', 'Y',   0,  0,  0, 0, 0x1358,
  'R', 'Y', 'A',  0,  0, 0, 0x1358,
  'R', 'Y', 'a',  0,  0, 0, 0x1358,
  SYLW2('R', 0x1228)
  UPPER_S
  SYLW('T', 0x1320)
  'U',  0,  0,  0,  0, 0, 0x12A1,
  'U','U',  0,  0,  0, 0, 0x12D1,
  SYLW('V', 0x1268)
  SYL('W', 0x12c8)
  SYLW('X', 0x1238)
  SYL('Y', 0x12e8)
  SYLW('Z', 0x12e0)

  /* much, much work to be done for lone vowels */
  'a',  0,  0,  0,  0, 0, 0x12A3,
  'a','a',  0,  0,  0, 0, 0x12D3,
  'a','a','a',  0,  0, 0, 0x12D0,
  'a','a','a','a',  0, 0, 0x12A0,
  'a','a','a','a','a', 0, 0x12A3,  /* circle end */
  SYLW('b', 0x1260)
  SYLW('c', 0x1278)
  SYLW('d', 0x12f0)
  'e',  0,  0,  0,  0, 0, 0x12A5,
  'e','A',  0,  0,  0, 0, 0x12A3,
  'e','E',  0,  0,  0, 0, 0x12A4,
  'e','I',  0,  0,  0, 0, 0x12A2,
  'e','O',  0,  0,  0, 0, 0x12A6,
  'e','U',  0,  0,  0, 0, 0x12A1,
  'e','W',  0,  0,  0, 0, 0x12A7,
  'e','a',  0,  0,  0, 0, 0x12D0,
  'e','e',  0,  0,  0, 0, 0x12D5,
  'e','e','E',  0,  0, 0, 0x12D4,
  'e','e','a',  0,  0, 0, 0x12D3,
  'e','e','e',  0,  0, 0, 0x12D0,
  'e','e','e','e',  0, 0, 0x12D4,
  'e','e','e','e','e', 0, 0x12A4,
  'e','e','e','e','e', 'e', 0x12A5,  /* circle end */
  'e','e','i',  0,  0, 0, 0x12D2,
  'e','e','o',  0,  0, 0, 0x12D6,
  'e','e','u',  0,  0, 0, 0x12D1,
  'e','i',  0,  0,  0, 0, 0x12A2,
  'e','o',  0,  0,  0, 0, 0x12A6,
  'e','u',  0,  0,  0, 0, 0x12A1,
  SYLW1('f', 0x1348)
  'f', 'Y',   0,  0,  0, 0, 0x135A,
  'f', 'Y', 'A',  0,  0, 0, 0x135A,
  'f', 'Y', 'a',  0,  0, 0, 0x135A,
  SYLW2('f', 0x1348)
  SYLWW('g', 0x1308)
  ET_LOWER_H
  'i',  0,  0,  0,  0, 0, 0x12A2,
  'i', 'i', 0,  0,  0, 0, 0x12D2,
  SYLW('j', 0x1300)
  SYLWW('k', 0x12a8)
  SYLW('l', 0x1208)
  SYLW1('m', 0x1218)
  'm', 'Y',   0,  0,  0, 0, 0x1359,
  'm', 'Y', 'A',  0,  0, 0, 0x1359,
  'm', 'Y', 'a',  0,  0, 0, 0x1359,
  SYLW2('m', 0x1218)
  SYLW('n', 0x1290)
  'o',  0,  0,  0,  0, 0, 0x12A6,
  'o','o',  0,  0,  0, 0, 0x12D6,
  SYLW('p', 0x1350)
  SYLWW('q', 0x1240)
  SYLW1('r', 0x1228)
  'r', 'Y',   0,  0,  0, 0, 0x1358,
  'r', 'Y', 'A',  0,  0, 0, 0x1358,
  'r', 'Y', 'a',  0,  0, 0, 0x1358,
  SYLW2('r', 0x1228)
  ET_LOWER_S
  SYLW('t', 0x1270)
  'u',  0,  0,  0,  0, 0, 0x12A1,
  'u','u',  0,  0,  0, 0, 0x12D1,
  SYLW('v', 0x1268)
  SYL('w', 0x12c8)
  SYLW('x', 0x1238)
  SYL('y', 0x12e8)
  SYLW('z', 0x12d8)
  GDK_Shift_L, GDK_space, 0, 0, 0, 0, 0x1361,
  GDK_Shift_R, GDK_space, 0, 0, 0, 0, 0x1361,
};

static void
ti_et_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (GtkIMContextEthiopicClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) ti_et_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (GtkIMContextEthiopic),
    0,
    (GInstanceInitFunc) ti_et_init,
  };

  type_ti_et_translit = 
    g_type_module_register_type (module,
				 GTK_TYPE_IM_CONTEXT_ETHIOPIC,
				 "GtkIMContextTigrignaEthiopia",
				 &object_info, 0);
}

static void
ti_et_class_init (GtkIMContextEthiopicClass *class)
{
}

static void
ti_et_init (GtkIMContextEthiopic *im_context)
{
  gtk_im_context_ethiopic_add_table (im_context,
				   ti_et_compose_seqs,
				   5,
				   G_N_ELEMENTS (ti_et_compose_seqs) / (5 + 2));
}

static const GtkIMContextInfo ti_et_info = { 
  "ti_et",		   /* ID */
  N_("Tigrigna-Ethiopian (EZ+)"),  /* Human readable name */
  "gtk+",		   /* Translation domain */
   LOCALEDIR,	   /* Dir for bindtextdomain (not strictly needed for "gtk+") */
  "ti"			   /* Languages for which this module is the default */
};

static const GtkIMContextInfo *info_list[] = {
  &ti_et_info
};

void
im_module_init (GTypeModule *module)
{
  ti_et_register_type (module);
}

void 
im_module_exit (void)
{
}

void 
im_module_list (const GtkIMContextInfo ***contexts,
		int                      *n_contexts)
{
  *contexts = info_list;
  *n_contexts = G_N_ELEMENTS (info_list);
}

GtkIMContext *
im_module_create (const gchar *context_id)
{
  if (strcmp (context_id, "ti_et") == 0)
    return GTK_IM_CONTEXT (g_object_new (type_ti_et_translit, NULL));
  else
    return NULL;
}
