/* $Id: imtamil99.c,v 1.4 2003/09/25 21:29:15 nlevitt Exp $ */
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
/*
 * GTK+ Tamil input method
 * modeled after Dinesh Nadarajah's 
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <gtk/gtkimcontext.h>
#include <gtk/gtkimmodule.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <string.h>

#include "im-extra-intl.h"


static GType type_tamil99 = 0;

enum { TAMIL99_MAX_COMPOSE_LEN = 2 };

typedef struct
{
  guint keys[TAMIL99_MAX_COMPOSE_LEN + 1];  /* 0-terminated */
  gchar *string;
}
ComposeSequence;

/* like GtkIMContextSimple */
static guint compose_buffer[TAMIL99_MAX_COMPOSE_LEN + 1];
static int n_compose = 0;

static ComposeSequence tamil99_compose_seqs[] = 
{
  { { GDK_quotedbl,     0,                0, }, "'"  },
  { { GDK_colon,        0,                0, }, ";"  },
  { { GDK_L,            0,                0, }, ":"  },
  { { GDK_K,            0,                0, }, "\"" },
  { { GDK_M,            0,                0, }, "/"  },
  { { GDK_P,            0,                0, }, "]"  },
  { { GDK_O,            0,                0, }, "["  },
  { { GDK_e,            0,                0, }, "ஊ"  },
  { { GDK_d,            0,                0, }, "உ"  },
  { { GDK_f,            0,                0, }, "ஃ"  },
  { { GDK_z,            0,                0, }, "ஔ"  },
  { { GDK_x,            0,                0, }, "ஓ"  },
  { { GDK_c,            0,                0, }, "ஒ"  },
  { { GDK_t,            0,                0, }, "ஏ"  },
  { { GDK_g,            0,                0, }, "எ"  },
  { { GDK_r,            0,                0, }, "ஐ"  },
  { { GDK_w,            0,                0, }, "ஈ"  },
  { { GDK_s,            0,                0, }, "இ"  },
  { { GDK_q,            0,                0, }, "ஆ"  },
  { { GDK_a,            0,                0, }, "அ"  },
  { { GDK_Y,            0,                0, }, "ஸ்ரீ" },
  { { GDK_R,            0,                0, }, "ஹ"  },
  { { GDK_E,            0,                0, }, "ஜ"  },
  { { GDK_W,            0,                0, }, "ஷ"  },
  { { GDK_Q,            0,                0, }, "ஸ"  },
  { { GDK_slash,        0,                0, }, "ழ"  },
  { { GDK_m,            0,                0, }, "ர"  },
  { { GDK_n,            0,                0, }, "ல"  },
  { { GDK_b,            0,                0, }, "ங"  },
  { { GDK_v,            0,                0, }, "வ"  },
  { { GDK_apostrophe,   0,                0, }, "ய"  },
  { { GDK_semicolon,    0,                0, }, "ந"  },
  { { GDK_l,            0,                0, }, "த"  },
  { { GDK_k,            0,                0, }, "ம"  },
  { { GDK_j,            0,                0, }, "ப"  },
  { { GDK_h,            0,                0, }, "க"  },
  { { GDK_bracketright, 0,                0, }, "ஞ"  },
  { { GDK_bracketleft,  0,                0, }, "ச"  },
  { { GDK_p,            0,                0, }, "ண"  },
  { { GDK_o,            0,                0, }, "ட"  },
  { { GDK_i,            0,                0, }, "ன"  },
  { { GDK_u,            0,                0, }, "ற"  },
  { { GDK_y,            0,                0, }, "ள"  },
  { { GDK_T,            0,                0, }, "க்ஷ" },
  { { GDK_T,            GDK_T,            0, }, "க்ஷ்" },

  { { GDK_R,            GDK_e,            0, }, "ஹூ" },
  { { GDK_E,            GDK_e,            0, }, "ஜூ" },
  { { GDK_W,            GDK_e,            0, }, "ஷூ" },
  { { GDK_Q,            GDK_e,            0, }, "ஸூ" },
  { { GDK_slash,        GDK_e,            0, }, "ழூ" },
  { { GDK_m,            GDK_e,            0, }, "ரூ" },
  { { GDK_n,            GDK_e,            0, }, "லூ" },
  { { GDK_b,            GDK_e,            0, }, "ஙூ" },
  { { GDK_v,            GDK_e,            0, }, "வூ" },
  { { GDK_apostrophe,   GDK_e,            0, }, "யூ" },
  { { GDK_semicolon,    GDK_e,            0, }, "நூ" },
  { { GDK_l,            GDK_e,            0, }, "தூ" },
  { { GDK_k,            GDK_e,            0, }, "மூ" },
  { { GDK_j,            GDK_e,            0, }, "பூ" },
  { { GDK_h,            GDK_e,            0, }, "கூ" },
  { { GDK_bracketright, GDK_e,            0, }, "ஞூ" },
  { { GDK_bracketleft,  GDK_e,            0, }, "சூ" },
  { { GDK_p,            GDK_e,            0, }, "ணூ" },
  { { GDK_o,            GDK_e,            0, }, "டூ" },
  { { GDK_i,            GDK_e,            0, }, "னூ" },
  { { GDK_u,            GDK_e,            0, }, "றூ" },
  { { GDK_y,            GDK_e,            0, }, "ளூ" },

  { { GDK_R,            GDK_d,            0, }, "ஹு" },
  { { GDK_E,            GDK_d,            0, }, "ஜு" },
  { { GDK_W,            GDK_d,            0, }, "ஷு" },
  { { GDK_Q,            GDK_d,            0, }, "ஸு" },
  { { GDK_slash,        GDK_d,            0, }, "ழு" },
  { { GDK_m,            GDK_d,            0, }, "ரு" },
  { { GDK_n,            GDK_d,            0, }, "லு" },
  { { GDK_b,            GDK_d,            0, }, "ஙு" },
  { { GDK_v,            GDK_d,            0, }, "வு" },
  { { GDK_apostrophe,   GDK_d,            0, }, "யு" },
  { { GDK_semicolon,    GDK_d,            0, }, "நு" },
  { { GDK_l,            GDK_d,            0, }, "து" },
  { { GDK_k,            GDK_d,            0, }, "மு" },
  { { GDK_j,            GDK_d,            0, }, "பு" },
  { { GDK_h,            GDK_d,            0, }, "கு" },
  { { GDK_bracketright, GDK_d,            0, }, "ஞு" },
  { { GDK_bracketleft,  GDK_d,            0, }, "சு" },
  { { GDK_p,            GDK_d,            0, }, "ணு" },
  { { GDK_o,            GDK_d,            0, }, "டு" },
  { { GDK_i,            GDK_d,            0, }, "னு" },
  { { GDK_u,            GDK_d,            0, }, "று" },
  { { GDK_y,            GDK_d,            0, }, "ளு" },

  { { GDK_R,            GDK_f,            0, }, "ஹ்"  },
  { { GDK_E,            GDK_f,            0, }, "ஜ"  },
  { { GDK_W,            GDK_f,            0, }, "ஷ"  },
  { { GDK_Q,            GDK_f,            0, }, "ஸ"  },
  { { GDK_slash,        GDK_f,            0, }, "ழ"  },
  { { GDK_m,            GDK_f,            0, }, "ர"  },
  { { GDK_n,            GDK_f,            0, }, "ல"  },
  { { GDK_b,            GDK_f,            0, }, "ங"  },
  { { GDK_v,            GDK_f,            0, }, "வ"  },
  { { GDK_apostrophe,   GDK_f,            0, }, "ய"  },
  { { GDK_semicolon,    GDK_f,            0, }, "ந"  },
  { { GDK_l,            GDK_f,            0, }, "த"  },
  { { GDK_k,            GDK_f,            0, }, "ம"  },
  { { GDK_j,            GDK_f,            0, }, "ப"  },
  { { GDK_h,            GDK_f,            0, }, "க"  },
  { { GDK_bracketright, GDK_f,            0, }, "ஞ"  },
  { { GDK_bracketleft,  GDK_f,            0, }, "ச"  },
  { { GDK_p,            GDK_f,            0, }, "ண"  },
  { { GDK_o,            GDK_f,            0, }, "ட"  },
  { { GDK_i,            GDK_f,            0, }, "ன"  },
  { { GDK_u,            GDK_f,            0, }, "ற"  },
  { { GDK_y,            GDK_f,            0, }, "ள"  },

  { { GDK_R,            GDK_z,            0, }, "ஹௌ" },
  { { GDK_E,            GDK_z,            0, }, "ஜௌ" },
  { { GDK_W,            GDK_z,            0, }, "ஷௌ" },
  { { GDK_Q,            GDK_z,            0, }, "ஸௌ" },
  { { GDK_slash,        GDK_z,            0, }, "ழௌ" },
  { { GDK_m,            GDK_z,            0, }, "ரௌ" },
  { { GDK_n,            GDK_z,            0, }, "லௌ" },
  { { GDK_b,            GDK_z,            0, }, "ஙௌ" },
  { { GDK_v,            GDK_z,            0, }, "வௌ" },
  { { GDK_apostrophe,   GDK_z,            0, }, "யௌ" },
  { { GDK_semicolon,    GDK_z,            0, }, "நௌ" },
  { { GDK_l,            GDK_z,            0, }, "தௌ" },
  { { GDK_k,            GDK_z,            0, }, "மௌ" },
  { { GDK_j,            GDK_z,            0, }, "பௌ" },
  { { GDK_h,            GDK_z,            0, }, "கௌ" },
  { { GDK_bracketright, GDK_z,            0, }, "ஞௌ" },
  { { GDK_bracketleft,  GDK_z,            0, }, "சௌ" },
  { { GDK_p,            GDK_z,            0, }, "ணௌ" },
  { { GDK_o,            GDK_z,            0, }, "டௌ" },
  { { GDK_i,            GDK_z,            0, }, "னௌ" },
  { { GDK_u,            GDK_z,            0, }, "றௌ" },
  { { GDK_y,            GDK_z,            0, }, "ளௌ" },

  { { GDK_R,            GDK_x,            0, }, "ஹோ" },
  { { GDK_E,            GDK_x,            0, }, "ஜோ" },
  { { GDK_W,            GDK_x,            0, }, "ஷோ" },
  { { GDK_Q,            GDK_x,            0, }, "ஸோ" },
  { { GDK_slash,        GDK_x,            0, }, "ழோ" },
  { { GDK_m,            GDK_x,            0, }, "ரோ" },
  { { GDK_n,            GDK_x,            0, }, "லோ" },
  { { GDK_b,            GDK_x,            0, }, "ஙோ" },
  { { GDK_v,            GDK_x,            0, }, "வோ" },
  { { GDK_apostrophe,   GDK_x,            0, }, "யோ" },
  { { GDK_semicolon,    GDK_x,            0, }, "நோ" },
  { { GDK_l,            GDK_x,            0, }, "தோ" },
  { { GDK_k,            GDK_x,            0, }, "மோ" },
  { { GDK_j,            GDK_x,            0, }, "போ" },
  { { GDK_h,            GDK_x,            0, }, "கோ" },
  { { GDK_bracketright, GDK_x,            0, }, "ஞோ" },
  { { GDK_bracketleft,  GDK_x,            0, }, "சோ" },
  { { GDK_p,            GDK_x,            0, }, "ணோ" },
  { { GDK_o,            GDK_x,            0, }, "டோ" },
  { { GDK_i,            GDK_x,            0, }, "னோ" },
  { { GDK_u,            GDK_x,            0, }, "றோ" },
  { { GDK_y,            GDK_x,            0, }, "ளோ" },

  { { GDK_R,            GDK_c,            0, }, "ஹொ" },
  { { GDK_E,            GDK_c,            0, }, "ஜொ" },
  { { GDK_W,            GDK_c,            0, }, "ஷொ" },
  { { GDK_Q,            GDK_c,            0, }, "ஸொ" },
  { { GDK_slash,        GDK_c,            0, }, "ழொ" },
  { { GDK_m,            GDK_c,            0, }, "ரொ" },
  { { GDK_n,            GDK_c,            0, }, "லொ" },
  { { GDK_b,            GDK_c,            0, }, "ஙொ" },
  { { GDK_v,            GDK_c,            0, }, "வொ" },
  { { GDK_apostrophe,   GDK_c,            0, }, "யொ" },
  { { GDK_semicolon,    GDK_c,            0, }, "நொ" },
  { { GDK_l,            GDK_c,            0, }, "தொ" },
  { { GDK_k,            GDK_c,            0, }, "மொ" },
  { { GDK_j,            GDK_c,            0, }, "பொ" },
  { { GDK_h,            GDK_c,            0, }, "கொ" },
  { { GDK_bracketright, GDK_c,            0, }, "ஞொ" },
  { { GDK_bracketleft,  GDK_c,            0, }, "சொ" },
  { { GDK_p,            GDK_c,            0, }, "ணொ" },
  { { GDK_o,            GDK_c,            0, }, "டொ" },
  { { GDK_i,            GDK_c,            0, }, "னொ" },
  { { GDK_u,            GDK_c,            0, }, "றொ" },
  { { GDK_y,            GDK_c,            0, }, "ளொ" },

  { { GDK_R,            GDK_t,            0, }, "ஹே" },
  { { GDK_E,            GDK_t,            0, }, "ஜே" },
  { { GDK_W,            GDK_t,            0, }, "ஷே" },
  { { GDK_Q,            GDK_t,            0, }, "ஸே" },
  { { GDK_slash,        GDK_t,            0, }, "ழே" },
  { { GDK_m,            GDK_t,            0, }, "ரே" },
  { { GDK_n,            GDK_t,            0, }, "லே" },
  { { GDK_b,            GDK_t,            0, }, "ஙே" },
  { { GDK_v,            GDK_t,            0, }, "வே" },
  { { GDK_apostrophe,   GDK_t,            0, }, "யே" },
  { { GDK_semicolon,    GDK_t,            0, }, "நே" },
  { { GDK_l,            GDK_t,            0, }, "தே" },
  { { GDK_k,            GDK_t,            0, }, "மே" },
  { { GDK_j,            GDK_t,            0, }, "பே" },
  { { GDK_h,            GDK_t,            0, }, "கே" },
  { { GDK_bracketright, GDK_t,            0, }, "ஞே" },
  { { GDK_bracketleft,  GDK_t,            0, }, "சே" },
  { { GDK_p,            GDK_t,            0, }, "ணே" },
  { { GDK_o,            GDK_t,            0, }, "டே" },
  { { GDK_i,            GDK_t,            0, }, "னே" },
  { { GDK_u,            GDK_t,            0, }, "றே" },
  { { GDK_y,            GDK_t,            0, }, "ளே" },

  { { GDK_R,            GDK_g,            0, }, "ஹெ" },
  { { GDK_E,            GDK_g,            0, }, "ஜெ" },
  { { GDK_W,            GDK_g,            0, }, "ஷெ" },
  { { GDK_Q,            GDK_g,            0, }, "ஸெ" },
  { { GDK_slash,        GDK_g,            0, }, "ழெ" },
  { { GDK_m,            GDK_g,            0, }, "ரெ" },
  { { GDK_n,            GDK_g,            0, }, "லெ" },
  { { GDK_b,            GDK_g,            0, }, "ஙெ" },
  { { GDK_v,            GDK_g,            0, }, "வெ" },
  { { GDK_apostrophe,   GDK_g,            0, }, "யெ" },
  { { GDK_semicolon,    GDK_g,            0, }, "நெ" },
  { { GDK_l,            GDK_g,            0, }, "தெ" },
  { { GDK_k,            GDK_g,            0, }, "மெ" },
  { { GDK_j,            GDK_g,            0, }, "பெ" },
  { { GDK_h,            GDK_g,            0, }, "கெ" },
  { { GDK_bracketright, GDK_g,            0, }, "ஞெ" },
  { { GDK_bracketleft,  GDK_g,            0, }, "செ" },
  { { GDK_p,            GDK_g,            0, }, "ணெ" },
  { { GDK_o,            GDK_g,            0, }, "டெ" },
  { { GDK_i,            GDK_g,            0, }, "னெ" },
  { { GDK_u,            GDK_g,            0, }, "றெ" },
  { { GDK_y,            GDK_g,            0, }, "ளெ" },

  { { GDK_R,            GDK_r,            0, }, "ஹை" },
  { { GDK_E,            GDK_r,            0, }, "ஜை" },
  { { GDK_W,            GDK_r,            0, }, "ஷை" },
  { { GDK_Q,            GDK_r,            0, }, "ஸை" },
  { { GDK_slash,        GDK_r,            0, }, "ழை" },
  { { GDK_m,            GDK_r,            0, }, "ரை" },
  { { GDK_n,            GDK_r,            0, }, "லை" },
  { { GDK_b,            GDK_r,            0, }, "ஙை" },
  { { GDK_v,            GDK_r,            0, }, "வை" },
  { { GDK_apostrophe,   GDK_r,            0, }, "யை" },
  { { GDK_semicolon,    GDK_r,            0, }, "நை" },
  { { GDK_l,            GDK_r,            0, }, "தை" },
  { { GDK_k,            GDK_r,            0, }, "மை" },
  { { GDK_j,            GDK_r,            0, }, "பை" },
  { { GDK_h,            GDK_r,            0, }, "கை" },
  { { GDK_bracketright, GDK_r,            0, }, "ஞை" },
  { { GDK_bracketleft,  GDK_r,            0, }, "சை" },
  { { GDK_p,            GDK_r,            0, }, "ணை" },
  { { GDK_o,            GDK_r,            0, }, "டை" },
  { { GDK_i,            GDK_r,            0, }, "னை" },
  { { GDK_u,            GDK_r,            0, }, "றை" },
  { { GDK_y,            GDK_r,            0, }, "ளை" },

  { { GDK_R,            GDK_w,            0, }, "ஹீ"  },
  { { GDK_E,            GDK_w,            0, }, "ஜீ"  },
  { { GDK_W,            GDK_w,            0, }, "ஷீ"  },
  { { GDK_Q,            GDK_w,            0, }, "ஸீ"  },
  { { GDK_slash,        GDK_w,            0, }, "ழீ"  },
  { { GDK_m,            GDK_w,            0, }, "ரீ"  },
  { { GDK_n,            GDK_w,            0, }, "லீ"  },
  { { GDK_b,            GDK_w,            0, }, "ஙீ"  },
  { { GDK_v,            GDK_w,            0, }, "வீ"  },
  { { GDK_apostrophe,   GDK_w,            0, }, "யீ"  },
  { { GDK_semicolon,    GDK_w,            0, }, "நீ"  },
  { { GDK_l,            GDK_w,            0, }, "தீ"  },
  { { GDK_k,            GDK_w,            0, }, "மீ"  },
  { { GDK_j,            GDK_w,            0, }, "பீ"  },
  { { GDK_h,            GDK_w,            0, }, "கீ"  },
  { { GDK_bracketright, GDK_w,            0, }, "ஞீ"  },
  { { GDK_bracketleft,  GDK_w,            0, }, "சீ"  },
  { { GDK_p,            GDK_w,            0, }, "ணீ"  },
  { { GDK_o,            GDK_w,            0, }, "டீ"  },
  { { GDK_i,            GDK_w,            0, }, "னீ"  },
  { { GDK_u,            GDK_w,            0, }, "றீ"  },
  { { GDK_y,            GDK_w,            0, }, "ளீ"  },

  { { GDK_R,            GDK_s,            0, }, "ஹி" },
  { { GDK_E,            GDK_s,            0, }, "ஜி" },
  { { GDK_W,            GDK_s,            0, }, "ஷி" },
  { { GDK_Q,            GDK_s,            0, }, "ஸி" },
  { { GDK_slash,        GDK_s,            0, }, "ழி" },
  { { GDK_m,            GDK_s,            0, }, "ரி" },
  { { GDK_n,            GDK_s,            0, }, "லி" },
  { { GDK_b,            GDK_s,            0, }, "ஙி" },
  { { GDK_v,            GDK_s,            0, }, "வி" },
  { { GDK_apostrophe,   GDK_s,            0, }, "யி" },
  { { GDK_semicolon,    GDK_s,            0, }, "நி" },
  { { GDK_l,            GDK_s,            0, }, "தி" },
  { { GDK_k,            GDK_s,            0, }, "மி" },
  { { GDK_j,            GDK_s,            0, }, "பி" },
  { { GDK_h,            GDK_s,            0, }, "கி" },
  { { GDK_bracketright, GDK_s,            0, }, "ஞி" },
  { { GDK_bracketleft,  GDK_s,            0, }, "சி" },
  { { GDK_p,            GDK_s,            0, }, "ணி" },
  { { GDK_o,            GDK_s,            0, }, "டி" },
  { { GDK_i,            GDK_s,            0, }, "னி" },
  { { GDK_u,            GDK_s,            0, }, "றி" },
  { { GDK_y,            GDK_s,            0, }, "ளி" },

  { { GDK_R,            GDK_q,            0, }, "ஹா" },
  { { GDK_E,            GDK_q,            0, }, "ஜா" },
  { { GDK_W,            GDK_q,            0, }, "ஷா" },
  { { GDK_Q,            GDK_q,            0, }, "ஸா" },
  { { GDK_slash,        GDK_q,            0, }, "ழா" },
  { { GDK_m,            GDK_q,            0, }, "ரா" },
  { { GDK_n,            GDK_q,            0, }, "லா" },
  { { GDK_b,            GDK_q,            0, }, "ஙா" },
  { { GDK_v,            GDK_q,            0, }, "வா" },
  { { GDK_apostrophe,   GDK_q,            0, }, "யா" },
  { { GDK_semicolon,    GDK_q,            0, }, "நா" },
  { { GDK_l,            GDK_q,            0, }, "தா" },
  { { GDK_k,            GDK_q,            0, }, "மா" },
  { { GDK_j,            GDK_q,            0, }, "பா" },
  { { GDK_h,            GDK_q,            0, }, "கா" },
  { { GDK_bracketright, GDK_q,            0, }, "ஞா" },
  { { GDK_bracketleft,  GDK_q,            0, }, "சா" },
  { { GDK_p,            GDK_q,            0, }, "ணா" },
  { { GDK_o,            GDK_q,            0, }, "டா" },
  { { GDK_i,            GDK_q,            0, }, "னா" },
  { { GDK_u,            GDK_q,            0, }, "றா" },
  { { GDK_y,            GDK_q,            0, }, "ளா" },

  /* XXX: not sure about this section */
  { { GDK_R,            GDK_a,            0, }, "ஹ"  },
  { { GDK_E,            GDK_a,            0, }, "ஜ"  },
  { { GDK_W,            GDK_a,            0, }, "ஷ"  },
  { { GDK_Q,            GDK_a,            0, }, "ஸ"  },
  { { GDK_slash,        GDK_a,            0, }, "ழ"  },
  { { GDK_m,            GDK_a,            0, }, "ர"  },
  { { GDK_n,            GDK_a,            0, }, "ல"  },
  { { GDK_b,            GDK_a,            0, }, "ங"  },
  { { GDK_v,            GDK_a,            0, }, "வ"  },
  { { GDK_apostrophe,   GDK_a,            0, }, "ய"  },
  { { GDK_semicolon,    GDK_a,            0, }, "ந"  },
  { { GDK_l,            GDK_a,            0, }, "த"  },
  { { GDK_k,            GDK_a,            0, }, "ம"  },
  { { GDK_j,            GDK_a,            0, }, "ப"  },
  { { GDK_h,            GDK_a,            0, }, "க"  },
  { { GDK_bracketright, GDK_a,            0, }, "ஞ"  },
  { { GDK_bracketleft,  GDK_a,            0, }, "ச"  },
  { { GDK_p,            GDK_a,            0, }, "ண"  },
  { { GDK_o,            GDK_a,            0, }, "ட"  },
  { { GDK_i,            GDK_a,            0, }, "ன"  },
  { { GDK_u,            GDK_a,            0, }, "ற"  },
  { { GDK_y,            GDK_a,            0, }, "ள"  },

  { { GDK_R,            GDK_R,            0, }, "ஹ்"  },
  { { GDK_E,            GDK_E,            0, }, "ஜ்"  },
  { { GDK_W,            GDK_W,            0, }, "ஷ்"  },
  { { GDK_Q,            GDK_Q,            0, }, "ஸ்"  },
  { { GDK_slash,        GDK_slash,        0, }, "ழ்"  },
  { { GDK_m,            GDK_m,            0, }, "ர்"  },
  { { GDK_n,            GDK_n,            0, }, "ல்"  },
  { { GDK_b,            GDK_b,            0, }, "ங்"  },
  { { GDK_v,            GDK_v,            0, }, "வ்"  },
  { { GDK_apostrophe,   GDK_apostrophe,   0, }, "ய்"  },
  { { GDK_semicolon,    GDK_semicolon,    0, }, "ந்"  },
  { { GDK_l,            GDK_l,            0, }, "த்"  },
  { { GDK_k,            GDK_k,            0, }, "ம்"  },
  { { GDK_j,            GDK_j,            0, }, "ப்"  },
  { { GDK_h,            GDK_h,            0, }, "க்"  },
  { { GDK_bracketright, GDK_bracketright, 0, }, "ஞ்"  },
  { { GDK_bracketleft,  GDK_bracketleft,  0, }, "ச்"  },
  { { GDK_p,            GDK_p,            0, }, "ண்"  },
  { { GDK_o,            GDK_o,            0, }, "ட்"  },
  { { GDK_i,            GDK_i,            0, }, "ன்"  },
  { { GDK_u,            GDK_u,            0, }, "ற்"  },
  { { GDK_y,            GDK_y,            0, }, "ள்"  },
};


