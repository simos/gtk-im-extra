/* GTK - The GIMP Toolkit
 * Copyright (C) 2000 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <stdlib.h>

#include <gtk/gtkaccelgroup.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkimcontextsimple.h>
#include <gtk/gtkclipboard.h>

#include <gtk/gtkimcontext.h>
#include <gtk/gtkimmodule.h>

#include <gdk/gdkkeysyms.h>
#include <string.h>

#include "im-extra-intl.h"

typedef struct _GtkComposeTable GtkComposeTable;

struct _GtkComposeTable 
{
  guint16 *data;
  gint max_seq_len;
  gint n_seqs;
};

/* The following table is based substantially upon the table in gtkimcontextsimple.c in gtk+ 2.x
 * That table claims that it is generated from the X compose tables include with
 * XFree86 4.0 using a set of Perl scripts. See that file for more information.
 *
 * According to the copy of this comment in that file, the following 
 * compose letter sequences conflicted:
 *   Dstroke/dstroke and ETH/eth; resolved to Dstroke (Croatian, Vietnamese, Lappish), over 
 *                                ETH (Icelandic, Faroese, old English, IPA)   [ D- -D d- -d ]
 *   (I kept this but added [ D/ /D d/ /d ] for ETH/eth)
 *   Amacron/amacron and ordfeminine; resolved to ordfeminine                  [ _A A_ a_ _a ]
 *   Amacron/amacron and Atilde/atilde; resolved to amacron	      	      [ -A A- a- -a ]
 *   ( This is a change from gtkimcontextsimple.c. My (AOM's) reasons for this change are:
 *   first, that it is more consistant for '-' to always produce a macron on a vowel, and second,
 *   that this allows more characters to be covered. Atilde/atilde are already made by [ ~A A~ a~ ~a ]
 *   Omacron/Omacron and masculine; resolved to masculine		      [ _A A_ a_ _a ]
 *   Omacron/omacron and Otilde/atilde; resolved to omacron		      [ -O O- o- -o ]
 *   (see above)
 *
 * [ Amacron and Omacron are in Latin-4 (Baltic). ordfeminine and masculine are used for
 *   spanish. atilde and otilde are used at least for Portuguese ]
 *
 *   at and Aring; resolved to Aring					      [ AA ]
 *   guillemotleft and caron; resolved to guillemotleft			      [ << ]
 *   ogonek and cedilla; resolved to cedilla				      [ ,, ]
 * 
 * The purpose of this input method is to serve as a drop-in replacement for the default
 * gtk+2 input method, with a large number of extra characters available.
 */

