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
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtkaccelgroup.h>
#include "gtkimcontextethio.h"

typedef struct _GtkComposeTable GtkComposeTable;

struct _GtkComposeTable 
{
  const guint16 *data;
  gint max_seq_len;
  gint n_seqs;
};

/* The following table was generated from the X compose tables include with
 * XFree86 4.0 using a set of Perl scripts. Contact Owen Taylor <otaylor@redhat.com>
 * to obtain the relevant perl scripts.
 *
 * The following compose letter letter sequences confliced
 *   Dstroke/dstroke and ETH/eth; resolved to Dstroke (Croation, Vietnamese, Lappish), over 
 *                                ETH (Icelandic, Faroese, old English, IPA)   [ D- -D d- -d ]
 *   Amacron/amacron and ordfeminine; resolved to ordfeminine                  [ _A A_ a_ _a ]
 *   Amacron/amacron and Atilde/atilde; resolved to atilde	      	      [ -A A- a- -a ]
 *   Omacron/Omacron and masculine; resolved to masculine		      [ _A A_ a_ _a ]
 *   Omacron/omacron and Otilde/atilde; resolved to otilde		      [ -O O- o- -o ]
 *
 * [ Amacron and Omacron are in Latin-4 (Baltic). ordfeminine and masculine are used for
 *   spanish. atilde and otilde are used at least for Portuguese ]
 *
 *   at and Aring; resolved to Aring					      [ AA ]
 *   guillemotleft and caron; resolved to guillemotleft			      [ << ]
 *   ogonek and cedilla; resolved to cedilla				      [ ,, ]
 * 
 * This probably should be resolved by first checking an additional set of compose tables
 * that depend on the locale or selected input method. 
 */