static const guint16 tamil99_compose_ignore[] = 
{
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
  GDK_Mode_switch,
};


static void
clear_compose_buffer ()
{
  memset (compose_buffer, 0, sizeof (compose_buffer));
  n_compose = 0;
}


/* returns the composed string iff keys exactly matches the compose
 * sequence keys */
static ComposeSequence *
find_complete_compose_sequence (guint *keys)
{
  gint i, j;

  for (i = 0;  i < G_N_ELEMENTS (tamil99_compose_seqs);  i++)
    for (j = 0;  j <= TAMIL99_MAX_COMPOSE_LEN;  j++)
      {
        if (keys[j] != tamil99_compose_seqs[i].keys[j])
          break;
        else if (keys[j] == 0 && keys[j] == tamil99_compose_seqs[i].keys[j])
          return tamil99_compose_seqs + i;
      }

  return NULL;
}


/* returns the composed string iff keys is a substring thang of the compose
 * sequence keys */
static ComposeSequence *
find_incomplete_compose_sequence (guint *keys)
{
  gint i, j;

  for (i = 0;  i < G_N_ELEMENTS (tamil99_compose_seqs);  i++)
    for (j = 0;  j <= TAMIL99_MAX_COMPOSE_LEN;  j++)
      {
        if (keys[j] == 0 && tamil99_compose_seqs[i].keys[j] != 0)
          return tamil99_compose_seqs + i;
        else if (keys[j] != tamil99_compose_seqs[i].keys[j])
          break;
      }

  return NULL;
}