static guint16 latinplus_compose_seqs[] = {
  GDK_dead_grave,	GDK_space,	0,	0,	0,	0x0060,	/* GRAVE_ACCENT */
  GDK_dead_grave,	GDK_A,	0,	0,	0,	0x00C0,	/* LATIN_CAPITAL_LETTER_A_WITH_GRAVE */
  GDK_dead_grave,	GDK_E,	0,	0,	0,	0x00C8,	/* LATIN_CAPITAL_LETTER_E_WITH_GRAVE */
  GDK_dead_grave,	GDK_I,	0,	0,	0,	0x00CC,	/* LATIN_CAPITAL_LETTER_I_WITH_GRAVE */
  GDK_dead_grave,	GDK_O,	0,	0,	0,	0x00D2,	/* LATIN_CAPITAL_LETTER_O_WITH_GRAVE */
  GDK_dead_grave,	GDK_U,	0,	0,	0,	0x00D9,	/* LATIN_CAPITAL_LETTER_U_WITH_GRAVE */
  GDK_dead_grave,	GDK_Y,	0,	0,	0,	0x1EF3,	/* LATIN_CAPITAL_LETTER_Y_WITH_GRAVE */
  GDK_dead_grave,	GDK_a,	0,	0,	0,	0x00E0,	/* LATIN_SMALL_LETTER_A_WITH_GRAVE */
  GDK_dead_grave,	GDK_e,	0,	0,	0,	0x00E8,	/* LATIN_SMALL_LETTER_E_WITH_GRAVE */
  GDK_dead_grave,	GDK_i,	0,	0,	0,	0x00EC,	/* LATIN_SMALL_LETTER_I_WITH_GRAVE */
  GDK_dead_grave,	GDK_o,	0,	0,	0,	0x00F2,	/* LATIN_SMALL_LETTER_O_WITH_GRAVE */
  GDK_dead_grave,	GDK_u,	0,	0,	0,	0x00F9,	/* LATIN_SMALL_LETTER_U_WITH_GRAVE */
  GDK_dead_grave,	GDK_y,	0,	0,	0,	0x1EF2,	/* LATIN_SMALL_LETTER_Y_WITH_GRAVE */
  GDK_dead_acute,	GDK_space,	0,	0,	0,	0x0027,	/* APOSTROPHE */
  GDK_dead_acute,	GDK_apostrophe,	0,	0,	0,	0x00B4,	/* ACUTE_ACCENT */
  GDK_dead_acute,	GDK_A,	0,	0,	0,	0x00C1,	/* LATIN_CAPITAL_LETTER_A_WITH_ACUTE */
  GDK_dead_acute,	GDK_C,	0,	0,	0,	0x00C7,	/* LATIN_CAPITAL_LETTER_C_WITH_CEDILLA */
  GDK_dead_acute,	GDK_E,	0,	0,	0,	0x00C9,	/* LATIN_CAPITAL_LETTER_E_WITH_ACUTE */
  GDK_dead_acute,	GDK_I,	0,	0,	0,	0x00CD,	/* LATIN_CAPITAL_LETTER_I_WITH_ACUTE */
  GDK_dead_acute,	GDK_O,	0,	0,	0,	0x00D3,	/* LATIN_CAPITAL_LETTER_O_WITH_ACUTE */
  GDK_dead_acute,	GDK_U,	0,	0,	0,	0x00DA,	/* LATIN_CAPITAL_LETTER_U_WITH_ACUTE */
  GDK_dead_acute,	GDK_Y,	0,	0,	0,	0x00DD,	/* LATIN_CAPITAL_LETTER_Y_WITH_ACUTE */
  GDK_dead_acute,	GDK_a,	0,	0,	0,	0x00E1,	/* LATIN_SMALL_LETTER_A_WITH_ACUTE */
  GDK_dead_acute,	GDK_c,	0,	0,	0,	0x00E7,	/* LATIN_SMALL_LETTER_C_WITH_CEDILLA */
  GDK_dead_acute,	GDK_e,	0,	0,	0,	0x00E9,	/* LATIN_SMALL_LETTER_E_WITH_ACUTE */
  GDK_dead_acute,	GDK_i,	0,	0,	0,	0x00ED,	/* LATIN_SMALL_LETTER_I_WITH_ACUTE */
  GDK_dead_acute,	GDK_o,	0,	0,	0,	0x00F3,	/* LATIN_SMALL_LETTER_O_WITH_ACUTE */
  GDK_dead_acute,	GDK_u,	0,	0,	0,	0x00FA,	/* LATIN_SMALL_LETTER_U_WITH_ACUTE */
  GDK_dead_acute,	GDK_y,	0,	0,	0,	0x00FD,	/* LATIN_SMALL_LETTER_Y_WITH_ACUTE */
  GDK_dead_acute,	GDK_acute,	0,	0,	0,	0x00B4,	/* ACUTE_ACCENT */
  GDK_dead_acute,	GDK_dead_acute,	0,	0,	0,	0x00B4,	/* ACUTE_ACCENT */
  GDK_dead_circumflex,	GDK_space,	0,	0,	0,	0x005E,	/* CIRCUMFLEX_ACCENT */
  GDK_dead_circumflex,	GDK_minus,	0,	0,	0,	0x00AF,	/* MACRON */
  GDK_dead_circumflex,	GDK_period,	0,	0,	0,	0x00B7,	/* MIDDLE_DOT */
  GDK_dead_circumflex,	GDK_slash,	0,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_dead_circumflex,	GDK_0,	0,	0,	0,	0x00B0,	/* DEGREE_SIGN */
  GDK_dead_circumflex,	GDK_1,	0,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_dead_circumflex,	GDK_2,	0,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_dead_circumflex,	GDK_3,	0,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_dead_circumflex,	GDK_A,	0,	0,	0,	0x00C2,	/* LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_C,	0,	0,	0,	0x0108,	/* LATIN_CAPITAL_LETTER_C_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_E,	0,	0,	0,	0x00CA,	/* LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_G,	0,	0,	0,	0x011C,	/* LATIN_CAPITAL_LETTER_G_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_H,	0,	0,	0,	0x0124,	/* LATIN_CAPITAL_LETTER_H_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_I,	0,	0,	0,	0x00CE,	/* LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_J,	0,	0,	0,	0x0134,	/* LATIN_CAPITAL_LETTER_J_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_O,	0,	0,	0,	0x00D4,	/* LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_S,	0,	0,	0,	0x015C,	/* LATIN_CAPITAL_LETTER_S_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_U,	0,	0,	0,	0x00DB,	/* LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_W,	0,	0,	0,	0x0174,	/* LATIN_CAPITAL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_Y,	0,	0,	0,	0x0176,	/* LATIN_CAPITAL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_underscore,	0,	0,	0,	0x00AF,	/* MACRON */
  GDK_dead_circumflex,	GDK_a,	0,	0,	0,	0x00E2,	/* LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_c,	0,	0,	0,	0x0109,	/* LATIN_SMALL_LETTER_C_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_e,	0,	0,	0,	0x00EA,	/* LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_g,	0,	0,	0,	0x011D,	/* LATIN_SMALL_LETTER_G_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_h,	0,	0,	0,	0x0125,	/* LATIN_SMALL_LETTER_H_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_i,	0,	0,	0,	0x00EE,	/* LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_j,	0,	0,	0,	0x0135,	/* LATIN_SMALL_LETTER_J_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_o,	0,	0,	0,	0x00F4,	/* LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_s,	0,	0,	0,	0x015D,	/* LATIN_SMALL_LETTER_S_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_u,	0,	0,	0,	0x00FB,	/* LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_w,	0,	0,	0,	0x0175,	/* LATIN_SMALL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_y,	0,	0,	0,	0x0177,	/* LATIN_SMALL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_dead_tilde,	GDK_space,	0,	0,	0,	0x007E,	/* TILDE */
  GDK_dead_tilde,	GDK_A,	0,	0,	0,	0x00C3,	/* LATIN_CAPITAL_LETTER_A_WITH_TILDE */
  GDK_dead_tilde,	GDK_I,	0,	0,	0,	0x0128,	/* LATIN_CAPITAL_LETTER_I_WITH_TILDE */
  GDK_dead_tilde,	GDK_N,	0,	0,	0,	0x00D1,	/* LATIN_CAPITAL_LETTER_N_WITH_TILDE */
  GDK_dead_tilde,	GDK_O,	0,	0,	0,	0x00D5,	/* LATIN_CAPITAL_LETTER_O_WITH_TILDE */
  GDK_dead_tilde,	GDK_U,	0,	0,	0,	0x0168,	/* LATIN_CAPITAL_LETTER_U_WITH_TILDE */
  GDK_dead_tilde,	GDK_a,	0,	0,	0,	0x00E3,	/* LATIN_SMALL_LETTER_A_WITH_TILDE */
  GDK_dead_tilde,	GDK_i,	0,	0,	0,	0x0129,	/* LATIN_SMALL_LETTER_I_WITH_TILDE */
  GDK_dead_tilde,	GDK_n,	0,	0,	0,	0x00F1,	/* LATIN_SMALL_LETTER_N_WITH_TILDE */
  GDK_dead_tilde,	GDK_o,	0,	0,	0,	0x00F5,	/* LATIN_SMALL_LETTER_O_WITH_TILDE */
  GDK_dead_tilde,	GDK_u,	0,	0,	0,	0x0169,	/* LATIN_SMALL_LETTER_U_WITH_TILDE */
  GDK_dead_tilde,	GDK_asciitilde,	0,	0,	0,	0x007E,	/* TILDE */
  GDK_dead_tilde,	GDK_dead_tilde,	0,	0,	0,	0x007E,	/* TILDE */
  GDK_dead_macron,	GDK_A,	0,	0,	0,	0x0100,	/* LATIN_CAPITAL_LETTER_A_WITH_MACRON */
  GDK_dead_macron,	GDK_E,	0,	0,	0,	0x0112,	/* LATIN_CAPITAL_LETTER_E_WITH_MACRON */
  GDK_dead_macron,	GDK_I,	0,	0,	0,	0x012A,	/* LATIN_CAPITAL_LETTER_I_WITH_MACRON */
  GDK_dead_macron,	GDK_O,	0,	0,	0,	0x014C,	/* LATIN_CAPITAL_LETTER_O_WITH_MACRON */
  GDK_dead_macron,	GDK_U,	0,	0,	0,	0x00D9,	/* LATIN_CAPITAL_LETTER_U_WITH_GRAVE */
  GDK_dead_macron,	GDK_a,	0,	0,	0,	0x0101,	/* LATIN_SMALL_LETTER_A_WITH_MACRON */
  GDK_dead_macron,	GDK_e,	0,	0,	0,	0x0113,	/* LATIN_SMALL_LETTER_E_WITH_MACRON */
  GDK_dead_macron,	GDK_i,	0,	0,	0,	0x012B,	/* LATIN_SMALL_LETTER_I_WITH_MACRON */
  GDK_dead_macron,	GDK_o,	0,	0,	0,	0x014D,	/* LATIN_SMALL_LETTER_O_WITH_MACRON */
  GDK_dead_macron,	GDK_u,	0,	0,	0,	0x016B,	/* LATIN_SMALL_LETTER_U_WITH_MACRON */
  GDK_dead_macron,	GDK_macron,	0,	0,	0,	0x00AF,	/* MACRON */
  GDK_dead_macron,	GDK_dead_macron,	0,	0,	0,	0x00AF,	/* MACRON */
  GDK_dead_breve,	GDK_A,	0,	0,	0,	0x0102,	/* LATIN_CAPITAL_LETTER_A_WITH_BREVE */
  GDK_dead_breve,	GDK_G,	0,	0,	0,	0x011E,	/* LATIN_CAPITAL_LETTER_G_WITH_BREVE */
  GDK_dead_breve,	GDK_U,	0,	0,	0,	0x016C,	/* LATIN_CAPITAL_LETTER_U_WITH_BREVE */
  GDK_dead_breve,	GDK_a,	0,	0,	0,	0x0103,	/* LATIN_SMALL_LETTER_A_WITH_BREVE */
  GDK_dead_breve,	GDK_g,	0,	0,	0,	0x011F,	/* LATIN_SMALL_LETTER_G_WITH_BREVE */
  GDK_dead_breve,	GDK_u,	0,	0,	0,	0x016D,	/* LATIN_SMALL_LETTER_U_WITH_BREVE */
  GDK_dead_abovedot,	GDK_E,	0,	0,	0,	0x0116,	/* LATIN_CAPITAL_LETTER_E_WITH_DOT_ABOVE */
  GDK_dead_abovedot,	GDK_I,	0,	0,	0,	0x0130,	/* LATIN_CAPITAL_LETTER_I_WITH_DOT_ABOVE */
  GDK_dead_abovedot,	GDK_e,	0,	0,	0,	0x0117,	/* LATIN_SMALL_LETTER_E_WITH_DOT_ABOVE */
  GDK_dead_abovedot,	GDK_i,	0,	0,	0,	0x0131,	/* LATIN_SMALL_LETTER_DOTLESS_I */
  GDK_dead_abovedot,	GDK_abovedot,	0,	0,	0,	0x02D9,	/* DOT_ABOVE */
  GDK_dead_abovedot,	GDK_dead_abovedot,	0,	0,	0,	0x02D9,	/* DOT_ABOVE */
  GDK_dead_diaeresis,	GDK_space,	0,	0,	0,	0x00A8,	/* DIAERESIS */
  GDK_dead_diaeresis,	GDK_quotedbl,	0,	0,	0,	0x00A8,	/* DIAERESIS */
  GDK_dead_diaeresis,	GDK_A,	0,	0,	0,	0x00C4,	/* LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_E,	0,	0,	0,	0x00CB,	/* LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_I,	0,	0,	0,	0x00CF,	/* LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_O,	0,	0,	0,	0x00D6,	/* LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_U,	0,	0,	0,	0x00DC,	/* LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_W,	0,	0,	0,	0x1E84,	/* LATIN_CAPITAL_LETTER_W_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_Y,	0,	0,	0,	0x0178,	/* LATIN_CAPITAL_LETTER_Y_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_a,	0,	0,	0,	0x00E4,	/* LATIN_SMALL_LETTER_A_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_e,	0,	0,	0,	0x00EB,	/* LATIN_SMALL_LETTER_E_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_i,	0,	0,	0,	0x00EF,	/* LATIN_SMALL_LETTER_I_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_o,	0,	0,	0,	0x00F6,	/* LATIN_SMALL_LETTER_O_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_u,	0,	0,	0,	0x00FC,	/* LATIN_SMALL_LETTER_U_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_w,	0,	0,	0,	0x1E85,	/* LATIN_SMALL_LETTER_W_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_y,	0,	0,	0,	0x00FF,	/* LATIN_SMALL_LETTER_Y_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_diaeresis,	0,	0,	0,	0x00A8,	/* DIAERESIS */
  GDK_dead_diaeresis,	GDK_dead_diaeresis,	0,	0,	0,	0x00A8,	/* DIAERESIS */
  GDK_dead_abovering,	GDK_A,	0,	0,	0,	0x00C5,	/* LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE */
  GDK_dead_abovering,	GDK_a,	0,	0,	0,	0x00E5,	/* LATIN_SMALL_LETTER_A_WITH_RING_ABOVE */
  GDK_dead_abovering,	GDK_dead_abovering,	0,	0,	0,	0x02DA,	/* RING_ABOVE */
  GDK_dead_caron,	GDK_C,	0,	0,	0,	0x010C,	/* LATIN_CAPITAL_LETTER_C_WITH_CARON */
  GDK_dead_caron,	GDK_D,	0,	0,	0,	0x010E,	/* LATIN_CAPITAL_LETTER_D_WITH_CARON */
  GDK_dead_caron,	GDK_E,	0,	0,	0,	0x011A,	/* LATIN_CAPITAL_LETTER_E_WITH_CARON */
  GDK_dead_caron,	GDK_S,	0,	0,	0,	0x0160,	/* LATIN_CAPITAL_LETTER_S_WITH_CARON */
  GDK_dead_caron,	GDK_Z,	0,	0,	0,	0x017D,	/* LATIN_CAPITAL_LETTER_Z_WITH_CARON */
  GDK_dead_caron,	GDK_c,	0,	0,	0,	0x010D,	/* LATIN_SMALL_LETTER_C_WITH_CARON */
  GDK_dead_caron,	GDK_d,	0,	0,	0,	0x010F,	/* LATIN_SMALL_LETTER_D_WITH_CARON */
  GDK_dead_caron,	GDK_e,	0,	0,	0,	0x011B,	/* LATIN_SMALL_LETTER_E_WITH_CARON */
  GDK_dead_caron,	GDK_s,	0,	0,	0,	0x0161,	/* LATIN_SMALL_LETTER_S_WITH_CARON */
  GDK_dead_caron,	GDK_z,	0,	0,	0,	0x017E,	/* LATIN_SMALL_LETTER_Z_WITH_CARON */
  GDK_dead_caron,	GDK_caron,	0,	0,	0,	0x02C7,	/* CARON */
  GDK_dead_caron,	GDK_dead_caron,	0,	0,	0,	0x02C7,	/* CARON */
  GDK_dead_cedilla,	GDK_comma,	0,	0,	0,	0x00B8,	/* CEDILLA */
  GDK_dead_cedilla,	GDK_minus,	0,	0,	0,	0x00AC,	/* NOT_SIGN */
  GDK_dead_cedilla,	GDK_C,	0,	0,	0,	0x00C7,	/* LATIN_CAPITAL_LETTER_C_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_G,	0,	0,	0,	0x0122,	/* LATIN_CAPITAL_LETTER_G_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_K,	0,	0,	0,	0x0136,	/* LATIN_CAPITAL_LETTER_K_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_L,	0,	0,	0,	0x013B,	/* LATIN_CAPITAL_LETTER_L_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_N,	0,	0,	0,	0x0145,	/* LATIN_CAPITAL_LETTER_N_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_R,	0,	0,	0,	0x0156,	/* LATIN_CAPITAL_LETTER_R_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_S,	0,	0,	0,	0x015E,	/* LATIN_CAPITAL_LETTER_S_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_c,	0,	0,	0,	0x00E7,	/* LATIN_SMALL_LETTER_C_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_g,	0,	0,	0,	0x0123,	/* LATIN_SMALL_LETTER_G_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_k,	0,	0,	0,	0x0137,	/* LATIN_SMALL_LETTER_K_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_l,	0,	0,	0,	0x013C,	/* LATIN_SMALL_LETTER_L_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_n,	0,	0,	0,	0x0146,	/* LATIN_SMALL_LETTER_N_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_r,	0,	0,	0,	0x0157,	/* LATIN_SMALL_LETTER_R_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_s,	0,	0,	0,	0x015F,	/* LATIN_SMALL_LETTER_S_WITH_CEDILLA */
  GDK_dead_cedilla,	GDK_cedilla,	0,	0,	0,	0x00B8,	/* CEDILLA */
  GDK_dead_cedilla,	GDK_dead_cedilla,	0,	0,	0,	0x00B8,	/* CEDILLA */
  GDK_dead_ogonek,	GDK_A,	0,	0,	0,	0x0104,	/* LATIN_CAPITAL_LETTER_A_WITH_OGONEK */
  GDK_dead_ogonek,	GDK_E,	0,	0,	0,	0x0118,	/* LATIN_CAPITAL_LETTER_E_WITH_OGONEK */
  GDK_dead_ogonek,	GDK_I,	0,	0,	0,	0x012E,	/* LATIN_CAPITAL_LETTER_I_WITH_OGONEK */
  GDK_dead_ogonek,	GDK_U,	0,	0,	0,	0x0172,	/* LATIN_CAPITAL_LETTER_U_WITH_OGONEK */
  GDK_dead_ogonek,	GDK_a,	0,	0,	0,	0x0105,	/* LATIN_SMALL_LETTER_A_WITH_OGONEK */
  GDK_dead_ogonek,	GDK_e,	0,	0,	0,	0x0119,	/* LATIN_SMALL_LETTER_E_WITH_OGONEK */
  GDK_dead_ogonek,	GDK_i,	0,	0,	0,	0x012F,	/* LATIN_SMALL_LETTER_I_WITH_OGONEK */
  GDK_dead_ogonek,	GDK_u,	0,	0,	0,	0x0173,	/* LATIN_SMALL_LETTER_U_WITH_OGONEK */
  GDK_dead_ogonek,	GDK_ogonek,	0,	0,	0,	0x02DB,	/* OGONEK */
  GDK_dead_ogonek,	GDK_dead_ogonek,	0,	0,	0,	0x02DB,	/* OGONEK */
  GDK_Multi_key,	GDK_space,	GDK_space,	0,	0,	0x00A0,	/* NOxBREAK_SPACE */
  GDK_Multi_key,	GDK_space,	GDK_apostrophe,	0,	0,	0x0027,	/* APOSTROPHE */
  GDK_Multi_key,	GDK_space,	GDK_minus,	0,	0,	0x007E,	/* TILDE */
  GDK_Multi_key,	GDK_space,	GDK_greater,	0,	0,	0x005E,	/* CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_space,	GDK_asciicircum,	0,	0,	0x005E,	/* CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_space,	GDK_grave,	0,	0,	0x0060,	/* GRAVE_ACCENT */
  GDK_Multi_key,	GDK_space,	GDK_asciitilde,	0,	0,	0x007E,	/* TILDE */
  GDK_Multi_key,	GDK_space,	GDK_Left,	0,	0,	0x2190,	/* LEFTWARDS_ARROW */
  GDK_Multi_key,	GDK_space,	GDK_Up,	0,	0,	0x2191,	/* UPWARDS_ARROW */
  GDK_Multi_key,	GDK_space,	GDK_Right,	0,	0,	0x2192,	/* RIGHTWARDS_ARROW */
  GDK_Multi_key,	GDK_space,	GDK_Down,	0,	0,	0x2193,	/* DOWNWARDS_ARROW */
  GDK_Multi_key,	GDK_space,	GDK_KP_Left,	0,	0,	0x2190,	/* LEFTWARDS_ARROW */
  GDK_Multi_key,	GDK_space,	GDK_KP_Up,	0,	0,	0x2191,	/* UPWARDS_ARROW */
  GDK_Multi_key,	GDK_space,	GDK_KP_Right,	0,	0,	0x2192,	/* RIGHTWARDS_ARROW */
  GDK_Multi_key,	GDK_space,	GDK_KP_Down,	0,	0,	0x2193,	/* DOWNWARDS_ARROW */
  GDK_Multi_key,	GDK_exclam,	GDK_exclam,	0,	0,	0x00A1,	/* INVERTED_EXCLAMATION_MARK */
  GDK_Multi_key,	GDK_exclam,	GDK_P,	0,	0,	0x00B6,	/* PILCROW_SIGN */
  GDK_Multi_key,	GDK_exclam,	GDK_S,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_exclam,	GDK_p,	0,	0,	0x00B6,	/* PILCROW_SIGN */
  GDK_Multi_key,	GDK_exclam,	GDK_s,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_quotedbl,	GDK_quotedbl,	0,	0,	0x00A8,	/* DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_A,	0,	0,	0x00C4,	/* LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_E,	0,	0,	0x00CB,	/* LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_I,	0,	0,	0x00CF,	/* LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_O,	0,	0,	0x00D6,	/* LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_U,	0,	0,	0x00DC,	/* LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_W,	0,	0,	0x1E84,	/* LATIN_CAPITAL_LETTER_W_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_Y,	0,	0,	0x0178,	/* LATIN_CAPITAL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_a,	0,	0,	0x00E4,	/* LATIN_SMALL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_e,	0,	0,	0x00EB,	/* LATIN_SMALL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_i,	0,	0,	0x00EF,	/* LATIN_SMALL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_o,	0,	0,	0x00F6,	/* LATIN_SMALL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_u,	0,	0,	0x00FC,	/* LATIN_SMALL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_w,	0,	0,	0x1E85,	/* LATIN_SMALL_LETTER_W_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_y,	0,	0,	0x00FF,	/* LATIN_SMALL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_Left,	0,	0,	0x0308,	/* COMBINING_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_KP_Left,	0,	0,	0x0308,	/* COMBINING_DIAERESIS */
  GDK_Multi_key,	GDK_numbersign,	GDK_numbersign,	0,	0,	0x266F,	/* MUSIC_SHARP_SIGN */
  GDK_Multi_key,	GDK_percent,	GDK_0,	0,	0,	0x2030,	/* PER_MILLE_SIGN */
  GDK_Multi_key,	GDK_ampersand,	GDK_A,	0,	0,	0x0391,	/* GREEK_CAPITAL_LETTER_ALPHA */
  GDK_Multi_key,	GDK_ampersand,	GDK_B,	0,	0,	0x0392,	/* GREEK_CAPITAL_LETTER_BETA */
  GDK_Multi_key,	GDK_ampersand,	GDK_C,	0,	0,	0x03A7,	/* GREEK_CAPITAL_LETTER_CHI */
  GDK_Multi_key,	GDK_ampersand,	GDK_D,	0,	0,	0x0394,	/* GREEK_CAPITAL_LETTER_DELTA */
  GDK_Multi_key,	GDK_ampersand,	GDK_E,	0,	0,	0x0395,	/* GREEK_CAPITAL_LETTER_EPSILON */
  GDK_Multi_key,	GDK_ampersand,	GDK_F,	0,	0,	0x03A6,	/* GREEK_CAPITAL_LETTER_PHI */
  GDK_Multi_key,	GDK_ampersand,	GDK_G,	0,	0,	0x0393,	/* GREEK_CAPITAL_LETTER_GAMMA */
  GDK_Multi_key,	GDK_ampersand,	GDK_H,	0,	0,	0x0397,	/* GREEK_CAPITAL_LETTER_ETA */
  GDK_Multi_key,	GDK_ampersand,	GDK_I,	0,	0,	0x0399,	/* GREEK_CAPITAL_LETTER_IOTA */
  GDK_Multi_key,	GDK_ampersand,	GDK_J,	0,	0,	0x03D1,	/* GREEK_THETA_SYMBOL */
  GDK_Multi_key,	GDK_ampersand,	GDK_K,	0,	0,	0x039A,	/* GREEK_CAPITAL_LETTER_KAPPA */
  GDK_Multi_key,	GDK_ampersand,	GDK_L,	0,	0,	0x039B,	/* GREEK_CAPITAL_LETTER_LAMBDA */
  GDK_Multi_key,	GDK_ampersand,	GDK_M,	0,	0,	0x039C,	/* GREEK_CAPITAL_LETTER_MU */
  GDK_Multi_key,	GDK_ampersand,	GDK_N,	0,	0,	0x039D,	/* GREEK_CAPITAL_LETTER_NU */
  GDK_Multi_key,	GDK_ampersand,	GDK_O,	0,	0,	0x039F,	/* GREEK_CAPITAL_LETTER_OMICRON */
  GDK_Multi_key,	GDK_ampersand,	GDK_P,	0,	0,	0x03A0,	/* GREEK_CAPITAL_LETTER_PI */
  GDK_Multi_key,	GDK_ampersand,	GDK_Q,	0,	0,	0x0398,	/* GREEK_CAPITAL_LETTER_THETA */
  GDK_Multi_key,	GDK_ampersand,	GDK_R,	0,	0,	0x03A1,	/* GREEK_CAPITAL_LETTER_RHO */
  GDK_Multi_key,	GDK_ampersand,	GDK_S,	0,	0,	0x03A3,	/* GREEK_CAPITAL_LETTER_SIGMA */
  GDK_Multi_key,	GDK_ampersand,	GDK_T,	0,	0,	0x03A4,	/* GREEK_CAPITAL_LETTER_TAU */
  GDK_Multi_key,	GDK_ampersand,	GDK_U,	0,	0,	0x03A5,	/* GREEK_CAPITAL_LETTER_UPSILON */
  GDK_Multi_key,	GDK_ampersand,	GDK_V,	0,	0,	0x03C2,	/* GREEK_SMALL_LETTER_FINAL_SIGMA */
  GDK_Multi_key,	GDK_ampersand,	GDK_W,	0,	0,	0x03A9,	/* GREEK_CAPITAL_LETTER_OMEGA */
  GDK_Multi_key,	GDK_ampersand,	GDK_X,	0,	0,	0x039E,	/* GREEK_CAPITAL_LETTER_XI */
  GDK_Multi_key,	GDK_ampersand,	GDK_Y,	0,	0,	0x03A8,	/* GREEK_CAPITAL_LETTER_PSI */
  GDK_Multi_key,	GDK_ampersand,	GDK_Z,	0,	0,	0x0396,	/* GREEK_CAPITAL_LETTER_ZETA */
  GDK_Multi_key,	GDK_ampersand,	GDK_a,	0,	0,	0x03B1,	/* GREEK_SMALL_LETTER_ALPHA */
  GDK_Multi_key,	GDK_ampersand,	GDK_b,	0,	0,	0x03B2,	/* GREEK_SMALL_LETTER_BETA */
  GDK_Multi_key,	GDK_ampersand,	GDK_c,	0,	0,	0x03C7,	/* GREEK_SMALL_LETTER_CHI */
  GDK_Multi_key,	GDK_ampersand,	GDK_d,	0,	0,	0x03B4,	/* GREEK_SMALL_LETTER_DELTA */
  GDK_Multi_key,	GDK_ampersand,	GDK_e,	0,	0,	0x03B5,	/* GREEK_SMALL_LETTER_EPSILON */
  GDK_Multi_key,	GDK_ampersand,	GDK_f,	0,	0,	0x03C6,	/* GREEK_SMALL_LETTER_PHI */
  GDK_Multi_key,	GDK_ampersand,	GDK_g,	0,	0,	0x03B3,	/* GREEK_SMALL_LETTER_GAMMA */
  GDK_Multi_key,	GDK_ampersand,	GDK_h,	0,	0,	0x03B7,	/* GREEK_SMALL_LETTER_ETA */
  GDK_Multi_key,	GDK_ampersand,	GDK_i,	0,	0,	0x03B9,	/* GREEK_SMALL_LETTER_IOTA */
  GDK_Multi_key,	GDK_ampersand,	GDK_j,	0,	0,	0x03D5,	/* GREEK_PHI_SYMBOL */
  GDK_Multi_key,	GDK_ampersand,	GDK_k,	0,	0,	0x03BA,	/* GREEK_SMALL_LETTER_KAPPA */
  GDK_Multi_key,	GDK_ampersand,	GDK_l,	0,	0,	0x03BB,	/* GREEK_SMALL_LETTER_LAMBDA */
  GDK_Multi_key,	GDK_ampersand,	GDK_m,	0,	0,	0x03BC,	/* GREEK_SMALL_LETTER_MU */
  GDK_Multi_key,	GDK_ampersand,	GDK_n,	0,	0,	0x03BD,	/* GREEK_SMALL_LETTER_NU */
  GDK_Multi_key,	GDK_ampersand,	GDK_o,	0,	0,	0x03BF,	/* GREEK_SMALL_LETTER_OMICRON */
  GDK_Multi_key,	GDK_ampersand,	GDK_p,	0,	0,	0x03C0,	/* GREEK_SMALL_LETTER_PI */
  GDK_Multi_key,	GDK_ampersand,	GDK_q,	0,	0,	0x03B8,	/* GREEK_SMALL_LETTER_THETA */
  GDK_Multi_key,	GDK_ampersand,	GDK_r,	0,	0,	0x03C1,	/* GREEK_SMALL_LETTER_RHO */
  GDK_Multi_key,	GDK_ampersand,	GDK_s,	0,	0,	0x03C3,	/* GREEK_SMALL_LETTER_SIGMA */
  GDK_Multi_key,	GDK_ampersand,	GDK_t,	0,	0,	0x03C4,	/* GREEK_SMALL_LETTER_TAU */
  GDK_Multi_key,	GDK_ampersand,	GDK_u,	0,	0,	0x03C5,	/* GREEK_SMALL_LETTER_UPSILON */
  GDK_Multi_key,	GDK_ampersand,	GDK_v,	0,	0,	0x03D6,	/* GREEK_PI_SYMBOL */
  GDK_Multi_key,	GDK_ampersand,	GDK_w,	0,	0,	0x03C9,	/* GREEK_SMALL_LETTER_OMEGA */
  GDK_Multi_key,	GDK_ampersand,	GDK_x,	0,	0,	0x03BE,	/* GREEK_SMALL_LETTER_XI */
  GDK_Multi_key,	GDK_ampersand,	GDK_y,	0,	0,	0x03C8,	/* GREEK_SMALL_LETTER_PSI */
  GDK_Multi_key,	GDK_ampersand,	GDK_z,	0,	0,	0x03B6,	/* GREEK_SMALL_LETTER_ZETA */
  GDK_Multi_key,	GDK_apostrophe,	GDK_space,	0,	0,	0x0027,	/* APOSTROPHE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_apostrophe,	0,	0,	0x00B4,	/* ACUTE_ACCENT */
  GDK_Multi_key,	GDK_apostrophe,	GDK_A,	0,	0,	0x00C1,	/* LATIN_CAPITAL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_C,	0,	0,	0x0106,	/* LATIN_CAPITAL_LETTER_C_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_E,	0,	0,	0x00C9,	/* LATIN_CAPITAL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_I,	0,	0,	0x00CD,	/* LATIN_CAPITAL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_L,	0,	0,	0x0139,	/* LATIN_CAPITAL_LETTER_L_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_N,	0,	0,	0x0143,	/* LATIN_CAPITAL_LETTER_N_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_O,	0,	0,	0x00D3,	/* LATIN_CAPITAL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_R,	0,	0,	0x0154,	/* LATIN_CAPITAL_LETTER_R_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_S,	0,	0,	0x015A,	/* LATIN_CAPITAL_LETTER_S_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_U,	0,	0,	0x00DA,	/* LATIN_CAPITAL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_Y,	0,	0,	0x00DD,	/* LATIN_CAPITAL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_Z,	0,	0,	0x0179,	/* LATIN_CAPITAL_LETTER_Z_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_a,	0,	0,	0x00E1,	/* LATIN_SMALL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_c,	0,	0,	0x0107,	/* LATIN_SMALL_LETTER_C_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_e,	0,	0,	0x00E9,	/* LATIN_SMALL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_i,	0,	0,	0x00ED,	/* LATIN_SMALL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_l,	0,	0,	0x013A,	/* LATIN_SMALL_LETTER_L_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_n,	0,	0,	0x0144,	/* LATIN_SMALL_LETTER_N_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_o,	0,	0,	0x00F3,	/* LATIN_SMALL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_r,	0,	0,	0x0155,	/* LATIN_SMALL_LETTER_R_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_s,	0,	0,	0x015B,	/* LATIN_SMALL_LETTER_S_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_u,	0,	0,	0x00FA,	/* LATIN_SMALL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_y,	0,	0,	0x00FD,	/* LATIN_SMALL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_z,	0,	0,	0x017A,	/* LATIN_SMALL_LETTER_Z_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_Left,	0,	0,	0x0301,	/* COMBINING_ACUTE_ACCENT */
  GDK_Multi_key,	GDK_apostrophe,	GDK_KP_Left,	0,	0,	0x0301,	/* COMBINING_ACUTE_ACCENT */
  GDK_Multi_key,	GDK_parenleft,	GDK_parenleft,	0,	0,	0x005B,	/* LEFT_SQUARE_BRACKET */
  GDK_Multi_key,	GDK_parenleft,	GDK_parenright,	0,	0,	0x25CB,	/* WHITE_CIRCLE */
  GDK_Multi_key,	GDK_parenleft,	GDK_asterisk,	0,	0,	0x262A,	/* STAR_AND_CRESCENT */
  GDK_Multi_key,	GDK_parenleft,	GDK_minus,	0,	0,	0x007B,	/* LEFT_CURLY_BRACKET */
  GDK_Multi_key,	GDK_parenleft,	GDK_A,	0,	0,	0x0102,	/* LATIN_CAPITAL_LETTER_A_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_E,	0,	0,	0x0114,	/* LATIN_CAPITAL_LETTER_E_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_G,	0,	0,	0x011E,	/* LATIN_CAPITAL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_I,	0,	0,	0x012C,	/* LATIN_CAPITAL_LETTER_I_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_O,	0,	0,	0x014E,	/* LATIN_CAPITAL_LETTER_O_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_U,	0,	0,	0x016C,	/* LATIN_CAPITAL_LETTER_U_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_c,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_parenleft,	GDK_a,	0,	0,	0x0103,	/* LATIN_SMALL_LETTER_A_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_e,	0,	0,	0x0115,	/* LATIN_SMALL_LETTER_E_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_g,	0,	0,	0x011F,	/* LATIN_SMALL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_i,	0,	0,	0x012D,	/* LATIN_SMALL_LETTER_I_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_o,	0,	0,	0x014F,	/* LATIN_SMALL_LETTER_O_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_r,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_parenleft,	GDK_u,	0,	0,	0x016D,	/* LATIN_SMALL_LETTER_U_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_Left,	0,	0,	0x0306,	/* COMBINING_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_KP_Left,	0,	0,	0x0306,	/* COMBINING_BREVE */
  GDK_Multi_key,	GDK_parenright,	GDK_parenright,	0,	0,	0x005D,	/* RIGHT_SQUARE_BRACKET */
  GDK_Multi_key,	GDK_parenright,	GDK_minus,	0,	0,	0x007D,	/* RIGHT_CURLY_BRACKET */
  GDK_Multi_key,	GDK_asterisk,	GDK_parenleft,	0,	0,	0x262A,	/* STAR_AND_CRESCENT */
  GDK_Multi_key,	GDK_asterisk,	GDK_asterisk,	0,	0,	0x2218,	/* RING_OPERATOR */
  GDK_Multi_key,	GDK_asterisk,	GDK_0,	0,	0,	0x00B0,	/* DEGREE_SIGN */
  GDK_Multi_key,	GDK_asterisk,	GDK_A,	0,	0,	0x00C5,	/* LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_asterisk,	GDK_U,	0,	0,	0x016E,	/* LATIN_CAPITAL_LETTER_U_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_asterisk,	GDK_a,	0,	0,	0x00E5,	/* LATIN_SMALL_LETTER_A_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_asterisk,	GDK_u,	0,	0,	0x016F,	/* LATIN_SMALL_LETTER_U_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_asterisk,	GDK_Left,	0,	0,	0x030A,	/* COMBINING_RING_ABOVE */
  GDK_Multi_key,	GDK_asterisk,	GDK_KP_Left,	0,	0,	0x030A,	/* COMBINING_RING_ABOVE */
  GDK_Multi_key,	GDK_plus,	GDK_plus,	0,	0,	0x0023,	/* NUMBER_SIGN */
  GDK_Multi_key,	GDK_plus,	GDK_minus,	0,	0,	0x00B1,	/* PLUSxMINUS_SIGN */
  GDK_Multi_key,	GDK_plus,	GDK_0,	0,	0,	0x2295,	/* CIRCLED_PLUS */
  GDK_Multi_key,	GDK_plus,	GDK_O,	0,	0,	0x2295,	/* CIRCLED_PLUS */
  GDK_Multi_key,	GDK_plus,	GDK_o,	0,	0,	0x2295,	/* CIRCLED_PLUS */
  GDK_Multi_key,	GDK_comma,	GDK_comma,	0,	0,	0x00B8,	/* CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_minus,	0,	0,	0x00AC,	/* NOT_SIGN */
  GDK_Multi_key,	GDK_comma,	GDK_A,	0,	0,	0x0104,	/* LATIN_CAPITAL_LETTER_A_WITH_OGONEK */
  GDK_Multi_key,	GDK_comma,	GDK_C,	0,	0,	0x00C7,	/* LATIN_CAPITAL_LETTER_C_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_E,	0,	0,	0x0118,	/* LATIN_CAPITAL_LETTER_E_WITH_OGONEK */
  GDK_Multi_key,	GDK_comma,	GDK_G,	0,	0,	0x0122,	/* LATIN_CAPITAL_LETTER_G_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_I,	0,	0,	0x012E,	/* LATIN_CAPITAL_LETTER_I_WITH_OGONEK */
  GDK_Multi_key,	GDK_comma,	GDK_K,	0,	0,	0x0136,	/* LATIN_CAPITAL_LETTER_K_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_L,	0,	0,	0x013B,	/* LATIN_CAPITAL_LETTER_L_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_N,	0,	0,	0x0145,	/* LATIN_CAPITAL_LETTER_N_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_R,	0,	0,	0x0156,	/* LATIN_CAPITAL_LETTER_R_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_S,	0,	0,	0x015E,	/* LATIN_CAPITAL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_T,	0,	0,	0x0162,	/* LATIN_CAPITAL_LETTER_T_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_U,	0,	0,	0x0172,	/* LATIN_CAPITAL_LETTER_U_WITH_OGONEK */
  GDK_Multi_key,	GDK_comma,	GDK_a,	0,	0,	0x0105,	/* LATIN_SMALL_LETTER_A_WITH_OGONEK */
  GDK_Multi_key,	GDK_comma,	GDK_c,	0,	0,	0x00E7,	/* LATIN_SMALL_LETTER_C_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_e,	0,	0,	0x0119,	/* LATIN_SMALL_LETTER_E_WITH_OGONEK */
  GDK_Multi_key,	GDK_comma,	GDK_g,	0,	0,	0x0123,	/* LATIN_SMALL_LETTER_G_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_i,	0,	0,	0x012F,	/* LATIN_SMALL_LETTER_I_WITH_OGONEK */
  GDK_Multi_key,	GDK_comma,	GDK_k,	0,	0,	0x0137,	/* LATIN_SMALL_LETTER_K_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_l,	0,	0,	0x013C,	/* LATIN_SMALL_LETTER_L_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_n,	0,	0,	0x0146,	/* LATIN_SMALL_LETTER_N_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_r,	0,	0,	0x0157,	/* LATIN_SMALL_LETTER_R_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_s,	0,	0,	0x015F,	/* LATIN_SMALL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_t,	0,	0,	0x0163,	/* LATIN_SMALL_LETTER_T_WITH_CEDILLA */
  GDK_Multi_key,	GDK_comma,	GDK_u,	0,	0,	0x0173,	/* LATIN_SMALL_LETTER_U_WITH_OGONEK */
  GDK_Multi_key,	GDK_comma,	GDK_Left,	0,	0,	0x0328,	/* COMBINING_OGONEK */
  GDK_Multi_key,	GDK_comma,	GDK_KP_Left,	0,	0,	0x0328,	/* COMBINING_OGONEK */
  GDK_Multi_key,	GDK_minus,	GDK_space,	0,	0,	0x007E,	/* TILDE */
  GDK_Multi_key,	GDK_minus,	GDK_parenleft,	0,	0,	0x007B,	/* LEFT_CURLY_BRACKET */
  GDK_Multi_key,	GDK_minus,	GDK_parenright,	0,	0,	0x007D,	/* RIGHT_CURLY_BRACKET */
  GDK_Multi_key,	GDK_minus,	GDK_plus,	0,	0,	0x00B1,	/* PLUSxMINUS_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_comma,	0,	0,	0x00AC,	/* NOT_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_minus,	0,	0,	0x00AD,	/* SOFT_HYPHEN */
  GDK_Multi_key,	GDK_minus,	GDK_colon,	0,	0,	0x00F7,	/* DIVISION_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_equal,	0,	0,	0x2261,	/* IDENTICAL_TO */
  GDK_Multi_key,	GDK_minus,	GDK_greater,	0,	0,	0x2192,	/* RIGHTWARDS_ARROW */
  GDK_Multi_key,	GDK_minus,	GDK_A,	0,	0,	0x0100,	/* LATIN_CAPITAL_LETTER_A_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_D,	0,	0,	0x0110,	/* LATIN_CAPITAL_LETTER_D_WITH_STROKE */
  GDK_Multi_key,	GDK_minus,	GDK_E,	0,	0,	0x0112,	/* LATIN_CAPITAL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_H,	0,	0,	0x0126,	/* LATIN_CAPITAL_LETTER_H_WITH_STROKE */
  GDK_Multi_key,	GDK_minus,	GDK_I,	0,	0,	0x012A,	/* LATIN_CAPITAL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_L,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_M,	0,	0,	0x2014,	/* EM_DASH */
  GDK_Multi_key,	GDK_minus,	GDK_N,	0,	0,	0x00D1,	/* LATIN_CAPITAL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_minus,	GDK_O,	0,	0,	0x014C,	/* LATIN_CAPITAL_LETTER_O_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_U,	0,	0,	0x016A,	/* LATIN_CAPITAL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_Y,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_bracketleft,	0,	0,	0x2208,	/* ELEMENT_OF */
  GDK_Multi_key,	GDK_minus,	GDK_bracketright,	0,	0,	0x220B,	/* CONTAINS_AS_MEMBER */
  GDK_Multi_key,	GDK_minus,	GDK_asciicircum,	0,	0,	0x00AF,	/* MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_a,	0,	0,	0x0101,	/* LATIN_SMALL_LETTER_A_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_d,	0,	0,	0x0111,	/* LATIN_SMALL_LETTER_D_WITH_STROKE */
  GDK_Multi_key,	GDK_minus,	GDK_e,	0,	0,	0x0113,	/* LATIN_SMALL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_h,	0,	0,	0x0127,	/* LATIN_SMALL_LETTER_H_WITH_STROKE */
  GDK_Multi_key,	GDK_minus,	GDK_i,	0,	0,	0x012B,	/* LATIN_SMALL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_l,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_m,	0,	0,	0x2014,	/* EM_DASH */
  GDK_Multi_key,	GDK_minus,	GDK_n,	0,	0,	0x00F1,	/* LATIN_SMALL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_minus,	GDK_o,	0,	0,	0x014D,	/* LATIN_SMALL_LETTER_O_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_s,	0,	0,	0x017F,	/* LATIN_SMALL_LETTER_LONG_S */
  GDK_Multi_key,	GDK_minus,	GDK_u,	0,	0,	0x016B,	/* LATIN_SMALL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_y,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_bar,	0,	0,	0x2020,	/* DAGGER */
  GDK_Multi_key,	GDK_minus,	GDK_Left,	0,	0,	0x0304,	/* COMBINING_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_KP_Left,	0,	0,	0x0304,	/* COMBINING_MACRON */
  GDK_Multi_key,	GDK_period,	GDK_period,	0,	0,	0x00B7,	/* MIDDLE_DOT */
  GDK_Multi_key,	GDK_period,	GDK_colon,	0,	0,	0x2234,	/* THEREFORE */
  GDK_Multi_key,	GDK_period,	GDK_B,	0,	0,	0x1E02,	/* LATIN_CAPITAL_LETTER_B_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_C,	0,	0,	0x010A,	/* LATIN_CAPITAL_LETTER_C_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_D,	0,	0,	0x1E0A,	/* LATIN_CAPITAL_LETTER_D_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_E,	0,	0,	0x0116,	/* LATIN_CAPITAL_LETTER_E_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_F,	0,	0,	0x1E1E,	/* LATIN_CAPITAL_LETTER_F_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_G,	0,	0,	0x0120,	/* LATIN_CAPITAL_LETTER_G_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_I,	0,	0,	0x0130,	/* LATIN_CAPITAL_LETTER_I_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_L,	0,	0,	0x013F,	/* LATIN_CAPITAL_LETTER_L_WITH_MIDDLE_DOT */
  GDK_Multi_key,	GDK_period,	GDK_M,	0,	0,	0x1E40,	/* LATIN_CAPITAL_LETTER_M_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_P,	0,	0,	0x1E56,	/* LATIN_CAPITAL_LETTER_P_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_S,	0,	0,	0x1E60,	/* LATIN_CAPITAL_LETTER_S_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_T,	0,	0,	0x1E6A,	/* LATIN_CAPITAL_LETTER_T_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_Z,	0,	0,	0x017B,	/* LATIN_CAPITAL_LETTER_Z_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_asciicircum,	0,	0,	0x02D9,	/* DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_b,	0,	0,	0x1E03,	/* LATIN_SMALL_LETTER_B_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_c,	0,	0,	0x010B,	/* LATIN_SMALL_LETTER_C_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_d,	0,	0,	0x1E0B,	/* LATIN_SMALL_LETTER_D_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_e,	0,	0,	0x0117,	/* LATIN_SMALL_LETTER_E_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_f,	0,	0,	0x1E1F,	/* LATIN_SMALL_LETTER_F_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_g,	0,	0,	0x0121,	/* LATIN_SMALL_LETTER_G_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_i,	0,	0,	0x0131,	/* LATIN_SMALL_LETTER_DOTLESS_I */
  GDK_Multi_key,	GDK_period,	GDK_l,	0,	0,	0x0140,	/* LATIN_SMALL_LETTER_L_WITH_MIDDLE_DOT */
  GDK_Multi_key,	GDK_period,	GDK_m,	0,	0,	0x1E41,	/* LATIN_SMALL_LETTER_M_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_p,	0,	0,	0x1E57,	/* LATIN_SMALL_LETTER_P_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_s,	0,	0,	0x1E61,	/* LATIN_SMALL_LETTER_S_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_t,	0,	0,	0x1E6B,	/* LATIN_SMALL_LETTER_T_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_z,	0,	0,	0x017C,	/* LATIN_SMALL_LETTER_Z_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_Left,	0,	0,	0x0307,	/* COMBINING_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_KP_Left,	0,	0,	0x0307,	/* COMBINING_DOT_ABOVE */
  GDK_Multi_key,	GDK_slash,	GDK_slash,	0,	0,	0x005C,	/* REVERSE_SOLIDUS */
  GDK_Multi_key,	GDK_slash,	GDK_0,	0,	0,	0x2205,	/* EMPTY_SET */
  GDK_Multi_key,	GDK_slash,	GDK_less,	0,	0,	0x005C,	/* REVERSE_SOLIDUS */
  GDK_Multi_key,	GDK_slash,	GDK_equal,	0,	0,	0x2260,	/* NOT_EQUAL_TO */
  GDK_Multi_key,	GDK_slash,	GDK_C,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_slash,	GDK_D,	0,	0,	0x00D0,	/* LATIN_CAPITAL_LETTER_ETH */
  GDK_Multi_key,	GDK_slash,	GDK_H,	0,	0,	0x210F,	/* PLANCK_CONSTANT_OVER_TWO_PI */
  GDK_Multi_key,	GDK_slash,	GDK_L,	0,	0,	0x0141,	/* LATIN_CAPITAL_LETTER_L_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_O,	0,	0,	0x00D8,	/* LATIN_CAPITAL_LETTER_O_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_R,	0,	0,	0x211E,	/* PRESCRIPTION_TAKE */
  GDK_Multi_key,	GDK_slash,	GDK_T,	0,	0,	0x0166,	/* LATIN_CAPITAL_LETTER_T_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_U,	0,	0,	0x00B5,	/* MICRO_SIGN */
  GDK_Multi_key,	GDK_slash,	GDK_backslash,	0,	0,	0x2227,	/* LOGICAL_AND */
  GDK_Multi_key,	GDK_slash,	GDK_asciicircum,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_Multi_key,	GDK_slash,	GDK_underscore,	0,	0,	0x2220,	/* ANGLE */
  GDK_Multi_key,	GDK_slash,	GDK_c,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_slash,	GDK_d,	0,	0,	0x00F0,	/* LATIN_SMALL_LETTER_ETH */
  GDK_Multi_key,	GDK_slash,	GDK_h,	0,	0,	0x210F,	/* PLANCK_CONSTANT_OVER_TWO_PI */
  GDK_Multi_key,	GDK_slash,	GDK_l,	0,	0,	0x0142,	/* LATIN_SMALL_LETTER_L_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_m,	0,	0,	0x20A5,	/* MILL_SIGN */
  GDK_Multi_key,	GDK_slash,	GDK_o,	0,	0,	0x00F8,	/* LATIN_SMALL_LETTER_O_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_r,	0,	0,	0x211E,	/* PRESCRIPTION_TAKE */
  GDK_Multi_key,	GDK_slash,	GDK_t,	0,	0,	0x0167,	/* LATIN_SMALL_LETTER_T_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_u,	0,	0,	0x00B5,	/* MICRO_SIGN */
  GDK_Multi_key,	GDK_slash,	GDK_v,	0,	0,	0x2123,	/* VERSICLE */
  GDK_Multi_key,	GDK_slash,	GDK_bar,	0,	0,	0x2224,	/* DOES_NOT_DIVIDE */
  GDK_Multi_key,	GDK_slash,	GDK_Left,	0,	0,	0x219A,	/* LEFTWARDS_ARROW_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_Right,	0,	0,	0x219B,	/* RIGHTWARDS_ARROW_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_KP_Left,	0,	0,	0x219A,	/* LEFTWARDS_ARROW_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_KP_Right,	0,	0,	0x219B,	/* RIGHTWARDS_ARROW_WITH_STROKE */
  GDK_Multi_key,	GDK_0,	GDK_parenleft,	0,	0,	0x221D,	/* PROPORTIONAL_TO */
  GDK_Multi_key,	GDK_0,	GDK_asterisk,	0,	0,	0x00B0,	/* DEGREE_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_plus,	0,	0,	0x2295,	/* CIRCLED_PLUS */
  GDK_Multi_key,	GDK_0,	GDK_slash,	0,	0,	0x2205,	/* EMPTY_SET */
  GDK_Multi_key,	GDK_0,	GDK_0,	0,	0,	0x221E,	/* INFINITY */
  GDK_Multi_key,	GDK_0,	GDK_C,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_P,	0,	0,	0x2117,	/* SOUND_RECORDING_COPYRIGHT */
  GDK_Multi_key,	GDK_0,	GDK_R,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_S,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_X,	0,	0,	0x2297,	/* CIRCLED_TIMES */
  GDK_Multi_key,	GDK_0,	GDK_asciicircum,	0,	0,	0x00B0,	/* DEGREE_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_underscore,	0,	0,	0x2080,	/* SUBSCRIPT_ZERO */
  GDK_Multi_key,	GDK_0,	GDK_c,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_p,	0,	0,	0x2117,	/* SOUND_RECORDING_COPYRIGHT */
  GDK_Multi_key,	GDK_0,	GDK_r,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_s,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_x,	0,	0,	0x2297,	/* CIRCLED_TIMES */
  GDK_Multi_key,	GDK_0,	GDK_Right,	0,	0,	0x21F4,	/* RIGHT_ARROW_WITH_SMALL_CIRCLE */
  GDK_Multi_key,	GDK_0,	GDK_KP_Right,	0,	0,	0x21F4,	/* RIGHT_ARROW_WITH_SMALL_CIRCLE */
  GDK_Multi_key,	GDK_1,	GDK_slash,	0,	0,	0x215F,	/* FRACTION_NUMERATOR_ONE */
  GDK_Multi_key,	GDK_1,	GDK_2,	0,	0,	0x00BD,	/* VULGAR_FRACTION_ONE_HALF */
  GDK_Multi_key,	GDK_1,	GDK_3,	0,	0,	0x2153,	/* VULGAR_FRACTION_ONE_THIRD */
  GDK_Multi_key,	GDK_1,	GDK_4,	0,	0,	0x00BC,	/* VULGAR_FRACTION_ONE_QUARTER */
  GDK_Multi_key,	GDK_1,	GDK_5,	0,	0,	0x2155,	/* VULGAR_FRACTION_ONE_FIFTH */
  GDK_Multi_key,	GDK_1,	GDK_6,	0,	0,	0x2159,	/* VULGAR_FRACTION_ONE_SIXTH */
  GDK_Multi_key,	GDK_1,	GDK_8,	0,	0,	0x215B,	/* VULGAR_FRACTION_ONE_EIGHTH */
  GDK_Multi_key,	GDK_1,	GDK_S,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_Multi_key,	GDK_1,	GDK_asciicircum,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_Multi_key,	GDK_1,	GDK_underscore,	0,	0,	0x2081,	/* SUBSCRIPT_ONE */
  GDK_Multi_key,	GDK_1,	GDK_s,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_Multi_key,	GDK_2,	GDK_3,	0,	0,	0x2154,	/* VULGAR FRACTION TWO THIRDS */
  GDK_Multi_key,	GDK_2,	GDK_5,	0,	0,	0x2156,	/* VULGAR FRACTION TWO FIFTHS */
  GDK_Multi_key,	GDK_2,	GDK_S,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_Multi_key,	GDK_2,	GDK_asciicircum,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_Multi_key,	GDK_2,	GDK_underscore,	0,	0,	0x2082,	/* SUBSCRIPT_TWO */
  GDK_Multi_key,	GDK_2,	GDK_s,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_Multi_key,	GDK_2,	GDK_Left,	0,	0,	0x21C7,	/* LEFTWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_2,	GDK_Up,	0,	0,	0x21C8,	/* UPWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_2,	GDK_Right,	0,	0,	0x21C9,	/* RIGHTWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_2,	GDK_Down,	0,	0,	0x21CA,	/* DOWNWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_2,	GDK_KP_Left,	0,	0,	0x21C7,	/* LEFTWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_2,	GDK_KP_Up,	0,	0,	0x21C8,	/* UPWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_2,	GDK_KP_Right,	0,	0,	0x21C9,	/* RIGHTWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_2,	GDK_KP_Down,	0,	0,	0x21CA,	/* DOWNWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_3,	GDK_4,	0,	0,	0x00BE,	/* VULGAR FRACTION THREE QUARTERS */
  GDK_Multi_key,	GDK_3,	GDK_5,	0,	0,	0x2157,	/* VULGAR FRACTION THREE FIFTHS */
  GDK_Multi_key,	GDK_3,	GDK_8,	0,	0,	0x215C,	/* VULGAR FRACTION THREE EIGHTHS */
  GDK_Multi_key,	GDK_3,	GDK_S,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_Multi_key,	GDK_3,	GDK_asciicircum,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_Multi_key,	GDK_3,	GDK_underscore,	0,	0,	0x2083,	/* SUBSCRIPT_THREE */
  GDK_Multi_key,	GDK_3,	GDK_s,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_Multi_key,	GDK_3,	GDK_Right,	0,	0,	0x21F6,	/* THREE_RIGHTWARDS_ARROWS */
  GDK_Multi_key,	GDK_3,	GDK_KP_Right,	0,	0,	0x21F6,	/* THREE_RIGHTWARDS_ARROWS */
  GDK_Multi_key,	GDK_4,	GDK_5,	0,	0,	0x2158,	/* VULGAR FRACTION FOUR FIFTHS */
  GDK_Multi_key,	GDK_4,	GDK_S,	0,	0,	0x2074,	/* SUPERSCRIPT_FOUR */
  GDK_Multi_key,	GDK_4,	GDK_asciicircum,	0,	0,	0x2074,	/* SUPERSCRIPT_FOUR */
  GDK_Multi_key,	GDK_4,	GDK_underscore,	0,	0,	0x2084,	/* SUBSCRIPT_FOUR */
  GDK_Multi_key,	GDK_4,	GDK_s,	0,	0,	0x2074,	/* SUPERSCRIPT_FOUR */
  GDK_Multi_key,	GDK_5,	GDK_6,	0,	0,	0x215A,	/* VULGAR FRACTION FIVE SIXTHS */
  GDK_Multi_key,	GDK_5,	GDK_8,	0,	0,	0x215D,	/* VULGAR FRACTION FIVE EIGHTHS */
  GDK_Multi_key,	GDK_5,	GDK_S,	0,	0,	0x2075,	/* SUPERSCRIPT_FIVE */
  GDK_Multi_key,	GDK_5,	GDK_asciicircum,	0,	0,	0x2075,	/* SUPERSCRIPT_FIVE */
  GDK_Multi_key,	GDK_5,	GDK_underscore,	0,	0,	0x2085,	/* SUBSCRIPT_FIVE */
  GDK_Multi_key,	GDK_5,	GDK_s,	0,	0,	0x2076,	/* SUPERSCRIPT_FIVE */
  GDK_Multi_key,	GDK_6,	GDK_S,	0,	0,	0x2076,	/* SUPERSCRIPT_SIX */
  GDK_Multi_key,	GDK_6,	GDK_asciicircum,	0,	0,	0x2076,	/* SUPERSCRIPT_SIX */
  GDK_Multi_key,	GDK_6,	GDK_underscore,	0,	0,	0x2086,	/* SUBSCRIPT_SIX */
  GDK_Multi_key,	GDK_6,	GDK_s,	0,	0,	0x2076,	/* SUPERSCRIPT_SIX */
  GDK_Multi_key,	GDK_7,	GDK_8,	0,	0,	0x215E,	/* VULGAR FRACTION SEVEN EIGHTHS */
  GDK_Multi_key,	GDK_7,	GDK_S,	0,	0,	0x2077,	/* SUPERSCRIPT_SEVEN */
  GDK_Multi_key,	GDK_7,	GDK_asciicircum,	0,	0,	0x2077,	/* SUPERSCRIPT_SEVEN */
  GDK_Multi_key,	GDK_7,	GDK_underscore,	0,	0,	0x2087,	/* SUBSCRIPT_SEVEN */
  GDK_Multi_key,	GDK_7,	GDK_s,	0,	0,	0x2077,	/* SUPERSCRIPT_SEVEN */
  GDK_Multi_key,	GDK_8,	GDK_8,	0,	0,	0x221E,	/* INFINITY */
  GDK_Multi_key,	GDK_8,	GDK_S,	0,	0,	0x2078,	/* SUPERSCRIPT_EIGHT */
  GDK_Multi_key,	GDK_8,	GDK_asciicircum,	0,	0,	0x2078,	/* SUPERSCRIPT_EIGHT */
  GDK_Multi_key,	GDK_8,	GDK_underscore,	0,	0,	0x2088,	/* SUBSCRIPT_EIGHT */
  GDK_Multi_key,	GDK_8,	GDK_s,	0,	0,	0x2078,	/* SUPERSCRIPT_EIGHT */
  GDK_Multi_key,	GDK_9,	GDK_S,	0,	0,	0x2079,	/* SUPERSCRIPT_NINE */
  GDK_Multi_key,	GDK_9,	GDK_asciicircum,	0,	0,	0x2079,	/* SUPERSCRIPT_NINE */
  GDK_Multi_key,	GDK_9,	GDK_underscore,	0,	0,	0x2089,	/* SUBSCRIPT_NINE */
  GDK_Multi_key,	GDK_9,	GDK_s,	0,	0,	0x2079,	/* SUPERSCRIPT_NINE */
  GDK_Multi_key,	GDK_colon,	GDK_parenleft,	0,	0,	0x2639,	/* WHITE_FROWNING_FACE */
  GDK_Multi_key,	GDK_colon,	GDK_parenright,	0,	0,	0x263A,	/* WHITE_SMILING_FACE */
  GDK_Multi_key,	GDK_colon,	GDK_minus,	0,	0,	0x00F7,	/* DIVISION_SIGN */
  GDK_Multi_key,	GDK_colon,	GDK_period,	0,	0,	0x2234,	/* THEREFORE */
  GDK_Multi_key,	GDK_colon,	GDK_colon,	0,	0,	0x2237,	/* PROPORTION */
  GDK_Multi_key,	GDK_colon,	GDK_O,	0,	0,	0x0150,	/* LATIN_CAPITAL_LETTER_O_WITH_DOUBLE_ACUTE */
  GDK_Multi_key,	GDK_colon,	GDK_U,	0,	0,	0x0170,	/* LATIN_CAPITAL_LETTER_U_WITH_DOUBLE_ACUTE */
  GDK_Multi_key,	GDK_colon,	GDK_o,	0,	0,	0x0151,	/* LATIN_SMALL_LETTER_O_WITH_DOUBLE_ACUTE */
  GDK_Multi_key,	GDK_colon,	GDK_u,	0,	0,	0x0171,	/* LATIN_SMALL_LETTER_U_WITH_DOUBLE_ACUTE */
  GDK_Multi_key,	GDK_colon,	GDK_Left,	0,	0,	0x030B,	/* COMBINING_DOUBLE_ACUTE_ACCENT */
  GDK_Multi_key,	GDK_colon,	GDK_KP_Left,	0,	0,	0x030B,	/* COMBINING_DOUBLE_ACUTE_ACCENT */
  GDK_Multi_key,	GDK_less,	GDK_minus,	0,	0,	0x2190,	/* LEFTWARDS_ARROW */
  GDK_Multi_key,	GDK_less,	GDK_slash,	0,	0,	0x005C,	/* REVERSE_SOLIDUS */
  GDK_Multi_key,	GDK_less,	GDK_3,	0,	0,	0x2665,	/* BLACK HEART SUIT */
  GDK_Multi_key,	GDK_less,	GDK_less,	0,	0,	0x00AB,	/* LEFTxPOINTING_DOUBLE_ANGLE_QUOTATION_MARK */
  GDK_Multi_key,	GDK_less,	GDK_equal,	0,	0,	0x2264,	/* LESS-THAN_OR_EQUAL_TO */
  GDK_Multi_key,	GDK_less,	GDK_greater,	0,	0,	0x25C7,	/* WHITE_DIAMOND */
  GDK_Multi_key,	GDK_less,	GDK_C,	0,	0,	0x010C,	/* LATIN_CAPITAL_LETTER_C_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_D,	0,	0,	0x010E,	/* LATIN_CAPITAL_LETTER_D_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_E,	0,	0,	0x011A,	/* LATIN_CAPITAL_LETTER_E_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_L,	0,	0,	0x013D,	/* LATIN_CAPITAL_LETTER_L_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_N,	0,	0,	0x0147,	/* LATIN_CAPITAL_LETTER_N_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_R,	0,	0,	0x0158,	/* LATIN_CAPITAL_LETTER_R_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_S,	0,	0,	0x0160,	/* LATIN_CAPITAL_LETTER_S_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_T,	0,	0,	0x0164,	/* LATIN_CAPITAL_LETTER_T_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_Z,	0,	0,	0x017D,	/* LATIN_CAPITAL_LETTER_Z_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_c,	0,	0,	0x010D,	/* LATIN_SMALL_LETTER_C_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_d,	0,	0,	0x010F,	/* LATIN_SMALL_LETTER_D_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_e,	0,	0,	0x011B,	/* LATIN_SMALL_LETTER_E_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_l,	0,	0,	0x013E,	/* LATIN_SMALL_LETTER_L_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_n,	0,	0,	0x0148,	/* LATIN_SMALL_LETTER_N_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_r,	0,	0,	0x0159,	/* LATIN_SMALL_LETTER_R_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_s,	0,	0,	0x0161,	/* LATIN_SMALL_LETTER_S_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_t,	0,	0,	0x0165,	/* LATIN_SMALL_LETTER_T_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_z,	0,	0,	0x017E,	/* LATIN_SMALL_LETTER_Z_WITH_CARON */
  GDK_Multi_key,	GDK_less,	GDK_Left,	0,	0,	0x030C,	/* COMBINING_CARON */
  GDK_Multi_key,	GDK_less,	GDK_KP_Left,	0,	0,	0x030C,	/* COMBINING_CARON */
  GDK_Multi_key,	GDK_less,	GDK_bar,	0,	0,	0x2206,	/* INCREMENT */
  GDK_Multi_key,	GDK_equal,	GDK_minus,	0,	0,	0x2261,	/* IDENTICAL_TO */
  GDK_Multi_key,	GDK_equal,	GDK_slash,	0,	0,	0x2260,	/* NOT_EQUAL_TO */
  GDK_Multi_key,	GDK_equal,	GDK_less,	0,	0,	0x2264,	/* LESS-THAN_OR_EQUAL_TO */
  GDK_Multi_key,	GDK_equal,	GDK_greater,	0,	0,	0x2265,	/* GREATER-THAN_OR_EQUAL_TO */
  GDK_Multi_key,	GDK_equal,	GDK_C,	0,	0,	0x20AC,	/* EURO_SIGN */
  GDK_Multi_key,	GDK_equal,	GDK_E,	0,	0,	0x20AC,	/* EURO_SIGN */
  GDK_Multi_key,	GDK_equal,	GDK_L,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_equal,	GDK_Y,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_equal,	GDK_e,	0,	0,	0x20AC,	/* EURO_SIGN */
  GDK_Multi_key,	GDK_equal,	GDK_l,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_equal,	GDK_y,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_equal,	GDK_bar,	0,	0,	0x2021,	/* DOUBLE_DAGGER */
  GDK_Multi_key,	GDK_equal,	GDK_Left,	0,	0,	0x21D0,	/* LEFTWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_equal,	GDK_Up,	0,	0,	0x21D1,	/* UPWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_equal,	GDK_Right,	0,	0,	0x21D2,	/* RIGHTWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_equal,	GDK_Down,	0,	0,	0x21D3,	/* DOWNWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_equal,	GDK_KP_Left,	0,	0,	0x21D0,	/* LEFTWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_equal,	GDK_KP_Up,	0,	0,	0x21D1,	/* UPWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_equal,	GDK_KP_Right,	0,	0,	0x21D2,	/* RIGHTWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_equal,	GDK_KP_Down,	0,	0,	0x21D3,	/* DOWNWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_greater,	GDK_space,	0,	0,	0x005E,	/* CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_greater,	GDK_equal,	0,	0,	0x2265,	/* GREATER-THAN_OR_EQUAL_TO */
  GDK_Multi_key,	GDK_greater,	GDK_greater,	0,	0,	0x00BB,	/* RIGHTxPOINTING_DOUBLE_ANGLE_QUOTATION_MARK */
  GDK_Multi_key,	GDK_greater,	GDK_A,	0,	0,	0x00C2,	/* LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_C,	0,	0,	0x0108,	/* LATIN_CAPITAL_LETTER_C_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_E,	0,	0,	0x00CA,	/* LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_G,	0,	0,	0x011C,	/* LATIN_CAPITAL_LETTER_G_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_H,	0,	0,	0x0124,	/* LATIN_CAPITAL_LETTER_H_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_I,	0,	0,	0x00CE,	/* LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_J,	0,	0,	0x0134,	/* LATIN_CAPITAL_LETTER_J_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_O,	0,	0,	0x00D4,	/* LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_S,	0,	0,	0x015C,	/* LATIN_CAPITAL_LETTER_S_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_U,	0,	0,	0x00DB,	/* LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_W,	0,	0,	0x0174,	/* LATIN_CAPITAL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_Y,	0,	0,	0x0176,	/* LATIN_CAPITAL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_a,	0,	0,	0x00E2,	/* LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_c,	0,	0,	0x0109,	/* LATIN_SMALL_LETTER_C_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_e,	0,	0,	0x00EA,	/* LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_g,	0,	0,	0x011D,	/* LATIN_SMALL_LETTER_G_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_h,	0,	0,	0x0125,	/* LATIN_SMALL_LETTER_H_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_i,	0,	0,	0x00EE,	/* LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_j,	0,	0,	0x0135,	/* LATIN_SMALL_LETTER_J_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_o,	0,	0,	0x00F4,	/* LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_s,	0,	0,	0x015D,	/* LATIN_SMALL_LETTER_S_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_u,	0,	0,	0x00FB,	/* LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_w,	0,	0,	0x0175,	/* LATIN_SMALL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_y,	0,	0,	0x0177,	/* LATIN_SMALL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_Left,	0,	0,	0x0302,	/* COMBINING_CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_greater,	GDK_KP_Left,	0,	0,	0x0302,	/* COMBINING_CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_question,	GDK_question,	0,	0,	0x00BF,	/* INVERTED_QUESTION_MARK */
  GDK_Multi_key,	GDK_A,	GDK_quotedbl,	0,	0,	0x00C4,	/* LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_A,	GDK_apostrophe,	0,	0,	0x00C1,	/* LATIN_CAPITAL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_A,	GDK_parenleft,	0,	0,	0x0102,	/* LATIN_CAPITAL_LETTER_A_WITH_BREVE */
  GDK_Multi_key,	GDK_A,	GDK_asterisk,	0,	0,	0x00C5,	/* LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_A,	GDK_comma,	0,	0,	0x0104,	/* LATIN_CAPITAL_LETTER_A_WITH_OGONEK */
  GDK_Multi_key,	GDK_A,	GDK_minus,	0,	0,	0x0100,	/* LATIN_CAPITAL_LETTER_A_WITH_MACRON */
  GDK_Multi_key,	GDK_A,	GDK_greater,	0,	0,	0x00C2,	/* LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_A,	GDK_A,	0,	0,	0x00C5,	/* LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_A,	GDK_E,	0,	0,	0x00C6,	/* LATIN_CAPITAL_LETTER_AE */
  GDK_Multi_key,	GDK_A,	GDK_asciicircum,	0,	0,	0x00C2,	/* LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_A,	GDK_underscore,	0,	0,	0x00AA,	/* FEMININE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_A,	GDK_grave,	0,	0,	0x00C0,	/* LATIN_CAPITAL_LETTER_A_WITH_GRAVE */
  GDK_Multi_key,	GDK_A,	GDK_asciitilde,	0,	0,	0x00C3,	/* LATIN_CAPITAL_LETTER_A_WITH_TILDE */
  GDK_Multi_key,	GDK_A,	GDK_diaeresis,	0,	0,	0x00C4,	/* LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_A,	GDK_acute,	0,	0,	0x00C1,	/* LATIN_CAPITAL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_A,	GDK_breve,	0,	0,	0x0102,	/* LATIN_CAPITAL_LETTER_A_WITH_BREVE */
  GDK_Multi_key,	GDK_B,	GDK_period,	0,	0,	0x1E02,	/* LATIN_CAPITAL_LETTER_B_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_C,	GDK_apostrophe,	0,	0,	0x0106,	/* LATIN_CAPITAL_LETTER_C_WITH_ACUTE */
  GDK_Multi_key,	GDK_C,	GDK_comma,	0,	0,	0x00C7,	/* LATIN_CAPITAL_LETTER_C_WITH_CEDILLA */
  GDK_Multi_key,	GDK_C,	GDK_period,	0,	0,	0x010A,	/* LATIN_CAPITAL_LETTER_C_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_C,	GDK_slash,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_0,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_less,	0,	0,	0x010C,	/* LATIN_CAPITAL_LETTER_C_WITH_CARON */
  GDK_Multi_key,	GDK_C,	GDK_equal,	0,	0,	0x20AC,	/* EURO_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_greater,	0,	0,	0x0108,	/* LATIN_CAPITAL_LETTER_C_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_C,	GDK_L,	0,	0,	0x2104,	/* CENTRE_LINE_SYMBOL */
  GDK_Multi_key,	GDK_C,	GDK_O,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_R,	0,	0,	0x20A2,	/* CRUZEIRO_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_asciicircum,	0,	0,	0x0108,	/* LATIN_CAPITAL_LETTER_C_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_C,	GDK_o,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_r,	0,	0,	0x20A2,	/* CRUZEIRO_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_bar,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_acute,	0,	0,	0x0106,	/* LATIN_CAPITAL_LETTER_C_WITH_ACUTE */
  GDK_Multi_key,	GDK_C,	GDK_cedilla,	0,	0,	0x00C7,	/* LATIN_CAPITAL_LETTER_C_WITH_CEDILLA */
  GDK_Multi_key,	GDK_D,	GDK_minus,	0,	0,	0x0110,	/* LATIN_CAPITAL_LETTER_D_WITH_STROKE */
  GDK_Multi_key,	GDK_D,	GDK_period,	0,	0,	0x1E0A,	/* LATIN_CAPITAL_LETTER_D_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_D,	GDK_slash,	0,	0,	0x00D0,	/* LATIN_CAPITAL_LETTER_ETH */
  GDK_Multi_key,	GDK_D,	GDK_less,	0,	0,	0x010E,	/* LATIN_CAPITAL_LETTER_D_WITH_CARON */
  GDK_Multi_key,	GDK_E,	GDK_quotedbl,	0,	0,	0x00CB,	/* LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_E,	GDK_apostrophe,	0,	0,	0x00C9,	/* LATIN_CAPITAL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_E,	GDK_parenleft,	0,	0,	0x0114,	/* LATIN_CAPITAL_LETTER_E_WITH_BREVE */
  GDK_Multi_key,	GDK_E,	GDK_comma,	0,	0,	0x0118,	/* LATIN_CAPITAL_LETTER_E_WITH_OGONEK */
  GDK_Multi_key,	GDK_E,	GDK_minus,	0,	0,	0x0112,	/* LATIN_CAPITAL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_E,	GDK_period,	0,	0,	0x0116,	/* LATIN_CAPITAL_LETTER_E_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_E,	GDK_less,	0,	0,	0x011A,	/* LATIN_CAPITAL_LETTER_E_WITH_CARON */
  GDK_Multi_key,	GDK_E,	GDK_equal,	0,	0,	0x20AC,	/* EURO_SIGN */
  GDK_Multi_key,	GDK_E,	GDK_greater,	0,	0,	0x00CA,	/* LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_E,	GDK_E,          0,	0,	0x2203,	/* THERE_EXISTS */
  GDK_Multi_key,	GDK_E,	GDK_asciicircum,	0,	0,	0x00CA,	/* LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_E,	GDK_underscore,	0,	0,	0x0112,	/* LATIN_CAPITAL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_E,	GDK_grave,	0,	0,	0x00C8,	/* LATIN_CAPITAL_LETTER_E_WITH_GRAVE */
  GDK_Multi_key,	GDK_E,	GDK_diaeresis,	0,	0,	0x00CB,	/* LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_E,	GDK_acute,	0,	0,	0x00C9,	/* LATIN_CAPITAL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_E,	GDK_breve,	0,	0,	0x0114,	/* LATIN_CAPITAL_LETTER_E_WITH_BREVE */
  GDK_Multi_key,	GDK_F,	GDK_period,	0,	0,	0x1E1E,	/* LATIN_CAPITAL_LETTER_F_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_G,	GDK_parenleft,	0,	0,	0x011E,	/* LATIN_CAPITAL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_G,	GDK_comma,	0,	0,	0x0122,	/* LATIN_CAPITAL_LETTER_G_WITH_CEDILLA */
  GDK_Multi_key,	GDK_G,	GDK_period,	0,	0,	0x0120,	/* LATIN_CAPITAL_LETTER_G_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_G,	GDK_greater,	0,	0,	0x011C,	/* LATIN_CAPITAL_LETTER_G_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_G,	GDK_U,	0,	0,	0x011E,	/* LATIN_CAPITAL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_G,	GDK_asciicircum,	0,	0,	0x011C,	/* LATIN_CAPITAL_LETTER_G_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_G,	GDK_cedilla,	0,	0,	0x0122,	/* LATIN_CAPITAL_LETTER_G_WITH_CEDILLA */
  GDK_Multi_key,	GDK_G,	GDK_breve,	0,	0,	0x011E,	/* LATIN_CAPITAL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_H,	GDK_minus,	0,	0,	0x0126,	/* LATIN_CAPITAL_LETTER_H_WITH_STROKE */
  GDK_Multi_key,	GDK_H,	GDK_slash,	0,	0,	0x210F,	/* PLANCK_CONSTANT_OVER_TWO_PI */
  GDK_Multi_key,	GDK_H,	GDK_greater,	0,	0,	0x0124,	/* LATIN_CAPITAL_LETTER_H_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_H,	GDK_asciicircum,	0,	0,	0x0124,	/* LATIN_CAPITAL_LETTER_H_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_I,	GDK_quotedbl,	0,	0,	0x00CF,	/* LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_I,	GDK_apostrophe,	0,	0,	0x00CD,	/* LATIN_CAPITAL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_I,	GDK_parenleft,	0,	0,	0x012C,	/* LATIN_CAPITAL_LETTER_I_WITH_BREVE */
  GDK_Multi_key,	GDK_I,	GDK_comma,	0,	0,	0x012E,	/* LATIN_CAPITAL_LETTER_I_WITH_OGONEK */
  GDK_Multi_key,	GDK_I,	GDK_minus,	0,	0,	0x012A,	/* LATIN_CAPITAL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_I,	GDK_period,	0,	0,	0x0130,	/* LATIN_CAPITAL_LETTER_I_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_I,	GDK_greater,	0,	0,	0x00CE,	/* LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_I,	GDK_asciicircum,	0,	0,	0x00CE,	/* LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_I,	GDK_underscore,	0,	0,	0x012A,	/* LATIN_CAPITAL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_I,	GDK_grave,	0,	0,	0x00CC,	/* LATIN_CAPITAL_LETTER_I_WITH_GRAVE */
  GDK_Multi_key,	GDK_I,	GDK_J,	0,	0,	0x0132,	/* LATIN_CAPITAL_LIGATURE_IJ */
  GDK_Multi_key,	GDK_I,	GDK_asciitilde,	0,	0,	0x0128,	/* LATIN_CAPITAL_LETTER_I_WITH_TILDE */
  GDK_Multi_key,	GDK_I,	GDK_diaeresis,	0,	0,	0x00CF,	/* LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_I,	GDK_acute,	0,	0,	0x00CD,	/* LATIN_CAPITAL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_I,	GDK_breve,	0,	0,	0x012C,	/* LATIN_CAPITAL_LETTER_I_WITH_BREVE */
  GDK_Multi_key,	GDK_J,	GDK_greater,	0,	0,	0x0134,	/* LATIN_CAPITAL_LETTER_J_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_J,	GDK_asciicircum,	0,	0,	0x0134,	/* LATIN_CAPITAL_LETTER_J_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_K,	GDK_comma,	0,	0,	0x0136,	/* LATIN_CAPITAL_LETTER_K_WITH_CEDILLA */
  GDK_Multi_key,	GDK_K,	GDK_cedilla,	0,	0,	0x0136,	/* LATIN_CAPITAL_LETTER_K_WITH_CEDILLA */
  GDK_Multi_key,	GDK_L,	GDK_apostrophe,	0,	0,	0x0139,	/* LATIN_CAPITAL_LETTER_L_WITH_ACUTE */
  GDK_Multi_key,	GDK_L,	GDK_comma,	0,	0,	0x013B,	/* LATIN_CAPITAL_LETTER_L_WITH_CEDILLA */
  GDK_Multi_key,	GDK_L,	GDK_minus,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_L,	GDK_period,	0,	0,	0x013F,	/* LATIN_CAPITAL_LETTER_L_WITH_MIDDLE_DOT */
  GDK_Multi_key,	GDK_L,	GDK_slash,	0,	0,	0x0141,	/* LATIN_CAPITAL_LETTER_L_WITH_STROKE */
  GDK_Multi_key,	GDK_L,	GDK_less,	0,	0,	0x013D,	/* LATIN_CAPITAL_LETTER_L_WITH_CARON */
  GDK_Multi_key,	GDK_L,	GDK_cedilla,	0,	0,	0x013B,	/* LATIN_CAPITAL_LETTER_L_WITH_CEDILLA */
  GDK_Multi_key,	GDK_L,	GDK_equal,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_L,	GDK_V,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_Multi_key,	GDK_M,	GDK_period,	0,	0,	0x1E40,	/* LATIN_CAPITAL_LETTER_M_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_M,	GDK_slash,	0,	0,	0x20A5,	/* MILL_SIGN */
  GDK_Multi_key,	GDK_N,	GDK_apostrophe,	0,	0,	0x0143,	/* LATIN_CAPITAL_LETTER_N_WITH_ACUTE */
  GDK_Multi_key,	GDK_N,	GDK_comma,	0,	0,	0x0145,	/* LATIN_CAPITAL_LETTER_N_WITH_CEDILLA */
  GDK_Multi_key,	GDK_N,	GDK_minus,	0,	0,	0x00D1,	/* LATIN_CAPITAL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_N,	GDK_less,	0,	0,	0x0147,	/* LATIN_CAPITAL_LETTER_N_WITH_CARON */
  GDK_Multi_key,	GDK_N,	GDK_G,	0,	0,	0x014A,	/* LATIN_CAPITAL_LETTER_ENG */
  GDK_Multi_key,	GDK_N,	GDK_N,	0,	0,	0x2229,	/* INTERSECTION */
  GDK_Multi_key,	GDK_N,	GDK_O,	0,	0,	0x2116,	/* NUMERO_SIGN */
  GDK_Multi_key,	GDK_N,	GDK_o,	0,	0,	0x2116,	/* NUMERO_SIGN */
  GDK_Multi_key,	GDK_N,	GDK_asciitilde,	0,	0,	0x00D1,	/* LATIN_CAPITAL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_N,	GDK_cedilla,	0,	0,	0x0145,	/* LATIN_CAPITAL_LETTER_N_WITH_CEDILLA */
  GDK_Multi_key,	GDK_O,	GDK_quotedbl,	0,	0,	0x00D6,	/* LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_O,	GDK_apostrophe,	0,	0,	0x00D3,	/* LATIN_CAPITAL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_O,	GDK_parenleft,	0,	0,	0x014E,	/* LATIN_CAPITAL_LETTER_O_WITH_BREVE */
  GDK_Multi_key,	GDK_O,	GDK_plus,	0,	0,	0x2295,	/* CIRCLED_PLUS */
  GDK_Multi_key,	GDK_O,	GDK_minus,	0,	0,	0x014C,	/* LATIN_CAPITAL_LETTER_O_WITH_MACRON */
  GDK_Multi_key,	GDK_O,	GDK_slash,	0,	0,	0x00D8,	/* LATIN_CAPITAL_LETTER_O_WITH_STROKE */
  GDK_Multi_key,	GDK_O,	GDK_colon,	0,	0,	0x0150,	/* LATIN_CAPITAL_LETTER_O_WITH_DOUBLE_ACUTE */
  GDK_Multi_key,	GDK_O,	GDK_greater,	0,	0,	0x00D4,	/* LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_O,	GDK_C,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_O,	GDK_E,	0,	0,	0x0152,	/* LATIN_CAPITAL_LIGATURE_OE */
  GDK_Multi_key,	GDK_O,	GDK_P,	0,	0,	0x2117,	/* SOUND_RECORDING_COPYRIGHT */
  GDK_Multi_key,	GDK_O,	GDK_R,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_O,	GDK_S,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_O,	GDK_X,	0,	0,	0x2297,	/* CIRCLED_TIMES */
  GDK_Multi_key,	GDK_O,	GDK_asciicircum,	0,	0,	0x00D4,	/* LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_O,	GDK_underscore,	0,	0,	0x00BA,	/* MASCULINE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_O,	GDK_grave,	0,	0,	0x00D2,	/* LATIN_CAPITAL_LETTER_O_WITH_GRAVE */
  GDK_Multi_key,	GDK_O,	GDK_c,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_O,	GDK_p,	0,	0,	0x2117,	/* SOUND_RECORDING_COPYRIGHT */
  GDK_Multi_key,	GDK_O,	GDK_r,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_O,	GDK_s,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_O,	GDK_x,	0,	0,	0x2297,	/* CIRCLED_TIMES */
  GDK_Multi_key,	GDK_O,	GDK_asciitilde,	0,	0,	0x00D5,	/* LATIN_CAPITAL_LETTER_O_WITH_TILDE */
  GDK_Multi_key,	GDK_O,	GDK_diaeresis,	0,	0,	0x00D6,	/* LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_O,	GDK_acute,	0,	0,	0x00D3,	/* LATIN_CAPITAL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_O,	GDK_breve,	0,	0,	0x014E,	/* LATIN_CAPITAL_LETTER_O_WITH_BREVE */
  GDK_Multi_key,	GDK_P,	GDK_exclam,	0,	0,	0x00B6,	/* PILCROW_SIGN */
  GDK_Multi_key,	GDK_P,	GDK_period,	0,	0,	0x1E56,	/* LATIN_CAPITAL_LETTER_P_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_P,	GDK_0,	0,	0,	0x2117,	/* SOUND_RECORDING_COPYRIGHT */
  GDK_Multi_key,	GDK_P,	GDK_D,	0,	0,	0x2202,	/* PARTIAL_DIFFERENTIAL */
  GDK_Multi_key,	GDK_P,	GDK_O,	0,	0,	0x2117,	/* SOUND_RECORDING_COPYRIGHT */
  GDK_Multi_key,	GDK_P,	GDK_T,	0,	0,	0x20A7,	/* PESETA_SIGN */
  GDK_Multi_key,	GDK_P,	GDK_o,	0,	0,	0x2117,	/* SOUND_RECORDING_COPYRIGHT */
  GDK_Multi_key,	GDK_P,	GDK_t,	0,	0,	0x20A7,	/* PESETA_SIGN */
  GDK_Multi_key,	GDK_P,	GDK_bar,	0,	0,	0x00B6,	/* PILCROW_SIGN */
  GDK_Multi_key,	GDK_R,	GDK_apostrophe,	0,	0,	0x0154,	/* LATIN_CAPITAL_LETTER_R_WITH_ACUTE */
  GDK_Multi_key,	GDK_R,	GDK_comma,	0,	0,	0x0156,	/* LATIN_CAPITAL_LETTER_R_WITH_CEDILLA */
  GDK_Multi_key,	GDK_R,	GDK_0,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_R,	GDK_less,	0,	0,	0x0158,	/* LATIN_CAPITAL_LETTER_R_WITH_CARON */
  GDK_Multi_key,	GDK_R,	GDK_O,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_R,	GDK_S,	0,	0,	0x20A8,	/* RUPEE_SIGN */
  GDK_Multi_key,	GDK_R,	GDK_X,	0,	0,	0x211E,	/* PRESCRIPTION_TAKE */
  GDK_Multi_key,	GDK_R,	GDK_o,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_R,	GDK_s,	0,	0,	0x20A8,	/* RUPEE_SIGN */
  GDK_Multi_key,	GDK_R,	GDK_x,	0,	0,	0x211E,	/* PRESCRIPTION_TAKE */
  GDK_Multi_key,	GDK_R,	GDK_acute,	0,	0,	0x0154,	/* LATIN_CAPITAL_LETTER_R_WITH_ACUTE */
  GDK_Multi_key,	GDK_R,	GDK_cedilla,	0,	0,	0x0156,	/* LATIN_CAPITAL_LETTER_R_WITH_CEDILLA */
  GDK_Multi_key,	GDK_S,	GDK_exclam,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_S,	GDK_apostrophe,	0,	0,	0x015A,	/* LATIN_CAPITAL_LETTER_S_WITH_ACUTE */
  GDK_Multi_key,	GDK_S,	GDK_comma,	0,	0,	0x015E,	/* LATIN_CAPITAL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_S,	GDK_period,	0,	0,	0x1E60,	/* LATIN_CAPITAL_LETTER_S_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_S,	GDK_0,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_S,	GDK_1,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_Multi_key,	GDK_S,	GDK_2,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_Multi_key,	GDK_S,	GDK_3,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_Multi_key,	GDK_S,	GDK_4,	0,	0,	0x2074,	/* SUPERSCRIPT_FOUR */
  GDK_Multi_key,	GDK_S,	GDK_5,	0,	0,	0x2075,	/* SUPERSCRIPT_FIVE */
  GDK_Multi_key,	GDK_S,	GDK_6,	0,	0,	0x2076,	/* SUPERSCRIPT_SIX */
  GDK_Multi_key,	GDK_S,	GDK_7,	0,	0,	0x2077,	/* SUPERSCRIPT_SEVEN */
  GDK_Multi_key,	GDK_S,	GDK_8,	0,	0,	0x2078,	/* SUPERSCRIPT_EIGHT */
  GDK_Multi_key,	GDK_S,	GDK_9,	0,	0,	0x2079,	/* SUPERSCRIPT_NINE */
  GDK_Multi_key,	GDK_S,	GDK_less,	0,	0,	0x0160,	/* LATIN_CAPITAL_LETTER_S_WITH_CARON */
  GDK_Multi_key,	GDK_S,	GDK_greater,	0,	0,	0x015C,	/* LATIN_CAPITAL_LETTER_S_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_S,	GDK_M,	0,	0,	0x2120,	/* SERVICE MARK */
  GDK_Multi_key,	GDK_S,	GDK_O,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_S,	GDK_asciicircum,	0,	0,	0x015C,	/* LATIN_CAPITAL_LETTER_S_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_S,	GDK_acute,	0,	0,	0x015A,	/* LATIN_CAPITAL_LETTER_S_WITH_ACUTE */
  GDK_Multi_key,	GDK_S,	GDK_cedilla,	0,	0,	0x015E,	/* LATIN_CAPITAL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_T,	GDK_comma,	0,	0,	0x0162,	/* LATIN_CAPITAL_LETTER_T_WITH_CEDILLA */
  GDK_Multi_key,	GDK_T,	GDK_minus,	0,	0,	0x0166,	/* LATIN_CAPITAL_LETTER_T_WITH_STROKE */
  GDK_Multi_key,	GDK_T,	GDK_period,	0,	0,	0x1E6A,	/* LATIN_CAPITAL_LETTER_T_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_T,	GDK_slash,	0,	0,	0x0166,	/* LATIN_CAPITAL_LETTER_T_WITH_STROKE */
  GDK_Multi_key,	GDK_T,	GDK_less,	0,	0,	0x0164,	/* LATIN_CAPITAL_LETTER_T_WITH_CARON */
  GDK_Multi_key,	GDK_T,	GDK_H,	0,	0,	0x00DE,	/* LATIN_CAPITAL_LETTER_THORN */
  GDK_Multi_key,	GDK_T,	GDK_M,	0,	0,	0x2122,	/* TRADE MARK SIGN */
  GDK_Multi_key,	GDK_T,	GDK_cedilla,	0,	0,	0x0162,	/* LATIN_CAPITAL_LETTER_T_WITH_CEDILLA */
  GDK_Multi_key,	GDK_U,	GDK_quotedbl,	0,	0,	0x00DC,	/* LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_U,	GDK_apostrophe,	0,	0,	0x00DA,	/* LATIN_CAPITAL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_U,	GDK_parenleft,	0,	0,	0x016C,	/* LATIN_CAPITAL_LETTER_U_WITH_BREVE */
  GDK_Multi_key,	GDK_U,	GDK_asterisk,	0,	0,	0x016E,	/* LATIN_CAPITAL_LETTER_U_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_U,	GDK_comma,	0,	0,	0x0172,	/* LATIN_CAPITAL_LETTER_U_WITH_OGONEK */
  GDK_Multi_key,	GDK_U,	GDK_minus,	0,	0,	0x016A,	/* LATIN_CAPITAL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_U,	GDK_slash,	0,	0,	0x00B5,	/* MICRO_SIGN */
  GDK_Multi_key,	GDK_U,	GDK_colon,	0,	0,	0x0170,	/* LATIN_CAPITAL_LETTER_U_WITH_DOUBLE_ACUTE */
  GDK_Multi_key,	GDK_U,	GDK_greater,	0,	0,	0x00DB,	/* LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_U,	GDK_U,	0,	0,	0x222A,	/* UNION */
  GDK_Multi_key,	GDK_U,	GDK_asciicircum,	0,	0,	0x00DB,	/* LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_U,	GDK_underscore,	0,	0,	0x00D9,	/* LATIN_CAPITAL_LETTER_U_WITH_GRAVE */
  GDK_Multi_key,	GDK_U,	GDK_grave,	0,	0,	0x00D9,	/* LATIN_CAPITAL_LETTER_U_WITH_GRAVE */
  GDK_Multi_key,	GDK_U,	GDK_asciitilde,	0,	0,	0x0168,	/* LATIN_CAPITAL_LETTER_U_WITH_TILDE */
  GDK_Multi_key,	GDK_U,	GDK_diaeresis,	0,	0,	0x00DC,	/* LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_U,	GDK_acute,	0,	0,	0x00DA,	/* LATIN_CAPITAL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_U,	GDK_breve,	0,	0,	0x016C,	/* LATIN_CAPITAL_LETTER_U_WITH_BREVE */
  GDK_Multi_key,	GDK_V,	GDK_slash,	0,	0,	0x2123,	/* VERSICLE */
  GDK_Multi_key,	GDK_V,	GDK_L,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_Multi_key,	GDK_W,	GDK_quotedbl,	0,	0,	0x1E84,	/* LATIN_CAPITAL_LETTER_W_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_W,	GDK_greater,	0,	0,	0x0174,	/* LATIN_CAPITAL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_W,	GDK_asciicircum,	0,	0,	0x0174,	/* LATIN_CAPITAL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_W,	GDK_diaeresis,	0,	0,	0x1E84,	/* LATIN_CAPITAL_LETTER_W_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_X,	GDK_0,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_X,	GDK_O,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_X,	GDK_o,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_Y,	GDK_quotedbl,	0,	0,	0x0178,	/* LATIN_CAPITAL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_Y,	GDK_apostrophe,	0,	0,	0x00DD,	/* LATIN_CAPITAL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_Y,	GDK_minus,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_Y,	GDK_equal,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_Y,	GDK_greater,	0,	0,	0x0176,	/* LATIN_CAPITAL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_Y,	GDK_asciicircum,	0,	0,	0x0176,	/* LATIN_CAPITAL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_Y,	GDK_grave,	0,	0,	0x1EF2,	/* LATIN_CAPITAL_LETTER_Y_WITH_GRAVE */
  GDK_Multi_key,	GDK_Y,	GDK_diaeresis,	0,	0,	0x0178,	/* LATIN_CAPITAL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_Y,	GDK_acute,	0,	0,	0x00DD,	/* LATIN_CAPITAL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_Z,	GDK_apostrophe,	0,	0,	0x0179,	/* LATIN_CAPITAL_LETTER_Z_WITH_ACUTE */
  GDK_Multi_key,	GDK_Z,	GDK_period,	0,	0,	0x017B,	/* LATIN_CAPITAL_LETTER_Z_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_Z,	GDK_less,	0,	0,	0x017D,	/* LATIN_CAPITAL_LETTER_Z_WITH_CARON */
  GDK_Multi_key,	GDK_Z,	GDK_acute,	0,	0,	0x0179,	/* LATIN_CAPITAL_LETTER_Z_WITH_ACUTE */
  GDK_Multi_key,	GDK_bracketleft,	GDK_minus,	0,	0,	0x2208,	/* ELEMENT_OF */
  GDK_Multi_key,	GDK_bracketleft,	GDK_bracketleft,	0,	0,	0x2282,	/* SUBSET_OF */
  GDK_Multi_key,	GDK_bracketleft,	GDK_bracketright,	0,	0,	0x25A1,	/* WHITE_SQUARE */
  GDK_Multi_key,	GDK_bracketleft,	GDK_underscore,	0,	0,	0x2286,	/* SUBSET_OF_OR_EQUAL_TO */
  GDK_Multi_key,	GDK_backslash,	GDK_slash,	0,	0,	0x2228,	/* LOGICAL_OR */
  GDK_Multi_key,	GDK_bracketright,	GDK_minus,	0,	0,	0x220B,	/* CONTAINS_AS_MEMBER */
  GDK_Multi_key,	GDK_bracketright,	GDK_bracketright,	0,	0,	0x2283,	/* SUPERSET_OF */
  GDK_Multi_key,	GDK_bracketright,	GDK_underscore,	0,	0,	0x2287,	/* SUPERSET_OF_OR_EQUAL_TO */
  GDK_Multi_key,	GDK_asciicircum,	GDK_space,	0,	0,	0x005E,	/* CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_asciicircum,	GDK_minus,	0,	0,	0x00AF,	/* MACRON */
  GDK_Multi_key,	GDK_asciicircum,	GDK_period,	0,	0,	0x02D9,	/* DOT_ABOVE */
  GDK_Multi_key,	GDK_asciicircum,	GDK_slash,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_Multi_key,	GDK_asciicircum,	GDK_0,	0,	0,	0x00B0,	/* DEGREE_SIGN */
  GDK_Multi_key,	GDK_asciicircum,	GDK_1,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_Multi_key,	GDK_asciicircum,	GDK_2,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_Multi_key,	GDK_asciicircum,	GDK_3,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_Multi_key,	GDK_asciicircum,	GDK_4,	0,	0,	0x2074,	/* SUPERSCRIPT_FOUR */
  GDK_Multi_key,	GDK_asciicircum,	GDK_5,	0,	0,	0x2075,	/* SUPERSCRIPT_FIVE */
  GDK_Multi_key,	GDK_asciicircum,	GDK_6,	0,	0,	0x2076,	/* SUPERSCRIPT_SIX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_7,	0,	0,	0x2077,	/* SUPERSCRIPT_SEVEN */
  GDK_Multi_key,	GDK_asciicircum,	GDK_8,	0,	0,	0x2078,	/* SUPERSCRIPT_EIGHT */
  GDK_Multi_key,	GDK_asciicircum,	GDK_9,	0,	0,	0x2079,	/* SUPERSCRIPT_NINE */
  GDK_Multi_key,	GDK_asciicircum,	GDK_A,	0,	0,	0x00C2,	/* LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_C,	0,	0,	0x0108,	/* LATIN_CAPITAL_LETTER_C_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_E,	0,	0,	0x00CA,	/* LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_G,	0,	0,	0x011C,	/* LATIN_CAPITAL_LETTER_G_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_H,	0,	0,	0x0124,	/* LATIN_CAPITAL_LETTER_H_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_I,	0,	0,	0x00CE,	/* LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_J,	0,	0,	0x0134,	/* LATIN_CAPITAL_LETTER_J_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_O,	0,	0,	0x00D4,	/* LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_S,	0,	0,	0x015C,	/* LATIN_CAPITAL_LETTER_S_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_U,	0,	0,	0x00DB,	/* LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_W,	0,	0,	0x0174,	/* LATIN_CAPITAL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_Y,	0,	0,	0x0176,	/* LATIN_CAPITAL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_underscore,	0,	0,	0x00AF,	/* MACRON */
  GDK_Multi_key,	GDK_asciicircum,	GDK_a,	0,	0,	0x00E2,	/* LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,        GDK_asciicircum,	GDK_c,  0,	0,	0x0109,	/* LATIN_SMALL_LETTER_C_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_e,	0,	0,	0x00EA,	/* LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,        GDK_g,	0,	0,	0x011D,	/* LATIN_SMALL_LETTER_G_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_h,  0,	0,	0x0125,	/* LATIN_SMALL_LETTER_H_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_i,	0,	0,	0x00EE,	/* LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_j,  0,	0,	0x0135,	/* LATIN_SMALL_LETTER_J_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_o,	0,	0,	0x00F4,	/* LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_s,	0,	0,	0x015D,	/* LATIN_SMALL_LETTER_S_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_u,	0,	0,	0x00FB,	/* LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_v,	0,	0,	0x2195,	/* UP_DOWN_ARROW */
  GDK_Multi_key,	GDK_asciicircum,	GDK_w,	0,	0,	0x0175,	/* LATIN_SMALL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_y,	0,	0,	0x0177,	/* LATIN_SMALL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_bar,	0,	0,	0x2191,	/* UPWARDS_ARROW */
  GDK_Multi_key,	GDK_asciicircum,	GDK_Left,	0,	0,	0x0302,	/* COMBINING_CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_asciicircum,	GDK_KP_Left,	0,	0,	0x0302,	/* COMBINING_CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_underscore,	GDK_slash,	0,	0,	0x2220,	/* ANGLE */
  GDK_Multi_key,	GDK_underscore,	GDK_0,	0,	0,	0x2080,	/* SUBSCRIPT_ZERO */
  GDK_Multi_key,	GDK_underscore,	GDK_1,	0,	0,	0x2081,	/* SUBSCRIPT_ONE */
  GDK_Multi_key,	GDK_underscore,	GDK_2,	0,	0,	0x2082,	/* SUBSCRIPT_TWO */
  GDK_Multi_key,	GDK_underscore,	GDK_3,	0,	0,	0x2083,	/* SUBSCRIPT_THREE */
  GDK_Multi_key,	GDK_underscore,	GDK_4,	0,	0,	0x2084,	/* SUBSCRIPT_FOUR */
  GDK_Multi_key,	GDK_underscore,	GDK_5,	0,	0,	0x2085,	/* SUBSCRIPT_FIVE */
  GDK_Multi_key,	GDK_underscore,	GDK_6,	0,	0,	0x2086,	/* SUBSCRIPT_SIX */
  GDK_Multi_key,	GDK_underscore,	GDK_7,	0,	0,	0x2087,	/* SUBSCRIPT_SEVEN */
  GDK_Multi_key,	GDK_underscore,	GDK_8,	0,	0,	0x2088,	/* SUBSCRIPT_EIGHT */
  GDK_Multi_key,	GDK_underscore,	GDK_9,	0,	0,	0x2089,	/* SUBSCRIPT_NINE */
  GDK_Multi_key,	GDK_underscore,	GDK_A,	0,	0,	0x00AA,	/* FEMININE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_underscore,	GDK_E,	0,	0,	0x0112,	/* LATIN_CAPITAL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_I,	0,	0,	0x012A,	/* LATIN_CAPITAL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_O,	0,	0,	0x00BA,	/* MASCULINE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_underscore,	GDK_U,	0,	0,	0x016A,	/* LATIN_CAPITAL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_bracketleft,	0,	0,	0x2286,	/* SUBSET_OF_OR_EQUAL_TO */
  GDK_Multi_key,	GDK_underscore,	GDK_bracketright,	0,	0,	0x2287,	/* SUPERSET_OF_OR_EQUAL_TO */
  GDK_Multi_key,	GDK_underscore,	GDK_asciicircum,	0,	0,	0x00AF,	/* MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_underscore,	0,	0,	0x00AF,	/* MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_a,	0,	0,	0x00AA,	/* FEMININE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_underscore,	GDK_e,	0,	0,	0x0113,	/* LATIN_SMALL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_i,	0,	0,	0x012B,	/* LATIN_SMALL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_o,	0,	0,	0x00BA,	/* MASCULINE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_underscore,	GDK_u,	0,	0,	0x016B,	/* LATIN_SMALL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_bar,	0,	0,	0x22A5,	/* UP_TACK */
  GDK_Multi_key,	GDK_underscore,	GDK_Left,	0,	0,	0x0332,	/* COMBINING_LOW_LINE */
  GDK_Multi_key,	GDK_underscore,	GDK_KP_Left,	0,	0,	0x0332,	/* COMBINING_LOW_LINE */
  GDK_Multi_key,	GDK_grave,	GDK_space,	0,	0,	0x0060,	/* GRAVE_ACCENT */
  GDK_Multi_key,	GDK_grave,	GDK_A,	0,	0,	0x00C0,	/* LATIN_CAPITAL_LETTER_A_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_E,	0,	0,	0x00C8,	/* LATIN_CAPITAL_LETTER_E_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_I,	0,	0,	0x00CC,	/* LATIN_CAPITAL_LETTER_I_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_O,	0,	0,	0x00D2,	/* LATIN_CAPITAL_LETTER_O_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_U,	0,	0,	0x00D9,	/* LATIN_CAPITAL_LETTER_U_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_Y,	0,	0,	0x1EF2,	/* LATIN_CAPITAL_LETTER_Y_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_a,	0,	0,	0x00E0,	/* LATIN_SMALL_LETTER_A_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_e,	0,	0,	0x00E8,	/* LATIN_SMALL_LETTER_E_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_i,	0,	0,	0x00EC,	/* LATIN_SMALL_LETTER_I_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_o,	0,	0,	0x00F2,	/* LATIN_SMALL_LETTER_O_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_u,	0,	0,	0x00F9,	/* LATIN_SMALL_LETTER_U_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_y,	0,	0,	0x1EF3,	/* LATIN_SMALL_LETTER_Y_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_Left,	0,	0,	0x0300,	/* COMBINING_GRAVE_ACCENT */
  GDK_Multi_key,	GDK_grave,	GDK_KP_Left,	0,	0,	0x0300,	/* COMBINING_GRAVE_ACCENT */
  GDK_Multi_key,	GDK_a,	GDK_quotedbl,	0,	0,	0x00E4,	/* LATIN_SMALL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_a,	GDK_apostrophe,	0,	0,	0x00E1,	/* LATIN_SMALL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_a,	GDK_parenleft,	0,	0,	0x0103,	/* LATIN_SMALL_LETTER_A_WITH_BREVE */
  GDK_Multi_key,	GDK_a,	GDK_comma,	0,	0,	0x0105,	/* LATIN_SMALL_LETTER_A_WITH_OGONEK */
  GDK_Multi_key,	GDK_a,	GDK_asterisk,	0,	0,	0x00E5,	/* LATIN_SMALL_LETTER_A_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_a,	GDK_minus,	0,	0,	0x0101,	/* LATIN_SMALL_LETTER_A_WITH_MACRON */
  GDK_Multi_key,	GDK_a,	GDK_greater,	0,	0,	0x00E2,	/* LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_a,	GDK_asciicircum,	0,	0,	0x00E2,	/* LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_a,	GDK_underscore,	0,	0,	0x00AA,	/* FEMININE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_a,	GDK_grave,	0,	0,	0x00E0,	/* LATIN_SMALL_LETTER_A_WITH_GRAVE */
  GDK_Multi_key,	GDK_a,	GDK_a,	0,	0,	0x00E5,	/* LATIN_SMALL_LETTER_A_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_a,	GDK_e,	0,	0,	0x00E6,	/* LATIN_SMALL_LETTER_AE */
  GDK_Multi_key,	GDK_a,	GDK_asciitilde,	0,	0,	0x00E3,	/* LATIN_SMALL_LETTER_A_WITH_TILDE */
  GDK_Multi_key,	GDK_a,	GDK_diaeresis,	0,	0,	0x00E4,	/* LATIN_SMALL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_a,	GDK_acute,	0,	0,	0x00E1,	/* LATIN_SMALL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_b,	GDK_period,	0,	0,	0x1E03,	/* LATIN_SMALL_LETTER_B_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_b,	GDK_b,	0,	0,	0x266D,	/* MUSIC_FLAT_SIGN */
  GDK_Multi_key,	GDK_c,	GDK_apostrophe,	0,	0,	0x0107,	/* LATIN_SMALL_LETTER_C_WITH_ACUTE */
  GDK_Multi_key,	GDK_c,	GDK_comma,	0,	0,	0x00E7,	/* LATIN_SMALL_LETTER_C_WITH_CEDILLA */
  GDK_Multi_key,	GDK_c,	GDK_period,	0,	0,	0x010B,	/* LATIN_SMALL_LETTER_C_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_c,	GDK_slash,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_c,	GDK_0,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_c,	GDK_less,	0,	0,	0x010D,	/* LATIN_SMALL_LETTER_C_WITH_CARON */
  GDK_Multi_key,	GDK_c,	GDK_greater,	0,	0,	0x0109,	/* LATIN_SMALL_LETTER_C_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_c,	GDK_O,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_c,	GDK_asciicircum,	0,	0,	0x0109,	/* LATIN_SMALL_LETTER_C_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_c,	GDK_l,	0,	0,	0x2104,	/* CENTRE_LINE_SYMBOL */
  GDK_Multi_key,	GDK_c,	GDK_o,	0,	0,	0x2105,	/* CARE_OF */
  GDK_Multi_key,	GDK_c,	GDK_r,	0,	0,	0x20A2,	/* CRUZEIRO_SIGN */
  GDK_Multi_key,	GDK_c,	GDK_u,	0,	0,	0x2106,	/* CADA_UNA */
  GDK_Multi_key,	GDK_c,	GDK_bar,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_d,	GDK_minus,	0,	0,	0x0111,	/* LATIN_SMALL_LETTER_D_WITH_STROKE */
  GDK_Multi_key,	GDK_d,	GDK_period,	0,	0,	0x1E0B,	/* LATIN_SMALL_LETTER_D_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_d,	GDK_slash,	0,	0,	0x00F0,	/* LATIN_SMALL_LETTER_ETH */
  GDK_Multi_key,	GDK_d,	GDK_less,	0,	0,	0x010F,	/* LATIN_SMALL_LETTER_D_WITH_CARON */
  GDK_Multi_key,	GDK_e,	GDK_quotedbl,	0,	0,	0x00EB,	/* LATIN_SMALL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_e,	GDK_apostrophe,	0,	0,	0x00E9,	/* LATIN_SMALL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_e,	GDK_parenleft,	0,	0,	0x0115,	/* LATIN_SMALL_LETTER_E_WITH_BREVE */
  GDK_Multi_key,	GDK_e,	GDK_comma,	0,	0,	0x0119,	/* LATIN_SMALL_LETTER_E_WITH_OGONEK */
  GDK_Multi_key,	GDK_e,	GDK_minus,	0,	0,	0x0113,	/* LATIN_SMALL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_e,	GDK_period,	0,	0,	0x0117,	/* LATIN_SMALL_LETTER_E_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_e,	GDK_equal,	0,	0,	0x20AC,	/* EURO_SIGN */
  GDK_Multi_key,	GDK_e,	GDK_less,	0,	0,	0x011B,	/* LATIN_SMALL_LETTER_E_WITH_CARON */
  GDK_Multi_key,	GDK_e,	GDK_equal,	0,	0,	0x20AC,	/* EURO_SIGN */
  GDK_Multi_key,	GDK_e,	GDK_greater,	0,	0,	0x00EA,	/* LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_e,	GDK_asciicircum,	0,	0,	0x00EA,	/* LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_e,	GDK_underscore,	0,	0,	0x0113,	/* LATIN_SMALL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_e,	GDK_grave,	0,	0,	0x00E8,	/* LATIN_SMALL_LETTER_E_WITH_GRAVE */
  GDK_Multi_key,	GDK_e,	GDK_e,	0,	0,	0x0259,	/* LATIN_SMALL_LETTER_SCHWA */
  GDK_Multi_key,	GDK_e,	GDK_diaeresis,	0,	0,	0x00EB,	/* LATIN_SMALL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_e,	GDK_acute,	0,	0,	0x00E9,	/* LATIN_SMALL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_e,	GDK_breve,	0,	0,	0x0115,	/* LATIN_SMALL_LETTER_E_WITH_BREVE */
  GDK_Multi_key,	GDK_f,	GDK_period,	0,	0,	0x1E1F,	/* LATIN_SMALL_LETTER_F_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_g,	GDK_parenleft,	0,	0,	0x011F,	/* LATIN_SMALL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_g,	GDK_comma,	0,	0,	0x0123,	/* LATIN_SMALL_LETTER_G_WITH_CEDILLA */
  GDK_Multi_key,	GDK_g,	GDK_period,	0,	0,	0x0121,	/* LATIN_SMALL_LETTER_G_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_g,	GDK_greater,	0,	0,	0x011D,	/* LATIN_SMALL_LETTER_G_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_g,	GDK_U,	0,	0,	0x011F,	/* LATIN_SMALL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_g,	GDK_asciicircum,	0,	0,	0x011D,	/* LATIN_SMALL_LETTER_G_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_g,	GDK_breve,	0,	0,	0x011F,	/* LATIN_SMALL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_h,	GDK_minus,	0,	0,	0x0127,	/* LATIN_SMALL_LETTER_H_WITH_STROKE */
  GDK_Multi_key,	GDK_h,	GDK_slash,	0,	0,	0x210F,	/* PLANCK_CONSTANT_OVER_TWO_PI */
  GDK_Multi_key,	GDK_h,	GDK_greater,	0,	0,	0x0125,	/* LATIN_SMALL_LETTER_H_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_h,	GDK_asciicircum,	0,	0,	0x0125,	/* LATIN_SMALL_LETTER_H_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_i,	GDK_quotedbl,	0,	0,	0x00EF,	/* LATIN_SMALL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_i,	GDK_apostrophe,	0,	0,	0x00ED,	/* LATIN_SMALL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_i,	GDK_parenleft,	0,	0,	0x012D,	/* LATIN_SMALL_LETTER_I_WITH_BREVE */
  GDK_Multi_key,	GDK_i,	GDK_comma,	0,	0,	0x012F,	/* LATIN_SMALL_LETTER_I_WITH_OGONEK */
  GDK_Multi_key,	GDK_i,	GDK_minus,	0,	0,	0x012B,	/* LATIN_SMALL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_i,	GDK_period,	0,	0,	0x0131,	/* LATIN_SMALL_LETTER_DOTLESS_I */
  GDK_Multi_key,	GDK_i,	GDK_greater,	0,	0,	0x00EE,	/* LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_i,	GDK_asciicircum,	0,	0,	0x00EE,	/* LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_i,	GDK_underscore,	0,	0,	0x012B,	/* LATIN_SMALL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_i,	GDK_grave,	0,	0,	0x00EC,	/* LATIN_SMALL_LETTER_I_WITH_GRAVE */
  GDK_Multi_key,	GDK_i,	GDK_j,	0,	0,	0x0133,	/* LATIN_SMALL_LIGATURE_IJ */
  GDK_Multi_key,	GDK_i,	GDK_asciitilde,	0,	0,	0x0129,	/* LATIN_SMALL_LETTER_I_WITH_TILDE */
  GDK_Multi_key,	GDK_i,	GDK_diaeresis,	0,	0,	0x00EF,	/* LATIN_SMALL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_i,	GDK_acute,	0,	0,	0x00ED,	/* LATIN_SMALL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_i,	GDK_breve,	0,	0,	0x012D,	/* LATIN_SMALL_LETTER_I_WITH_BREVE */
  GDK_Multi_key,	GDK_j,	GDK_greater,	0,	0,	0x0135,	/* LATIN_SMALL_LETTER_J_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_j,	GDK_asciicircum,	0,	0,	0x0135,	/* LATIN_SMALL_LETTER_J_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_k,	GDK_comma,	0,	0,	0x0137,	/* LATIN_SMALL_LETTER_K_WITH_CEDILLA */
  GDK_Multi_key,	GDK_k,	GDK_k,	0,	0,	0x0138,	/* LATIN_SMALL_LETTER_KRA */
  GDK_Multi_key,	GDK_l,	GDK_apostrophe,	0,	0,	0x013A,	/* LATIN_SMALL_LETTER_L_WITH_ACUTE */
  GDK_Multi_key,	GDK_l,	GDK_comma,	0,	0,	0x013C,	/* LATIN_SMALL_LETTER_L_WITH_CEDILLA */
  GDK_Multi_key,	GDK_l,	GDK_minus,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_l,	GDK_period,	0,	0,	0x0140,	/* LATIN_SMALL_LETTER_L_WITH_MIDDLE_DOT */
  GDK_Multi_key,	GDK_l,	GDK_slash,	0,	0,	0x0142,	/* LATIN_SMALL_LETTER_L_WITH_STROKE */
  GDK_Multi_key,	GDK_l,	GDK_less,	0,	0,	0x013E,	/* LATIN_SMALL_LETTER_L_WITH_CARON */
  GDK_Multi_key,	GDK_l,	GDK_equal,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_l,	GDK_v,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_Multi_key,	GDK_m,	GDK_minus,	0,	0,	0x2014,	/* EM_DASH */
  GDK_Multi_key,	GDK_m,	GDK_period,	0,	0,	0x1E41,	/* LATIN_SMALL_LETTER_M_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_m,	GDK_slash,	0,	0,	0x20A5,	/* MILL_SIGN */
  GDK_Multi_key,	GDK_n,	GDK_apostrophe,	0,	0,	0x0144,	/* LATIN_SMALL_LETTER_N_WITH_ACUTE */
  GDK_Multi_key,	GDK_n,	GDK_comma,	0,	0,	0x0146,	/* LATIN_SMALL_LETTER_N_WITH_CEDILLA */
  GDK_Multi_key,	GDK_n,	GDK_minus,	0,	0,	0x00F1,	/* LATIN_SMALL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_n,	GDK_less,	0,	0,	0x0148,	/* LATIN_SMALL_LETTER_N_WITH_CARON */
  GDK_Multi_key,	GDK_n,	GDK_g,	0,	0,	0x014B,	/* LATIN_SMALL_LETTER_ENG */
  GDK_Multi_key,	GDK_n,	GDK_n,	0,	0,	0x266E,	/* MUSIC_NATURAL_SIGN */
  GDK_Multi_key,	GDK_n,	GDK_o,	0,	0,	0x2116,	/* NUMERO_SIGN */
  GDK_Multi_key,	GDK_n,	GDK_asciitilde,	0,	0,	0x00F1,	/* LATIN_SMALL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_o,	GDK_quotedbl,	0,	0,	0x00F6,	/* LATIN_SMALL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_o,	GDK_apostrophe,	0,	0,	0x00F3,	/* LATIN_SMALL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_o,	GDK_parenleft,	0,	0,	0x014F,	/* LATIN_SMALL_LETTER_O_WITH_BREVE */
  GDK_Multi_key,	GDK_o,	GDK_plus,	0,	0,	0x2295,	/* CIRCLED_PLUS */
  GDK_Multi_key,	GDK_o,	GDK_minus,	0,	0,	0x014D,	/* LATIN_SMALL_LETTER_O_WITH_MACRON */
  GDK_Multi_key,	GDK_o,	GDK_slash,	0,	0,	0x00F8,	/* LATIN_SMALL_LETTER_O_WITH_STROKE */
  GDK_Multi_key,	GDK_o,	GDK_colon,	0,	0,	0x0151,	/* LATIN_SMALL_LETTER_O_WITH_DOUBLE_ACUTE */
  GDK_Multi_key,	GDK_o,	GDK_greater,	0,	0,	0x00F4,	/* LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_o,	GDK_C,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_P,	0,	0,	0x2117,	/* SOUND_RECORDING_COPYRIGHT */
  GDK_Multi_key,	GDK_o,	GDK_R,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_S,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_X,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_asciicircum,	0,	0,	0x00F4,	/* LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_o,	GDK_underscore,	0,	0,	0x00BA,	/* MASCULINE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_o,	GDK_grave,	0,	0,	0x00F2,	/* LATIN_SMALL_LETTER_O_WITH_GRAVE */
  GDK_Multi_key,	GDK_o,	GDK_c,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_e,	0,	0,	0x0153,	/* LATIN_SMALL_LIGATURE_OE */
  GDK_Multi_key,	GDK_o,	GDK_p,	0,	0,	0x2117,	/* SOUND_RECORDING_COPYRIGHT */
  GDK_Multi_key,	GDK_o,	GDK_r,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_s,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_x,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_asciitilde,	0,	0,	0x00F5,	/* LATIN_SMALL_LETTER_O_WITH_TILDE */
  GDK_Multi_key,	GDK_o,	GDK_diaeresis,	0,	0,	0x00F6,	/* LATIN_SMALL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_o,	GDK_acute,	0,	0,	0x00F3,	/* LATIN_SMALL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_o,	GDK_breve,	0,	0,	0x014F,	/* LATIN_SMALL_LETTER_O_WITH_BREVE */
  GDK_Multi_key,	GDK_p,	GDK_exclam,	0,	0,	0x00B6,	/* PILCROW_SIGN */
  GDK_Multi_key,	GDK_p,	GDK_period,	0,	0,	0x1E57,	/* LATIN_SMALL_LETTER_P_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_p,	GDK_0,	0,	0,	0x2117,	/* SOUND_RECORDING_COPYRIGHT */
  GDK_Multi_key,	GDK_p,	GDK_O,	0,	0,	0x2117,	/* SOUND_RECORDING_COPYRIGHT */
  GDK_Multi_key,	GDK_p,	GDK_d,	0,	0,	0x2202,	/* PARTIAL_DIFFERENTIAL */
  GDK_Multi_key,	GDK_p,	GDK_t,	0,	0,	0x20A7,	/* PESETA_SIGN */
  GDK_Multi_key,	GDK_p,	GDK_bar,	0,	0,	0x00B6,	/* PILCROW_SIGN */
  GDK_Multi_key,	GDK_r,	GDK_apostrophe,	0,	0,	0x0155,	/* LATIN_SMALL_LETTER_R_WITH_ACUTE */
  GDK_Multi_key,	GDK_r,	GDK_comma,	0,	0,	0x0157,	/* LATIN_SMALL_LETTER_R_WITH_CEDILLA */
  GDK_Multi_key,	GDK_r,	GDK_slash,	0,	0,	0x211E,	/* PRESCRIPTION_TAKE */
  GDK_Multi_key,	GDK_r,	GDK_less,	0,	0,	0x0159,	/* LATIN_SMALL_LETTER_R_WITH_CARON */
  GDK_Multi_key,	GDK_r,	GDK_s,	0,	0,	0x20A8,	/* RUPEE_SIGN */
  GDK_Multi_key,	GDK_r,	GDK_x,	0,	0,	0x211E,	/* PRESCRIPTION_TAKE */
  GDK_Multi_key,	GDK_r,	GDK_acute,	0,	0,	0x0155,	/* LATIN_SMALL_LETTER_R_WITH_ACUTE */
  GDK_Multi_key,	GDK_s,	GDK_exclam,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_s,	GDK_apostrophe,	0,	0,	0x015B,	/* LATIN_SMALL_LETTER_S_WITH_ACUTE */
  GDK_Multi_key,	GDK_s,	GDK_comma,	0,	0,	0x015F,	/* LATIN_SMALL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_s,	GDK_minus,	0,	0,	0x017F,	/* LATIN_SMALL_LETTER_LONG_S */
  GDK_Multi_key,	GDK_s,	GDK_period,	0,	0,	0x1E61,	/* LATIN_SMALL_LETTER_S_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_s,	GDK_0,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_s,	GDK_1,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_Multi_key,	GDK_s,	GDK_2,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_Multi_key,	GDK_s,	GDK_3,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_Multi_key,	GDK_s,	GDK_4,	0,	0,	0x2074,	/* SUPERSCRIPT_FOUR */
  GDK_Multi_key,	GDK_s,	GDK_5,	0,	0,	0x2075,	/* SUPERSCRIPT_FIVE */
  GDK_Multi_key,	GDK_s,	GDK_6,	0,	0,	0x2076,	/* SUPERSCRIPT_SIX */
  GDK_Multi_key,	GDK_s,	GDK_7,	0,	0,	0x2077,	/* SUPERSCRIPT_SEVEN */
  GDK_Multi_key,	GDK_s,	GDK_8,	0,	0,	0x2078,	/* SUPERSCRIPT_EIGHT */
  GDK_Multi_key,	GDK_s,	GDK_9,	0,	0,	0x2079,	/* SUPERSCRIPT_NINE */
  GDK_Multi_key,	GDK_s,	GDK_less,	0,	0,	0x0161,	/* LATIN_SMALL_LETTER_S_WITH_CARON */
  GDK_Multi_key,	GDK_s,	GDK_greater,	0,	0,	0x015D,	/* LATIN_SMALL_LETTER_S_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_s,	GDK_asciicircum,	0,	0,	0x015D,	/* LATIN_SMALL_LETTER_S_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_s,	GDK_m,	0,	0,	0x2120,	/* SERVICE MARK */
  GDK_Multi_key,	GDK_s,	GDK_o,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_s,	GDK_s,	0,	0,	0x00DF,	/* LATIN_SMALL_LETTER_SHARP_S */
  GDK_Multi_key,	GDK_s,	GDK_z,	0,	0,	0x00DF,	/* LATIN_SMALL_LETTER_SHARP_S */
  GDK_Multi_key,	GDK_s,	GDK_acute,	0,	0,	0x015B,	/* LATIN_SMALL_LETTER_S_WITH_ACUTE */
  GDK_Multi_key,	GDK_s,	GDK_cedilla,	0,	0,	0x015F,	/* LATIN_SMALL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_t,	GDK_comma,	0,	0,	0x0163,	/* LATIN_SMALL_LETTER_T_WITH_CEDILLA */
  GDK_Multi_key,	GDK_t,	GDK_minus,	0,	0,	0x0167,	/* LATIN_SMALL_LETTER_T_WITH_STROKE */
  GDK_Multi_key,	GDK_t,	GDK_period,	0,	0,	0x1E6B,	/* LATIN_SMALL_LETTER_T_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_t,	GDK_slash,	0,	0,	0x0167,	/* LATIN_SMALL_LETTER_T_WITH_STROKE */
  GDK_Multi_key,	GDK_t,	GDK_less,	0,	0,	0x0165,	/* LATIN_SMALL_LETTER_T_WITH_CARON */
  GDK_Multi_key,	GDK_t,	GDK_h,	0,	0,	0x00FE,	/* LATIN_SMALL_LETTER_THORN */
  GDK_Multi_key,	GDK_t,	GDK_m,	0,	0,	0x2122,	/* TRADE MARK SIGN */
  GDK_Multi_key,	GDK_t,	GDK_cedilla,	0,	0,	0x0163,	/* LATIN_SMALL_LETTER_T_WITH_CEDILLA */
  GDK_Multi_key,	GDK_u,	GDK_quotedbl,	0,	0,	0x00FC,	/* LATIN_SMALL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_u,	GDK_apostrophe,	0,	0,	0x00FA,	/* LATIN_SMALL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_u,	GDK_parenleft,	0,	0,	0x016D,	/* LATIN_SMALL_LETTER_U_WITH_BREVE */
  GDK_Multi_key,	GDK_u,	GDK_asterisk,	0,	0,	0x016F,	/* LATIN_SMALL_LETTER_U_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_u,	GDK_comma,	0,	0,	0x0173,	/* LATIN_SMALL_LETTER_U_WITH_OGONEK */
  GDK_Multi_key,	GDK_u,	GDK_minus,	0,	0,	0x016B,	/* LATIN_SMALL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_u,	GDK_slash,	0,	0,	0x00B5,	/* MICRO_SIGN */
  GDK_Multi_key,	GDK_u,	GDK_colon,	0,	0,	0x0171,	/* LATIN_SMALL_LETTER_U_WITH_DOUBLE_ACUTE */
  GDK_Multi_key,	GDK_u,	GDK_greater,	0,	0,	0x00FB,	/* LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_u,	GDK_asciicircum,	0,	0,	0x00FB,	/* LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_u,	GDK_underscore,	0,	0,	0x016B,	/* LATIN_SMALL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_u,	GDK_grave,	0,	0,	0x00F9,	/* LATIN_SMALL_LETTER_U_WITH_GRAVE */
  GDK_Multi_key,	GDK_u,	GDK_asciitilde,	0,	0,	0x0169,	/* LATIN_SMALL_LETTER_U_WITH_TILDE */
  GDK_Multi_key,	GDK_u,	GDK_diaeresis,	0,	0,	0x00FC,	/* LATIN_SMALL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_u,	GDK_acute,	0,	0,	0x00FA,	/* LATIN_SMALL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_u,	GDK_breve,	0,	0,	0x016D,	/* LATIN_SMALL_LETTER_U_WITH_BREVE */
  GDK_Multi_key,	GDK_v,	GDK_slash,	0,	0,	0x2123,	/* VERSICLE */
  GDK_Multi_key,	GDK_v,	GDK_2,	0,	0,	0x221A,	/* SQUARE ROOT */
  GDK_Multi_key,	GDK_v,	GDK_3,	0,	0,	0x221B,	/* CUBE ROOT */
  GDK_Multi_key,	GDK_v,	GDK_4,	0,	0,	0x221C,	/* FOURTH ROOT */
  GDK_Multi_key,	GDK_v,	GDK_Z,	0,	0,	0x017D,	/* LATIN_CAPITAL_LETTER_Z_WITH_CARON */
  GDK_Multi_key,	GDK_v,	GDK_asciicircum,	0,	0,	0x2195,	/* UP_DOWN_ARROW */
  GDK_Multi_key,	GDK_v,	GDK_l,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_Multi_key,	GDK_v,	GDK_z,	0,	0,	0x017E,	/* LATIN_SMALL_LETTER_Z_WITH_CARON */
  GDK_Multi_key,	GDK_v,	GDK_bar,	0,	0,	0x2193,	/* DOWNWARDS_ARROW */
  GDK_Multi_key,	GDK_w,	GDK_quotedbl,	0,	0,	0x1E85,	/* LATIN_SMALL_LETTER_W_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_w,	GDK_greater,	0,	0,	0x0175,	/* LATIN_SMALL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_w,	GDK_asciicircum,	0,	0,	0x0175,	/* LATIN_SMALL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_w,	GDK_diaeresis,	0,	0,	0x1E85,	/* LATIN_SMALL_LETTER_W_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_x,	GDK_0,	0,	0,	0x2297,	/* CIRCLED_TIMES */
  GDK_Multi_key,	GDK_x,	GDK_colon,	0,	0,	0x203B,	/* REFERENCE_MARK */
  GDK_Multi_key,	GDK_x,	GDK_O,	0,	0,	0x2297,	/* CIRCLED_TIMES */
  GDK_Multi_key,	GDK_x,	GDK_o,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_x,	GDK_x,	0,	0,	0x00D7,	/* MULTIPLICATION_SIGN */
  GDK_Multi_key,	GDK_y,	GDK_quotedbl,	0,	0,	0x00FF,	/* LATIN_SMALL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_y,	GDK_apostrophe,	0,	0,	0x00FD,	/* LATIN_SMALL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_y,	GDK_minus,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_y,	GDK_equal,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_y,	GDK_greater,	0,	0,	0x0177,	/* LATIN_SMALL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_y,	GDK_asciicircum,	0,	0,	0x0177,	/* LATIN_SMALL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_y,	GDK_grave,	0,	0,	0x1EF3,	/* LATIN_SMALL_LETTER_Y_WITH_GRAVE */
  GDK_Multi_key,	GDK_y,	GDK_diaeresis,	0,	0,	0x00FF,	/* LATIN_SMALL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_y,	GDK_acute,	0,	0,	0x00FD,	/* LATIN_SMALL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_z,	GDK_apostrophe,	0,	0,	0x017A,	/* LATIN_SMALL_LETTER_Z_WITH_ACUTE */
  GDK_Multi_key,	GDK_z,	GDK_period,	0,	0,	0x017C,	/* LATIN_SMALL_LETTER_Z_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_z,	GDK_less,	0,	0,	0x017E,	/* LATIN_SMALL_LETTER_Z_WITH_CARON */
  GDK_Multi_key,	GDK_z,	GDK_acute,	0,	0,	0x017A,	/* LATIN_SMALL_LETTER_Z_WITH_ACUTE */
  GDK_Multi_key,	GDK_bar,	GDK_minus,	0,	0,	0x2020,	/* DAGGER */
  GDK_Multi_key,	GDK_bar,	GDK_slash,	0,	0,	0x2224,	/* DOES_NOT_DIVIDE */
  GDK_Multi_key,	GDK_bar,	GDK_equal,	0,	0,	0x2021,	/* DOUBLE_DAGGER */
  GDK_Multi_key,	GDK_bar,	GDK_greater,	0,	0,	0x2207,	/* NABLA */
  GDK_Multi_key,	GDK_bar,	GDK_C,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_bar,	GDK_V,	0,	0,	0x2193,	/* DOWNWARDS_ARROW */
  GDK_Multi_key,	GDK_bar,	GDK_asciicircum,	0,	0,	0x2191,	/* UPWARDS_ARROW */
  GDK_Multi_key,	GDK_bar,	GDK_underscore,	0,	0,	0x22A5,	/* UP_TACK */
  GDK_Multi_key,	GDK_bar,	GDK_c,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_bar,	GDK_v,	0,	0,	0x2193,	/* DOWNWARDS_ARROW */
  GDK_Multi_key,	GDK_bar,	GDK_bar,	0,	0,	0x2225,	/* PARALLEL_TO */
  GDK_Multi_key,	GDK_bar,	GDK_Left,	0,	0,	0x21F7,	/* LEFTWARDS_ARROW_WITH_VERTICAL_STROKE */
  GDK_Multi_key,	GDK_bar,	GDK_Right,	0,	0,	0x21F8,	/* RIGHTWARDS_ARROW_WITH_VERTICAL_STROKE */
  GDK_Multi_key,	GDK_bar,	GDK_KP_Left,	0,	0,	0x21F7,	/* LEFTWARDS_ARROW_WITH_VERTICAL_STROKE */
  GDK_Multi_key,	GDK_bar,	GDK_KP_Right,	0,	0,	0x21F8,	/* RIGHTWARDS_ARROW_WITH_VERTICAL_STROKE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_space,	0,	0,	0x007E,	/* TILDE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_A,	0,	0,	0x00C3,	/* LATIN_CAPITAL_LETTER_A_WITH_TILDE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_I,	0,	0,	0x0128,	/* LATIN_CAPITAL_LETTER_I_WITH_TILDE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_N,	0,	0,	0x00D1,	/* LATIN_CAPITAL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_O,	0,	0,	0x00D5,	/* LATIN_CAPITAL_LETTER_O_WITH_TILDE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_U,	0,	0,	0x0168,	/* LATIN_CAPITAL_LETTER_U_WITH_TILDE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_a,	0,	0,	0x00E3,	/* LATIN_SMALL_LETTER_A_WITH_TILDE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_i,	0,	0,	0x0129,	/* LATIN_SMALL_LETTER_I_WITH_TILDE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_n,	0,	0,	0x00F1,	/* LATIN_SMALL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_o,	0,	0,	0x00F5,	/* LATIN_SMALL_LETTER_O_WITH_TILDE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_u,	0,	0,	0x0169,	/* LATIN_SMALL_LETTER_U_WITH_TILDE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_Left,	0,	0,	0x0303,	/* COMBINING_TILDE */
  GDK_Multi_key,	GDK_asciitilde,	GDK_KP_Left,	0,	0,	0x0303,	/* COMBINING_TILDE */
  GDK_Multi_key,	GDK_diaeresis,	GDK_A,	0,	0,	0x00C4,	/* LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_E,	0,	0,	0x00CB,	/* LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_I,	0,	0,	0x00CF,	/* LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_O,	0,	0,	0x00D6,	/* LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_U,	0,	0,	0x00DC,	/* LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_W,	0,	0,	0x1E84,	/* LATIN_CAPITAL_LETTER_W_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_Y,	0,	0,	0x0178,	/* LATIN_CAPITAL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_a,	0,	0,	0x00E4,	/* LATIN_SMALL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_e,	0,	0,	0x00EB,	/* LATIN_SMALL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_i,	0,	0,	0x00EF,	/* LATIN_SMALL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_o,	0,	0,	0x00F6,	/* LATIN_SMALL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_u,	0,	0,	0x00FC,	/* LATIN_SMALL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_w,	0,	0,	0x1E85,	/* LATIN_SMALL_LETTER_W_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_y,	0,	0,	0x00FF,	/* LATIN_SMALL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_Left,	0,	0,	0x0308,	/* COMBINING_DIAERESIS */
  GDK_Multi_key,	GDK_acute,	GDK_A,	0,	0,	0x00C1,	/* LATIN_CAPITAL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_C,	0,	0,	0x0106,	/* LATIN_CAPITAL_LETTER_C_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_E,	0,	0,	0x00C9,	/* LATIN_CAPITAL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_I,	0,	0,	0x00CD,	/* LATIN_CAPITAL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_L,	0,	0,	0x0139,	/* LATIN_CAPITAL_LETTER_L_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_N,	0,	0,	0x0143,	/* LATIN_CAPITAL_LETTER_N_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_O,	0,	0,	0x00D3,	/* LATIN_CAPITAL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_R,	0,	0,	0x0154,	/* LATIN_CAPITAL_LETTER_R_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_S,	0,	0,	0x015A,	/* LATIN_CAPITAL_LETTER_S_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_U,	0,	0,	0x00DA,	/* LATIN_CAPITAL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_Y,	0,	0,	0x00DD,	/* LATIN_CAPITAL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_Z,	0,	0,	0x0179,	/* LATIN_CAPITAL_LETTER_Z_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_a,	0,	0,	0x00E1,	/* LATIN_SMALL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_c,	0,	0,	0x0107,	/* LATIN_SMALL_LETTER_C_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_e,	0,	0,	0x00E9,	/* LATIN_SMALL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_i,	0,	0,	0x00ED,	/* LATIN_SMALL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_l,	0,	0,	0x013A,	/* LATIN_SMALL_LETTER_L_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_n,	0,	0,	0x0144,	/* LATIN_SMALL_LETTER_N_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_o,	0,	0,	0x00F3,	/* LATIN_SMALL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_r,	0,	0,	0x0155,	/* LATIN_SMALL_LETTER_R_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_s,	0,	0,	0x015B,	/* LATIN_SMALL_LETTER_S_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_u,	0,	0,	0x00FA,	/* LATIN_SMALL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_y,	0,	0,	0x00FD,	/* LATIN_SMALL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_z,	0,	0,	0x017A,	/* LATIN_SMALL_LETTER_Z_WITH_ACUTE */
  GDK_Multi_key,	GDK_cedilla,	GDK_C,	0,	0,	0x00C7,	/* LATIN_CAPITAL_LETTER_C_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_G,	0,	0,	0x0122,	/* LATIN_CAPITAL_LETTER_G_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_K,	0,	0,	0x0136,	/* LATIN_CAPITAL_LETTER_K_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_L,	0,	0,	0x013B,	/* LATIN_CAPITAL_LETTER_L_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_N,	0,	0,	0x0145,	/* LATIN_CAPITAL_LETTER_N_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_R,	0,	0,	0x0156,	/* LATIN_CAPITAL_LETTER_R_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_S,	0,	0,	0x015E,	/* LATIN_CAPITAL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_c,	0,	0,	0x00E7,	/* LATIN_SMALL_LETTER_C_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_g,	0,	0,	0x0123,	/* LATIN_SMALL_LETTER_G_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_k,	0,	0,	0x0137,	/* LATIN_SMALL_LETTER_K_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_l,	0,	0,	0x013C,	/* LATIN_SMALL_LETTER_L_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_n,	0,	0,	0x0146,	/* LATIN_SMALL_LETTER_N_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_r,	0,	0,	0x0157,	/* LATIN_SMALL_LETTER_R_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_s,	0,	0,	0x015F,	/* LATIN_SMALL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_breve,	GDK_A,	0,	0,	0x0102,	/* LATIN_CAPITAL_LETTER_A_WITH_BREVE */
  GDK_Multi_key,	GDK_breve,	GDK_E,	0,	0,	0x0114,	/* LATIN_CAPITAL_LETTER_E_WITH_BREVE */
  GDK_Multi_key,	GDK_breve,	GDK_G,	0,	0,	0x011E,	/* LATIN_CAPITAL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_breve,	GDK_I,	0,	0,	0x012C,	/* LATIN_CAPITAL_LETTER_I_WITH_BREVE */
  GDK_Multi_key,	GDK_breve,	GDK_O,	0,	0,	0x014E,	/* LATIN_CAPITAL_LETTER_O_WITH_BREVE */
  GDK_Multi_key,	GDK_breve,	GDK_U,	0,	0,	0x016C,	/* LATIN_CAPITAL_LETTER_U_WITH_BREVE */
  GDK_Multi_key,	GDK_breve,	GDK_a,	0,	0,	0x0103,	/* LATIN_SMALL_LETTER_A_WITH_BREVE */
  GDK_Multi_key,	GDK_breve,	GDK_e,	0,	0,	0x0115,	/* LATIN_SMALL_LETTER_E_WITH_BREVE */
  GDK_Multi_key,	GDK_breve,	GDK_g,	0,	0,	0x011F,	/* LATIN_SMALL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_breve,	GDK_i,	0,	0,	0x012D,	/* LATIN_SMALL_LETTER_I_WITH_BREVE */
  GDK_Multi_key,	GDK_breve,	GDK_o,	0,	0,	0x014F,	/* LATIN_SMALL_LETTER_O_WITH_BREVE */
  GDK_Multi_key,	GDK_breve,	GDK_u,	0,	0,	0x016D,	/* LATIN_SMALL_LETTER_U_WITH_BREVE */
  GDK_Multi_key,	GDK_Left,	GDK_space,	0,	0,	0x2190,	/* LEFTWARDS_ARROW */
  GDK_Multi_key,	GDK_Left,	GDK_quotedbl,	0,	0,	0x0308,	/* COMBINING_DIAERESIS */
  GDK_Multi_key,	GDK_Left,	GDK_apostrophe,	0,	0,	0x0301,	/* COMBINING_ACUTE_ACCENT */
  GDK_Multi_key,	GDK_Left,	GDK_parenleft,	0,	0,	0x0306,	/* COMBINING_BREVE */
  GDK_Multi_key,	GDK_Left,	GDK_asterisk,	0,	0,	0x030A,	/* COMBINING_RING_ABOVE */
  GDK_Multi_key,	GDK_Left,	GDK_comma,	0,	0,	0x0328,	/* COMBINING_OGONEK */
  GDK_Multi_key,	GDK_Left,	GDK_minus,	0,	0,	0x0304,	/* COMBINING_MACRON */
  GDK_Multi_key,	GDK_Left,	GDK_period,	0,	0,	0x0307,	/* COMBINING_DOT_ABOVE */
  /* GDK_Multi_key,	GDK_Left,	GDK_slash,	0,	0,	0x0337,	*//* COMBINING_LONG_SOLIDUS_OVERLAY */
  GDK_Multi_key,	GDK_Left,	GDK_slash,	0,	0,	0x219A,	/* LEFTWARDS_ARROW_WITH_STROKE */
  GDK_Multi_key,	GDK_Left,	GDK_2,	0,	0,	0x21C7,	/* LEFTWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_Left,	GDK_colon,	0,	0,	0x030B,	/* COMBINING_DOUBLE_ACUTE_ACCENT */
  GDK_Multi_key,	GDK_Left,	GDK_less,	0,	0,	0x030C,	/* COMBINING_CARON */
  GDK_Multi_key,	GDK_Left,	GDK_equal,	0,	0,	0x21D0,	/* LEFTWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_Left,	GDK_greater,	0,	0,	0x0302,	/* COMBINING_CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_Left,	GDK_asciicircum,	0,	0,	0x0302,	/* COMBINING_CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_Left,	GDK_underscore,	0,	0,	0x0332,	/* COMBINING_LOW_LINE */
  GDK_Multi_key,	GDK_Left,	GDK_grave,	0,	0,	0x0300,	/* COMBINING_GRAVE_ACCENT */
  GDK_Multi_key,	GDK_Left,	GDK_bar,	0,	0,	0x21F7,	/* LEFTWARDS_ARROW_WITH_VERTICAL_STROKE */
  GDK_Multi_key,	GDK_Left,	GDK_asciitilde,	0,	0,	0x0303,	/* COMBINING_TILDE */
  GDK_Multi_key,	GDK_Left,	GDK_diaeresis,	0,	0,	0x0308,	/* COMBINING_DIAERESIS */
  GDK_Multi_key,	GDK_Left,	GDK_Left,	0,	0,	0x2190,	/* LEFTWARDS_ARROW */
  GDK_Multi_key,	GDK_Left,	GDK_Up,	0,	0,	0x2196,	/* NORTH_WEST_ARROW */
  GDK_Multi_key,	GDK_Left,	GDK_Right,	0,	0,	0x2194,	/* LEFT_RIGHT_ARROW */
  GDK_Multi_key,	GDK_Left,	GDK_Down,	0,	0,	0x2199,	/* SOUTH_WEST_ARROW */
  GDK_Multi_key,	GDK_Up,	GDK_space,	0,	0,	0x2191,	/* UPWARDS_ARROW */
  GDK_Multi_key,	GDK_Up,	GDK_2,	0,	0,	0x21C8,	/* UPWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_Up,	GDK_equal,	0,	0,	0x21D1,	/* UPWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_Up,	GDK_Left,	0,	0,	0x2196,	/* NORTH_WEST_ARROW */
  GDK_Multi_key,	GDK_Up,	GDK_Up,	0,	0,	0x2191,	/* UPWARDS_ARROW */
  GDK_Multi_key,	GDK_Up,	GDK_Right,	0,	0,	0x2197,	/* NORTH_EAST_ARROW */
  GDK_Multi_key,	GDK_Up,	GDK_Down,	0,	0,	0x2195,	/* UP_DOWN_ARROW */
  GDK_Multi_key,	GDK_Right,	GDK_space,	0,	0,	0x2192,	/* RIGHTWARDS_ARROW */
  GDK_Multi_key,	GDK_Right,	GDK_slash,	0,	0,	0x219B,	/* RIGHTWARDS_ARROW_WITH_STROKE */
  GDK_Multi_key,	GDK_Right,	GDK_0,	0,	0,	0x21F4,	/* RIGHT_ARROW_WITH_SMALL_CIRCLE */
  GDK_Multi_key,	GDK_Right,	GDK_2,	0,	0,	0x21C9,	/* RIGHTWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_Right,	GDK_3,	0,	0,	0x21F6,	/* THREE_RIGHTWARDS_ARROWS */
  GDK_Multi_key,	GDK_Right,	GDK_equal,	0,	0,	0x21D2,	/* RIGHTWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_Right,	GDK_bar,	0,	0,	0x21F8,	/* RIGHTWARDS_ARROW_WITH_VERTICAL_STROKE */
  GDK_Multi_key,	GDK_Right,	GDK_Left,	0,	0,	0x2194,	/* LEFT_RIGHT_ARROW */
  GDK_Multi_key,	GDK_Right,	GDK_Up,	0,	0,	0x2197,	/* NORTH_EAST_ARROW */
  GDK_Multi_key,	GDK_Right,	GDK_Right,	0,	0,	0x2192,	/* RIGHTWARDS_ARROW */
  GDK_Multi_key,	GDK_Right,	GDK_Down,	0,	0,	0x2198,	/* SOUTH_EAST_ARROW */
  GDK_Multi_key,	GDK_Down,	GDK_space,	0,	0,	0x2193,	/* DOWNWARDS_ARROW */
  GDK_Multi_key,	GDK_Down,	GDK_2,	0,	0,	0x21CA,	/* DOWNWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_Down,	GDK_equal,	0,	0,	0x21D3,	/* DOWNWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_Down,	GDK_Left,	0,	0,	0x2199,	/* SOUTH_WEST_ARROW */
  GDK_Multi_key,	GDK_Down,	GDK_Up,	0,	0,	0x2195,	/* UP_DOWN_ARROW */
  GDK_Multi_key,	GDK_Down,	GDK_Right,	0,	0,	0x2198,	/* SOUTH_EAST_ARROW */
  GDK_Multi_key,	GDK_Down,	GDK_Down,	0,	0,	0x2193,	/* DOWNWARDS_ARROW */
  GDK_Multi_key,	GDK_KP_Left,	GDK_space,	0,	0,	0x2190,	/* LEFTWARDS_ARROW */
  GDK_Multi_key,	GDK_KP_Left,	GDK_quotedbl,	0,	0,	0x0308,	/* COMBINING_DIAERESIS */
  GDK_Multi_key,	GDK_KP_Left,	GDK_apostrophe,	0,	0,	0x0301,	/* COMBINING_ACUTE_ACCENT */
  GDK_Multi_key,	GDK_KP_Left,	GDK_parenleft,	0,	0,	0x0306,	/* COMBINING_BREVE */
  GDK_Multi_key,	GDK_KP_Left,	GDK_asterisk,	0,	0,	0x030A,	/* COMBINING_RING_ABOVE */
  GDK_Multi_key,	GDK_KP_Left,	GDK_minus,	0,	0,	0x0304,	/* COMBINING_MACRON */
  GDK_Multi_key,	GDK_KP_Left,	GDK_period,	0,	0,	0x0307,	/* COMBINING_DOT_ABOVE */
  /* GDK_Multi_key,	GDK_KP_Left,	GDK_slash,	0,	0,	0x0337,	*//* COMBINING_LONG_SOLIDUS_OVERLAY */
  GDK_Multi_key,	GDK_KP_Left,	GDK_slash,	0,	0,	0x219A,	/* LEFTWARDS_ARROW_WITH_STROKE */
  GDK_Multi_key,	GDK_KP_Left,	GDK_2,	0,	0,	0x21C7,	/* LEFTWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_KP_Left,	GDK_colon,	0,	0,	0x030B,	/* COMBINING_DOUBLE_ACUTE_ACCENT */
  GDK_Multi_key,	GDK_KP_Left,	GDK_equal,	0,	0,	0x21D0,	/* LEFTWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_KP_Left,	GDK_asciicircum,	0,	0,	0x0302,	/* COMBINING_CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_KP_Left,	GDK_underscore,	0,	0,	0x0332,	/* COMBINING_LOW_LINE */
  GDK_Multi_key,	GDK_KP_Left,	GDK_grave,	0,	0,	0x0300,	/* COMBINING_GRAVE_ACCENT */
  GDK_Multi_key,	GDK_KP_Left,	GDK_bar,	0,	0,	0x21F7,	/* LEFTWARDS_ARROW_WITH_VERTICAL_STROKE */
  GDK_Multi_key,	GDK_KP_Left,	GDK_asciitilde,	0,	0,	0x0303,	/* COMBINING_TILDE */
  GDK_Multi_key,	GDK_KP_Left,	GDK_diaeresis,	0,	0,	0x0308,	/* COMBINING_DIAERESIS */
  GDK_Multi_key,	GDK_KP_Left,	GDK_KP_Left,	0,	0,	0x2190,	/* LEFTWARDS_ARROW */
  GDK_Multi_key,	GDK_KP_Left,	GDK_KP_Up,	0,	0,	0x2196,	/* NORTH_WEST_ARROW */
  GDK_Multi_key,	GDK_KP_Left,	GDK_KP_Right,	0,	0,	0x2194,	/* LEFT_RIGHT_ARROW */
  GDK_Multi_key,	GDK_KP_Left,	GDK_KP_Down,	0,	0,	0x2199,	/* SOUTH_WEST_ARROW */
  GDK_Multi_key,	GDK_KP_Up,	GDK_space,	0,	0,	0x2191,	/* UPWARDS_ARROW */
  GDK_Multi_key,	GDK_KP_Up,	GDK_2,	0,	0,	0x21C8,	/* UPWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_KP_Up,	GDK_equal,	0,	0,	0x21D1,	/* UPWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_KP_Up,	GDK_KP_Left,	0,	0,	0x2196,	/* NORTH_WEST_ARROW */
  GDK_Multi_key,	GDK_KP_Up,	GDK_KP_Up,	0,	0,	0x2191,	/* UPWARDS_ARROW */
  GDK_Multi_key,	GDK_KP_Up,	GDK_KP_Right,	0,	0,	0x2197,	/* NORTH_EAST_ARROW */
  GDK_Multi_key,	GDK_KP_Up,	GDK_KP_Down,	0,	0,	0x2195,	/* UP_DOWN_ARROW */
  GDK_Multi_key,	GDK_KP_Right,	GDK_space,	0,	0,	0x2192,	/* RIGHTWARDS_ARROW */
  GDK_Multi_key,	GDK_KP_Right,	GDK_slash,	0,	0,	0x219B,	/* RIGHTWARDS_ARROW_WITH_STROKE */
  GDK_Multi_key,	GDK_KP_Right,	GDK_0,	0,	0,	0x21F4,	/* RIGHT_ARROW_WITH_SMALL_CIRCLE */
  GDK_Multi_key,	GDK_KP_Right,	GDK_2,	0,	0,	0x21C9,	/* RIGHTWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_KP_Right,	GDK_3,	0,	0,	0x21F6,	/* THREE_RIGHTWARDS_ARROWS */
  GDK_Multi_key,	GDK_KP_Right,	GDK_equal,	0,	0,	0x21D2,	/* RIGHTWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_KP_Right,	GDK_bar,	0,	0,	0x21F8,	/* RIGHTWARDS_ARROW_WITH_VERTICAL_STROKE */
  GDK_Multi_key,	GDK_KP_Right,	GDK_KP_Left,	0,	0,	0x2194,	/* LEFT_RIGHT_ARROW */
  GDK_Multi_key,	GDK_KP_Right,	GDK_KP_Up,	0,	0,	0x2197,	/* NORTH_EAST_ARROW */
  GDK_Multi_key,	GDK_KP_Right,	GDK_KP_Right,	0,	0,	0x2192,	/* RIGHTWARDS_ARROW */
  GDK_Multi_key,	GDK_KP_Right,	GDK_KP_Down,	0,	0,	0x2198,	/* SOUTH_EAST_ARROW */
  GDK_Multi_key,	GDK_KP_Down,	GDK_space,	0,	0,	0x2193,	/* DOWNWARDS_ARROW */
  GDK_Multi_key,	GDK_KP_Down,	GDK_2,	0,	0,	0x21CA,	/* DOWNWARDS_PAIRED_ARROWS */
  GDK_Multi_key,	GDK_KP_Down,	GDK_equal,	0,	0,	0x21D3,	/* DOWNWARDS_DOUBLE_ARROW */
  GDK_Multi_key,	GDK_KP_Down,	GDK_KP_Left,	0,	0,	0x2199,	/* SOUTH_WEST_ARROW */
  GDK_Multi_key,	GDK_KP_Down,	GDK_KP_Up,	0,	0,	0x2195,	/* UP_DOWN_ARROW */
  GDK_Multi_key,	GDK_KP_Down,	GDK_KP_Right,	0,	0,	0x2198,	/* SOUTH_EAST_ARROW */
  GDK_Multi_key,	GDK_KP_Down,	GDK_KP_Down,	0,	0,	0x2193,	/* DOWNWARDS_ARROW */
};