static const guint16 gtk_compose_seqs[] = {
  GDK_dead_grave,	GDK_space,	0,	0,	0,	0x0060,	/* GRAVE_ACCENT */
  GDK_dead_grave,	GDK_A,	0,	0,	0,	0x00C0,	/* LATIN_CAPITAL_LETTER_A_WITH_GRAVE */
  GDK_dead_grave,	GDK_E,	0,	0,	0,	0x00C8,	/* LATIN_CAPITAL_LETTER_E_WITH_GRAVE */
  GDK_dead_grave,	GDK_I,	0,	0,	0,	0x00CC,	/* LATIN_CAPITAL_LETTER_I_WITH_GRAVE */
  GDK_dead_grave,	GDK_O,	0,	0,	0,	0x00D2,	/* LATIN_CAPITAL_LETTER_O_WITH_GRAVE */
  GDK_dead_grave,	GDK_U,	0,	0,	0,	0x00D9,	/* LATIN_CAPITAL_LETTER_U_WITH_GRAVE */
  GDK_dead_grave,       GDK_grave,      0,      0,      0,      0x0060, /* GRAVE_ACCENT */
  GDK_dead_grave,	GDK_a,	0,	0,	0,	0x00E0,	/* LATIN_SMALL_LETTER_A_WITH_GRAVE */
  GDK_dead_grave,	GDK_e,	0,	0,	0,	0x00E8,	/* LATIN_SMALL_LETTER_E_WITH_GRAVE */
  GDK_dead_grave,	GDK_i,	0,	0,	0,	0x00EC,	/* LATIN_SMALL_LETTER_I_WITH_GRAVE */
  GDK_dead_grave,	GDK_o,	0,	0,	0,	0x00F2,	/* LATIN_SMALL_LETTER_O_WITH_GRAVE */
  GDK_dead_grave,	GDK_u,	0,	0,	0,	0x00F9,	/* LATIN_SMALL_LETTER_U_WITH_GRAVE */
  GDK_dead_grave,       GDK_dead_grave, 0,      0,      0,      0x0060, /* GRAVE_ACCENT */  
  GDK_dead_acute,	GDK_space,	0,	0,	0,	0x0027,	/* APOSTROPHE */
  GDK_dead_acute,	GDK_apostrophe,	0,	0,	0,	0x00B4,	/* ACUTE_ACCENT */
  GDK_dead_acute,	GDK_A,	0,	0,	0,	0x00C1,	/* LATIN_CAPITAL_LETTER_A_WITH_ACUTE */
  GDK_dead_acute,	GDK_C,	0,	0,	0,	0x0106,	/* LATIN_CAPITAL_LETTER_C_WITH_ACUTE */
  GDK_dead_acute,	GDK_E,	0,	0,	0,	0x00C9,	/* LATIN_CAPITAL_LETTER_E_WITH_ACUTE */
  GDK_dead_acute,	GDK_I,	0,	0,	0,	0x00CD,	/* LATIN_CAPITAL_LETTER_I_WITH_ACUTE */
  GDK_dead_acute,	GDK_L,	0,	0,	0,	0x0139,	/* LATIN_CAPITAL_LETTER_L_WITH_ACUTE */
  GDK_dead_acute,	GDK_N,	0,	0,	0,	0x0143,	/* LATIN_CAPITAL_LETTER_N_WITH_ACUTE */
  GDK_dead_acute,	GDK_O,	0,	0,	0,	0x00D3,	/* LATIN_CAPITAL_LETTER_O_WITH_ACUTE */
  GDK_dead_acute,	GDK_R,	0,	0,	0,	0x0154,	/* LATIN_CAPITAL_LETTER_R_WITH_ACUTE */
  GDK_dead_acute,	GDK_S,	0,	0,	0,	0x015A,	/* LATIN_CAPITAL_LETTER_S_WITH_ACUTE */
  GDK_dead_acute,	GDK_U,	0,	0,	0,	0x00DA,	/* LATIN_CAPITAL_LETTER_U_WITH_ACUTE */
  GDK_dead_acute,	GDK_Y,	0,	0,	0,	0x00DD,	/* LATIN_CAPITAL_LETTER_Y_WITH_ACUTE */
  GDK_dead_acute,	GDK_Z,	0,	0,	0,	0x0179,	/* LATIN_CAPITAL_LETTER_Z_WITH_ACUTE */
  GDK_dead_acute,	GDK_a,	0,	0,	0,	0x00E1,	/* LATIN_SMALL_LETTER_A_WITH_ACUTE */
  GDK_dead_acute,	GDK_c,	0,	0,	0,	0x0107,	/* LATIN_SMALL_LETTER_C_WITH_ACUTE */
  GDK_dead_acute,	GDK_e,	0,	0,	0,	0x00E9,	/* LATIN_SMALL_LETTER_E_WITH_ACUTE */
  GDK_dead_acute,	GDK_i,	0,	0,	0,	0x00ED,	/* LATIN_SMALL_LETTER_I_WITH_ACUTE */
  GDK_dead_acute,	GDK_l,	0,	0,	0,	0x013A,	/* LATIN_SMALL_LETTER_L_WITH_ACUTE */
  GDK_dead_acute,	GDK_n,	0,	0,	0,	0x0144,	/* LATIN_SMALL_LETTER_N_WITH_ACUTE */
  GDK_dead_acute,	GDK_o,	0,	0,	0,	0x00F3,	/* LATIN_SMALL_LETTER_O_WITH_ACUTE */
  GDK_dead_acute,	GDK_r,	0,	0,	0,	0x0155,	/* LATIN_SMALL_LETTER_R_WITH_ACUTE */
  GDK_dead_acute,	GDK_s,	0,	0,	0,	0x015B,	/* LATIN_SMALL_LETTER_S_WITH_ACUTE */
  GDK_dead_acute,	GDK_u,	0,	0,	0,	0x00FA,	/* LATIN_SMALL_LETTER_U_WITH_ACUTE */
  GDK_dead_acute,	GDK_y,	0,	0,	0,	0x00FD,	/* LATIN_SMALL_LETTER_Y_WITH_ACUTE */
  GDK_dead_acute,	GDK_z,	0,	0,	0,	0x017A,	/* LATIN_SMALL_LETTER_Z_WITH_ACUTE */
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
  GDK_dead_circumflex,	GDK_E,	0,	0,	0,	0x00CA,	/* LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_I,	0,	0,	0,	0x00CE,	/* LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_O,	0,	0,	0,	0x00D4,	/* LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_U,	0,	0,	0,	0x00DB,	/* LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_asciicircum,	0,	0,	0,	0x005E,	/* CIRCUMFLEX_ACCENT */
  GDK_dead_circumflex,	GDK_underscore,	0,	0,	0,	0x00AF,	/* MACRON */
  GDK_dead_circumflex,	GDK_a,	0,	0,	0,	0x00E2,	/* LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_e,	0,	0,	0,	0x00EA,	/* LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_i,	0,	0,	0,	0x00EE,	/* LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_o,	0,	0,	0,	0x00F4,	/* LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_u,	0,	0,	0,	0x00FB,	/* LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_dead_circumflex,	GDK_dead_circumflex,	0,	0,	0,	0x005E,	/* CIRCUMFLEX_ACCENT */
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
  GDK_dead_macron,	GDK_U,	0,	0,	0,	0x016A,	/* LATIN_CAPITAL_LETTER_U_WITH_MACRON */
  GDK_dead_macron,	GDK_a,	0,	0,	0,	0x0101,	/* LATIN_SMALL_LETTER_A_WITH_MACRON */
  GDK_dead_macron,	GDK_e,	0,	0,	0,	0x0113,	/* LATIN_SMALL_LETTER_E_WITH_MACRON */
  GDK_dead_macron,	GDK_i,	0,	0,	0,	0x012B,	/* LATIN_SMALL_LETTER_I_WITH_MACRON */
  GDK_dead_macron,	GDK_o,	0,	0,	0,	0x014D,	/* LATIN_SMALL_LETTER_O_WITH_MACRON */
  GDK_dead_macron,	GDK_u,	0,	0,	0,	0x016B,	/* LATIN_SMALL_LETTER_U_WITH_MACRON */
  GDK_dead_macron,	GDK_macron,	0,	0,	0,	0x00AF,	/* MACRON */
  GDK_dead_macron,	GDK_dead_macron,	0,	0,	0,	0x00AF,	/* MACRON */
  GDK_dead_breve,	GDK_space,	0,	0,	0,	0x02D8,	/* BREVE */
  GDK_dead_breve,	GDK_A,	0,	0,	0,	0x0102,	/* LATIN_CAPITAL_LETTER_A_WITH_BREVE */
  GDK_dead_breve,	GDK_G,	0,	0,	0,	0x011E,	/* LATIN_CAPITAL_LETTER_G_WITH_BREVE */
  GDK_dead_breve,	GDK_a,	0,	0,	0,	0x0103,	/* LATIN_SMALL_LETTER_A_WITH_BREVE */
  GDK_dead_breve,	GDK_g,	0,	0,	0,	0x011F,	/* LATIN_SMALL_LETTER_G_WITH_BREVE */
  GDK_dead_breve,	GDK_dead_breve,	0,	0,	0,	0x02D8,	/* BREVE */
  GDK_dead_abovedot,	GDK_space,	0,	0,	0,	0x02D9,	/* DOT_ABOVE */
  GDK_dead_abovedot,	GDK_E,	0,	0,	0,	0x0116,	/* LATIN_CAPITAL_LETTER_E_WITH_DOT_ABOVE */
  GDK_dead_abovedot,	GDK_I,	0,	0,	0,	0x0130,	/* LATIN_CAPITAL_LETTER_I_WITH_DOT_ABOVE */
  GDK_dead_abovedot,	GDK_Z,	0,	0,	0,	0x017B,	/* LATIN_CAPITAL_LETTER_Z_WITH_DOT_ABOVE */
  GDK_dead_abovedot,	GDK_e,	0,	0,	0,	0x0117,	/* LATIN_SMALL_LETTER_E_WITH_DOT_ABOVE */
  GDK_dead_abovedot,	GDK_i,	0,	0,	0,	0x0131,	/* LATIN_SMALL_LETTER_DOTLESS_I */
  GDK_dead_abovedot,	GDK_z,	0,	0,	0,	0x017C,	/* LATIN_SMALL_LETTER_Z_WITH_DOT_ABOVE */
  GDK_dead_abovedot,	GDK_abovedot,	0,	0,	0,	0x02D9,	/* DOT_ABOVE */
  GDK_dead_abovedot,	GDK_dead_abovedot,	0,	0,	0,	0x02D9,	/* DOT_ABOVE */
  GDK_dead_diaeresis,	GDK_space,	0,	0,	0,	0x0022,	/* QUOTATION_MARK */
  GDK_dead_diaeresis,	GDK_quotedbl,	0,	0,	0,	0x00A8,	/* DIAERESIS */
  GDK_dead_diaeresis,	GDK_A,	0,	0,	0,	0x00C4,	/* LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_E,	0,	0,	0,	0x00CB,	/* LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_I,	0,	0,	0,	0x00CF,	/* LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_O,	0,	0,	0,	0x00D6,	/* LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_U,	0,	0,	0,	0x00DC,	/* LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_Y,	0,	0,	0,	0x0178,	/* LATIN_CAPITAL_LETTER_Y_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_a,	0,	0,	0,	0x00E4,	/* LATIN_SMALL_LETTER_A_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_e,	0,	0,	0,	0x00EB,	/* LATIN_SMALL_LETTER_E_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_i,	0,	0,	0,	0x00EF,	/* LATIN_SMALL_LETTER_I_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_o,	0,	0,	0,	0x00F6,	/* LATIN_SMALL_LETTER_O_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_u,	0,	0,	0,	0x00FC,	/* LATIN_SMALL_LETTER_U_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_y,	0,	0,	0,	0x00FF,	/* LATIN_SMALL_LETTER_Y_WITH_DIAERESIS */
  GDK_dead_diaeresis,	GDK_diaeresis,	0,	0,	0,	0x00A8,	/* DIAERESIS */
  GDK_dead_diaeresis,	GDK_dead_diaeresis,	0,	0,	0,	0x00A8,	/* DIAERESIS */
  GDK_dead_abovering,	GDK_space,	0,	0,	0,	0x02DA,	/* RING_ABOVE */
  GDK_dead_abovering,	GDK_A,	0,	0,	0,	0x00C5,	/* LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE */
  GDK_dead_abovering,	GDK_U,	0,	0,	0,	0x016E,	/* LATIN_CAPITAL_LETTER_U_WITH_RING_ABOVE */
  GDK_dead_abovering,	GDK_a,	0,	0,	0,	0x00E5,	/* LATIN_SMALL_LETTER_A_WITH_RING_ABOVE */
  GDK_dead_abovering,	GDK_u,	0,	0,	0,	0x016F,	/* LATIN_SMALL_LETTER_U_WITH_RING_ABOVE */
  GDK_dead_abovering,	GDK_dead_abovering,	0,	0,	0,	0x02DA,	/* RING_ABOVE */
  GDK_dead_doubleacute,	GDK_space,	0,	0,	0,	0x02DD,	/* DOUBLE_ACUTE_ACCENT */
  GDK_dead_doubleacute,	GDK_O,	0,	0,	0,	0x0150,	/* LATIN_CAPITAL_LETTER_O_WITH_DOUBLE_ACUTE */
  GDK_dead_doubleacute,	GDK_U,	0,	0,	0,	0x0170,	/* LATIN_CAPITAL_LETTER_U_WITH_DOUBLE_ACUTE */
  GDK_dead_doubleacute,	GDK_o,	0,	0,	0,	0x0151,	/* LATIN_SMALL_LETTER_O_WITH_DOUBLE_ACUTE */
  GDK_dead_doubleacute,	GDK_u,	0,	0,	0,	0x0171,	/* LATIN_SMALL_LETTER_U_WITH_DOUBLE_ACUTE */
  GDK_dead_doubleacute,	GDK_dead_doubleacute,	0,	0,	0,	0x02DD,	/* DOUBLE_ACUTE_ACCENT */
  GDK_dead_caron,	GDK_space,	0,	0,	0,	0x02C7,	/* CARON */
  GDK_dead_caron,	GDK_C,	0,	0,	0,	0x010C,	/* LATIN_CAPITAL_LETTER_C_WITH_CARON */
  GDK_dead_caron,	GDK_D,	0,	0,	0,	0x010E,	/* LATIN_CAPITAL_LETTER_D_WITH_CARON */
  GDK_dead_caron,	GDK_E,	0,	0,	0,	0x011A,	/* LATIN_CAPITAL_LETTER_E_WITH_CARON */
  GDK_dead_caron,	GDK_L,	0,	0,	0,	0x013D,	/* LATIN_CAPITAL_LETTER_L_WITH_CARON */
  GDK_dead_caron,	GDK_N,	0,	0,	0,	0x0147,	/* LATIN_CAPITAL_LETTER_N_WITH_CARON */
  GDK_dead_caron,	GDK_R,	0,	0,	0,	0x0158,	/* LATIN_CAPITAL_LETTER_R_WITH_CARON */
  GDK_dead_caron,	GDK_S,	0,	0,	0,	0x0160,	/* LATIN_CAPITAL_LETTER_S_WITH_CARON */
  GDK_dead_caron,	GDK_T,	0,	0,	0,	0x0164,	/* LATIN_CAPITAL_LETTER_T_WITH_CARON */
  GDK_dead_caron,	GDK_Z,	0,	0,	0,	0x017D,	/* LATIN_CAPITAL_LETTER_Z_WITH_CARON */
  GDK_dead_caron,	GDK_c,	0,	0,	0,	0x010D,	/* LATIN_SMALL_LETTER_C_WITH_CARON */
  GDK_dead_caron,	GDK_d,	0,	0,	0,	0x010F,	/* LATIN_SMALL_LETTER_D_WITH_CARON */
  GDK_dead_caron,	GDK_e,	0,	0,	0,	0x011B,	/* LATIN_SMALL_LETTER_E_WITH_CARON */
  GDK_dead_caron,	GDK_l,	0,	0,	0,	0x013E,	/* LATIN_SMALL_LETTER_L_WITH_CARON */
  GDK_dead_caron,	GDK_n,	0,	0,	0,	0x0148,	/* LATIN_SMALL_LETTER_N_WITH_CARON */
  GDK_dead_caron,	GDK_r,	0,	0,	0,	0x0159,	/* LATIN_SMALL_LETTER_R_WITH_CARON */
  GDK_dead_caron,	GDK_s,	0,	0,	0,	0x0161,	/* LATIN_SMALL_LETTER_S_WITH_CARON */
  GDK_dead_caron,	GDK_t,	0,	0,	0,	0x0165,	/* LATIN_SMALL_LETTER_T_WITH_CARON */
  GDK_dead_caron,	GDK_z,	0,	0,	0,	0x017E,	/* LATIN_SMALL_LETTER_Z_WITH_CARON */
  GDK_dead_caron,	GDK_caron,	0,	0,	0,	0x02C7,	/* CARON */
  GDK_dead_caron,	GDK_dead_caron,	0,	0,	0,	0x02C7,	/* CARON */
  GDK_dead_cedilla,	GDK_space,	0,	0,	0,	0x00B8,	/* CEDILLA */
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
  GDK_dead_ogonek,	GDK_space,	0,	0,	0,	0x02DB,	/* OGONEK */
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
  GDK_Multi_key,	GDK_space,	GDK_parenleft,	0,	0,	0x02D8,	/* BREVE */
  GDK_Multi_key,	GDK_space,	GDK_minus,	0,	0,	0x007E,	/* TILDE */
  GDK_Multi_key,	GDK_space,	GDK_less,	0,	0,	0x02C7,	/* CARON */
  GDK_Multi_key,	GDK_space,	GDK_greater,	0,	0,	0x005E,	/* CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_space,	GDK_asciicircum,	0,	0,	0x005E,	/* CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_space,	GDK_grave,	0,	0,	0x0060,	/* GRAVE_ACCENT */
  GDK_Multi_key,	GDK_space,	GDK_asciitilde,	0,	0,	0x007E,	/* TILDE */
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
  GDK_Multi_key,	GDK_quotedbl,	GDK_Y,	0,	0,	0x0178,	/* LATIN_CAPITAL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_a,	0,	0,	0x00E4,	/* LATIN_SMALL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_e,	0,	0,	0x00EB,	/* LATIN_SMALL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_i,	0,	0,	0x00EF,	/* LATIN_SMALL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_o,	0,	0,	0x00F6,	/* LATIN_SMALL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_u,	0,	0,	0x00FC,	/* LATIN_SMALL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_quotedbl,	GDK_y,	0,	0,	0x00FF,	/* LATIN_SMALL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_apostrophe,	GDK_space,	0,	0,	0x0027,	/* APOSTROPHE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_apostrophe,	0,	0,	0x00B4,	/* ACUTE_ACCENT */
  GDK_Multi_key,	GDK_apostrophe,	GDK_A,  0,      0,      0x00C1, /* LATIN_CAPITAL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_C,  0,      0,      0x0106, /* LATIN_CAPITAL_LETTER_C_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_E,  0,      0,      0x00C9, /* LATIN_CAPITAL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_I,  0,      0,      0x00CD, /* LATIN_CAPITAL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_L,  0,      0,      0x0139, /* LATIN_CAPITAL_LETTER_L_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_N,  0,      0,      0x0143, /* LATIN_CAPITAL_LETTER_N_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_O,  0,      0,      0x00D3, /* LATIN_CAPITAL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_R,  0,      0,      0x0154, /* LATIN_CAPITAL_LETTER_R_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_S,  0,      0,      0x015A, /* LATIN_CAPITAL_LETTER_S_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_U,  0,      0,      0x00DA, /* LATIN_CAPITAL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_Y,  0,      0,      0x00DD, /* LATIN_CAPITAL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_Z,  0,      0,      0x0179, /* LATIN_CAPITAL_LETTER_Z_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_a,  0,      0,      0x00E1, /* LATIN_SMALL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_c,  0,      0,      0x0107, /* LATIN_SMALL_LETTER_C_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_e,  0,      0,      0x00E9, /* LATIN_SMALL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_i,  0,      0,      0x00ED, /* LATIN_SMALL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_l,  0,      0,      0x013A, /* LATIN_SMALL_LETTER_L_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_n,  0,      0,      0x0144, /* LATIN_SMALL_LETTER_N_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_o,  0,      0,      0x00F3, /* LATIN_SMALL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_r,  0,      0,      0x0155, /* LATIN_SMALL_LETTER_R_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_s,  0,      0,      0x015B, /* LATIN_SMALL_LETTER_S_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_u,  0,      0,      0x00FA, /* LATIN_SMALL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_y,  0,      0,      0x00FD, /* LATIN_SMALL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_apostrophe,	GDK_z,  0,      0,      0x017A, /* LATIN_SMALL_LETTER_Z_WITH_ACUTE */
  GDK_Multi_key,	GDK_parenleft,	GDK_space,	0,	0,	0x02D8,	/* BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_parenleft,	0,	0,	0x005B,	/* LEFT_SQUARE_BRACKET */
  GDK_Multi_key,	GDK_parenleft,	GDK_minus,	0,	0,	0x007B,	/* LEFT_CURLY_BRACKET */
  GDK_Multi_key,	GDK_parenleft,	GDK_A,	0,	0,	0x0102,	/* LATIN_CAPITAL_LETTER_A_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_G,	0,	0,	0x011E,	/* LATIN_CAPITAL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_c,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_parenleft,	GDK_a,	0,	0,	0x0103,	/* LATIN_SMALL_LETTER_A_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_g,	0,	0,	0x011F,	/* LATIN_SMALL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_parenleft,	GDK_r,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_parenright,	GDK_parenright,	0,	0,	0x005D,	/* RIGHT_SQUARE_BRACKET */
  GDK_Multi_key,	GDK_parenright,	GDK_minus,	0,	0,	0x007D,	/* RIGHT_CURLY_BRACKET */
  GDK_Multi_key,	GDK_asterisk,	GDK_0,	0,	0,	0x00B0,	/* DEGREE_SIGN */
  GDK_Multi_key,	GDK_asterisk,	GDK_A,	0,	0,	0x00C5,	/* LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_asterisk,	GDK_U,	0,	0,	0x016E,	/* LATIN_CAPITAL_LETTER_U_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_asterisk,	GDK_a,	0,	0,	0x00E5,	/* LATIN_SMALL_LETTER_A_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_asterisk,	GDK_u,	0,	0,	0x016F,	/* LATIN_SMALL_LETTER_U_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_plus,	GDK_plus,	0,	0,	0x0023,	/* NUMBER_SIGN */
  GDK_Multi_key,	GDK_plus,	GDK_minus,	0,	0,	0x00B1,	/* PLUSxMINUS_SIGN */
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
  GDK_Multi_key,	GDK_comma,	GDK_u,	0,	0,	0x0173,	/* LATIN_SMALL_LETTER_U_WITH_OGONEK */
  GDK_Multi_key,	GDK_minus,	GDK_space,	0,	0,	0x007E,	/* TILDE */
  GDK_Multi_key,	GDK_minus,	GDK_parenleft,	0,	0,	0x007B,	/* LEFT_CURLY_BRACKET */
  GDK_Multi_key,	GDK_minus,	GDK_parenright,	0,	0,	0x007D,	/* RIGHT_CURLY_BRACKET */
  GDK_Multi_key,	GDK_minus,	GDK_plus,	0,	0,	0x00B1,	/* PLUSxMINUS_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_comma,	0,	0,	0x00AC,	/* NOT_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_minus,	0,	0,	0x00AD,	/* SOFT_HYPHEN */
  GDK_Multi_key,	GDK_minus,	GDK_colon,	0,	0,	0x00F7,	/* DIVISION_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_A,	0,	0,	0x00C3,	/* LATIN_CAPITAL_LETTER_A_WITH_TILDE */
  GDK_Multi_key,	GDK_minus,	GDK_D,	0,	0,	0x0110,	/* LATIN_CAPITAL_LETTER_D_WITH_STROKE */
  GDK_Multi_key,	GDK_minus,	GDK_E,	0,	0,	0x0112,	/* LATIN_CAPITAL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_I,	0,	0,	0x012A,	/* LATIN_CAPITAL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_L,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_N,	0,	0,	0x00D1,	/* LATIN_CAPITAL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_minus,	GDK_O,	0,	0,	0x00D5,	/* LATIN_CAPITAL_LETTER_O_WITH_TILDE */
  GDK_Multi_key,	GDK_minus,	GDK_U,	0,	0,	0x016A,	/* LATIN_CAPITAL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_Y,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_asciicircum,	0,	0,	0x00AF,	/* MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_a,	0,	0,	0x00E3,	/* LATIN_SMALL_LETTER_A_WITH_TILDE */
  GDK_Multi_key,	GDK_minus,	GDK_d,	0,	0,	0x0111,	/* LATIN_SMALL_LETTER_D_WITH_STROKE */
  GDK_Multi_key,	GDK_minus,	GDK_e,	0,	0,	0x0113,	/* LATIN_SMALL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_i,	0,	0,	0x012B,	/* LATIN_SMALL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_l,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_minus,	GDK_n,	0,	0,	0x00F1,	/* LATIN_SMALL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_minus,	GDK_o,	0,	0,	0x00F5,	/* LATIN_SMALL_LETTER_O_WITH_TILDE */
  GDK_Multi_key,	GDK_minus,	GDK_u,	0,	0,	0x016B,	/* LATIN_SMALL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_minus,	GDK_y,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_period,	GDK_period,	0,	0,	0x02D9,	/* DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_B,	0,	0,	0x1E02,	/* LATIN_CAPITAL_LETTER_B_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_C,	0,	0,	0x010A,	/* LATIN_CAPITAL_LETTER_C_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_D,	0,	0,	0x1E0A,	/* LATIN_CAPITAL_LETTER_D_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_E,	0,	0,	0x0116,	/* LATIN_CAPITAL_LETTER_E_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_F,	0,	0,	0x1E1E,	/* LATIN_CAPITAL_LETTER_F_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_G,	0,	0,	0x0120,	/* LATIN_CAPITAL_LETTER_G_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_I,	0,	0,	0x0130,	/* LATIN_CAPITAL_LETTER_I_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_M,	0,	0,	0x1E40,	/* LATIN_CAPITAL_LETTER_M_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_P,	0,	0,	0x1E56,	/* LATIN_CAPITAL_LETTER_P_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_S,	0,	0,	0x1E60,	/* LATIN_CAPITAL_LETTER_S_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_T,	0,	0,	0x1E6A,	/* LATIN_CAPITAL_LETTER_T_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_Z,	0,	0,	0x017B,	/* LATIN_CAPITAL_LETTER_Z_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_asciicircum,	0,	0,	0x00B7,	/* MIDDLE_DOT */
  GDK_Multi_key,	GDK_period,	GDK_b,	0,	0,	0x1E03,	/* LATIN_SMALL_LETTER_B_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_c,	0,	0,	0x010B,	/* LATIN_SMALL_LETTER_C_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_d,	0,	0,	0x1E0B,	/* LATIN_SMALL_LETTER_D_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_e,	0,	0,	0x0117,	/* LATIN_SMALL_LETTER_E_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_f,	0,	0,	0x1E1F,	/* LATIN_SMALL_LETTER_F_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_g,	0,	0,	0x0121,	/* LATIN_SMALL_LETTER_G_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_i,	0,	0,	0x0131,	/* LATIN_SMALL_LETTER_DOTLESS_I */
  GDK_Multi_key,	GDK_period,	GDK_m,	0,	0,	0x1E41,	/* LATIN_SMALL_LETTER_M_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_p,	0,	0,	0x1E57,	/* LATIN_SMALL_LETTER_P_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_s,	0,	0,	0x1E61,	/* LATIN_SMALL_LETTER_S_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_t,	0,	0,	0x1E6B,	/* LATIN_SMALL_LETTER_T_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_period,	GDK_z,	0,	0,	0x017C,	/* LATIN_SMALL_LETTER_Z_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_slash,	GDK_slash,	0,	0,	0x005C,	/* REVERSE_SOLIDUS */
  GDK_Multi_key,	GDK_slash,	GDK_less,	0,	0,	0x005C,	/* REVERSE_SOLIDUS */
  GDK_Multi_key,	GDK_slash,	GDK_C,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_slash,	GDK_O,	0,	0,	0x00D8,	/* LATIN_CAPITAL_LETTER_O_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_T,	0,	0,	0x0166,	/* LATIN_CAPITAL_LETTER_T_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_U,	0,	0,	0x00B5,	/* MICRO_SIGN */
  GDK_Multi_key,	GDK_slash,	GDK_asciicircum,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_Multi_key,	GDK_slash,	GDK_c,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_slash,	GDK_o,	0,	0,	0x00F8,	/* LATIN_SMALL_LETTER_O_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_t,	0,	0,	0x0167,	/* LATIN_SMALL_LETTER_T_WITH_STROKE */
  GDK_Multi_key,	GDK_slash,	GDK_u,	0,	0,	0x00B5,	/* MICRO_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_asterisk,	0,	0,	0x00B0,	/* DEGREE_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_C,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_S,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_X,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_asciicircum,	0,	0,	0x00B0,	/* DEGREE_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_c,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_s,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_0,	GDK_x,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_1,	GDK_S,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_Multi_key,	GDK_1,	GDK_asciicircum,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_Multi_key,	GDK_1,	GDK_s,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_Multi_key,	GDK_2,	GDK_S,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_Multi_key,	GDK_2,	GDK_asciicircum,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_Multi_key,	GDK_2,	GDK_s,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_Multi_key,	GDK_3,	GDK_S,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_Multi_key,	GDK_3,	GDK_asciicircum,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_Multi_key,	GDK_3,	GDK_s,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_Multi_key,	GDK_colon,	GDK_minus,	0,	0,	0x00F7,	/* DIVISION_SIGN */
  GDK_Multi_key,	GDK_less,	GDK_space,	0,	0,	0x02C7,	/* CARON */
  GDK_Multi_key,	GDK_less,	GDK_slash,	0,	0,	0x005C,	/* REVERSE_SOLIDUS */
  GDK_Multi_key,	GDK_less,	GDK_less,	0,	0,	0x00AB,	/* LEFTxPOINTING_DOUBLE_ANGLE_QUOTATION_MARK */
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
  GDK_Multi_key,	GDK_equal,	GDK_C,	0,	0,	0x20AC,	/* EURO_SIGN */
  GDK_Multi_key,	GDK_equal,	GDK_L,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_equal,	GDK_Y,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_equal,	GDK_e,	0,	0,	0x20AC,	/* EURO_SIGN */
  GDK_Multi_key,	GDK_equal,	GDK_l,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_equal,	GDK_y,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_greater,	GDK_space,	0,	0,	0x005E,	/* CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_greater,	GDK_greater,	0,	0,	0x00BB,	/* RIGHTxPOINTING_DOUBLE_ANGLE_QUOTATION_MARK */
  GDK_Multi_key,	GDK_greater,	GDK_A,	0,	0,	0x00C2,	/* LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_E,	0,	0,	0x00CA,	/* LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_I,	0,	0,	0x00CE,	/* LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_O,	0,	0,	0x00D4,	/* LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_U,	0,	0,	0x00DB,	/* LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_a,	0,	0,	0x00E2,	/* LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_e,	0,	0,	0x00EA,	/* LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_i,	0,	0,	0x00EE,	/* LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_o,	0,	0,	0x00F4,	/* LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_greater,	GDK_u,	0,	0,	0x00FB,	/* LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_question,	GDK_question,	0,	0,	0x00BF,	/* INVERTED_QUESTION_MARK */
  GDK_Multi_key,	GDK_A,	GDK_quotedbl,	0,	0,	0x00C4,	/* LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_A,	GDK_apostrophe,	0,	0,	0x00C1,	/* LATIN_CAPITAL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_A,	GDK_parenleft,	0,	0,	0x0102,	/* LATIN_CAPITAL_LETTER_A_WITH_BREVE */
  GDK_Multi_key,	GDK_A,	GDK_asterisk,	0,	0,	0x00C5,	/* LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_A,	GDK_comma,	0,	0,	0x0104,	/* LATIN_CAPITAL_LETTER_A_WITH_OGONEK */
  GDK_Multi_key,	GDK_A,	GDK_minus,	0,	0,	0x00C3,	/* LATIN_CAPITAL_LETTER_A_WITH_TILDE */
  GDK_Multi_key,	GDK_A,	GDK_greater,	0,	0,	0x00C2,	/* LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_A,	GDK_A,	0,	0,	0x00C5,	/* LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_A,	GDK_E,	0,	0,	0x00C6,	/* LATIN_CAPITAL_LETTER_AE */
  GDK_Multi_key,	GDK_A,	GDK_asciicircum,	0,	0,	0x00C2,	/* LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_A,	GDK_underscore,	0,	0,	0x00AA,	/* FEMININE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_A,	GDK_grave,	0,	0,	0x00C0,	/* LATIN_CAPITAL_LETTER_A_WITH_GRAVE */
  GDK_Multi_key,	GDK_A,	GDK_asciitilde,	0,	0,	0x00C3,	/* LATIN_CAPITAL_LETTER_A_WITH_TILDE */
  GDK_Multi_key,	GDK_A,	GDK_diaeresis,	0,	0,	0x00C4,	/* LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_A,	GDK_acute,	0,	0,	0x00C1,	/* LATIN_CAPITAL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_B,	GDK_period,	0,	0,	0x1E02,	/* LATIN_CAPITAL_LETTER_B_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_C,  GDK_apostrophe,	0,      0,      0x0106, /* LATIN_CAPITAL_LETTER_C_WITH_ACUTE */
  GDK_Multi_key,	GDK_C,	GDK_comma,	0,	0,	0x00C7,	/* LATIN_CAPITAL_LETTER_C_WITH_CEDILLA */
  GDK_Multi_key,	GDK_C,	GDK_period,	0,	0,	0x010A,	/* LATIN_CAPITAL_LETTER_C_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_C,	GDK_slash,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_less,	0,	0,	0x010C,	/* LATIN_CAPITAL_LETTER_C_WITH_CARON */
  GDK_Multi_key,	GDK_C,	GDK_equal,	0,	0,	0x20AC,	/* EURO_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_0,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_O,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_o,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_C,	GDK_bar,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_D,	GDK_minus,	0,	0,	0x0110,	/* LATIN_CAPITAL_LETTER_D_WITH_STROKE */
  GDK_Multi_key,	GDK_D,	GDK_period,	0,	0,	0x1E0A,	/* LATIN_CAPITAL_LETTER_D_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_D,	GDK_less,	0,	0,	0x010E,	/* LATIN_CAPITAL_LETTER_D_WITH_CARON */
  GDK_Multi_key,	GDK_E,	GDK_quotedbl,	0,	0,	0x00CB,	/* LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_E,	GDK_apostrophe,	0,	0,	0x00C9,	/* LATIN_CAPITAL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_E,	GDK_comma,	0,	0,	0x0118,	/* LATIN_CAPITAL_LETTER_E_WITH_OGONEK */
  GDK_Multi_key,	GDK_E,	GDK_minus,	0,	0,	0x0112,	/* LATIN_CAPITAL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_E,	GDK_period,	0,	0,	0x0116,	/* LATIN_CAPITAL_LETTER_E_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_E,	GDK_less,	0,	0,	0x011A,	/* LATIN_CAPITAL_LETTER_E_WITH_CARON */
  GDK_Multi_key,	GDK_E,	GDK_greater,	0,	0,	0x00CA,	/* LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_E,	GDK_asciicircum,	0,	0,	0x00CA,	/* LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_E,	GDK_underscore,	0,	0,	0x0112,	/* LATIN_CAPITAL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_E,	GDK_grave,	0,	0,	0x00C8,	/* LATIN_CAPITAL_LETTER_E_WITH_GRAVE */
  GDK_Multi_key,	GDK_E,	GDK_diaeresis,	0,	0,	0x00CB,	/* LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_E,	GDK_acute,	0,	0,	0x00C9,	/* LATIN_CAPITAL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_F,	GDK_period,	0,	0,	0x1E1E,	/* LATIN_CAPITAL_LETTER_F_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_G,	GDK_parenleft,	0,	0,	0x011E,	/* LATIN_CAPITAL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_G,	GDK_comma,	0,	0,	0x0122,	/* LATIN_CAPITAL_LETTER_G_WITH_CEDILLA */
  GDK_Multi_key,	GDK_G,	GDK_period,	0,	0,	0x0120,	/* LATIN_CAPITAL_LETTER_G_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_G,	GDK_U,	0,	0,	0x011E,	/* LATIN_CAPITAL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_G,	GDK_breve,	0,	0,	0x011E,	/* LATIN_CAPITAL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_I,	GDK_quotedbl,	0,	0,	0x00CF,	/* LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_I,	GDK_apostrophe,	0,	0,	0x00CD,	/* LATIN_CAPITAL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_I,	GDK_comma,	0,	0,	0x012E,	/* LATIN_CAPITAL_LETTER_I_WITH_OGONEK */
  GDK_Multi_key,	GDK_I,	GDK_minus,	0,	0,	0x012A,	/* LATIN_CAPITAL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_I,	GDK_period,	0,	0,	0x0130,	/* LATIN_CAPITAL_LETTER_I_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_I,	GDK_greater,	0,	0,	0x00CE,	/* LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_I,	GDK_asciicircum,	0,	0,	0x00CE,	/* LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_I,	GDK_underscore,	0,	0,	0x012A,	/* LATIN_CAPITAL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_I,	GDK_grave,	0,	0,	0x00CC,	/* LATIN_CAPITAL_LETTER_I_WITH_GRAVE */
  GDK_Multi_key,	GDK_I,	GDK_asciitilde,	0,	0,	0x0128,	/* LATIN_CAPITAL_LETTER_I_WITH_TILDE */
  GDK_Multi_key,	GDK_I,	GDK_diaeresis,	0,	0,	0x00CF,	/* LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_I,	GDK_acute,	0,	0,	0x00CD,	/* LATIN_CAPITAL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_K,	GDK_comma,	0,	0,	0x0136,	/* LATIN_CAPITAL_LETTER_K_WITH_CEDILLA */
  GDK_Multi_key,	GDK_L,  GDK_apostrophe,	0,      0,      0x0139, /* LATIN_CAPITAL_LETTER_L_WITH_ACUTE */
  GDK_Multi_key,	GDK_L,	GDK_comma,	0,	0,	0x013B,	/* LATIN_CAPITAL_LETTER_L_WITH_CEDILLA */
  GDK_Multi_key,	GDK_L,	GDK_minus,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_L,	GDK_less,	0,	0,	0x013D,	/* LATIN_CAPITAL_LETTER_L_WITH_CARON */
  GDK_Multi_key,	GDK_L,	GDK_equal,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_L,	GDK_V,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_Multi_key,	GDK_M,	GDK_period,	0,	0,	0x1E40,	/* LATIN_CAPITAL_LETTER_M_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_N,  GDK_apostrophe,	0,      0,      0x0143, /* LATIN_CAPITAL_LETTER_N_WITH_ACUTE */
  GDK_Multi_key,	GDK_N,	GDK_comma,	0,	0,	0x0145,	/* LATIN_CAPITAL_LETTER_N_WITH_CEDILLA */
  GDK_Multi_key,	GDK_N,	GDK_minus,	0,	0,	0x00D1,	/* LATIN_CAPITAL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_N,	GDK_less,	0,	0,	0x0147,	/* LATIN_CAPITAL_LETTER_N_WITH_CARON */
  GDK_Multi_key,	GDK_N,	GDK_G,	0,	0,	0x014A,	/* LATIN_CAPITAL_LETTER_ENG */
  GDK_Multi_key,	GDK_N,	GDK_asciitilde,	0,	0,	0x00D1,	/* LATIN_CAPITAL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_O,	GDK_quotedbl,	0,	0,	0x00D6,	/* LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_O,	GDK_apostrophe,	0,	0,	0x00D3,	/* LATIN_CAPITAL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_O,	GDK_minus,	0,	0,	0x00D5,	/* LATIN_CAPITAL_LETTER_O_WITH_TILDE */
  GDK_Multi_key,	GDK_O,	GDK_slash,	0,	0,	0x00D8,	/* LATIN_CAPITAL_LETTER_O_WITH_STROKE */
  GDK_Multi_key,	GDK_O,	GDK_greater,	0,	0,	0x00D4,	/* LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_O,	GDK_C,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_O,	GDK_E,	0,	0,	0x0152,	/* LATIN_CAPITAL_LIGATURE_OE */
  GDK_Multi_key,	GDK_O,	GDK_R,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_O,	GDK_S,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_O,	GDK_X,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_O,	GDK_asciicircum,	0,	0,	0x00D4,	/* LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_O,	GDK_underscore,	0,	0,	0x00BA,	/* MASCULINE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_O,	GDK_grave,	0,	0,	0x00D2,	/* LATIN_CAPITAL_LETTER_O_WITH_GRAVE */
  GDK_Multi_key,	GDK_O,	GDK_c,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_O,	GDK_x,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_O,	GDK_asciitilde,	0,	0,	0x00D5,	/* LATIN_CAPITAL_LETTER_O_WITH_TILDE */
  GDK_Multi_key,	GDK_O,	GDK_diaeresis,	0,	0,	0x00D6,	/* LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_O,	GDK_acute,	0,	0,	0x00D3,	/* LATIN_CAPITAL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_P,	GDK_exclam,	0,	0,	0x00B6,	/* PILCROW_SIGN */
  GDK_Multi_key,	GDK_P,	GDK_period,	0,	0,	0x1E56,	/* LATIN_CAPITAL_LETTER_P_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_R,	GDK_apostrophe,	0,	0,	0x0154,	/* LATIN_CAPITAL_LETTER_R_WITH_ACUTE */
  GDK_Multi_key,	GDK_R,	GDK_comma,	0,	0,	0x0156,	/* LATIN_CAPITAL_LETTER_R_WITH_CEDILLA */
  GDK_Multi_key,	GDK_R,	GDK_less,	0,	0,	0x0158,	/* LATIN_CAPITAL_LETTER_R_WITH_CARON */
  GDK_Multi_key,	GDK_R,	GDK_O,	0,	0,	0x00AE,	/* REGISTERED_SIGN */
  GDK_Multi_key,	GDK_S,	GDK_exclam,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_S,	GDK_apostrophe,	0,      0,      0x015A, /* LATIN_CAPITAL_LETTER_S_WITH_ACUTE */
  GDK_Multi_key,	GDK_S,	GDK_comma,	0,	0,	0x015E,	/* LATIN_CAPITAL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_S,	GDK_period,	0,	0,	0x1E60,	/* LATIN_CAPITAL_LETTER_S_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_S,	GDK_0,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_S,	GDK_1,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_Multi_key,	GDK_S,	GDK_2,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_Multi_key,	GDK_S,	GDK_3,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_Multi_key,	GDK_S,	GDK_less,	0,	0,	0x0160,	/* LATIN_CAPITAL_LETTER_S_WITH_CARON */
  GDK_Multi_key,	GDK_S,	GDK_O,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_S,	GDK_cedilla,	0,	0,	0x015E,	/* LATIN_CAPITAL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_T,	GDK_minus,	0,	0,	0x0166,	/* LATIN_CAPITAL_LETTER_T_WITH_STROKE */
  GDK_Multi_key,	GDK_T,	GDK_period,	0,	0,	0x1E6A,	/* LATIN_CAPITAL_LETTER_T_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_T,	GDK_slash,	0,	0,	0x0166,	/* LATIN_CAPITAL_LETTER_T_WITH_STROKE */
  GDK_Multi_key,	GDK_t,	GDK_less,	0,	0,	0x0165,	/* LATIN_SMALL_LETTER_T_WITH_CARON */
  GDK_Multi_key,	GDK_T,	GDK_H,	0,	0,	0x00DE,	/* LATIN_CAPITAL_LETTER_THORN */
  GDK_Multi_key,	GDK_U,	GDK_quotedbl,	0,	0,	0x00DC,	/* LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_U,	GDK_apostrophe,	0,	0,	0x00DA,	/* LATIN_CAPITAL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_U,	GDK_asterisk,	0,	0,	0x016E,	/* LATIN_CAPITAL_LETTER_U_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_U,	GDK_comma,	0,	0,	0x0172,	/* LATIN_CAPITAL_LETTER_U_WITH_OGONEK */
  GDK_Multi_key,	GDK_U,	GDK_minus,	0,	0,	0x016B,	/* LATIN_CAPITAL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_U,	GDK_slash,	0,	0,	0x00B5,	/* MICRO_SIGN */
  GDK_Multi_key,	GDK_U,	GDK_greater,	0,	0,	0x00DB,	/* LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_U,	GDK_asciicircum,	0,	0,	0x00DB,	/* LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_U,	GDK_underscore,	0,	0,	0x016B,	/* LATIN_CAPITAL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_U,	GDK_grave,	0,	0,	0x00D9,	/* LATIN_CAPITAL_LETTER_U_WITH_GRAVE */
  GDK_Multi_key,	GDK_U,	GDK_asciitilde,	0,	0,	0x0168,	/* LATIN_CAPITAL_LETTER_U_WITH_TILDE */
  GDK_Multi_key,	GDK_U,	GDK_diaeresis,	0,	0,	0x00DC,	/* LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_U,	GDK_acute,	0,	0,	0x00DA,	/* LATIN_CAPITAL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_V,	GDK_L,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_Multi_key,	GDK_W,	GDK_asciicircum,	0,	0,	0x0174,	/* LATIN_CAPITAL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_X,	GDK_0,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_X,	GDK_O,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_X,	GDK_o,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_Y,	GDK_quotedbl,	0,	0,	0x0178,	/* LATIN_CAPITAL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_Y,	GDK_apostrophe,	0,	0,	0x00DD,	/* LATIN_CAPITAL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_Y,	GDK_minus,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_Y,	GDK_equal,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_Y,	GDK_asciicircum,	0,	0,	0x0176,	/* LATIN_CAPITAL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_Y,	GDK_diaeresis,	0,	0,	0x0178,	/* LATIN_CAPITAL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_Y,	GDK_acute,	0,	0,	0x00DD,	/* LATIN_CAPITAL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_Z,	GDK_apostrophe,	0,	0,	0x0179,	/* LATIN_CAPITAL_LETTER_Z_WITH_ACUTE */
  GDK_Multi_key,	GDK_Z,	GDK_period,	0,	0,	0x017B,	/* LATIN_CAPITAL_LETTER_Z_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_Z,	GDK_less,	0,	0,	0x017D,	/* LATIN_CAPITAL_LETTER_Z_WITH_CARON */
  GDK_Multi_key,	GDK_asciicircum,	GDK_space,	0,	0,	0x005E,	/* CIRCUMFLEX_ACCENT */
  GDK_Multi_key,	GDK_asciicircum,	GDK_minus,	0,	0,	0x00AF,	/* MACRON */
  GDK_Multi_key,	GDK_asciicircum,	GDK_period,	0,	0,	0x00B7,	/* MIDDLE_DOT */
  GDK_Multi_key,	GDK_asciicircum,	GDK_slash,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_Multi_key,	GDK_asciicircum,	GDK_0,	0,	0,	0x00B0,	/* DEGREE_SIGN */
  GDK_Multi_key,	GDK_asciicircum,	GDK_1,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_Multi_key,	GDK_asciicircum,	GDK_2,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_Multi_key,	GDK_asciicircum,	GDK_3,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_Multi_key,	GDK_asciicircum,	GDK_A,	0,	0,	0x00C2,	/* LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_E,	0,	0,	0x00CA,	/* LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_I,	0,	0,	0x00CE,	/* LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_O,	0,	0,	0x00D4,	/* LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_U,	0,	0,	0x00DB,	/* LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_W,	0,	0,	0x0174,	/* LATIN_CAPITAL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_Y,	0,	0,	0x0176,	/* LATIN_CAPITAL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_underscore,	0,	0,	0x00AF,	/* MACRON */
  GDK_Multi_key,	GDK_asciicircum,	GDK_a,	0,	0,	0x00E2,	/* LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_e,	0,	0,	0x00EA,	/* LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_i,	0,	0,	0x00EE,	/* LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_o,	0,	0,	0x00F4,	/* LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_u,	0,	0,	0x00FB,	/* LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_w,	0,	0,	0x0175,	/* LATIN_SMALL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_asciicircum,	GDK_y,	0,	0,	0x0177,	/* LATIN_SMALL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_underscore,	GDK_A,	0,	0,	0x00AA,	/* FEMININE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_underscore,	GDK_E,	0,	0,	0x0112,	/* LATIN_CAPITAL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_I,	0,	0,	0x012A,	/* LATIN_CAPITAL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_O,	0,	0,	0x00BA,	/* MASCULINE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_underscore,	GDK_U,	0,	0,	0x016A,	/* LATIN_CAPITAL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_asciicircum,	0,	0,	0x00AF,	/* MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_underscore,	0,	0,	0x00AF,	/* MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_a,	0,	0,	0x00AA,	/* FEMININE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_underscore,	GDK_e,	0,	0,	0x0113,	/* LATIN_SMALL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_i,	0,	0,	0x012B,	/* LATIN_SMALL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_underscore,	GDK_o,	0,	0,	0x00BA,	/* MASCULINE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_underscore,	GDK_u,	0,	0,	0x016B,	/* LATIN_SMALL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_grave,	GDK_space,	0,	0,	0x0060,	/* GRAVE_ACCENT */
  GDK_Multi_key,	GDK_grave,	GDK_A,	0,	0,	0x00C0,	/* LATIN_CAPITAL_LETTER_A_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_E,	0,	0,	0x00C8,	/* LATIN_CAPITAL_LETTER_E_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_I,	0,	0,	0x00CC,	/* LATIN_CAPITAL_LETTER_I_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_O,	0,	0,	0x00D2,	/* LATIN_CAPITAL_LETTER_O_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_U,	0,	0,	0x00D9,	/* LATIN_CAPITAL_LETTER_U_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_a,	0,	0,	0x00E0,	/* LATIN_SMALL_LETTER_A_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_e,	0,	0,	0x00E8,	/* LATIN_SMALL_LETTER_E_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_i,	0,	0,	0x00EC,	/* LATIN_SMALL_LETTER_I_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_o,	0,	0,	0x00F2,	/* LATIN_SMALL_LETTER_O_WITH_GRAVE */
  GDK_Multi_key,	GDK_grave,	GDK_u,	0,	0,	0x00F9,	/* LATIN_SMALL_LETTER_U_WITH_GRAVE */
  GDK_Multi_key,	GDK_a,	GDK_quotedbl,	0,	0,	0x00E4,	/* LATIN_SMALL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_a,	GDK_apostrophe,	0,	0,	0x00E1,	/* LATIN_SMALL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_a,	GDK_parenleft,	0,	0,	0x0103,	/* LATIN_SMALL_LETTER_A_WITH_BREVE */
  GDK_Multi_key,	GDK_a,	GDK_asterisk,	0,	0,	0x00E5,	/* LATIN_SMALL_LETTER_A_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_a,	GDK_comma,	0,	0,	0x0105,	/* LATIN_SMALL_LETTER_A_WITH_OGONEK */
  GDK_Multi_key,	GDK_a,	GDK_minus,	0,	0,	0x00E3,	/* LATIN_SMALL_LETTER_A_WITH_TILDE */
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
  GDK_Multi_key,	GDK_c,	GDK_comma,	0,	0,	0x00E7,	/* LATIN_SMALL_LETTER_C_WITH_CEDILLA */
  GDK_Multi_key,	GDK_c,	GDK_period,	0,	0,	0x010B,	/* LATIN_SMALL_LETTER_C_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_c,	GDK_slash,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_c,	GDK_0,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_c,	GDK_less,	0,	0,	0x010D,	/* LATIN_SMALL_LETTER_C_WITH_CARON */
  GDK_Multi_key,	GDK_c,	GDK_O,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_c,	GDK_o,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_c,	GDK_bar,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_d,	GDK_minus,	0,	0,	0x0111,	/* LATIN_SMALL_LETTER_D_WITH_STROKE */
  GDK_Multi_key,	GDK_d,	GDK_period,	0,	0,	0x1E0B,	/* LATIN_SMALL_LETTER_D_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_d,	GDK_less,	0,	0,	0x010F,	/* LATIN_SMALL_LETTER_D_WITH_CARON */
  GDK_Multi_key,	GDK_e,	GDK_quotedbl,	0,	0,	0x00EB,	/* LATIN_SMALL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_e,	GDK_apostrophe,	0,	0,	0x00E9,	/* LATIN_SMALL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_e,	GDK_comma,	0,	0,	0x0119,	/* LATIN_SMALL_LETTER_E_WITH_OGONEK */
  GDK_Multi_key,	GDK_e,	GDK_minus,	0,	0,	0x0113,	/* LATIN_SMALL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_e,	GDK_period,	0,	0,	0x0117,	/* LATIN_SMALL_LETTER_E_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_e,	GDK_equal,	0,	0,	0x20AC,	/* EURO_SIGN */
  GDK_Multi_key,	GDK_e,	GDK_less,	0,	0,	0x011B,	/* LATIN_SMALL_LETTER_E_WITH_CARON */
  GDK_Multi_key,	GDK_e,	GDK_greater,	0,	0,	0x00EA,	/* LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_e,	GDK_asciicircum,	0,	0,	0x00EA,	/* LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_e,	GDK_underscore,	0,	0,	0x0113,	/* LATIN_SMALL_LETTER_E_WITH_MACRON */
  GDK_Multi_key,	GDK_e,	GDK_grave,	0,	0,	0x00E8,	/* LATIN_SMALL_LETTER_E_WITH_GRAVE */
  GDK_Multi_key,	GDK_e,	GDK_diaeresis,	0,	0,	0x00EB,	/* LATIN_SMALL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_e,	GDK_acute,	0,	0,	0x00E9,	/* LATIN_SMALL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_f,	GDK_period,	0,	0,	0x1E1F,	/* LATIN_SMALL_LETTER_F_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_g,	GDK_parenleft,	0,	0,	0x011F,	/* LATIN_SMALL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_g,	GDK_comma,	0,	0,	0x0123,	/* LATIN_SMALL_LETTER_G_WITH_CEDILLA */
  GDK_Multi_key,	GDK_g,	GDK_period,	0,	0,	0x0121,	/* LATIN_SMALL_LETTER_G_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_g,	GDK_U,	0,	0,	0x011F,	/* LATIN_SMALL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_g,	GDK_breve,	0,	0,	0x011F,	/* LATIN_SMALL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_i,	GDK_quotedbl,	0,	0,	0x00EF,	/* LATIN_SMALL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_i,	GDK_apostrophe,	0,	0,	0x00ED,	/* LATIN_SMALL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_i,	GDK_comma,	0,	0,	0x012F,	/* LATIN_SMALL_LETTER_I_WITH_OGONEK */
  GDK_Multi_key,	GDK_i,	GDK_minus,	0,	0,	0x012B,	/* LATIN_SMALL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_i,	GDK_period,	0,	0,	0x0131,	/* LATIN_SMALL_LETTER_DOTLESS_I */
  GDK_Multi_key,	GDK_i,	GDK_greater,	0,	0,	0x00EE,	/* LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_i,	GDK_asciicircum,	0,	0,	0x00EE,	/* LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_i,	GDK_underscore,	0,	0,	0x012B,	/* LATIN_SMALL_LETTER_I_WITH_MACRON */
  GDK_Multi_key,	GDK_i,	GDK_grave,	0,	0,	0x00EC,	/* LATIN_SMALL_LETTER_I_WITH_GRAVE */
  GDK_Multi_key,	GDK_i,	GDK_asciitilde,	0,	0,	0x0129,	/* LATIN_SMALL_LETTER_I_WITH_TILDE */
  GDK_Multi_key,	GDK_i,	GDK_diaeresis,	0,	0,	0x00EF,	/* LATIN_SMALL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_i,	GDK_acute,	0,	0,	0x00ED,	/* LATIN_SMALL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_k,	GDK_comma,	0,	0,	0x0137,	/* LATIN_SMALL_LETTER_K_WITH_CEDILLA */
  GDK_Multi_key,	GDK_k,	GDK_k,	0,	0,	0x0138,	/* LATIN_SMALL_LETTER_KRA */
  GDK_Multi_key,	GDK_l,	GDK_apostrophe,	0,	0,	0x013A,	/* LATIN_SMALL_LETTER_L_WITH_ACUTE */
  GDK_Multi_key,	GDK_l,	GDK_comma,	0,	0,	0x013C,	/* LATIN_SMALL_LETTER_L_WITH_CEDILLA */
  GDK_Multi_key,	GDK_l,	GDK_minus,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_l,	GDK_less,	0,	0,	0x013E,	/* LATIN_SMALL_LETTER_L_WITH_CARON */
  GDK_Multi_key,	GDK_l,	GDK_equal,	0,	0,	0x00A3,	/* POUND_SIGN */
  GDK_Multi_key,	GDK_l,	GDK_v,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_Multi_key,	GDK_m,	GDK_period,	0,	0,	0x1E41,	/* LATIN_SMALL_LETTER_M_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_n,	GDK_apostrophe,	0,	0,	0x0144,	/* LATIN_SMALL_LETTER_N_WITH_ACUTE */
  GDK_Multi_key,	GDK_n,	GDK_comma,	0,	0,	0x0146,	/* LATIN_SMALL_LETTER_N_WITH_CEDILLA */
  GDK_Multi_key,	GDK_n,	GDK_minus,	0,	0,	0x00F1,	/* LATIN_SMALL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_n,	GDK_less,	0,	0,	0x0148,	/* LATIN_SMALL_LETTER_N_WITH_CARON */
  GDK_Multi_key,	GDK_n,	GDK_g,	0,	0,	0x014B,	/* LATIN_SMALL_LETTER_ENG */
  GDK_Multi_key,	GDK_n,	GDK_asciitilde,	0,	0,	0x00F1,	/* LATIN_SMALL_LETTER_N_WITH_TILDE */
  GDK_Multi_key,	GDK_o,	GDK_quotedbl,	0,	0,	0x00F6,	/* LATIN_SMALL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_o,	GDK_apostrophe,	0,	0,	0x00F3,	/* LATIN_SMALL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_o,	GDK_minus,	0,	0,	0x00F5,	/* LATIN_SMALL_LETTER_O_WITH_TILDE */
  GDK_Multi_key,	GDK_o,	GDK_slash,	0,	0,	0x00F8,	/* LATIN_SMALL_LETTER_O_WITH_STROKE */
  GDK_Multi_key,	GDK_o,	GDK_greater,	0,	0,	0x00F4,	/* LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_o,	GDK_C,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_X,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_asciicircum,	0,	0,	0x00F4,	/* LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_o,	GDK_underscore,	0,	0,	0x00BA,	/* MASCULINE_ORDINAL_INDICATOR */
  GDK_Multi_key,	GDK_o,	GDK_grave,	0,	0,	0x00F2,	/* LATIN_SMALL_LETTER_O_WITH_GRAVE */
  GDK_Multi_key,	GDK_o,	GDK_c,	0,	0,	0x00A9,	/* COPYRIGHT_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_e,	0,	0,	0x0153,	/* LATIN_SMALL_LIGATURE_OE */
  GDK_Multi_key,	GDK_o,	GDK_s,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_x,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_o,	GDK_asciitilde,	0,	0,	0x00F5,	/* LATIN_SMALL_LETTER_O_WITH_TILDE */
  GDK_Multi_key,	GDK_o,	GDK_diaeresis,	0,	0,	0x00F6,	/* LATIN_SMALL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_o,	GDK_acute,	0,	0,	0x00F3,	/* LATIN_SMALL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_p,	GDK_exclam,	0,	0,	0x00B6,	/* PILCROW_SIGN */
  GDK_Multi_key,	GDK_p,	GDK_period,	0,	0,	0x1E57,	/* LATIN_SMALL_LETTER_P_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_r,	GDK_apostrophe,	0,	0,	0x0155,	/* LATIN_SMALL_LETTER_R_WITH_ACUTE */
  GDK_Multi_key,	GDK_r,	GDK_comma,	0,	0,	0x0157,	/* LATIN_SMALL_LETTER_R_WITH_CEDILLA */
  GDK_Multi_key,	GDK_r,	GDK_less,	0,	0,	0x0159,	/* LATIN_SMALL_LETTER_R_WITH_CARON */
  GDK_Multi_key,	GDK_s,	GDK_exclam,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_s,	GDK_apostrophe,	0,	0,	0x015B,	/* LATIN_SMALL_LETTER_S_WITH_ACUTE */
  GDK_Multi_key,	GDK_s,	GDK_comma,	0,	0,	0x015F,	/* LATIN_SMALL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_s,	GDK_period,	0,	0,	0x1E61,	/* LATIN_SMALL_LETTER_S_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_s,	GDK_0,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_s,	GDK_1,	0,	0,	0x00B9,	/* SUPERSCRIPT_ONE */
  GDK_Multi_key,	GDK_s,	GDK_2,	0,	0,	0x00B2,	/* SUPERSCRIPT_TWO */
  GDK_Multi_key,	GDK_s,	GDK_3,	0,	0,	0x00B3,	/* SUPERSCRIPT_THREE */
  GDK_Multi_key,	GDK_s,	GDK_less,	0,	0,	0x0161,	/* LATIN_SMALL_LETTER_S_WITH_CARON */
  GDK_Multi_key,	GDK_s,	GDK_o,	0,	0,	0x00A7,	/* SECTION_SIGN */
  GDK_Multi_key,	GDK_s,	GDK_s,	0,	0,	0x00DF,	/* LATIN_SMALL_LETTER_SHARP_S */
  GDK_Multi_key,	GDK_s,	GDK_cedilla,	0,	0,	0x015F,	/* LATIN_SMALL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_t,	GDK_minus,	0,	0,	0x0167,	/* LATIN_SMALL_LETTER_T_WITH_STROKE */
  GDK_Multi_key,	GDK_t,	GDK_period,	0,	0,	0x1E6B,	/* LATIN_SMALL_LETTER_T_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_t,	GDK_slash,	0,	0,	0x0167,	/* LATIN_SMALL_LETTER_T_WITH_STROKE */
  GDK_Multi_key,	GDK_t,	GDK_less,	0,	0,	0x0165,	/* LATIN_SMALL_LETTER_T_WITH_CARON */
  GDK_Multi_key,	GDK_t,	GDK_h,	0,	0,	0x00FE,	/* LATIN_SMALL_LETTER_THORN */
  GDK_Multi_key,	GDK_u,	GDK_quotedbl,	0,	0,	0x00FC,	/* LATIN_SMALL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_u,	GDK_apostrophe,	0,	0,	0x00FA,	/* LATIN_SMALL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_u,	GDK_asterisk,	0,	0,	0x016F,	/* LATIN_SMALL_LETTER_U_WITH_RING_ABOVE */
  GDK_Multi_key,	GDK_u,	GDK_comma,	0,	0,	0x0173,	/* LATIN_SMALL_LETTER_U_WITH_OGONEK */
  GDK_Multi_key,	GDK_u,	GDK_minus,	0,	0,	0x016B,	/* LATIN_SMALL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_u,	GDK_slash,	0,	0,	0x00B5,	/* MICRO_SIGN */
  GDK_Multi_key,	GDK_u,	GDK_greater,	0,	0,	0x00FB,	/* LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_u,	GDK_asciicircum,	0,	0,	0x00FB,	/* LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_u,	GDK_underscore,	0,	0,	0x016B,	/* LATIN_SMALL_LETTER_U_WITH_MACRON */
  GDK_Multi_key,	GDK_u,	GDK_grave,	0,	0,	0x00F9,	/* LATIN_SMALL_LETTER_U_WITH_GRAVE */
  GDK_Multi_key,	GDK_u,	GDK_asciitilde,	0,	0,	0x0169,	/* LATIN_SMALL_LETTER_U_WITH_TILDE */
  GDK_Multi_key,	GDK_u,	GDK_diaeresis,	0,	0,	0x00FC,	/* LATIN_SMALL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_u,	GDK_acute,	0,	0,	0x00FA,	/* LATIN_SMALL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_v,	GDK_Z,	0,	0,	0x017D,	/* LATIN_CAPITAL_LETTER_Z_WITH_CARON */
  GDK_Multi_key,	GDK_v,	GDK_l,	0,	0,	0x007C,	/* VERTICAL_LINE */
  GDK_Multi_key,	GDK_v,	GDK_z,	0,	0,	0x017E,	/* LATIN_SMALL_LETTER_Z_WITH_CARON */
  GDK_Multi_key,	GDK_w,	GDK_asciicircum,	0,	0,	0x0175,	/* LATIN_SMALL_LETTER_W_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_x,	GDK_0,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_x,	GDK_O,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_x,	GDK_o,	0,	0,	0x00A4,	/* CURRENCY_SIGN */
  GDK_Multi_key,	GDK_x,	GDK_x,	0,	0,	0x00D7,	/* MULTIPLICATION_SIGN */
  GDK_Multi_key,	GDK_y,	GDK_quotedbl,	0,	0,	0x00FF,	/* LATIN_SMALL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_y,	GDK_apostrophe,	0,	0,	0x00FD,	/* LATIN_SMALL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_y,	GDK_minus,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_y,	GDK_equal,	0,	0,	0x00A5,	/* YEN_SIGN */
  GDK_Multi_key,	GDK_y,	GDK_asciicircum,	0,	0,	0x0177,	/* LATIN_SMALL_LETTER_Y_WITH_CIRCUMFLEX */
  GDK_Multi_key,	GDK_y,	GDK_diaeresis,	0,	0,	0x00FF,	/* LATIN_SMALL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_y,	GDK_acute,	0,	0,	0x00FD,	/* LATIN_SMALL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_z,	GDK_apostrophe,	0,	0,	0x017A,	/* LATIN_SMALL_LETTER_Z_WITH_ACUTE */
  GDK_Multi_key,	GDK_z,	GDK_period,	0,	0,	0x017C,	/* LATIN_SMALL_LETTER_Z_WITH_DOT_ABOVE */
  GDK_Multi_key,	GDK_z,	GDK_less,	0,	0,	0x017E,	/* LATIN_SMALL_LETTER_Z_WITH_CARON */
  GDK_Multi_key,	GDK_bar,	GDK_C,	0,	0,	0x00A2,	/* CENT_SIGN */
  GDK_Multi_key,	GDK_bar,	GDK_c,	0,	0,	0x00A2,	/* CENT_SIGN */
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
  GDK_Multi_key,	GDK_diaeresis,	GDK_A,	0,	0,	0x00C4,	/* LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_E,	0,	0,	0x00CB,	/* LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_I,	0,	0,	0x00CF,	/* LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_O,	0,	0,	0x00D6,	/* LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_U,	0,	0,	0x00DC,	/* LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_Y,	0,	0,	0x0178,	/* LATIN_CAPITAL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_a,	0,	0,	0x00E4,	/* LATIN_SMALL_LETTER_A_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_e,	0,	0,	0x00EB,	/* LATIN_SMALL_LETTER_E_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_i,	0,	0,	0x00EF,	/* LATIN_SMALL_LETTER_I_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_o,	0,	0,	0x00F6,	/* LATIN_SMALL_LETTER_O_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_u,	0,	0,	0x00FC,	/* LATIN_SMALL_LETTER_U_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_diaeresis,	GDK_y,	0,	0,	0x00FF,	/* LATIN_SMALL_LETTER_Y_WITH_DIAERESIS */
  GDK_Multi_key,	GDK_acute,	GDK_A,	0,	0,	0x00C1,	/* LATIN_CAPITAL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_E,	0,	0,	0x00C9,	/* LATIN_CAPITAL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_I,	0,	0,	0x00CD,	/* LATIN_CAPITAL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_O,	0,	0,	0x00D3,	/* LATIN_CAPITAL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_U,	0,	0,	0x00DA,	/* LATIN_CAPITAL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_Y,	0,	0,	0x00DD,	/* LATIN_CAPITAL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_a,	0,	0,	0x00E1,	/* LATIN_SMALL_LETTER_A_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_e,	0,	0,	0x00E9,	/* LATIN_SMALL_LETTER_E_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_i,	0,	0,	0x00ED,	/* LATIN_SMALL_LETTER_I_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_o,	0,	0,	0x00F3,	/* LATIN_SMALL_LETTER_O_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_u,	0,	0,	0x00FA,	/* LATIN_SMALL_LETTER_U_WITH_ACUTE */
  GDK_Multi_key,	GDK_acute,	GDK_y,	0,	0,	0x00FD,	/* LATIN_SMALL_LETTER_Y_WITH_ACUTE */
  GDK_Multi_key,	GDK_cedilla,	GDK_S,	0,	0,	0x015E,	/* LATIN_CAPITAL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_cedilla,	GDK_s,	0,	0,	0x015F,	/* LATIN_SMALL_LETTER_S_WITH_CEDILLA */
  GDK_Multi_key,	GDK_breve,	GDK_G,	0,	0,	0x011E,	/* LATIN_CAPITAL_LETTER_G_WITH_BREVE */
  GDK_Multi_key,	GDK_breve,	GDK_g,	0,	0,	0x011F,	/* LATIN_SMALL_LETTER_G_WITH_BREVE */
};