static void     
tamil99_get_preedit_string (GtkIMContext   *context,
                            gchar         **str,
                            PangoAttrList **attrs,
                            gint           *cursor_pos)
{
  ComposeSequence *comp_seq;
  gchar *string;
  gint len;

  comp_seq = find_complete_compose_sequence (compose_buffer);
  if (comp_seq == NULL)
    string = "";
  else
    string = comp_seq->string;

  *str = g_strdup (string);
  len = strlen (*str);

  if (attrs)
    {
      *attrs = pango_attr_list_new ();

      /* this can cause a pango crash pre-1.2.3 (so we require >= 1.2.3)
       * http://bugzilla.gnome.org/show_bug.cgi?id=104976 */
      if (len != 0)
        {
          PangoAttribute *attr;
          attr = pango_attr_underline_new (PANGO_UNDERLINE_SINGLE);
          attr->start_index = 0;
          attr->end_index = len;
          pango_attr_list_insert (*attrs, attr);
        }
    }

  if (cursor_pos)
    *cursor_pos = len;
}


static void
tamil99_reset (GtkIMContext *context)
{
  clear_compose_buffer ();
  g_signal_emit_by_name (context, "preedit-changed");
}


static gboolean
no_sequence_matches (GtkIMContext *context, 
                     GdkEventKey *event)
{
  gunichar uc;
  gchar buf[7];

  uc = gdk_keyval_to_unicode (event->keyval);
  if (uc != 0)
    {
      buf[g_unichar_to_utf8 (uc, buf)] = '\0';
      g_signal_emit_by_name (context, "commit", buf);
      g_signal_emit_by_name (context, "preedit-changed");

      return TRUE;
    }
  else
    return FALSE;
}


