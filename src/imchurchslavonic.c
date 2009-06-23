/* $Id: imchurchslavonic.c,v 1.1 2004/01/22 20:04:46 mariuslj Exp $ */
/*
 * Copyright (c) 2004 Marius L. Jøhndal
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

static guint16 churchslavonic_compose_seqs[] = {
  GDK_A,               0,               0,         0,     0, 0x0410, /* CYRILLIC CAPITAL LETTER A */
  GDK_B,               0,               0,         0,     0, 0x0411, /* CYRILLIC CAPITAL LETTER BE */
  GDK_C,               0,               0,         0,     0, 0x0426, /* CYRILLIC CAPITAL LETTER TSE */
  GDK_C,               GDK_H,           0,         0,     0, 0x0427, /* CYRILLIC CAPITAL LETTER CHE */
  GDK_C,               GDK_h,           0,         0,     0, 0x0427, /* CYRILLIC CAPITAL LETTER CHE */
  GDK_D,               0,               0,         0,     0, 0x0414, /* CYRILLIC CAPITAL LETTER DE */
  GDK_E,               0,               0,         0,     0, 0x0415, /* CYRILLIC CAPITAL LETTER IE */
  GDK_E,               GDK_H,           0,         0,     0, 0x0466, /* CYRILLIC CAPITAL LETTER LITTLE YUS */
  GDK_E,               GDK_h,           0,         0,     0, 0x0466, /* CYRILLIC CAPITAL LETTER LITTLE YUS */
  GDK_F,               0,               0,         0,     0, 0x0424, /* CYRILLIC CAPITAL LETTER EF */
  GDK_G,               0,               0,         0,     0, 0x0413, /* CYRILLIC CAPITAL LETTER GHE */
  GDK_I,               0,               0,         0,     0, 0x0418, /* CYRILLIC CAPITAL LETTER I */
  GDK_I,               GDK_H,           0,         0,     0, 0x042C, /* CYRILLIC CAPITAL LETTER SOFT SIGN */
  GDK_I,               GDK_I,           0,         0,     0, 0x0406, /* CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I */
  GDK_I,               GDK_h,           0,         0,     0, 0x042C, /* CYRILLIC CAPITAL LETTER SOFT SIGN */
  GDK_I,               GDK_i,           0,         0,     0, 0x0406, /* CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I */
  GDK_J,               GDK_A,           0,         0,     0, 0x042F, /* CYRILLIC CAPITAL LETTER YA */
  GDK_J,               GDK_E,           0,         0,     0, 0x0464, /* CYRILLIC CAPITAL LETTER IOTIFIED E */
  GDK_J,               GDK_E,           GDK_H,     0,     0, 0x0468, /* CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS */
  GDK_J,               GDK_O,           GDK_H,     0,     0, 0x046C, /* CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS */
  GDK_J,               GDK_U,           0,         0,     0, 0x042E, /* CYRILLIC CAPITAL LETTER YU */
  GDK_J,               GDK_a,           0,         0,     0, 0x042F, /* CYRILLIC CAPITAL LETTER YA */
  GDK_J,               GDK_e,           0,         0,     0, 0x0464, /* CYRILLIC CAPITAL LETTER IOTIFIED E */
  GDK_J,               GDK_e,           GDK_h,     0,     0, 0x0468, /* CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS */
  GDK_J,               GDK_o,           GDK_h,     0,     0, 0x046C, /* CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS */
  GDK_J,               GDK_u,           0,         0,     0, 0x042E, /* CYRILLIC CAPITAL LETTER YU */
  GDK_J,               GDK_dead_ogonek, GDK_E,     0,     0, 0x0468, /* CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS */
  GDK_J,               GDK_dead_ogonek, GDK_O,     0,     0, 0x046C, /* CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS */
  GDK_J,               GDK_dead_ogonek, GDK_e,     0,     0, 0x0468, /* CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS */
  GDK_J,               GDK_dead_ogonek, GDK_o,     0,     0, 0x046C, /* CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS */
  GDK_J,               GDK_Multi_key,   GDK_comma, GDK_E, 0, 0x0468, /* CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS */
  GDK_J,               GDK_Multi_key,   GDK_comma, GDK_O, 0, 0x046C, /* CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS */
  GDK_J,               GDK_Multi_key,   GDK_comma, GDK_e, 0, 0x0468, /* CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS */
  GDK_J,               GDK_Multi_key,   GDK_comma, GDK_o, 0, 0x046C, /* CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS */
  GDK_K,               0,               0,         0,     0, 0x041A, /* CYRILLIC CAPITAL LETTER KA */
  GDK_K,               GDK_S,           0,         0,     0, 0x046E, /* CYRILLIC CAPITAL LETTER KSI */
  GDK_K,               GDK_s,           0,         0,     0, 0x046E, /* CYRILLIC CAPITAL LETTER KSI */
  GDK_L,               0,               0,         0,     0, 0x041B, /* CYRILLIC CAPITAL LETTER EL */
  GDK_M,               0,               0,         0,     0, 0x041C, /* CYRILLIC CAPITAL LETTER EM */
  GDK_N,               0,               0,         0,     0, 0x041D, /* CYRILLIC CAPITAL LETTER EN */
  GDK_O,               0,               0,         0,     0, 0x041E, /* CYRILLIC CAPITAL LETTER O */
  GDK_O,               GDK_H,           0,         0,     0, 0x046A, /* CYRILLIC CAPITAL LETTER BIG YUS */
  GDK_O,               GDK_T,           0,         0,     0, 0x047E, /* CYRILLIC CAPITAL LETTER OT */
  GDK_O,               GDK_h,           0,         0,     0, 0x046A, /* CYRILLIC CAPITAL LETTER BIG YUS */
  GDK_O,               GDK_t,           0,         0,     0, 0x047E, /* CYRILLIC CAPITAL LETTER OT */
  GDK_P,               0,               0,         0,     0, 0x041F, /* CYRILLIC CAPITAL LETTER PE */
  GDK_P,               GDK_S,           0,         0,     0, 0x0470, /* CYRILLIC CAPITAL LETTER PSI */
  GDK_P,               GDK_s,           0,         0,     0, 0x0470, /* CYRILLIC CAPITAL LETTER PSI */
  GDK_Q,               0,               0,         0,     0, 0x0462, /* CYRILLIC CAPITAL LETTER YAT */
  GDK_R,               0,               0,         0,     0, 0x0420, /* CYRILLIC CAPITAL LETTER ER */
  GDK_S,               0,               0,         0,     0, 0x0421, /* CYRILLIC CAPITAL LETTER ES */
  GDK_S,               GDK_H,           0,         0,     0, 0x0428, /* CYRILLIC CAPITAL LETTER SHA */
  GDK_S,               GDK_H,           GDK_T,     0,     0, 0x0429, /* CYRILLIC CAPITAL LETTER SHCHA */
  GDK_S,               GDK_h,           0,         0,     0, 0x0428, /* CYRILLIC CAPITAL LETTER SHA */
  GDK_S,               GDK_h,           GDK_t,     0,     0, 0x0429, /* CYRILLIC CAPITAL LETTER SHCHA */
  GDK_T,               0,               0,         0,     0, 0x0422, /* CYRILLIC CAPITAL LETTER TE */
  GDK_T,               GDK_H,           0,         0,     0, 0x0472, /* CYRILLIC CAPITAL LETTER FITA */
  GDK_T,               GDK_h,           0,         0,     0, 0x0472, /* CYRILLIC CAPITAL LETTER FITA */
  GDK_U,               0,               0,         0,     0, 0x0423, /* CYRILLIC CAPITAL LETTER U */
  GDK_U,               GDK_H,           0,         0,     0, 0x042A, /* CYRILLIC CAPITAL LETTER HARD SIGN */
  GDK_U,               GDK_U,           0,         0,     0, 0x0478, /* CYRILLIC CAPITAL LETTER UK */
  GDK_U,               GDK_h,           0,         0,     0, 0x042A, /* CYRILLIC CAPITAL LETTER HARD SIGN */
  GDK_U,               GDK_u,           0,         0,     0, 0x0478, /* CYRILLIC CAPITAL LETTER UK */
  GDK_V,               0,               0,         0,     0, 0x0412, /* CYRILLIC CAPITAL LETTER VE */
  GDK_W,               0,               0,         0,     0, 0x0460, /* CYRILLIC CAPITAL LETTER OMEGA */
  GDK_W,               GDK_T,           0,         0,     0, 0x047E, /* CYRILLIC CAPITAL LETTER OT */
  GDK_W,               GDK_W,           0,         0,     0, 0x047A, /* CYRILLIC CAPITAL LETTER ROUND OMEGA */
  GDK_W,               GDK_t,           0,         0,     0, 0x047E, /* CYRILLIC CAPITAL LETTER OT */
  GDK_W,               GDK_w,           0,         0,     0, 0x047A, /* CYRILLIC CAPITAL LETTER ROUND OMEGA */
  GDK_X,               0,               0,         0,     0, 0x0425, /* CYRILLIC CAPITAL LETTER HA */
  GDK_Y,               0,               0,         0,     0, 0x042B, /* CYRILLIC CAPITAL LETTER YERU */
  GDK_Y,               GDK_H,           0,         0,     0, 0x0474, /* CYRILLIC CAPITAL LETTER IZHITSA */
  GDK_Y,               GDK_h,           0,         0,     0, 0x0474, /* CYRILLIC CAPITAL LETTER IZHITSA */
  GDK_Z,               0,               0,         0,     0, 0x0417, /* CYRILLIC CAPITAL LETTER ZE */
  GDK_Z,               GDK_H,           0,         0,     0, 0x0416, /* CYRILLIC CAPITAL LETTER ZHE */
  GDK_Z,               GDK_Z,           0,         0,     0, 0x0405, /* CYRILLIC CAPITAL LETTER DZE */
  GDK_Z,               GDK_h,           0,         0,     0, 0x0416, /* CYRILLIC CAPITAL LETTER ZHE */
  GDK_Z,               GDK_z,           0,         0,     0, 0x0405, /* CYRILLIC CAPITAL LETTER DZE */

  GDK_a,               0,               0,         0,     0, 0x0430, /* CYRILLIC SMALL LETTER A */
  GDK_b,               0,               0,         0,     0, 0x0431, /* CYRILLIC SMALL LETTER BE */
  GDK_c,               0,               0,         0,     0, 0x0446, /* CYRILLIC SMALL LETTER TSE */
  GDK_c,               GDK_h,           0,         0,     0, 0x0447, /* CYRILLIC SMALL LETTER CHE */
  GDK_d,               0,               0,         0,     0, 0x0434, /* CYRILLIC SMALL LETTER DE */
  GDK_e,               0,               0,         0,     0, 0x0435, /* CYRILLIC SMALL LETTER IE */
  GDK_e,               GDK_e,           0,         0,     0, 0x0454, /* CYRILLIC SMALL LETTER UKRAINIAN IE */
  GDK_e,               GDK_h,           0,         0,     0, 0x0467, /* CYRILLIC SMALL LETTER LITTLE YUS */
  GDK_f,               0,               0,         0,     0, 0x0444, /* CYRILLIC SMALL LETTER EF */
  GDK_g,               0,               0,         0,     0, 0x0433, /* CYRILLIC SMALL LETTER GHE */
  GDK_i,               0,               0,         0,     0, 0x0438, /* CYRILLIC SMALL LETTER I */
  GDK_i,               GDK_i,           0,         0,     0, 0x0456, /* CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I */
  GDK_i,               GDK_h,           0,         0,     0, 0x044C, /* CYRILLIC SMALL LETTER SOFT SIGN */
  GDK_j,               GDK_a,           0,         0,     0, 0x044f, /* CYRILLIC SMALL LETTER YA */
  GDK_j,               GDK_e,           0,         0,     0, 0x0465, /* CYRILLIC SMALL LETTER IOTIFIED E */
  GDK_j,               GDK_e,           GDK_h,     0,     0, 0x0469, /* CYRILLIC SMALL LETTER IOTIFIED LITTLE YUS */
  GDK_j,               GDK_o,           GDK_h,     0,     0, 0x046D, /* CYRILLIC SMALL LETTER IOTIFIED BIG YUS */
  GDK_j,               GDK_u,           0,         0,     0, 0x044E, /* CYRILLIC SMALL LETTER YU */
  GDK_j,               GDK_dead_ogonek, GDK_e,     0,     0, 0x0469, /* CYRILLIC SMALL LETTER IOTIFIED LITTLE YUS */
  GDK_j,               GDK_dead_ogonek, GDK_o,     0,     0, 0x046D, /* CYRILLIC SMALL LETTER IOTIFIED BIG YUS */
  GDK_j,               GDK_Multi_key,   GDK_comma, GDK_e, 0, 0x0469, /* CYRILLIC SMALL LETTER IOTIFIED LITTLE YUS */
  GDK_j,               GDK_Multi_key,   GDK_comma, GDK_o, 0, 0x046D, /* CYRILLIC SMALL LETTER IOTIFIED BIG YUS */
  GDK_k,               0,               0,         0,     0, 0x043A, /* CYRILLIC SMALL LETTER KA */
  GDK_k,               GDK_s,           0,         0,     0, 0x046F, /* CYRILLIC SMALL LETTER KSI */
  GDK_l,               0,               0,         0,     0, 0x043B, /* CYRILLIC SMALL LETTER EL */
  GDK_m,               0,               0,         0,     0, 0x043C, /* CYRILLIC SMALL LETTER EM */
  GDK_n,               0,               0,         0,     0, 0x043D, /* CYRILLIC SMALL LETTER EN */
  GDK_o,               0,               0,         0,     0, 0x043E, /* CYRILLIC SMALL LETTER O */
  GDK_o,               GDK_h,           0,         0,     0, 0x046B, /* CYRILLIC SMALL LETTER BIG YUS */
  GDK_o,               GDK_t,           0,         0,     0, 0x047F, /* CYRILLIC SMALL LETTER OT */
  GDK_p,               0,               0,         0,     0, 0x043F, /* CYRILLIC SMALL LETTER PE */
  GDK_p,               GDK_s,           0,         0,     0, 0x0471, /* CYRILLIC SMALL LETTER PSI */
  GDK_q,               0,               0,         0,     0, 0x0463, /* CYRILLIC SMALL LETTER YAT */
  GDK_r,               0,               0,         0,     0, 0x0440, /* CYRILLIC SMALL LETTER ER */
  GDK_s,               0,               0,         0,     0, 0x0441, /* CYRILLIC SMALL LETTER ES */
  GDK_s,               GDK_h,           0,         0,     0, 0x0448, /* CYRILLIC SMALL LETTER SHA */
  GDK_s,               GDK_h,           GDK_t,     0,     0, 0x0449, /* CYRILLIC SMALL LETTER SHCHA */
  GDK_t,               0,               0,         0,     0, 0x0442, /* CYRILLIC SMALL LETTER TE */
  GDK_t,               GDK_h,           0,         0,     0, 0x0473, /* CYRILLIC SMALL LETTER FITA */
  GDK_u,               0,               0,         0,     0, 0x0443, /* CYRILLIC SMALL LETTER U */
  GDK_u,               GDK_h,           0,         0,     0, 0x044A, /* CYRILLIC SMALL LETTER HARD SIGN */
  GDK_u,               GDK_u,           0,         0,     0, 0x0479, /* CYRILLIC SMALL LETTER UK */
  GDK_v,               0,               0,         0,     0, 0x0432, /* CYRILLIC SMALL LETTER VE */
  GDK_w,               0,               0,         0,     0, 0x0461, /* CYRILLIC SMALL LETTER OMEGA */
  GDK_w,               GDK_t,           0,         0,     0, 0x047F, /* CYRILLIC SMALL LETTER OT */
  GDK_w,               GDK_w,           0,         0,     0, 0x047B, /* CYRILLIC SMALL LETTER ROUND OMEGA */
  GDK_x,               0,               0,         0,     0, 0x0445, /* CYRILLIC SMALL LETTER HA */
  GDK_y,               0,               0,         0,     0, 0x044B, /* CYRILLIC SMALL LETTER YERU */
  GDK_y,               GDK_h,           0,         0,     0, 0x0475, /* CYRILLIC SMALL LETTER IZHITSA */
  GDK_z,               0,               0,         0,     0, 0x0437, /* CYRILLIC SMALL LETTER ZE */
  GDK_z,               GDK_h,           0,         0,     0, 0x0436, /* CYRILLIC SMALL LETTER ZHE */
  GDK_z,               GDK_z,           0,         0,     0, 0x0455, /* CYRILLIC SMALL LETTER DZE */

  GDK_dead_breve,      GDK_I,           0,         0,     0, 0x042C, /* CYRILLIC CAPITAL LETTER SOFT SIGN */
  GDK_dead_breve,      GDK_U,           0,         0,     0, 0x042A, /* CYRILLIC CAPITAL LETTER HARD SIGN */
  GDK_dead_breve,      GDK_i,           0,         0,     0, 0x044C, /* CYRILLIC SMALL LETTER SOFT SIGN */
  GDK_dead_breve,      GDK_u,           0,         0,     0, 0x044A, /* CYRILLIC SMALL LETTER HARD SIGN */
  GDK_dead_diaeresis,  GDK_Y,           0,         0,     0, 0x0474, /* CYRILLIC CAPITAL LETTER IZHITSA */
  GDK_dead_diaeresis,  GDK_i,           0,         0,     0, 0x0457, /* CYRILLIC SMALL LETTER YI */
  GDK_dead_diaeresis,  GDK_y,           0,         0,     0, 0x0475, /* CYRILLIC SMALL LETTER IZHITSA */
  GDK_dead_caron,      GDK_E,           0,         0,     0, 0x0462, /* CYRILLIC CAPITAL LETTER YAT */
  GDK_dead_caron,      GDK_e,           0,         0,     0, 0x0463, /* CYRILLIC SMALL LETTER YAT */
  GDK_dead_ogonek,     GDK_E,           0,         0,     0, 0x0466, /* CYRILLIC CAPITAL LETTER LITTLE YUS */
  GDK_dead_ogonek,     GDK_J,           GDK_E,     0,     0, 0x0468, /* CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS */
  GDK_dead_ogonek,     GDK_J,           GDK_O,     0,     0, 0x046C, /* CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS */
  GDK_dead_ogonek,     GDK_J,           GDK_e,     0,     0, 0x0468, /* CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS */
  GDK_dead_ogonek,     GDK_J,           GDK_o,     0,     0, 0x046C, /* CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS */
  GDK_dead_ogonek,     GDK_O,           0,         0,     0, 0x046A, /* CYRILLIC CAPITAL LETTER BIG YUS */
  GDK_dead_ogonek,     GDK_e,           0,         0,     0, 0x0467, /* CYRILLIC SMALL LETTER LITTLE YUS */
  GDK_dead_ogonek,     GDK_j,           GDK_e,     0,     0, 0x0469, /* CYRILLIC SMALL LETTER IOTIFIED LITTLE YUS */
  GDK_dead_ogonek,     GDK_j,           GDK_o,     0,     0, 0x046D, /* CYRILLIC SMALL LETTER IOTIFIED BIG YUS */
  GDK_dead_ogonek,     GDK_o,           0,         0,     0, 0x046B, /* CYRILLIC SMALL LETTER BIG YUS */

  GDK_Multi_key,       GDK_quotedbl,    GDK_Y,     0,     0, 0x0474, /* CYRILLIC CAPITAL LETTER IZHITSA */
  GDK_Multi_key,       GDK_quotedbl,    GDK_i,     0,     0, 0x0457, /* CYRILLIC SMALL LETTER YI */
  GDK_Multi_key,       GDK_quotedbl,    GDK_y,     0,     0, 0x0475, /* CYRILLIC SMALL LETTER IZHITSA */
  GDK_Multi_key,       GDK_parenleft,   GDK_I,     0,     0, 0x042C, /* CYRILLIC CAPITAL LETTER SOFT SIGN */
  GDK_Multi_key,       GDK_parenleft,   GDK_U,     0,     0, 0x042A, /* CYRILLIC CAPITAL LETTER HARD SIGN */
  GDK_Multi_key,       GDK_parenleft,   GDK_i,     0,     0, 0x044C, /* CYRILLIC SMALL LETTER SOFT SIGN */
  GDK_Multi_key,       GDK_parenleft,   GDK_u,     0,     0, 0x044A, /* CYRILLIC SMALL LETTER HARD SIGN */
  GDK_Multi_key,       GDK_comma,       GDK_E,     0,     0, 0x0466, /* CYRILLIC CAPITAL LETTER LITTLE YUS */
  GDK_Multi_key,       GDK_comma,       GDK_J,     GDK_E, 0, 0x0468, /* CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS */
  GDK_Multi_key,       GDK_comma,       GDK_J,     GDK_O, 0, 0x046C, /* CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS */
  GDK_Multi_key,       GDK_comma,       GDK_J,     GDK_e, 0, 0x0468, /* CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS */
  GDK_Multi_key,       GDK_comma,       GDK_J,     GDK_o, 0, 0x046C, /* CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS */
  GDK_Multi_key,       GDK_comma,       GDK_O,     0,     0, 0x046A, /* CYRILLIC CAPITAL LETTER BIG YUS */
  GDK_Multi_key,       GDK_comma,       GDK_e,     0,     0, 0x0467, /* CYRILLIC SMALL LETTER LITTLE YUS */
  GDK_Multi_key,       GDK_comma,       GDK_j,     GDK_e, 0, 0x0469, /* CYRILLIC SMALL LETTER IOTIFIED LITTLE YUS */
  GDK_Multi_key,       GDK_comma,       GDK_j,     GDK_o, 0, 0x046D, /* CYRILLIC SMALL LETTER IOTIFIED BIG YUS */
  GDK_Multi_key,       GDK_comma,       GDK_o,     0,     0, 0x046B, /* CYRILLIC SMALL LETTER BIG YUS */
  GDK_Multi_key,       GDK_less,        GDK_E,     0,     0, 0x0462, /* CYRILLIC CAPITAL LETTER YAT */
  GDK_Multi_key,       GDK_less,        GDK_e,     0,     0, 0x0463, /* CYRILLIC SMALL LETTER YAT */

  GDK_Multi_key,       GDK_diaeresis,   GDK_Y,     0,     0, 0x0474, /* CYRILLIC CAPITAL LETTER IZHITSA */
  GDK_Multi_key,       GDK_diaeresis,   GDK_i,     0,     0, 0x0457, /* CYRILLIC SMALL LETTER YI */
  GDK_Multi_key,       GDK_diaeresis,   GDK_y,     0,     0, 0x0475, /* CYRILLIC SMALL LETTER IZHITSA */
  GDK_Multi_key,       GDK_breve,       GDK_I,     0,     0, 0x042C, /* CYRILLIC CAPITAL LETTER SOFT SIGN */
  GDK_Multi_key,       GDK_breve,       GDK_U,     0,     0, 0x042A, /* CYRILLIC CAPITAL LETTER HARD SIGN */
  GDK_Multi_key,       GDK_breve,       GDK_i,     0,     0, 0x044C, /* CYRILLIC SMALL LETTER SOFT SIGN */
  GDK_Multi_key,       GDK_breve,       GDK_u,     0,     0, 0x044A, /* CYRILLIC SMALL LETTER HARD SIGN */
  GDK_Multi_key,       GDK_ogonek,      GDK_E,     0,     0, 0x0466, /* CYRILLIC CAPITAL LETTER LITTLE YUS */
  GDK_Multi_key,       GDK_ogonek,      GDK_J,     GDK_E, 0, 0x0468, /* CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS */
  GDK_Multi_key,       GDK_ogonek,      GDK_J,     GDK_O, 0, 0x046C, /* CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS */
  GDK_Multi_key,       GDK_ogonek,      GDK_J,     GDK_e, 0, 0x0468, /* CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS */
  GDK_Multi_key,       GDK_ogonek,      GDK_J,     GDK_o, 0, 0x046C, /* CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS */
  GDK_Multi_key,       GDK_ogonek,      GDK_O,     0,     0, 0x046A, /* CYRILLIC CAPITAL LETTER BIG YUS */
  GDK_Multi_key,       GDK_ogonek,      GDK_e,     0,     0, 0x0467, /* CYRILLIC SMALL LETTER LITTLE YUS */
  GDK_Multi_key,       GDK_ogonek,      GDK_j,     GDK_e, 0, 0x0469, /* CYRILLIC SMALL LETTER IOTIFIED LITTLE YUS */
  GDK_Multi_key,       GDK_ogonek,      GDK_j,     GDK_o, 0, 0x046D, /* CYRILLIC SMALL LETTER IOTIFIED BIG YUS */
  GDK_Multi_key,       GDK_ogonek,      GDK_o,     0,     0, 0x046B, /* CYRILLIC SMALL LETTER BIG YUS */
  GDK_Multi_key,       GDK_caron,       GDK_E,     0,     0, 0x0462, /* CYRILLIC CAPITAL LETTER YAT */
  GDK_Multi_key,       GDK_caron,       GDK_e,     0,     0, 0x0463, /* CYRILLIC SMALL LETTER YAT */
};