static const GtkComposeTable gtk_compose_table = {
  gtk_compose_seqs,
  4,
  G_N_ELEMENTS (gtk_compose_seqs) / 6
};

static const guint16 gtk_im_et_apostrophe_ignore[] = {
  0x124b,
  0x125b,
  0x128b,
  0x12b3,
  0x12c3,
  0x1313
};

static const guint16 gtk_compose_ignore[] = {
  GDK_Shift_L,
  GDK_Shift_R,
  GDK_Control_L,
  GDK_Control_R,
  GDK_Caps_Lock,
  GDK_Shift_Lock,
  GDK_Meta_L,
  GDK_Meta_R,
  GDK_Alt_L,
  GDK_Alt_R,
  GDK_Super_L,
  GDK_Super_R,
  GDK_Hyper_L,
  GDK_Hyper_R,
  GDK_Mode_switch
};


static const char *gtk_im_ethiopic_circles[] = {
	"aaaa",
	"eeeee",
	/* "AA", */
	"qoo",
	"Qoo",
	"hhoo",
	"koo",
	"Koo",
	"goo",
	":::",
	",,",
	";;",
	"??"
};


static void     gtk_im_context_ethiopic_class_init         (GtkIMContextEthiopicClass  *class);
static void     gtk_im_context_ethiopic_init               (GtkIMContextEthiopic       *im_context_ethiopic);
static void     gtk_im_context_ethiopic_finalize           (GObject                    *obj);
static gboolean gtk_im_context_ethiopic_filter_keypress    (GtkIMContext               *context,
                                                            GdkEventKey                *key);