static gboolean
tamil99_filter_keypress (GtkIMContext *context,
                         GdkEventKey  *event)
{
  ComposeSequence *comp_seq;
  gint i;

  if (event->type == GDK_KEY_RELEASE)
    return FALSE;

  /* don't filter key events with accelerator modifiers held down */
  if (event->state 
      & (gtk_accelerator_get_default_mod_mask () & ~GDK_SHIFT_MASK))
    return FALSE;

  for (i = 0;  i < G_N_ELEMENTS (tamil99_compose_ignore);  i++)
    if (event->keyval == tamil99_compose_ignore[i])
      return FALSE;

  /* '|' commits what we have */
  if (event->keyval == GDK_bar && n_compose > 0) 
    {
      comp_seq = find_complete_compose_sequence (compose_buffer);
      if (comp_seq != NULL)
        {
          g_signal_emit_by_name (context, "commit", comp_seq->string);
          clear_compose_buffer ();
        }

      g_signal_emit_by_name (context, "preedit-changed");
      return TRUE;
    }

  compose_buffer[n_compose] = event->keyval;
  n_compose++;

  if (find_incomplete_compose_sequence (compose_buffer) != NULL)
    {
      g_signal_emit_by_name (context, "preedit-changed");
      return TRUE;
    }

  comp_seq = find_complete_compose_sequence (compose_buffer);
  if (comp_seq != NULL)
    {
      g_signal_emit_by_name (context, "commit", comp_seq->string);
      clear_compose_buffer ();
      g_signal_emit_by_name (context, "preedit-changed");
      return TRUE;
    }

  /* if we reach this point, the sequence *with the key just pressed*
   * cannot be a complete or incomplete match, so: commit the old sequence,
   * then reprocess this key */

  n_compose--;
  compose_buffer[n_compose] = 0;

  if (n_compose > 0)
    {
      comp_seq = find_complete_compose_sequence (compose_buffer);
      g_signal_emit_by_name (context, "commit", comp_seq->string);
      clear_compose_buffer ();
      g_signal_emit_by_name (context, "preedit-changed");

      return tamil99_filter_keypress (context, event);
    }

  return no_sequence_matches (context, event);
}


