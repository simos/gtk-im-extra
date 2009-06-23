/* GTK - The GIMP Toolkit
 * Copyright (C) 2000 Red Hat Software
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
 * Author: Owen Taylor <otaylor@redhat.com>
 *
 */

#include <config.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtkimcontextsimple.h>
#include <gtk/gtkimmodule.h>

#include "im-extra-intl.h"

GType type_ipa = 0;

static void ipa_xsampa_class_init (GtkIMContextSimpleClass *class);
static void ipa_xsampa_init (GtkIMContextSimple *im_context);

static void
ipa_xsampa_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (GtkIMContextSimpleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) ipa_xsampa_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (GtkIMContextSimple),
    0,
    (GInstanceInitFunc) ipa_xsampa_init,
  };

  type_ipa = 
    g_type_module_register_type (module,
				 GTK_TYPE_IM_CONTEXT_SIMPLE,
				 "GtkIMContextIpaXSampa",
				 &object_info, 0);
}

/* The sequences here match X-SAMPA.
 * http://www.phon.ucl.ac.uk/home/sampa/x-sampa.htm
 * They allow entering all characters in iso-8859-5
 */
static guint16 ipa_xsampa_compose_seqs[] = {
  GDK_exclam, 0,              0,      0,      0,      0x2193,   /* DOWNWARDS ARROW */
  GDK_exclam, GDK_backslash,  0,      0,      0,      0x1C3,    /* LATIN LETTER RETROFLEX CLICK */
  GDK_quotedbl, 0,            0,      0,      0,      0x2C8,    /* MODIFIER LETTER VERTICAL LINE */
  GDK_percent , 0,            0,      0,      0,      0x2CC,    /* MODIFIER LETTER LOW VERTICAL LINE */
  GDK_ampersand, 0,           0,      0,      0,      0x276,    /* LATIN LETTER SMALL CAPITAL OE */
  GDK_apostrophe, 0,          0,      0,      0,      0x2B2,    /* MODIFIER LETTER SMALL J */
  GDK_minus,  0,              0,      0,      0,      0x02D,    /* HYPHEN-MINUS */
  GDK_minus,  GDK_backslash,  0,      0,      0,      0x203F,   /* UNDERTIE */

  GDK_1,      0,              0,      0,      0,      0x268,    /* LATIN SMALL LETTER I WITH STROKE */
  GDK_2,      0,              0,      0,      0,      0x0F8,    /* LATIN SMALL LETTER O WITH STROKE */
  GDK_3,      0,              0,      0,      0,      0x25C,    /* LATIN SMALL LETTER REVERSED OPEN E */
  GDK_3,      GDK_backslash,  0,      0,      0,      0x25E,    /* LATIN SMALL LETTER CLOSED REVERSED OPEN E */
  GDK_4,      0,              0,      0,      0,      0x27E,    /* LATIN SMALL LETTER R WITH FISHHOOK */
  GDK_5,      0,              0,      0,      0,      0x26B,    /* LATIN SMALL LETTER L WITH MIDDLE TILDE */
  GDK_6,      0,              0,      0,      0,      0x250,    /* LATIN SMALL LETTER TURNED A */
  GDK_7,      0,              0,      0,      0,      0x264,    /* LATIN SMALL LETTER RAMS HORN */
  GDK_8,      0,              0,      0,      0,      0x275,    /* LATIN SMALL LETTER BARRED O */
  GDK_9,      0,              0,      0,      0,      0x153,    /* LATIN SMALL LIGATURE OE */

  GDK_colon,  0,              0,      0,      0,      0x2D0,    /* MODIFIER LETTER TRIANGULAR COLON */
  GDK_colon,  GDK_backslash,  0,      0,      0,      0x2D1,    /* MODIFIER LETTER HALF TRIANGULAR COLON */
  GDK_less,   0,              0,      0,      0,      0x03C,    /* LESS-THAN SIGN */
  GDK_less,   GDK_B,          GDK_greater, 0, 0,      0x2E9,    /* MODIFIER LETTER EXTRA-LOW TONE BAR */
  GDK_less,   GDK_F,          GDK_greater, 0, 0,      0x2198,   /* SOUTH EAST ARROW */
  GDK_less,   GDK_H,          GDK_greater, 0, 0,      0x2E6,    /* MODIFIER LETTER HIGH TONE BAR */
  GDK_less,   GDK_L,          GDK_greater, 0, 0,      0x2E8,    /* MODIFIER LETTER LOW TONE BAR */
  GDK_less,   GDK_M,          GDK_greater, 0, 0,      0x2E7,    /* MODIFIER LETTER MID TONE BAR */
  GDK_less,   GDK_R,          GDK_greater, 0, 0,      0x2197,   /* NORTH EAST ARROW */
  GDK_less,   GDK_T,          GDK_greater, 0, 0,      0x2E5,    /* MODIFIER LETTER EXTRA-HIGH TONE BAR */
  GDK_less,   GDK_backslash,  0,      0,      0,      0x2A2,    /* LATIN LETTER REVERSED GLOTTAL STOP WITH STROKE */
  GDK_equal,  0,              0,      0,      0,      0x329,    /* COMBINING VERTICAL LINE BELOW */
  GDK_equal,  GDK_backslash,  0,      0,      0,      0x1C2,    /* LATIN LETTER ALVEOLAR CLICK */
  GDK_greater, 0,             0,      0,      0,      0x03E,    /* GREATER-THAN SIGN */
  GDK_greater, GDK_backslash, 0,      0,      0,      0x2A1,    /* LATIN LETTER GLOTTAL STOP WITH STROKE */
  GDK_question, 0,            0,      0,      0,      0x294,    /* LATIN LETTER GLOTTAL STOP */
  GDK_question, GDK_backslash, 0,     0,      0,      0x295,    /* LATIN LETTER PHARYNGEAL VOICED FRICATIVE */
  GDK_at,     0,              0,      0,      0,      0x259,    /* LATIN SMALL LETTER SCHWA */
  GDK_at,     GDK_backslash,  0,      0,      0,      0x258,    /* LATIN SMALL LETTER REVERSED E */
  GDK_at,     GDK_grave,      0,      0,      0,      0x25A,    /* LATIN SMALL LETTER SCHWA WITH HOOK */

  GDK_A,      0,              0,      0,      0,      0x251, 	/* LATIN SMALL LETTER ALPHA */
  GDK_B,      0,              0,      0,      0,      0x3B2,    /* GREEK SMALL LETTER BETA */
  GDK_B,      GDK_backslash,  0,      0,      0,      0x299,    /* LATIN LETTER SMALL CAPITAL B */
  GDK_C,      0,              0,      0,      0,      0x0E7,    /* LATIN SMALL LETTER C WITH CEDILLA */
  GDK_D,      0,              0,      0,      0,      0x00F0,   /* LATIN SMALL LETTER ETH */
  GDK_E,      0,              0,      0,      0,      0x25B,    /* LATIN SMALL LETTER OPEN E */
  GDK_E,      GDK_grave,      0,      0,      0,      0x25D,    /* LATIN SMALL LETTER REVERSED OPEN E WITH HOOK */
  GDK_F,      0,              0,      0,      0,      0x271,    /*LATIN SMALL LETTER M WITH HOOK */
  GDK_G,      0,              0,      0,      0,      0x263,    /* LATIN SMALL LETTER GAMMA */
  GDK_G,      0,              0,      0,      0,      0x263,    /* LATIN SMALL LETTER GAMMA */
  GDK_G,      GDK_backslash,  0,      0,      0,      0x262,    /* LATIN LETTER SMALL CAPITAL G */
  GDK_G, GDK_backslash, GDK_underscore, GDK_less, 0,  0x29B,    /* LATIN LETTER SMALL CAPITAL G WITH HOOK */
  GDK_H,      0,              0,      0,      0,      0x265,    /* LATIN SMALL LETTER TURNED H */
  GDK_H,      GDK_backslash,  0,      0,      0,      0x029C,   /* LATIN LETTER SMALL CAPITAL H */
  GDK_I,      0,              0,      0,      0,      0x26A, 	/* LATIN LETTER SMALL CAPITAL I */
  /* not in IPA, nor Unicode 4.0 */
  /* GDK_I,      GDK_backslash,  0,      0,      0,      0x000, */ /* LATIN LETTER SMALL CAPITAL I WITH STROKE */
  GDK_J,      0,              0,      0,      0,      0x272,    /* LATIN SMALL LETTER N WITH LEFT HOOK */
  GDK_J,      GDK_backslash,  0,      0,      0,      0x25F,    /* LATIN SMALL LETTER DOTLESS J WITH STROKE */
  GDK_J, GDK_backslash, GDK_underscore, GDK_less, 0,  0x284,    /* LATIN SMALL LETTER DOTLESS J WITH STROKE AND HOOK */
  GDK_K,      0,              0,      0,      0,      0x26C,    /* LATIN SMALL LETTER L WITH BELT */
  GDK_K,      GDK_backslash,  0,      0,      0,      0x26E,    /* LATIN SMALL LETTER LEZH */
  GDK_L,      0,              0,      0,      0,      0x28E,    /* LATIN SMALL LETTER TURNED Y */
  GDK_L,      GDK_backslash,  0,      0,      0,      0x29F,    /* LATIN LETTER SMALL CAPITAL L */
  GDK_M,      0,              0,      0,      0,      0x26F,    /* LATIN SMALL LETTER TURNED M */
  GDK_M,      GDK_backslash,  0,      0,      0,      0x270,    /* LATIN SMALL LETTER TURNED M WITH LONG LEG */
  GDK_N,      0,              0,      0,      0,      0x14B,    /* LATIN SMALL LETTER ENG */
  GDK_N,      GDK_backslash,  0,      0,      0,      0x274,    /* LATIN LETTER SMALL CAPITAL N */
  GDK_O,      0,              0,      0,      0,      0x254,    /* LATIN SMALL LETTER OPEN O */
  GDK_O,      GDK_backslash,  0,      0,      0,      0x298,    /* LATIN LETTER BILABIAL CLICK */
  GDK_P,      0,              0,      0,      0,      0x28B,    /* LATIN SMALL LETTER V WITH HOOK */
  GDK_Q,      0,              0,      0,      0,      0x252,    /* LATIN SMALL LETTER TURNED ALPHA */
  GDK_R,      0,              0,      0,      0,      0x281,    /* LATIN LETTER SMALL CAPITAL INVERTED R */
  GDK_R,      GDK_backslash,  0,      0,      0,      0x280,    /* LATIN LETTER SMALL CAPITAL R */
  GDK_S,      0,              0,      0,      0,      0x283,    /* LATIN SMALL LETTER ESH */
  GDK_T,      0,              0,      0,      0,      0x3B8, 	/* GREEK SMALL LETTER THETA */
  GDK_U,      0,              0,      0,      0,      0x28A,    /* LATIN SMALL LETTER UPSILON */
  /*  not in IPA, nor Unicode 4.0 */
  /* GDK_U,      GDK_backslash,  0,      0,      0,      0x000, */ /* LATIN SMALL LETTER UPSILON WITH STROKE */
  GDK_V,      0,              0,      0,      0,      0x28C,    /* LATIN SMALL LETTER TURNED V */
  GDK_W,      0,              0,      0,      0,      0x28D,    /* LATIN SMALL LETTER TURNED W */
  GDK_X,      0,              0,      0,      0,      0x3C7,    /* GREEK SMALL LETTER CHI */
  GDK_X,      GDK_backslash,  0,      0,      0,      0x127,    /* LATIN SMALL LETTER H WITH STROKE */
  GDK_Y,      0,              0,      0,      0,      0x28F,    /* LATIN LETTER SMALL CAPITAL Y */
  GDK_Z,      0,              0,      0,      0,      0x292,    /* LATIN SMALL LETTER EZH */
  GDK_Z,      GDK_backslash,  0,      0,      0,      0x293,    /* LATIN SMALL LETTER EZH WITH CURL */

  GDK_asciicircum, 0,         0,      0,      0,      0x2191,   /* UPWARDS ARROW */
  GDK_underscore, GDK_quotedbl, 0,    0,      0,      0x308,    /* COMBINING DIAERESIS */
  GDK_underscore, GDK_plus,   0,      0,      0,      0x31F,    /* COMBINING PLUS SIGN BELOW */
  GDK_underscore, GDK_minus,  0,      0,      0,      0x320,    /* COMBINING MINUS SIGN BELOW */
  GDK_underscore, GDK_slash,  0,      0,      0,      0x30C,    /* COMBINING CARON */
  GDK_underscore, GDK_0,      0,      0,      0,      0x325,    /* COMBINING RING BELOW */
  GDK_underscore, GDK_1,      0,      0,      0,      0x2081,   /* SUBSCRIPT ONE */
  GDK_underscore, GDK_2,      0,      0,      0,      0x2082,   /* SUBSCRIPT TWO */
  GDK_underscore, GDK_3,      0,      0,      0,      0x2083,   /* SUBSCRIPT THREE */
  GDK_underscore, GDK_4,      0,      0,      0,      0x2084,   /* SUBSCRIPT FOUR */
  GDK_underscore, GDK_5,      0,      0,      0,      0x2085,   /* SUBSCRIPT FIVE */
  GDK_underscore, GDK_equal,  0,      0,      0,      0x329,    /* COMBINING VERTICAL LINE BELOW */
  GDK_underscore, GDK_greater, 0,     0,      0,      0x2BC,    /* MODIFIER LETTER APOSTROPHE */
  GDK_underscore, GDK_A,      0,      0,      0,      0x318,    /* COMBINING LEFT TACK BELOW */
  GDK_underscore, GDK_B,      0,      0,      0,      0x30F,    /* COMBINING DOUBLE GRAVE ACCENT */
  /* not in Unicode */
  /*  GDK_underscore, GDK_B, GDK_underscore, GDK_L, 0,    0x000, */
  GDK_underscore, GDK_F,      0,      0,      0,      0x302,    /* COMBINING CIRCUMFLEX ACCENT */
  GDK_underscore, GDK_G,      0,      0,      0,      0x2E0,    /* MODIFIER LETTER SMALL GAMMA */
  GDK_underscore, GDK_H,      0,      0,      0,      0x301,    /* COMBINING ACUTE ACCENT */
  /* not in Unicode */
  /*  GDK_underscore, GDK_H, GDK_underscore, GDK_T, 0,    0x000, */
  GDK_underscore, GDK_L,      0,      0,      0,      0x300,    /* COMBINING GRAVE ACCENT */
  GDK_underscore, GDK_M,      0,      0,      0,      0x304,    /* COMBINING MACRON */
  GDK_underscore, GDK_N,      0,      0,      0,      0x33C,    /* COMBINING SEAGULL BELOW */
  GDK_underscore, GDK_O,      0,      0,      0,      0x339,    /* COMBINING RIGHT HALF RING BELOW */
  GDK_underscore, GDK_R,      0,      0,      0,      0x30C,    /* COMBINING CARON */
  /* not in Unicode */
  /*  GDK_underscore, GDK_R, GDK_underscore, GDK_F, 0,    0x000, */
  GDK_underscore, GDK_T,      0,      0,      0,      0x30B,    /* COMBINING DOUBLE ACUTE ACCENT */
  GDK_underscore, GDK_X,      0,      0,      0,      0x2D8,    /* BREVE */
  GDK_underscore, GDK_question, GDK_backslash, 0, 0,  0x2E4,    /* MODIFIER LETTER SMALL REVERSED GLOTTAL STOP */
  GDK_underscore, GDK_backslash, 0,   0,      0,      0x302,    /* COMBINING CIRCUMFLEX ACCENT */
  GDK_underscore, GDK_asciicircum, 0, 0,      0,      0x32F,    /* COMBINING INVERTED BREVE BELOW */
  GDK_underscore, GDK_a,      0,      0,      0,      0x33A,    /* COMBINING INVERTED BRIDGE BELOW */
  GDK_underscore, GDK_c,      0,      0,      0,      0x31C,    /* COMBINING LEFT HALF RING BELOW */
  GDK_underscore, GDK_d,      0,      0,      0,      0x32A,    /* COMBINING BRIDGE BELOW */
  GDK_underscore, GDK_e,      0,      0,      0,      0x334,    /* COMBINING TILDE OVERLAY */
  GDK_underscore, GDK_h,      0,      0,      0,      0x2B0,    /* MODIFIER LETTER SMALL H */
  GDK_underscore, GDK_j,      0,      0,      0,      0x2B2,    /* MODIFIER LETTER SMALL J */
  GDK_underscore, GDK_k,      0,      0,      0,      0x330,    /* COMBINING TILDE BELOW */
  GDK_underscore, GDK_l,      0,      0,      0,      0x3E1,    /* MODIFIER LETTER SMALL L */
  GDK_underscore, GDK_m,      0,      0,      0,      0x33B,    /* COMBINING SQUARE BELOW */
  GDK_underscore, GDK_n,      0,      0,      0,      0x207F,   /* SUPERSCRIPT LATIN SMALL LETTER N */
  GDK_underscore, GDK_o,      0,      0,      0,      0x31E,    /* COMBINING DOWN TACK BELOW */
  GDK_underscore, GDK_q,      0,      0,      0,      0x319,    /* COMBINING RIGHT TACK BELOW */
  GDK_underscore, GDK_r,      0,      0,      0,      0x31D,    /* COMBINING UP TACK BELOW */
  GDK_underscore, GDK_t,      0,      0,      0,      0x324,    /* COMBINING DIAERESIS BELOW */
  GDK_underscore, GDK_v,      0,      0,      0,      0x32C,    /* COMBINING CARON BELOW */
  GDK_underscore, GDK_w,      0,      0,      0,      0x2B7,    /* MODIFIER LETTER SMALL W */
  GDK_underscore, GDK_x,      0,      0,      0,      0x33D,    /* COMBINING X ABOVE */
  GDK_underscore, GDK_braceright, 0,  0,      0,      0x31A,    /* COMBINING LEFT ANGLE ABOVE */ /* not working */
  GDK_underscore, GDK_asciitilde, 0,  0,      0,      0x303,    /* COMBINING TILDE */

  GDK_grave,  0,              0,      0,      0,      0x2DE,    /* MODIFIER LETTER RHOTIC HOOK */

  GDK_b,      0,              0,      0,      0,      0x062, 	/* LATIN SMALL LETTER B */
  GDK_b,      GDK_underscore, GDK_less, 0,    0,      0x253,    /* LATIN SMALL LETTER B WITH HOOK */
  GDK_d,      0,              0,      0,      0,      0x064, 	/* LATIN SMALL LETTER D */
  GDK_d,      GDK_underscore, GDK_less, 0,    0,      0x257,    /* LATIN SMALL LETTER D WITH HOOK */
  GDK_d,      GDK_grave,      0,      0,      0,      0x256,    /* LATIN SMALL LETTER D WITH TAIL */
  GDK_g,      0,              0,      0,      0,      0x261,    /* LATIN SMALL LETTER SCRIPT G */
  GDK_g,      GDK_underscore, GDK_less, 0,    0,      0x260,    /* LATIN SMALL LETTER G WITH HOOK */
  GDK_h,      0,              0,      0,      0,      0x068, 	/* LATIN SMALL LETTER H */
  GDK_h,      GDK_backslash,  0,      0,      0,      0x266,    /* LATIN SMALL LETTER H WITH HOOK */
  GDK_j,      0,              0,      0,      0,      0x06A, 	/* LATIN SMALL LETTER J */
  GDK_j,      GDK_backslash,  0,      0,      0,      0x29D,    /* LATIN SMALL LETTER J WITH CROSSED-TAIL */
  GDK_l,      0,              0,      0,      0,      0x06C, 	/* LATIN SMALL LETTER L */
  GDK_l,      GDK_backslash,  0,      0,      0,      0x27A,    /* LATIN SMALL LETTER TURNED R WITH LONG LEG */
  GDK_l,      GDK_backslash,  GDK_backslash, 0, 0,    0x27C,    /* LATIN SMALL LETTER R WITH LONG LEG */
  GDK_l,      GDK_grave,      0,      0,      0,      0x26D,    /* LATIN SMALL LETTER L WITH RETROFLEX HOOK */
  GDK_n,      0,              0,      0,      0,      0x06E, 	/* LATIN SMALL LETTER N */
  GDK_n,      GDK_grave,      0,      0,      0,      0x273,    /* LATIN SMALL LETTER N WITH RETROFLEX HOOK */
  GDK_p,      0,              0,      0,      0,      0x070, 	/* LATIN SMALL LETTER P */
  GDK_p,      GDK_backslash,  0,      0,      0,      0x278,    /* LATIN SMALL LETTER PHI */
  GDK_r,      0,              0,      0,      0,      0x072, 	/* LATIN SMALL LETTER R */
  GDK_r,      GDK_backslash,  0,      0,      0,      0x279,    /* LATIN SMALL LETTER TURNED R */
  GDK_r,      GDK_backslash,  GDK_grave, 0,   0,      0x27B,    /* LATIN SMALL LETTER TURNED R WITH HOOK */
  GDK_r,      GDK_grave,      0,      0,      0,      0x27D,    /* LATIN SMALL LETTER R WITH TAIL */
  GDK_s,      0,              0,      0,      0,      0x073, 	/* LATIN SMALL LETTER S */
  GDK_s,      GDK_backslash,  0,      0,      0,      0x255,    /* LATIN SMALL LETTER C WITH CURL */
  GDK_s,      GDK_grave,      0,      0,      0,      0x282,    /* LATIN SMALL LETTER S WITH HOOK */
  GDK_t,      0,              0,      0,      0,      0x074, 	/* LATIN SMALL LETTER T */
  GDK_t,      GDK_grave,      0,      0,      0,      0x288,    /* LATIN SMALL LETTER T WITH RETROFLEX HOOK */
  GDK_v,      0,              0,      0,      0,      0x076, 	/* LATIN SMALL LETTER V */
  GDK_v,      GDK_backslash,  0,      0,      0,      0x28B,    /* LATIN SMALL LETTER V WITH HOOK */
  GDK_x,      0,              0,      0,      0,      0x078, 	/* LATIN SMALL LETTER X */
  GDK_x,      GDK_backslash,  0,      0,      0,      0x267,    /* LATIN SMALL LETTER HENG WITH HOOK */
  GDK_z,      0,              0,      0,      0,      0x07A, 	/* LATIN SMALL LETTER Z */
  GDK_z,      GDK_backslash,  0,      0,      0,      0x291,    /* LATIN SMALL LETTER Z WITH CURL */
  GDK_z,      GDK_grave,      0,      0,      0,      0x290,    /* LATIN SMALL LETTER Z WITH RETROFLEX HOOK */

  GDK_braceleft, 0,           0,      0,      0,      0x0E6,    /* LATIN SMALL LETTER AE */
  GDK_bar,    0,              0,      0,      0,      0x07C,    /* VERTICAL LINE */
  GDK_bar,    GDK_backslash,  0,      0,      0,      0x1C0,    /* LATIN LETTER DENTAL CLICK */
  GDK_bar, GDK_backslash, GDK_bar, GDK_backslash, 0,  0x1C1,    /* LATIN LETTER LATERAL CLICK */
  GDK_bar,    GDK_bar,        0,      0,      0,      0x2016,   /* DOUBLE VERTICAL LINE */
  GDK_braceright, 0,          0,      0,      0,      0x289,    /* LATIN SMALL LETTER U BAR */
  GDK_asciitilde, 0,          0,      0,      0,      0x303,    /* COMBINING TILDE */

};

static void
ipa_xsampa_class_init (GtkIMContextSimpleClass *class)
{
}

static void
ipa_xsampa_init (GtkIMContextSimple *im_context)
{
  gtk_im_context_simple_add_table (im_context,
				   ipa_xsampa_compose_seqs,
				   4,
				   G_N_ELEMENTS (ipa_xsampa_compose_seqs) / (4 + 2));
}

static const GtkIMContextInfo ipa_xsampa_info = { 
  "ipa",		   /* ID */
  N_("IPA (XSAMPA)"),      /* Human readable name */
  GETTEXT_PACKAGE,	   /* Translation domain */
  LOCALEDIR,		   /* Dir for bindtextdomain (not strictly needed for "gtk+") */
  ""		           /* Languages for which this module is the default */
};

static const GtkIMContextInfo *info_list[] = {
  &ipa_xsampa_info
};

void
im_module_init (GTypeModule *module)
{
  ipa_xsampa_register_type (module);
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
  if (strcmp (context_id, "ipa_xsampa") == 0)
    return g_object_new (type_ipa, NULL);
  else
    return NULL;
}