static void     gtk_im_context_ethiopic_reset              (GtkIMContext               *context);
static void     gtk_im_context_ethiopic_get_preedit_string (GtkIMContext               *context,
                                                            gchar                     **str,
                                                            PangoAttrList             **attrs,
                                                            gint                       *cursor_pos);

// static GObjectClass *parent_class;


static void gtk_im_context_parent_get_preedit_string (GtkIMContext   *context,
                                                      gchar         **str,
                                                      PangoAttrList **attrs,
                                                      gint           *cursor_pos);

static gboolean
no_sequence_matches (GtkIMContextEthiopic *context_ethiopic,
                     gint                  n_compose,
                     GdkEventKey          *event);
static gboolean
check_table (GtkIMContextEthiopic  *context_ethiopic,
             const GtkComposeTable *table,
             gint                   n_compose);

static gboolean gtk_im_et_check_for_hex (GtkIMContext *context,
                                         GdkEventKey  *key,
                                         int           n_compose);

GType
gtk_im_context_ethiopic_get_type (void)
{
  static GType im_context_ethiopic_type = 0;

  im_context_ethiopic_type = g_type_from_name ( "GtkIMContextEthiopic" );

  if (!im_context_ethiopic_type)
    {
      static const GTypeInfo im_context_ethiopic_info =
      {
        sizeof (GtkIMContextEthiopicClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) gtk_im_context_ethiopic_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (GtkIMContextEthiopic),
        0,              /* n_preallocs */
        (GInstanceInitFunc) gtk_im_context_ethiopic_init,
      };
      
      im_context_ethiopic_type =
        g_type_register_static (GTK_TYPE_IM_CONTEXT, "GtkIMContextEthiopic",
                                &im_context_ethiopic_info, 0);
    }

  return im_context_ethiopic_type;
}