static const GtkComposeTable latinplus_compose_table = {
  latinplus_compose_seqs,
  4,
  G_N_ELEMENTS (latinplus_compose_seqs) / 6
};

static void     latinplus_class_init         (GtkIMContextSimpleClass  *class);
static void     latinplus_init               (GtkIMContextSimple       *im_context_simple);

static GtkIMContextSimpleClass *parent_class = NULL;

static GType type_latinplus = 0;

static const GtkIMContextInfo latinplus_info = 
{
  "latin-plus",     /* ID */
  N_("Latin Plus"), /* Human readable name */
  GETTEXT_PACKAGE, /* Translation domain */
  LOCALEDIR,       /* Dir for bindtextdomain */
  "br:ca:ch:cs:cy:da:de:en:eo:es:et:eu:fi:fo:fr:fy:ga:gd:gl:hr:hu:id:is:it:kl:lb:lt:lv:mi:mt:nl:no:oc:pl:pt:rm:sk:sl:sq:sv:tk:tl:tn",            /* Languages for which this module is the default */  
};

static void
latinplus_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (GtkIMContextSimpleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) latinplus_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (GtkIMContextSimple),
    0,              /* n_preallocs */
    (GInstanceInitFunc) latinplus_init,
  };
  type_latinplus = 
    g_type_module_register_type (module,
                                 GTK_TYPE_IM_CONTEXT_SIMPLE,
                                 "GtkIMContextSimpleLatinplus",
                                 &object_info, 0);
}