static void
tamil99_class_init (GtkIMContextClass *clazz)
{
  clazz->filter_keypress = tamil99_filter_keypress;
  clazz->get_preedit_string = tamil99_get_preedit_string;
  clazz->reset = tamil99_reset;
}


void 
im_module_exit ()
{
}


static void
tamil99_init (GtkIMContext *im_context)
{
}


static void
tamil99_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (GtkIMContextClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) tamil99_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (GtkIMContext),
    0,
    (GInstanceInitFunc) tamil99_init,
  };

  type_tamil99 = 
    g_type_module_register_type (module,
                                 GTK_TYPE_IM_CONTEXT,
                                 "GtkIMContextTamil99",
                                 &object_info, 0);
}


static const GtkIMContextInfo tamil99_info = 
{
  "tamil99",       /* ID */
  N_("Tamil99"),   /* Human readable name */
  GETTEXT_PACKAGE, /* Translation domain */
  LOCALEDIR,       /* Dir for bindtextdomain */
  "ta",            /* Languages for which this module is the default */
};


static const GtkIMContextInfo *info_list[] = 
{
  &tamil99_info,
};


void
im_module_init (GTypeModule *module)
{
  tamil99_register_type (module);
}


void 
im_module_list (const GtkIMContextInfo ***contexts, gint *n_contexts)
{
  *contexts = info_list;
  *n_contexts = G_N_ELEMENTS (info_list);
}


GtkIMContext *
im_module_create (const gchar *context_id)
{
  if (strcmp (context_id, "tamil99") == 0)
    return GTK_IM_CONTEXT (g_object_new (type_tamil99, NULL));
  else
    return NULL;
}