/*============================================================================*
 *
 *                        Ethiopic Specific Methods
 *
 *============================================================================*/


/* gtk_im_et_check_for_vowel will return true if the passed char is a
 * valid composition vowel, false otherwise.
 */
static gboolean
gtk_im_et_check_for_vowel ( guint ch )
{

  /* strtok or a loop could be used here */
  return
    (
      ( ch == 'a' ) || ( ch == 'e' ) || ( ch == 'i' ) || ( ch == 'o' ) || ( ch == 'u' ) ||
      ( ch == 'A' ) || ( ch == 'E' ) || ( ch == 'I' ) || ( ch == 'O' ) || ( ch == 'U' )
    )
    ? TRUE
    : FALSE
  ;
}

/* gtk_im_et_check_labiovelar will return true if the passed unichar is a
 * candiate for zemene-form (labiovelar) composition
 */
static gboolean
gtk_im_et_check_for_labiovelar ( gunichar ch )
{

  return
    (
      ( ch%8 != 5 ) || 
      ( ch > 0x1355 ) || ( ch == 0x12a0 ) || ( ch == ETHIOPIC_W ) || ( ch == 0x12d3 ) || ( ch == 0x12ed ) || ( ch == 0x1345 )
    )
    ? FALSE
    : TRUE
  ;
}