static void churchslavonic_class_init         (GtkIMContextSimpleClass  *class);
static void churchslavonic_init               (GtkIMContextSimple       *im_context_simple);

static GType type_churchslavonic = 0;

static const GtkIMContextInfo churchslavonic_info = 
{
  "churchslavonic",      /* ID */
  N_("Church Slavonic"), /* Human readable name */
  GETTEXT_PACKAGE,       /* Translation domain */
  LOCALEDIR,             /* Dir for bindtextdomain */
  "",                    /* Languages for which this module is the default */  
};

static void
churchslavonic_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (GtkIMContextSimpleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) churchslavonic_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (GtkIMContextSimple),
    0,              /* n_preallocs */
    (GInstanceInitFunc) churchslavonic_init,
  };
  type_churchslavonic = 
    g_type_module_register_type (module,
                                 GTK_TYPE_IM_CONTEXT_SIMPLE,
                                 "GtkIMContextSimpleChurchSlavonic",
                                 &object_info, 0);
}

static void
churchslavonic_class_init (GtkIMContextSimpleClass *class)
{

}

void 
im_module_exit ()
{

}

static void
churchslavonic_init (GtkIMContextSimple *im_context_simple)
{
  gtk_im_context_simple_add_table (im_context_simple,
				   churchslavonic_compose_seqs,
				   4,
				   G_N_ELEMENTS (churchslavonic_compose_seqs) / 6);
}


static const GtkIMContextInfo *info_list[] = 
{
  &churchslavonic_info,
};


void
im_module_init (GTypeModule *module)
{
  churchslavonic_register_type (module);
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
  if (strcmp (context_id, "churchslavonic") == 0)
    return GTK_IM_CONTEXT_SIMPLE (g_object_new (type_churchslavonic, NULL));
  else
    return NULL;
}