static void clipboard_request_test_cb (GtkClipboard *clipboard,
					const gchar *text,
					gpointer data)
{
  GtkIMContext *context = (GtkIMContext *) data;
  if (text)
    g_signal_emit_by_name (context, "commit", text);
}

static gboolean
latinplus_filter_keypress (GtkIMContext *context,
				       GdkEventKey  *event)
{
    GtkIMContextSimple *context_simple = GTK_IM_CONTEXT_SIMPLE (context);

    if (event->keyval == GDK_Insert &&  context_simple->compose_buffer[0] == GDK_Multi_key)
      if (event->type == GDK_KEY_PRESS)
	return TRUE;
      else
      {
	gtk_clipboard_request_text (gtk_clipboard_get(GDK_SELECTION_PRIMARY),
				      clipboard_request_test_cb,
				      context);
	context_simple->compose_buffer[0] = 0;

      }
    return ((GtkIMContextClass *)parent_class)->filter_keypress(context, event);
}

static void
latinplus_class_init (GtkIMContextSimpleClass *klass)
{
  parent_class = GTK_IM_CONTEXT_SIMPLE_CLASS (g_type_class_peek (g_type_parent (type_latinplus)));
  ((GtkIMContextClass *)klass)->filter_keypress = latinplus_filter_keypress;
}

void 
im_module_exit ()
{

}

static void
latinplus_init (GtkIMContextSimple *im_context_simple)
{
  gtk_im_context_simple_add_table (im_context_simple,
				   latinplus_compose_seqs,
				   4,
				   G_N_ELEMENTS (latinplus_compose_seqs) / 6);
}


static const GtkIMContextInfo *info_list[] = 
{
  &latinplus_info,
};


void
im_module_init (GTypeModule *module)
{
  latinplus_register_type (module);
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
  if (strcmp (context_id, "latin-plus") == 0)
    return GTK_IM_CONTEXT_SIMPLE (g_object_new (type_latinplus, NULL));
  else
    return NULL;
}