static gboolean
gtk_im_et_check_for_apostrophe (GtkIMContext *context, GdkEventKey *event, int *n_compose)
{
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);


  if (context_ethiopic->tentative_match &&
      event->keyval == GDK_apostrophe && 
      context_ethiopic->tentative_match_len == 1 &&
      (event->state & gtk_accelerator_get_default_mod_mask()) == 0)
    {
       register int i;
       gboolean apostrophe_ok = TRUE;

       for (i=0; i < G_N_ELEMENTS (gtk_im_et_apostrophe_ignore); i++)
         if (context_ethiopic->tentative_match == gtk_im_et_apostrophe_ignore[i])
           apostrophe_ok = FALSE;

       if (apostrophe_ok)
         {
           gtk_im_context_reset (context);
           context_ethiopic->compose_buffer[0] = GDK_apostrophe;
           return TRUE;
         }
    }
  else if (  ( context_ethiopic->compose_buffer[0] == GDK_apostrophe ) )
    {
      if ( '0' < event->keyval && event->keyval <= '9' )
        {
          context_ethiopic->compose_buffer[0]   = event->keyval;
          context_ethiopic->compose_buffer[1]   = '\0';
          context_ethiopic->tentative_match_len = 1;
          context_ethiopic->in_ahaze_sequence   = TRUE;

          g_signal_emit_by_name (context_ethiopic, "preedit_changed");
          return TRUE;
        }
      else if ( event->keyval != GDK_apostrophe )
        context_ethiopic->compose_buffer[0] = *n_compose = 0;
    }

  return FALSE;
}

static gboolean
gtk_im_et_check_for_w (GtkIMContext *context, GdkEventKey *event, int *n_compose)
{
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);
  gchar outbuf[7];

  if (  ( event->type == GDK_KEY_PRESS                           )
          && ( context_ethiopic->tentative_match_len == 2        )
          && ( context_ethiopic->compose_buffer[2] == ETHIOPIC_W )
       )
    {
       if ( event->keyval == 'w' )
         {
           // reset for diqala-rabi
           context_ethiopic->tentative_match = context_ethiopic->compose_buffer[1];
           if ( context_ethiopic->compose_buffer[1] == ETHIOPIC_NEGUSSE_S )
             {
               context_ethiopic->tentative_match_len =    1;
               context_ethiopic->compose_buffer[1]   = 's';
               context_ethiopic->compose_buffer[2]   = '\0';
               *n_compose = 2;
             }
           else
             {
               context_ethiopic->tentative_match_len =    1;
               context_ethiopic->compose_buffer[1]   = '\0';
               *n_compose = 1;
             }
         event->keyval = 'W';
         }
       else if ( gtk_im_et_check_for_vowel ( event->keyval ) )
         {
           g_unichar_to_utf8 (context_ethiopic->compose_buffer[1], outbuf);
           outbuf[3] = '\0';

           context_ethiopic->tentative_match     =    0;
           context_ethiopic->tentative_match_len =    0;
           context_ethiopic->compose_buffer[0]   = '\0';

           g_signal_emit_by_name (context_ethiopic, "preedit_changed");
           g_signal_emit_by_name (context, "commit", &outbuf);

           context_ethiopic->tentative_match     = ETHIOPIC_W;
           context_ethiopic->tentative_match_len =    1;
           context_ethiopic->compose_buffer[0]   =  'w';
           context_ethiopic->compose_buffer[1]   = '\0';
           *n_compose = 1;
         }
       else
         {
           // commit original-sadis
           // reset for w-sadis
           g_unichar_to_utf8 (context_ethiopic->compose_buffer[1], outbuf  );
           g_unichar_to_utf8 (context_ethiopic->compose_buffer[2], outbuf+3);
           outbuf[6] = '\0';

           context_ethiopic->tentative_match     =    0;
           context_ethiopic->tentative_match_len =    0;
           context_ethiopic->compose_buffer[0]   = '\0';

           g_signal_emit_by_name (context_ethiopic, "preedit_changed");
           g_signal_emit_by_name (context, "commit", &outbuf);
           *n_compose = 0;
         }
    }
  else if (  ( event->type == GDK_KEY_PRESS      ) &&
             ( context_ethiopic->tentative_match ) &&
             ( event->keyval == 'w'              ) &&
             ( gtk_im_et_check_for_labiovelar ( context_ethiopic->tentative_match ) )
       )
    {
        context_ethiopic->in_zemene_sequence = TRUE;

        g_signal_emit_by_name (context_ethiopic, "preedit_changed");

        return TRUE;
    }

  return FALSE;
}

static void
gtk_im_et_grow_compose_buffer ( GtkIMContext *context )
{
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);
  register int i = 0;

  guint *tmpCompose = (guint*) g_malloc ( (GTK_MAX_AHAZE_ASCII_COMPOSE_LEN+1) * sizeof(guint) );

  while (context_ethiopic->compose_buffer[i] != 0)
    { tmpCompose[i] = context_ethiopic->compose_buffer[i]; i++; }

  tmpCompose[i] = 0;

  g_free ( context_ethiopic->compose_buffer );

  context_ethiopic->compose_buffer = tmpCompose;

}

/*  Convert Western to Ethiopic numberals as per the Ilf-Radix algorithm:
 *  http://www.ethiopic/org/Nuermals/
 */
static gunichar*
ArabicToEthiopic (compose_buffer)
  guint* compose_buffer;
{

gunichar *ethioNumString, *returnString;
register int i, m, n;
guint asciiOne, asciiTen;
guint gezOne, gezTen, sep;
int place, pos;
char number[GTK_MAX_AHAZE_ASCII_COMPOSE_LEN + 1];


  i = 0;
  while ( compose_buffer[i] ) { number[i] = compose_buffer[i]; i++; }
  n = i - 1;
  number[i] = '\0';

  if ( n == 0 ) {
    /* Only one digit, don't bother with the loop,
     * map onto Ethiopic "ones" and return:
     * */
    returnString = (gunichar *) g_malloc ( 2 * sizeof (gunichar) );
    returnString[0] = (gunichar)number[0] + ETHIOPIC_ONE - '1';
    returnString[1] = '\0';
    return returnString;
  }
  else if ( (n % 2) == 0 ) {
     /* precondition the string to always have the leading
      * tens place populated, this avoids at least one check
      * in the loop:
      */
     i = 0;
     n++;
     number[n+1] = '\0';
     for ( i=n; i>=0; i-- )
      number[i] = number[i-1];
     number[0] = '0';
  }


  /* make a big unicode digit holder: */
  ethioNumString = (gunichar *) g_malloc ( (4*n+1) * sizeof (gunichar) );

  m = 0;

  for ( place = n; place >= 0; place-- )
    {

      /* initialize values: */
      gezOne   = gezTen = 0x0;

      /* set ascii values: */
      asciiTen = (guint)number[n-place]; 
      place--;
      asciiOne = (guint)number[n-place]; 

      /* set ethiopic values: */
      if ( asciiOne != '0' )
        gezOne = asciiOne + (ETHIOPIC_ONE - '1');  /* map onto Ethiopic "ones" */

      if ( asciiTen != '0' )
        gezTen = asciiTen + (ETHIOPIC_TEN - '1');  /* map onto Ethiopic "tens" */

      /* pos indicates if our grouping subscript is even or odd       */
      pos = ( place % 4 ) / 2;

      /* find a separator, if any, to follow Ethiopic ten and one     */
      sep
      = ( place )
        ? ( pos )
          ? ( gezOne || gezTen )
            ? ETHIOPIC_HUNDRED
            : 0x0
          : ETHIOPIC_TENTHOUSAND
        : 0x0
      ;


      /* we want to clear ETHIOPIC_ONE when it is superfluous  */
      if ( ( gezOne == ETHIOPIC_ONE ) && gezTen == 0x0 )      /* one without a leading ten  */
        if ( ( sep == ETHIOPIC_HUNDRED ) || (place+1) == n )  /* following (100) or leading */
          gezOne = 0x0;                              /* the sequence               */

      /* put it all together: */
      if ( gezTen )
        ethioNumString[m++] = (gunichar)gezTen;

      if ( gezOne )
        ethioNumString[m++] = (gunichar)gezOne;

      if ( sep )
        ethioNumString[m++] = (gunichar)sep;

    }

  returnString = ( gunichar * ) g_malloc ( (m+1) * sizeof (gunichar) );

  for (i=0; i<m; i++)
    returnString[i] = ethioNumString[i];

  returnString[m] = '\0';

  g_free ( ethioNumString );

  return returnString;

}

static void     
gtk_im_et_commit_ahaze ( GtkIMContext *context )
{
  char outbuf[GTK_MAX_AHAZE_COMPOSE_LEN*3+1];
  register int i = 0;
  gint len = 0;
  gunichar* ahaze;
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);

  ahaze = ArabicToEthiopic ( context_ethiopic->compose_buffer );

  while ( ahaze[i] )
    len += g_unichar_to_utf8 (ahaze[i++], outbuf+len);
  outbuf[len] = '\0';

  context_ethiopic->tentative_match     =    0;
  context_ethiopic->tentative_match_len =    0;
  context_ethiopic->compose_buffer[0]   = '\0';

  context_ethiopic->in_ahaze_sequence = FALSE;

  g_signal_emit_by_name (context_ethiopic, "preedit_changed");
  g_signal_emit_by_name (context, "commit", &outbuf);

  g_free ( ahaze );

  if ( i > GTK_MAX_COMPOSE_LEN ) {
    g_free ( context_ethiopic->compose_buffer );
    context_ethiopic->compose_buffer = (guint*) g_malloc ( (GTK_MAX_COMPOSE_LEN+1)*sizeof(guint) );
    context_ethiopic->compose_buffer[0] = 0;
  }
}

static gboolean
gtk_im_et_check_for_ahaze (GtkIMContext *context, GdkEventKey *event, int *n_compose)
{
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);


  if ( ( context_ethiopic->tentative_match >= ETHIOPIC_ONE ) &&
         ( context_ethiopic->tentative_match <= ETHIOPIC_TENTHOUSAND ) &&
         ( *n_compose >= GTK_MAX_COMPOSE_LEN )
       )
    {
        // commit the number if we've hit the limit of what we can compose.
        if ( *n_compose == GTK_MAX_AHAZE_ASCII_COMPOSE_LEN )
	  { gtk_im_et_commit_ahaze ( context ); return TRUE; }
        else if ( *n_compose == GTK_MAX_COMPOSE_LEN )
          gtk_im_et_grow_compose_buffer ( context );
          
    }
  if (  ( event->type == GDK_KEY_PRESS ) &&
           ( context_ethiopic->tentative_match >= ETHIOPIC_ONE ) &&
           ( context_ethiopic->tentative_match <= ETHIOPIC_TENTHOUSAND ) &&
           ( event->keyval >= '0' ) && ( event->keyval <= '9' )
       )
    {
      context_ethiopic->compose_buffer[(*n_compose)++] = event->keyval;

      context_ethiopic->compose_buffer[*n_compose]    = '\0';
      // context_ethiopic->tentative_match_len           = *n_compose;
      context_ethiopic->in_ahaze_sequence             = TRUE;
      g_signal_emit_by_name (context_ethiopic, "preedit_changed");

      return TRUE;
    }
  else if ( ( event->type == GDK_KEY_PRESS ) &&
             ( context_ethiopic->tentative_match >= ETHIOPIC_ONE ) &&
             ( context_ethiopic->tentative_match <= ETHIOPIC_TENTHOUSAND ) &&
             ( ( event->keyval < '0' ) || ( '9' < event->keyval ) )
       )
    {
        // commit the number if a non numeric char appears.
        gtk_im_et_commit_ahaze ( context );
        *n_compose = 0;
    }

  return FALSE;
}

static gboolean
gtk_im_et_check_for_table (GtkIMContext *context, GdkEventKey *event, int n_compose)
{
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);
  GSList *tmp_list;  

  context_ethiopic->compose_buffer[n_compose++] = event->keyval;
  context_ethiopic->compose_buffer[n_compose] = 0;

  tmp_list = context_ethiopic->tables;
  while (tmp_list)
    {
      if (check_table (context_ethiopic, tmp_list->data, n_compose))
        return TRUE;
      tmp_list = tmp_list->next;
    }

  if (check_table (context_ethiopic, &gtk_compose_table, n_compose))
    return TRUE;

  return FALSE;
}

static gboolean
gtk_im_et_check_for_circular (GtkIMContext *context, GdkEventKey *event, int n_compose)
{
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);

  int circle, i=0;
  int end = G_N_ELEMENTS (gtk_im_ethiopic_circles);


  for (circle=0; circle < end; circle++)
    if ( context_ethiopic->compose_buffer[0] == gtk_im_ethiopic_circles[circle][0] )
	    break;

  if ( circle == end || n_compose != strlen(gtk_im_ethiopic_circles[circle]) )
    return FALSE;

  /* this takes the place of strcmp */
  while (context_ethiopic->compose_buffer[i] != 0)
    {
      if ( (char)context_ethiopic->compose_buffer[i] != gtk_im_ethiopic_circles[circle][i] )
        return FALSE;
      i++;
    }

  if ( event->keyval == gtk_im_ethiopic_circles[circle][0] )
    {
       context_ethiopic->compose_buffer[0] = 0;
       context_ethiopic->tentative_match = 0;
       context_ethiopic->tentative_match_len = 0;
       return gtk_im_et_check_for_table(context, event, 0);
    }
  else if ( event->keyval == gtk_im_ethiopic_circles[circle][i-1] )
    {
       char c = gtk_im_ethiopic_circles[circle][0];
       int n_compose = 1;

       if ( c != 'q' && c != 'Q' && c != 'h' && c != 'k' && c != 'K' && c != 'g' )
         return FALSE;

       if ( c == 'h' )
         n_compose = 2;

       context_ethiopic->compose_buffer[n_compose] = 0;
       context_ethiopic->tentative_match = 0;
       context_ethiopic->tentative_match_len = 1;
       return gtk_im_et_check_for_table(context, event, n_compose);
    }

  return FALSE;

}

static void
gtk_im_context_ethiopic_get_preedit_string (GtkIMContext   *context,
                                            gchar         **str,
                                            PangoAttrList **attrs,
                                            gint           *cursor_pos)
{
  char outbuf[GTK_MAX_AHAZE_COMPOSE_LEN*3+1];
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);
  gint len = 0;

  if ( !context_ethiopic->in_zemene_sequence && !context_ethiopic->in_ahaze_sequence )
    return gtk_im_context_parent_get_preedit_string ( context, str, attrs, cursor_pos );


  if ( context_ethiopic->in_zemene_sequence ) {
    len = g_unichar_to_utf8 (context_ethiopic->tentative_match, outbuf);
    len += g_unichar_to_utf8 (ETHIOPIC_W, outbuf+len);

    context_ethiopic->compose_buffer[1]   = context_ethiopic->tentative_match;
    context_ethiopic->compose_buffer[2]   = ETHIOPIC_W;
    context_ethiopic->compose_buffer[3]   = '\0';
    context_ethiopic->tentative_match_len = 2;
    context_ethiopic->in_zemene_sequence  = FALSE;
  }
  else if ( context_ethiopic->in_ahaze_sequence ) {
    register int i = 0;
    gunichar* ahaze;

    ahaze = ArabicToEthiopic ( context_ethiopic->compose_buffer );

    while ( ahaze[i] )
      len += g_unichar_to_utf8 (ahaze[i++], outbuf+len);
  
    context_ethiopic->tentative_match_len = i;
    context_ethiopic->tentative_match = ahaze[i-1];

    g_free ( ahaze );
  }


  g_assert (len <= (GTK_MAX_AHAZE_COMPOSE_LEN*3) );
  outbuf[len] = '\0';      
  
  if (str)
    *str = g_strdup (outbuf);

  if (cursor_pos)
    *cursor_pos = (context_ethiopic->tentative_match ? len : 0);

  if (attrs)
    {
      *attrs = pango_attr_list_new ();
    
      PangoAttribute *attr = pango_attr_underline_new (PANGO_UNDERLINE_SINGLE);
      attr->start_index = 0;
      attr->end_index   = len;
      pango_attr_list_insert (*attrs, attr);
    }
}

static gboolean
gtk_im_context_check_for_ignore_compose ( GdkEventKey * event )
{
register int i;

  /* Ignore modifier key presses
   */
  for (i=0; i < G_N_ELEMENTS (gtk_compose_ignore); i++)
    if (event->keyval == gtk_compose_ignore[i])
      return TRUE;

  return FALSE;
}

static gboolean
gtk_im_context_ethiopic_filter_keypress (GtkIMContext *context,
                                          GdkEventKey  *event)
{
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);
  int n_compose = 0;

  if ( gtk_im_context_check_for_ignore_compose ( event ) )
    return FALSE;


  while (context_ethiopic->compose_buffer[n_compose] != 0)
    n_compose++;


  if (gtk_im_et_check_for_hex(context, event, n_compose))
    return TRUE;
  else if (gtk_im_et_check_for_apostrophe(context, event, &n_compose))
    return TRUE;
  else if (gtk_im_et_check_for_w(context, event, &n_compose))
    return TRUE;
  else if (gtk_im_et_check_for_ahaze(context, event, &n_compose))
    return TRUE;
  else if (event->type == GDK_KEY_RELEASE)
    return FALSE;
  else if (gtk_im_et_check_for_circular(context, event, n_compose))
    return TRUE;
  else if (gtk_im_et_check_for_table(context, event, n_compose))
    return TRUE;
  else
    return no_sequence_matches (context_ethiopic, n_compose, event);

}


/*============================================================================*
 *
 *  All Routines Below This Point are Unmodified from gtkimcontsimple.c
 *  except that "simple" is replaced with "ethiopic".
 *  gtk_im_et_check_for_hex is also added in this section
 *
 *============================================================================*/


static void
gtk_im_context_ethiopic_class_init (GtkIMContextEthiopicClass *class)
{
  GtkIMContextClass *im_context_class = GTK_IM_CONTEXT_CLASS (class);
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  // parent_class = g_type_class_peek_parent (class);

  im_context_class->filter_keypress = gtk_im_context_ethiopic_filter_keypress;
  im_context_class->reset = gtk_im_context_ethiopic_reset;
  im_context_class->get_preedit_string = gtk_im_context_ethiopic_get_preedit_string;
  gobject_class->finalize = gtk_im_context_ethiopic_finalize;
}

static void
gtk_im_context_ethiopic_init (GtkIMContextEthiopic *im_context_ethiopic)
{  
  im_context_ethiopic->compose_buffer = (guint*) g_malloc ( (GTK_MAX_COMPOSE_LEN+1)*sizeof(guint) );
  im_context_ethiopic->compose_buffer[0] = 0;

}

static void
gtk_im_context_ethiopic_finalize (GObject *obj)
{
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (obj);

  if (context_ethiopic->tables)
    {
      g_slist_foreach (context_ethiopic->tables, (GFunc)g_free, NULL);
      g_slist_free (context_ethiopic->tables);

      context_ethiopic->tables = NULL;
    }

  g_free ( context_ethiopic->compose_buffer );

}

/** 
 * gtk_im_context_ethiopic_new:
 * 
 * Creates a new #GtkIMContextEthiopic.
 *
 * Returns: a new #GtkIMContextEthiopic.
 **/
GtkIMContext *
gtk_im_context_ethiopic_new (void)
{
  return g_object_new (GTK_TYPE_IM_CONTEXT_ETHIOPIC, NULL);
}

static void
gtk_im_context_ethiopic_commit_char (GtkIMContext *context,
                                     gunichar ch)
{
  gchar buf[10];
  gint len;

  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);

  g_return_if_fail (g_unichar_validate (ch));
  
  len = g_unichar_to_utf8 (ch, buf);
  buf[len] = '\0';

  context_ethiopic->in_hex_sequence = FALSE;
  
  if (context_ethiopic->tentative_match)
    {
      context_ethiopic->tentative_match = 0;
      context_ethiopic->tentative_match_len = 0;
      g_signal_emit_by_name (context_ethiopic, "preedit_changed");
    }

  g_signal_emit_by_name (context, "commit", &buf);
}

static int
compare_seq (const void *key, const void *value)
{
  int i = 0;
  const guint *keysyms = key;
  const guint16 *seq = value;

  while (keysyms[i])
    {
      if (keysyms[i] < seq[i])
        return -1;
      else if (keysyms[i] > seq[i])
        return 1;
  
      i++;
    }

  return 0;
}

static gboolean
check_table (GtkIMContextEthiopic    *context_ethiopic,
	     const GtkComposeTable *table,
	     gint                   n_compose)
{
  gint row_stride = table->max_seq_len + 2; 
  guint16 *seq = bsearch (context_ethiopic->compose_buffer,
			  table->data, table->n_seqs,
			  sizeof (guint16) *  row_stride, 
			  compare_seq);

  if (seq)
    {
      guint16 *prev_seq;

      /* Back up to the first sequence that matches to make sure
       * we find the exact match if their is one.
       */
      while (seq > table->data)
	{
	  prev_seq = seq - row_stride;
	  if (compare_seq (context_ethiopic->compose_buffer, prev_seq) != 0)
	    break;
	  seq = prev_seq;
	}
      
      if (n_compose == table->max_seq_len ||
	  seq[n_compose] == 0) /* complete sequence */
	{
	  guint16 *next_seq;
	  gunichar value = 
	    0x10000 * seq[table->max_seq_len] + seq[table->max_seq_len + 1];

	  
	  /* We found a tentative match. See if there are any longer
	   * sequences containing this subsequence
	   */
	  next_seq = seq + row_stride;
	  if (next_seq < table->data + row_stride * table->n_seqs)
	    {
	      if (compare_seq (context_ethiopic->compose_buffer, next_seq) == 0)
		{
		  context_ethiopic->tentative_match = value;
		  context_ethiopic->tentative_match_len = n_compose;

		  g_signal_emit_by_name (context_ethiopic, "preedit_changed");

		  return TRUE;
		}
	    }

	  gtk_im_context_ethiopic_commit_char (GTK_IM_CONTEXT (context_ethiopic), value);
	  context_ethiopic->compose_buffer[0] = 0;
	}
      
      return TRUE;
    }

  return FALSE;
}

/* In addition to the table-driven sequences, we allow Unicode hex
 * codes entered with Ctrl-Shift held down as specified in ISO
 * 14755. 14755 actually allows a different set of modifiers to be
 * used at our discretion, but for now using Ctrl-Shift as in their
 * examples. While holding Ctrl-Shift, pressing space commits the
 * character, and pressing a non-hex-digit is an error.
 */

#define ISO_14755_MOD_MASK (GDK_CONTROL_MASK | GDK_SHIFT_MASK)

static gboolean
check_hex (GtkIMContextEthiopic *context_ethiopic,
           gint                n_compose)
{
  /* See if this is a hex sequence, return TRUE if so */
  gint i;
  GString *str;
  gulong n;
  gchar *nptr = NULL;
  gchar buf[7];
  
  str = g_string_new (NULL);
  
  i = 0;
  while (i < n_compose)
    {
      gunichar ch;
      
      ch = gdk_keyval_to_unicode (context_ethiopic->compose_buffer[i]);
      
      if (ch == 0)
        return FALSE;

      if (!g_unichar_isxdigit (ch))
        return FALSE;

      buf[g_unichar_to_utf8 (ch, buf)] = '\0';

      g_string_append (str, buf);
      
      ++i;
    }

  n = strtoul (str->str, &nptr, 16);

  /* if strtoul fails it probably means non-latin digits were used;
   * we should in principle handle that, but we probably don't.
   */
  if (str->str == nptr)
    {
      g_string_free (str, TRUE);
      return FALSE;
    }
  else
    g_string_free (str, TRUE);

  /* don't allow invalid Unicode or nul bytes */
  if (n == 0 || !g_unichar_validate (n))
    return FALSE;
  
  context_ethiopic->tentative_match = n;
  context_ethiopic->tentative_match_len = n_compose;
  
  g_signal_emit_by_name (context_ethiopic, "preedit_changed");
  
  return TRUE;
}

static gboolean
no_sequence_matches (GtkIMContextEthiopic *context_ethiopic,
                     gint                n_compose,
                     GdkEventKey        *event)
{
  GtkIMContext *context;
  gunichar ch;
  
  context = GTK_IM_CONTEXT (context_ethiopic);
  
  /* No compose sequences found, check first if we have a partial
   * match pending.
   */
  if (context_ethiopic->tentative_match)
    {
      gint len = context_ethiopic->tentative_match_len;
      int i;
      
      gtk_im_context_ethiopic_commit_char (context, context_ethiopic->tentative_match);
      context_ethiopic->compose_buffer[0] = 0;
      
      for (i=0; i < n_compose - len - 1; i++)
	{
	  GdkEvent *tmp_event = gdk_event_copy ((GdkEvent *)event);
	  tmp_event->key.keyval = context_ethiopic->compose_buffer[len + i];
	  
	  gtk_im_context_filter_keypress (context, (GdkEventKey *)tmp_event);
	  gdk_event_free (tmp_event);
	}

      return gtk_im_context_filter_keypress (context, event);
    }
  else
    {
      context_ethiopic->compose_buffer[0] = 0;
      if (n_compose > 1)		/* Invalid sequence */
	{
	  gdk_display_beep (gdk_drawable_get_display (event->window));
	  return TRUE;
	}
  
      ch = gdk_keyval_to_unicode (event->keyval);
      if (ch != 0)
	{
	  gtk_im_context_ethiopic_commit_char (context, ch);
	  return TRUE;
	}
      else
	return FALSE;
    }
}

static gboolean
is_hex_keyval (guint keyval)
{
  gunichar ch = gdk_keyval_to_unicode (keyval);

  return g_unichar_isxdigit (ch);
}

static guint
canonical_hex_keyval (GdkEventKey *event)
{
  GdkKeymap *keymap = gdk_keymap_get_for_display (gdk_drawable_get_display (event->window));
  guint keyval;
  guint *keyvals = NULL;
  gint n_vals = 0;
  gint i;
  
  /* See if the keyval is already a hex digit */
  if (is_hex_keyval (event->keyval))
    return event->keyval;

  /* See if this key would have generated a hex keyval in
   * any other state, and return that hex keyval if so
   */
  gdk_keymap_get_entries_for_keycode (keymap,
				      event->hardware_keycode,
				      NULL,
				      &keyvals, &n_vals);

  keyval = 0;
  i = 0;
  while (i < n_vals)
    {
      if (is_hex_keyval (keyvals[i]))
        {
          keyval = keyvals[i];
          break;
        }

      ++i;
    }

  g_free (keyvals);
  
  if (keyval)
    return keyval;
  else
    /* No way to make it a hex digit
     */
    return 0;
}

static gboolean
gtk_im_et_check_for_hex (GtkIMContext *context,
                         GdkEventKey  *event,
                         int n_compose)
{
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);
  gboolean have_hex_mods;
  guint hex_keyval;

  if (event->type == GDK_KEY_RELEASE)
    {
      if (context_ethiopic->in_hex_sequence &&
           (event->keyval == GDK_Control_L || event->keyval == GDK_Control_R ||
            event->keyval == GDK_Shift_L   || event->keyval == GDK_Shift_R))
        {
          if (context_ethiopic->tentative_match)
            {
              gtk_im_context_ethiopic_commit_char (context, context_ethiopic->tentative_match);
              context_ethiopic->compose_buffer[0] = 0;
            }
          else
            {
              context_ethiopic->in_hex_sequence = 0;
            }

          return TRUE;
        }
      else
        return FALSE;
    }

  have_hex_mods = (event->state & (ISO_14755_MOD_MASK)) == ISO_14755_MOD_MASK;
  hex_keyval = canonical_hex_keyval (event);

  /* If we are already in a non-hex sequence, or
   * this keystroke is not 14755 modifiers + hex digit, don't filter 
   * key events with accelerator modifiers held down.
   */
  if ((n_compose > 0 && !context_ethiopic->in_hex_sequence) || !have_hex_mods || !hex_keyval)
    {
      if (event->state &
          (gtk_accelerator_get_default_mod_mask () & ~GDK_SHIFT_MASK))
        return FALSE;
    }
  
  /* First key in sequence; decide if it's a 14755 hex sequence */
  if (n_compose == 0)
    context_ethiopic->in_hex_sequence = have_hex_mods;
  
  /* Then, check for compose sequences
   */
  // if (context_ethiopic->in_hex_sequence)
    // context_ethiopic->compose_buffer[n_compose++] = hex_keyval ? hex_keyval : event->keyval;
  /*
  else
    context_ethiopic->compose_buffer[n_compose++] = event->keyval;

  context_ethiopic->compose_buffer[n_compose] = 0;
  */

  if (context_ethiopic->in_hex_sequence)
    {
    context_ethiopic->compose_buffer[n_compose++] = hex_keyval ? hex_keyval : event->keyval;
    context_ethiopic->compose_buffer[n_compose] = 0;
      /* If the modifiers are still held down, consider the sequence again */
      if (have_hex_mods)
        {
          /* space ends the sequence, and we eat the space */
          if (n_compose > 1 &&
              (event->keyval == GDK_space ||
               event->keyval == GDK_KP_Space))
            {
              gtk_im_context_ethiopic_commit_char (context, context_ethiopic->tentative_match);
              context_ethiopic->compose_buffer[0] = 0;
              return TRUE;
            }
          else if (check_hex (context_ethiopic, n_compose))
            return TRUE;
        }
    }
  
  /* The current compose_buffer doesn't match anything */
  return FALSE;
}

static void
gtk_im_context_ethiopic_reset (GtkIMContext *context)
{
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);
  
  context_ethiopic->compose_buffer[0] = 0;

  if (context_ethiopic->tentative_match)
    gtk_im_context_ethiopic_commit_char (context, context_ethiopic->tentative_match);

  context_ethiopic->in_hex_sequence = FALSE;
}

static void     
gtk_im_context_parent_get_preedit_string (GtkIMContext   *context,
					  gchar         **str,
					  PangoAttrList **attrs,
					  gint           *cursor_pos)
{
  char outbuf[37]; /* up to 6 hex digits */
  int len = 0;
  
  GtkIMContextEthiopic *context_ethiopic = GTK_IM_CONTEXT_ETHIOPIC (context);

  if (context_ethiopic->tentative_match)
    {
      if (context_ethiopic->in_hex_sequence)
        {
          int hexchars = 0;
          
          while (context_ethiopic->compose_buffer[hexchars] != 0)
            {
              len += g_unichar_to_utf8 (gdk_keyval_to_unicode (context_ethiopic->compose_buffer[hexchars]),
                                        outbuf + len);
              ++hexchars;
            }
        }
      else
        {
          len = g_unichar_to_utf8 (context_ethiopic->tentative_match, outbuf);
        }
      
      g_assert (len < 25);
      outbuf[len] = '\0';      
    }
  else
    {
      outbuf[0] = '\0';
    }
  
  if (str)
    *str = g_strdup (outbuf);

  if (attrs)
    {
      *attrs = pango_attr_list_new ();
      
      if (len)
	{
	  PangoAttribute *attr = pango_attr_underline_new (PANGO_UNDERLINE_SINGLE);
	  attr->start_index = 0;
          attr->end_index = len;
	  pango_attr_list_insert (*attrs, attr);
	}
    }

  if (cursor_pos)
    *cursor_pos = (context_ethiopic->tentative_match ? len : 0);
}

/**
 * gtk_im_context_ethiopic_add_table:
 * @context_ethiopic: A #GtkIMContextEthiopic
 * @data: the table 
 * @max_seq_len: Maximum length of a sequence in the table
 *               (cannot be greater than #GTK_MAX_COMPOSE_LEN)
 * @n_seqs: number of sequences in the table
 * 
 * Adds an additional table to search to the input context.
 * Each row of the table consists of @max_seq_len key symbols
 * followed by two #guint16 interpreted as the high and low
 * words of a #gunicode value. Tables are searched starting
 * from the last added.
 *
 * The table must be sorted in dictionary order on the
 * numeric value of the key symbol fields. (Values beyond
 * the length of the sequence should be zero.)
 **/
void
gtk_im_context_ethiopic_add_table (GtkIMContextEthiopic *context_ethiopic,
				 guint16            *data,
				 gint                max_seq_len,
				 gint                n_seqs)
{
  GtkComposeTable *table;

  g_return_if_fail (GTK_IS_IM_CONTEXT_ETHIOPIC (context_ethiopic));
  g_return_if_fail (data != NULL);
  g_return_if_fail (max_seq_len <= GTK_MAX_COMPOSE_LEN);
  
  table = g_new (GtkComposeTable, 1);
  table->data = data;
  table->max_seq_len = max_seq_len;
  table->n_seqs = n_seqs;

  context_ethiopic->tables = g_slist_prepend (context_ethiopic->tables, table);
}

