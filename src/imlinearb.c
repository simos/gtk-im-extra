/* $Id: imlinearb.c,v 1.1 2006/06/10 11:53:20 mariuslj Exp $
/*
 * Copyright (c) 2004, 2006 Marius L. Jøhndal
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

/* Characters can be input by their lower case alphabetic
 * transcription, e.g. "wa" or "a2". All characters, including unknown
 * symbols and ideograms, can also be input by number, e.g. "54" for
 * "wa", "25" for "a2", "18" for unknown symbol 18, and "100" for
 * ideogram VIR.
 */

typedef struct _GtkComposeTable GtkComposeTable;

static guint16 linearb_compose_seqs[] = {

  GDK_1, 0,     0,     0,     0, 0x0001, 0x0005, /* LINEAR B SYLLABLE B001 DA */
  GDK_1, GDK_0, 0,     0,     0, 0x0001, 0x0004, /* LINEAR B SYLLABLE B010 U */
  GDK_1, GDK_0, GDK_0, 0,     0, 0x0001, 0x0080, /* LINEAR B IDEOGRAM B100 MAN */
  GDK_1, GDK_0, GDK_2, 0,     0, 0x0001, 0x0081, /* LINEAR B IDEOGRAM B102 WOMAN */
  GDK_1, GDK_0, GDK_4, 0,     0, 0x0001, 0x0082, /* LINEAR B IDEOGRAM B104 DEER */
  GDK_1, GDK_0, GDK_5, 0,     0, 0x0001, 0x0083, /* LINEAR B IDEOGRAM B105 EQUID */
  GDK_1, GDK_0, GDK_5, GDK_f, 0, 0x0001, 0x0084, /* LINEAR B IDEOGRAM B105F MARE */
  GDK_1, GDK_0, GDK_5, GDK_m, 0, 0x0001, 0x0085, /* LINEAR B IDEOGRAM B105M STALLION */
  GDK_1, GDK_0, GDK_6, GDK_f, 0, 0x0001, 0x0086, /* LINEAR B IDEOGRAM B106F EWE */
  GDK_1, GDK_0, GDK_6, GDK_m, 0, 0x0001, 0x0087, /* LINEAR B IDEOGRAM B106M RAM */
  GDK_1, GDK_0, GDK_7, GDK_f, 0, 0x0001, 0x0088, /* LINEAR B IDEOGRAM B107F SHE-GOAT */
  GDK_1, GDK_0, GDK_7, GDK_m, 0, 0x0001, 0x0089, /* LINEAR B IDEOGRAM B107M HE-GOAT */
  GDK_1, GDK_0, GDK_8, GDK_f, 0, 0x0001, 0x008A, /* LINEAR B IDEOGRAM B108F SOW */
  GDK_1, GDK_0, GDK_8, GDK_m, 0, 0x0001, 0x008B, /* LINEAR B IDEOGRAM B108M BOAR */
  GDK_1, GDK_0, GDK_9, GDK_f, 0, 0x0001, 0x008C, /* LINEAR B IDEOGRAM B109F COW */
  GDK_1, GDK_0, GDK_9, GDK_m, 0, 0x0001, 0x008D, /* LINEAR B IDEOGRAM B109M BULL */
  GDK_1, GDK_1, 0,     0,     0, 0x0001, 0x0021, /* LINEAR B SYLLABLE B011 PO */
  GDK_1, GDK_2, 0,     0,     0, 0x0001, 0x0030, /* LINEAR B SYLLABLE B012 SO */
  GDK_1, GDK_2, GDK_0, 0,     0, 0x0001, 0x008E, /* LINEAR B IDEOGRAM B120 WHEAT */
  GDK_1, GDK_2, GDK_1, 0,     0, 0x0001, 0x008F, /* LINEAR B IDEOGRAM B121 BARLEY */
  GDK_1, GDK_2, GDK_2, 0,     0, 0x0001, 0x0090, /* LINEAR B IDEOGRAM B122 OLIVE */
  GDK_1, GDK_2, GDK_3, 0,     0, 0x0001, 0x0091, /* LINEAR B IDEOGRAM B123 SPICE */
  GDK_1, GDK_2, GDK_5, 0,     0, 0x0001, 0x0092, /* LINEAR B IDEOGRAM B125 CYPERUS */
  GDK_1, GDK_2, GDK_7, 0,     0, 0x0001, 0x0093, /* LINEAR B MONOGRAM B127 KAPO */
  GDK_1, GDK_2, GDK_8, 0,     0, 0x0001, 0x0094, /* LINEAR B MONOGRAM B128 KANAKO */
  GDK_1, GDK_3, 0,     0,     0, 0x0001, 0x0015, /* LINEAR B SYLLABLE B013 ME */
  GDK_1, GDK_3, GDK_0, 0,     0, 0x0001, 0x0095, /* LINEAR B IDEOGRAM B130 OIL */
  GDK_1, GDK_3, GDK_1, 0,     0, 0x0001, 0x0096, /* LINEAR B IDEOGRAM B131 WINE */
  GDK_1, GDK_3, GDK_2, 0,     0, 0x0001, 0x0097, /* LINEAR B IDEOGRAM B132 */
  GDK_1, GDK_3, GDK_3, 0,     0, 0x0001, 0x0098, /* LINEAR B MONOGRAM B133 AREPA */
  GDK_1, GDK_3, GDK_5, 0,     0, 0x0001, 0x0099, /* LINEAR B MONOGRAM B135 MERI */
  GDK_1, GDK_4, 0,     0,     0, 0x0001, 0x0008, /* LINEAR B SYLLABLE B014 DO */
  GDK_1, GDK_4, GDK_0, 0,     0, 0x0001, 0x009A, /* LINEAR B IDEOGRAM B140 BRONZE */
  GDK_1, GDK_4, GDK_1, 0,     0, 0x0001, 0x009B, /* LINEAR B IDEOGRAM B141 GOLD */
  GDK_1, GDK_4, GDK_2, 0,     0, 0x0001, 0x009C, /* LINEAR B IDEOGRAM B142 */
  GDK_1, GDK_4, GDK_5, 0,     0, 0x0001, 0x009D, /* LINEAR B IDEOGRAM B145 WOOL */
  GDK_1, GDK_4, GDK_6, 0,     0, 0x0001, 0x009E, /* LINEAR B IDEOGRAM B146 */
  GDK_1, GDK_5, 0,     0,     0, 0x0001, 0x0017, /* LINEAR B SYLLABLE B015 MO */
  GDK_1, GDK_5, GDK_0, 0,     0, 0x0001, 0x009F, /* LINEAR B IDEOGRAM B150 */
  GDK_1, GDK_5, GDK_1, 0,     0, 0x0001, 0x00A0, /* LINEAR B IDEOGRAM B151 HORN */
  GDK_1, GDK_5, GDK_2, 0,     0, 0x0001, 0x00A1, /* LINEAR B IDEOGRAM B152 */
  GDK_1, GDK_5, GDK_3, 0,     0, 0x0001, 0x00A2, /* LINEAR B IDEOGRAM B153 */
  GDK_1, GDK_5, GDK_4, 0,     0, 0x0001, 0x00A3, /* LINEAR B IDEOGRAM B154 */
  GDK_1, GDK_5, GDK_5, 0,     0, 0x0001, 0x00DE, /* LINEAR B IDEOGRAM VESSEL B155 */
  GDK_1, GDK_5, GDK_6, 0,     0, 0x0001, 0x00A4, /* LINEAR B MONOGRAM B156 TURO2 */
  GDK_1, GDK_5, GDK_7, 0,     0, 0x0001, 0x00A5, /* LINEAR B IDEOGRAM B157 */
  GDK_1, GDK_5, GDK_8, 0,     0, 0x0001, 0x00A6, /* LINEAR B IDEOGRAM B158 */
  GDK_1, GDK_5, GDK_9, 0,     0, 0x0001, 0x00A7, /* LINEAR B IDEOGRAM B159 CLOTH */
  GDK_1, GDK_6, 0,     0,     0, 0x0001, 0x0023, /* LINEAR B SYLLABLE B016 QA */
  GDK_1, GDK_6, GDK_0, 0,     0, 0x0001, 0x00A8, /* LINEAR B IDEOGRAM B160 */
  GDK_1, GDK_6, GDK_1, 0,     0, 0x0001, 0x00A9, /* LINEAR B IDEOGRAM B161 */
  GDK_1, GDK_6, GDK_2, 0,     0, 0x0001, 0x00AA, /* LINEAR B IDEOGRAM B162 GARMENT */
  GDK_1, GDK_6, GDK_3, 0,     0, 0x0001, 0x00AB, /* LINEAR B IDEOGRAM B163 ARMOUR */
  GDK_1, GDK_6, GDK_4, 0,     0, 0x0001, 0x00AC, /* LINEAR B IDEOGRAM B164 */
  GDK_1, GDK_6, GDK_5, 0,     0, 0x0001, 0x00AD, /* LINEAR B IDEOGRAM B165 */
  GDK_1, GDK_6, GDK_6, 0,     0, 0x0001, 0x00AE, /* LINEAR B IDEOGRAM B166 */
  GDK_1, GDK_6, GDK_7, 0,     0, 0x0001, 0x00AF, /* LINEAR B IDEOGRAM B167 */
  GDK_1, GDK_6, GDK_8, 0,     0, 0x0001, 0x00B0, /* LINEAR B IDEOGRAM B168 */
  GDK_1, GDK_6, GDK_9, 0,     0, 0x0001, 0x00B1, /* LINEAR B IDEOGRAM B169 */
  GDK_1, GDK_7, 0,     0,     0, 0x0001, 0x003C, /* LINEAR B SYLLABLE B017 ZA */
  GDK_1, GDK_7, GDK_0, 0,     0, 0x0001, 0x00B2, /* LINEAR B IDEOGRAM B170 */
  GDK_1, GDK_7, GDK_1, 0,     0, 0x0001, 0x00B3, /* LINEAR B IDEOGRAM B171 */
  GDK_1, GDK_7, GDK_2, 0,     0, 0x0001, 0x00B4, /* LINEAR B IDEOGRAM B172 */
  GDK_1, GDK_7, GDK_3, 0,     0, 0x0001, 0x00B5, /* LINEAR B IDEOGRAM B173 MONTH */
  GDK_1, GDK_7, GDK_4, 0,     0, 0x0001, 0x00B6, /* LINEAR B IDEOGRAM B174 */
  GDK_1, GDK_7, GDK_6, 0,     0, 0x0001, 0x00B7, /* LINEAR B IDEOGRAM B176 TREE */
  GDK_1, GDK_7, GDK_7, 0,     0, 0x0001, 0x00B8, /* LINEAR B IDEOGRAM B177 */
  GDK_1, GDK_7, GDK_8, 0,     0, 0x0001, 0x00B9, /* LINEAR B IDEOGRAM B178 */
  GDK_1, GDK_7, GDK_9, 0,     0, 0x0001, 0x00BA, /* LINEAR B IDEOGRAM B179 */
  GDK_1, GDK_8, 0,     0,     0, 0x0001, 0x0050, /* LINEAR B SYMBOL B018 */
  GDK_1, GDK_8, GDK_0, 0,     0, 0x0001, 0x00BB, /* LINEAR B IDEOGRAM B180 */
  GDK_1, GDK_8, GDK_1, 0,     0, 0x0001, 0x00BC, /* LINEAR B IDEOGRAM B181 */
  GDK_1, GDK_8, GDK_2, 0,     0, 0x0001, 0x00BD, /* LINEAR B IDEOGRAM B182 */
  GDK_1, GDK_8, GDK_3, 0,     0, 0x0001, 0x00BE, /* LINEAR B IDEOGRAM B183 */
  GDK_1, GDK_8, GDK_4, 0,     0, 0x0001, 0x00BF, /* LINEAR B IDEOGRAM B184 */
  GDK_1, GDK_8, GDK_5, 0,     0, 0x0001, 0x00C0, /* LINEAR B IDEOGRAM B185 */
  GDK_1, GDK_8, GDK_9, 0,     0, 0x0001, 0x00C1, /* LINEAR B IDEOGRAM B189 */
  GDK_1, GDK_9, 0,     0,     0, 0x0001, 0x0051, /* LINEAR B SYMBOL B019 */
  GDK_1, GDK_9, GDK_0, 0,     0, 0x0001, 0x00C2, /* LINEAR B IDEOGRAM B190 */
  GDK_1, GDK_9, GDK_1, 0,     0, 0x0001, 0x00C3, /* LINEAR B IDEOGRAM B191 HELMET */

  GDK_2, 0,     0,     0,     0, 0x0001, 0x002B, /* LINEAR B SYLLABLE B002 RO */
  GDK_2, GDK_0, 0,     0,     0, 0x0001, 0x003F, /* LINEAR B SYLLABLE B020 ZO */
  GDK_2, GDK_0, GDK_0, 0,     0, 0x0001, 0x00DF, /* LINEAR B IDEOGRAM VESSEL B200 */
  GDK_2, GDK_0, GDK_1, 0,     0, 0x0001, 0x00E0, /* LINEAR B IDEOGRAM VESSEL B201 */
  GDK_2, GDK_0, GDK_2, 0,     0, 0x0001, 0x00E1, /* LINEAR B IDEOGRAM VESSEL B202 */
  GDK_2, GDK_0, GDK_3, 0,     0, 0x0001, 0x00E2, /* LINEAR B IDEOGRAM VESSEL B203 */
  GDK_2, GDK_0, GDK_4, 0,     0, 0x0001, 0x00E3, /* LINEAR B IDEOGRAM VESSEL B204 */
  GDK_2, GDK_0, GDK_5, 0,     0, 0x0001, 0x00E4, /* LINEAR B IDEOGRAM VESSEL B205 */
  GDK_2, GDK_0, GDK_6, 0,     0, 0x0001, 0x00E5, /* LINEAR B IDEOGRAM VESSEL B206 */
  GDK_2, GDK_0, GDK_7, 0,     0, 0x0001, 0x00E6, /* LINEAR B IDEOGRAM VESSEL B207 */
  GDK_2, GDK_0, GDK_8, 0,     0, 0x0001, 0x00E7, /* LINEAR B IDEOGRAM VESSEL B208 */
  GDK_2, GDK_0, GDK_9, 0,     0, 0x0001, 0x00E8, /* LINEAR B IDEOGRAM VESSEL B209 */
  GDK_2, GDK_1, 0,     0,     0, 0x0001, 0x0025, /* LINEAR B SYLLABLE B021 QI */
  GDK_2, GDK_1, GDK_0, 0,     0, 0x0001, 0x00E9, /* LINEAR B IDEOGRAM VESSEL B210 */
  GDK_2, GDK_1, GDK_1, 0,     0, 0x0001, 0x00EA, /* LINEAR B IDEOGRAM VESSEL B211 */
  GDK_2, GDK_1, GDK_2, 0,     0, 0x0001, 0x00EB, /* LINEAR B IDEOGRAM VESSEL B212 */
  GDK_2, GDK_1, GDK_3, 0,     0, 0x0001, 0x00EC, /* LINEAR B IDEOGRAM VESSEL B213 */
  GDK_2, GDK_1, GDK_4, 0,     0, 0x0001, 0x00ED, /* LINEAR B IDEOGRAM VESSEL B214 */
  GDK_2, GDK_1, GDK_5, 0,     0, 0x0001, 0x00EE, /* LINEAR B IDEOGRAM VESSEL B215 */
  GDK_2, GDK_1, GDK_6, 0,     0, 0x0001, 0x00EF, /* LINEAR B IDEOGRAM VESSEL B216 */
  GDK_2, GDK_1, GDK_7, 0,     0, 0x0001, 0x00F0, /* LINEAR B IDEOGRAM VESSEL B217 */
  GDK_2, GDK_1, GDK_8, 0,     0, 0x0001, 0x00F1, /* LINEAR B IDEOGRAM VESSEL B218 */
  GDK_2, GDK_1, GDK_9, 0,     0, 0x0001, 0x00F2, /* LINEAR B IDEOGRAM VESSEL B219 */
  GDK_2, GDK_2, 0,     0,     0, 0x0001, 0x0052, /* LINEAR B SYMBOL B022 */
  GDK_2, GDK_2, GDK_0, 0,     0, 0x0001, 0x00C4, /* LINEAR B IDEOGRAM B220 FOOTSTOOL */
  GDK_2, GDK_2, GDK_1, 0,     0, 0x0001, 0x00F3, /* LINEAR B IDEOGRAM VESSEL B221 */
  GDK_2, GDK_2, GDK_2, 0,     0, 0x0001, 0x00F4, /* LINEAR B IDEOGRAM VESSEL B222 */
  GDK_2, GDK_2, GDK_5, 0,     0, 0x0001, 0x00C5, /* LINEAR B IDEOGRAM B225 BATHTUB */
  GDK_2, GDK_2, GDK_6, 0,     0, 0x0001, 0x00F5, /* LINEAR B IDEOGRAM VESSEL B226 */
  GDK_2, GDK_2, GDK_7, 0,     0, 0x0001, 0x00F6, /* LINEAR B IDEOGRAM VESSEL B227 */
  GDK_2, GDK_2, GDK_8, 0,     0, 0x0001, 0x00F7, /* LINEAR B IDEOGRAM VESSEL B228 */
  GDK_2, GDK_2, GDK_9, 0,     0, 0x0001, 0x00F8, /* LINEAR B IDEOGRAM VESSEL B229 */
  GDK_2, GDK_3, 0,     0,     0, 0x0001, 0x0018, /* LINEAR B SYLLABLE B023 MU */
  GDK_2, GDK_3, GDK_0, 0,     0, 0x0001, 0x00C6, /* LINEAR B IDEOGRAM B230 SPEAR */
  GDK_2, GDK_3, GDK_1, 0,     0, 0x0001, 0x00C7, /* LINEAR B IDEOGRAM B231 ARROW */
  GDK_2, GDK_3, GDK_2, 0,     0, 0x0001, 0x00C8, /* LINEAR B IDEOGRAM B232 */
  GDK_2, GDK_3, GDK_3, 0,     0, 0x0001, 0x00C9, /* LINEAR B IDEOGRAM B233 SWORD */
  GDK_2, GDK_3, GDK_4, 0,     0, 0x0001, 0x00CA, /* LINEAR B IDEOGRAM B234 */
  GDK_2, GDK_3, GDK_6, 0,     0, 0x0001, 0x00CB, /* LINEAR B IDEOGRAM B236 */
  GDK_2, GDK_4, 0,     0,     0, 0x0001, 0x001A, /* LINEAR B SYLLABLE B024 NE */
  GDK_2, GDK_4, GDK_0, 0,     0, 0x0001, 0x00CC, /* LINEAR B IDEOGRAM B240 WHEELED CHARIOT */
  GDK_2, GDK_4, GDK_1, 0,     0, 0x0001, 0x00CD, /* LINEAR B IDEOGRAM B241 CHARIOT */
  GDK_2, GDK_4, GDK_2, 0,     0, 0x0001, 0x00CE, /* LINEAR B IDEOGRAM B242 CHARIOT FRAME */
  GDK_2, GDK_4, GDK_3, 0,     0, 0x0001, 0x00CF, /* LINEAR B IDEOGRAM B243 WHEEL */
  GDK_2, GDK_4, GDK_5, 0,     0, 0x0001, 0x00D0, /* LINEAR B IDEOGRAM B245 */
  GDK_2, GDK_4, GDK_6, 0,     0, 0x0001, 0x00D1, /* LINEAR B IDEOGRAM B246 */
  GDK_2, GDK_4, GDK_7, 0,     0, 0x0001, 0x00D2, /* LINEAR B MONOGRAM B247 DIPTE */
  GDK_2, GDK_4, GDK_8, 0,     0, 0x0001, 0x00D3, /* LINEAR B IDEOGRAM B248 */
  GDK_2, GDK_4, GDK_9, 0,     0, 0x0001, 0x00D4, /* LINEAR B IDEOGRAM B249 */
  GDK_2, GDK_5, 0,     0,     0, 0x0001, 0x0040, /* LINEAR B SYLLABLE B025 A2 */
  GDK_2, GDK_5, GDK_0, 0,     0, 0x0001, 0x00F9, /* LINEAR B IDEOGRAM VESSEL B250 */
  GDK_2, GDK_5, GDK_1, 0,     0, 0x0001, 0x00D5, /* LINEAR B IDEOGRAM B251 */
  GDK_2, GDK_5, GDK_2, 0,     0, 0x0001, 0x00D6, /* LINEAR B IDEOGRAM B252 */
  GDK_2, GDK_5, GDK_3, 0,     0, 0x0001, 0x00D7, /* LINEAR B IDEOGRAM B253 */
  GDK_2, GDK_5, GDK_4, 0,     0, 0x0001, 0x00D8, /* LINEAR B IDEOGRAM B254 DART */
  GDK_2, GDK_5, GDK_5, 0,     0, 0x0001, 0x00D9, /* LINEAR B IDEOGRAM B255 */
  GDK_2, GDK_5, GDK_6, 0,     0, 0x0001, 0x00DA, /* LINEAR B IDEOGRAM B256 */
  GDK_2, GDK_5, GDK_7, 0,     0, 0x0001, 0x00DB, /* LINEAR B IDEOGRAM B257 */
  GDK_2, GDK_5, GDK_8, 0,     0, 0x0001, 0x00DC, /* LINEAR B IDEOGRAM B258 */
  GDK_2, GDK_5, GDK_9, 0,     0, 0x0001, 0x00DD, /* LINEAR B IDEOGRAM B259 */
  GDK_2, GDK_6, 0,     0,     0, 0x0001, 0x002C, /* LINEAR B SYLLABLE B026 RU */
  GDK_2, GDK_7, 0,     0,     0, 0x0001, 0x0029, /* LINEAR B SYLLABLE B027 RE */
  GDK_2, GDK_8, 0,     0,     0, 0x0001, 0x0002, /* LINEAR B SYLLABLE B028 I */
  GDK_2, GDK_9, 0,     0,     0, 0x0001, 0x0046, /* LINEAR B SYLLABLE B029 PU2 */

  GDK_3, 0,     0,     0,     0, 0x0001, 0x001E, /* LINEAR B SYLLABLE B003 PA */
  GDK_3, GDK_0, 0,     0,     0, 0x0001, 0x001B, /* LINEAR B SYLLABLE B030 NI */
  GDK_3, GDK_0, GDK_5, 0,     0, 0x0001, 0x00FA, /* LINEAR B IDEOGRAM VESSEL B305 */
  GDK_3, GDK_1, 0,     0,     0, 0x0001, 0x002D, /* LINEAR B SYLLABLE B031 SA */
  GDK_3, GDK_2, 0,     0,     0, 0x0001, 0x0026, /* LINEAR B SYLLABLE B032 QO */
  GDK_3, GDK_3, 0,     0,     0, 0x0001, 0x0049, /* LINEAR B SYLLABLE B033 RA3 */
  GDK_3, GDK_4, 0,     0,     0, 0x0001, 0x0053, /* LINEAR B SYMBOL B034 */
  GDK_3, GDK_6, 0,     0,     0, 0x0001, 0x000D, /* LINEAR B SYLLABLE B036 JO */
  GDK_3, GDK_7, 0,     0,     0, 0x0001, 0x0034, /* LINEAR B SYLLABLE B037 TI */
  GDK_3, GDK_8, 0,     0,     0, 0x0001, 0x0001, /* LINEAR B SYLLABLE B038 E */
  GDK_3, GDK_9, 0,     0,     0, 0x0001, 0x0020, /* LINEAR B SYLLABLE B039 PI */

  GDK_4, 0,     0,     0,     0, 0x0001, 0x0033, /* LINEAR B SYLLABLE B004 TE */
  GDK_4, GDK_0, 0,     0,     0, 0x0001, 0x0039, /* LINEAR B SYLLABLE B040 WI */
  GDK_4, GDK_1, 0,     0,     0, 0x0001, 0x002F, /* LINEAR B SYLLABLE B041 SI */
  GDK_4, GDK_2, 0,     0,     0, 0x0001, 0x003A, /* LINEAR B SYLLABLE B042 WO */
  GDK_4, GDK_3, 0,     0,     0, 0x0001, 0x0041, /* LINEAR B SYLLABLE B043 A3 */
  GDK_4, GDK_4, 0,     0,     0, 0x0001, 0x0010, /* LINEAR B SYLLABLE B044 KE */
  GDK_4, GDK_5, 0,     0,     0, 0x0001, 0x0006, /* LINEAR B SYLLABLE B045 DE */
  GDK_4, GDK_6, 0,     0,     0, 0x0001, 0x000B, /* LINEAR B SYLLABLE B046 JE */
  GDK_4, GDK_7, 0,     0,     0, 0x0001, 0x0054, /* LINEAR B SYMBOL B047 */
  GDK_4, GDK_8, 0,     0,     0, 0x0001, 0x0045, /* LINEAR B SYLLABLE B048 NWA */
  GDK_4, GDK_9, 0,     0,     0, 0x0001, 0x0055, /* LINEAR B SYMBOL B049 */

  GDK_5, 0,     0,     0,     0, 0x0001, 0x0035, /* LINEAR B SYLLABLE B005 TO */
  GDK_5, GDK_0, 0,     0,     0, 0x0001, 0x0022, /* LINEAR B SYLLABLE B050 PU */
  GDK_5, GDK_1, 0,     0,     0, 0x0001, 0x0009, /* LINEAR B SYLLABLE B051 DU */
  GDK_5, GDK_2, 0,     0,     0, 0x0001, 0x001C, /* LINEAR B SYLLABLE B052 NO */
  GDK_5, GDK_3, 0,     0,     0, 0x0001, 0x002A, /* LINEAR B SYLLABLE B053 RI */
  GDK_5, GDK_4, 0,     0,     0, 0x0001, 0x0037, /* LINEAR B SYLLABLE B054 WA */
  GDK_5, GDK_5, 0,     0,     0, 0x0001, 0x001D, /* LINEAR B SYLLABLE B055 NU */
  GDK_5, GDK_6, 0,     0,     0, 0x0001, 0x0056, /* LINEAR B SYMBOL B056 */
  GDK_5, GDK_7, 0,     0,     0, 0x0001, 0x000A, /* LINEAR B SYLLABLE B057 JA */
  GDK_5, GDK_8, 0,     0,     0, 0x0001, 0x0031, /* LINEAR B SYLLABLE B058 SU */
  GDK_5, GDK_9, 0,     0,     0, 0x0001, 0x0032, /* LINEAR B SYLLABLE B059 TA */

  GDK_6, 0,     0,     0,     0, 0x0001, 0x0019, /* LINEAR B SYLLABLE B006 NA */
  GDK_6, GDK_0, 0,     0,     0, 0x0001, 0x0028, /* LINEAR B SYLLABLE B060 RA */
  GDK_6, GDK_1, 0,     0,     0, 0x0001, 0x0003, /* LINEAR B SYLLABLE B061 O */
  GDK_6, GDK_2, 0,     0,     0, 0x0001, 0x0047, /* LINEAR B SYLLABLE B062 PTE */
  GDK_6, GDK_3, 0,     0,     0, 0x0001, 0x0057, /* LINEAR B SYMBOL B063 */
  GDK_6, GDK_4, 0,     0,     0, 0x0001, 0x0058, /* LINEAR B SYMBOL B064 */
  GDK_6, GDK_5, 0,     0,     0, 0x0001, 0x000E, /* LINEAR B SYLLABLE B065 JU */
  GDK_6, GDK_6, 0,     0,     0, 0x0001, 0x004B, /* LINEAR B SYLLABLE B066 TA2 */
  GDK_6, GDK_7, 0,     0,     0, 0x0001, 0x0011, /* LINEAR B SYLLABLE B067 KI */
  GDK_6, GDK_8, 0,     0,     0, 0x0001, 0x004A, /* LINEAR B SYLLABLE B068 RO2 */
  GDK_6, GDK_9, 0,     0,     0, 0x0001, 0x0036, /* LINEAR B SYLLABLE B069 TU */

  GDK_7, 0,     0,     0,     0, 0x0001, 0x0007, /* LINEAR B SYLLABLE B007 DI */
  GDK_7, GDK_0, 0,     0,     0, 0x0001, 0x0012, /* LINEAR B SYLLABLE B070 KO */
  GDK_7, GDK_1, 0,     0,     0, 0x0001, 0x0043, /* LINEAR B SYLLABLE B071 DWE */
  GDK_7, GDK_2, 0,     0,     0, 0x0001, 0x001F, /* LINEAR B SYLLABLE B072 PE */
  GDK_7, GDK_3, 0,     0,     0, 0x0001, 0x0016, /* LINEAR B SYLLABLE B073 MI */
  GDK_7, GDK_4, 0,     0,     0, 0x0001, 0x003D, /* LINEAR B SYLLABLE B074 ZE */
  GDK_7, GDK_5, 0,     0,     0, 0x0001, 0x0038, /* LINEAR B SYLLABLE B075 WE */
  GDK_7, GDK_6, 0,     0,     0, 0x0001, 0x0048, /* LINEAR B SYLLABLE B076 RA2 */
  GDK_7, GDK_7, 0,     0,     0, 0x0001, 0x000F, /* LINEAR B SYLLABLE B077 KA */
  GDK_7, GDK_8, 0,     0,     0, 0x0001, 0x0024, /* LINEAR B SYLLABLE B078 QE */
  GDK_7, GDK_9, 0,     0,     0, 0x0001, 0x0059, /* LINEAR B SYMBOL B079 */

  GDK_8, 0,     0,     0,     0, 0x0001, 0x0000, /* LINEAR B SYLLABLE B008 A */
  GDK_8, GDK_0, 0,     0,     0, 0x0001, 0x0014, /* LINEAR B SYLLABLE B080 MA */
  GDK_8, GDK_1, 0,     0,     0, 0x0001, 0x0013, /* LINEAR B SYLLABLE B081 KU */
  GDK_8, GDK_2, 0,     0,     0, 0x0001, 0x005A, /* LINEAR B SYMBOL B082 */
  GDK_8, GDK_3, 0,     0,     0, 0x0001, 0x005B, /* LINEAR B SYMBOL B083 */
  GDK_8, GDK_5, 0,     0,     0, 0x0001, 0x0042, /* LINEAR B SYLLABLE B085 AU */
  GDK_8, GDK_6, 0,     0,     0, 0x0001, 0x005C, /* LINEAR B SYMBOL B086 */
  GDK_8, GDK_7, 0,     0,     0, 0x0001, 0x004C, /* LINEAR B SYLLABLE B087 TWE */
  GDK_8, GDK_9, 0,     0,     0, 0x0001, 0x005D, /* LINEAR B SYMBOL B089 */

  GDK_9, 0,     0,     0,     0, 0x0001, 0x002E, /* LINEAR B SYLLABLE B009 SE */
  GDK_9, GDK_0, 0,     0,     0, 0x0001, 0x0044, /* LINEAR B SYLLABLE B090 DWO */
  GDK_9, GDK_1, 0,     0,     0, 0x0001, 0x004D, /* LINEAR B SYLLABLE B091 TWO */

  GDK_a, 0,     0,     0,     0, 0x0001, 0x0000, /* LINEAR B SYLLABLE B008 A */
  GDK_a, GDK_2, 0,     0,     0, 0x0001, 0x0040, /* LINEAR B SYLLABLE B025 A2 */
  GDK_a, GDK_3, 0,     0,     0, 0x0001, 0x0041, /* LINEAR B SYLLABLE B043 A3 */
  GDK_a, GDK_u, 0,     0,     0, 0x0001, 0x0042, /* LINEAR B SYLLABLE B085 AU */

  GDK_d, GDK_a, 0,     0,     0, 0x0001, 0x0005, /* LINEAR B SYLLABLE B001 DA */
  GDK_d, GDK_e, 0,     0,     0, 0x0001, 0x0006, /* LINEAR B SYLLABLE B045 DE */
  GDK_d, GDK_i, 0,     0,     0, 0x0001, 0x0007, /* LINEAR B SYLLABLE B007 DI */
  GDK_d, GDK_o, 0,     0,     0, 0x0001, 0x0008, /* LINEAR B SYLLABLE B014 DO */
  GDK_d, GDK_u, 0,     0,     0, 0x0001, 0x0009, /* LINEAR B SYLLABLE B051 DU */

  GDK_d, GDK_w, GDK_e, 0,     0, 0x0001, 0x0043, /* LINEAR B SYLLABLE B071 DWE */
  GDK_d, GDK_w, GDK_o, 0,     0, 0x0001, 0x0044, /* LINEAR B SYLLABLE B090 DWO */

  GDK_e, 0,     0,     0,     0, 0x0001, 0x0001, /* LINEAR B SYLLABLE B038 E */

  GDK_i, 0,     0,     0,     0, 0x0001, 0x0002, /* LINEAR B SYLLABLE B028 I */

  GDK_j, GDK_a, 0,     0,     0, 0x0001, 0x000A, /* LINEAR B SYLLABLE B057 JA */
  GDK_j, GDK_e, 0,     0,     0, 0x0001, 0x000B, /* LINEAR B SYLLABLE B046 JE */
  GDK_j, GDK_o, 0,     0,     0, 0x0001, 0x000D, /* LINEAR B SYLLABLE B036 JO */
  GDK_j, GDK_u, 0,     0,     0, 0x0001, 0x000E, /* LINEAR B SYLLABLE B065 JU */

  GDK_k, GDK_a, 0,     0,     0, 0x0001, 0x000F, /* LINEAR B SYLLABLE B077 KA */
  GDK_k, GDK_e, 0,     0,     0, 0x0001, 0x0010, /* LINEAR B SYLLABLE B044 KE */
  GDK_k, GDK_i, 0,     0,     0, 0x0001, 0x0011, /* LINEAR B SYLLABLE B067 KI */
  GDK_k, GDK_o, 0,     0,     0, 0x0001, 0x0012, /* LINEAR B SYLLABLE B070 KO */
  GDK_k, GDK_u, 0,     0,     0, 0x0001, 0x0013, /* LINEAR B SYLLABLE B081 KU */

  GDK_m, GDK_a, 0,     0,     0, 0x0001, 0x0014, /* LINEAR B SYLLABLE B080 MA */
  GDK_m, GDK_e, 0,     0,     0, 0x0001, 0x0015, /* LINEAR B SYLLABLE B013 ME */
  GDK_m, GDK_i, 0,     0,     0, 0x0001, 0x0016, /* LINEAR B SYLLABLE B073 MI */
  GDK_m, GDK_o, 0,     0,     0, 0x0001, 0x0017, /* LINEAR B SYLLABLE B015 MO */
  GDK_m, GDK_u, 0,     0,     0, 0x0001, 0x0018, /* LINEAR B SYLLABLE B023 MU */

  GDK_n, GDK_a, 0,     0,     0, 0x0001, 0x0019, /* LINEAR B SYLLABLE B006 NA */
  GDK_n, GDK_e, 0,     0,     0, 0x0001, 0x001A, /* LINEAR B SYLLABLE B024 NE */
  GDK_n, GDK_i, 0,     0,     0, 0x0001, 0x001B, /* LINEAR B SYLLABLE B030 NI */
  GDK_n, GDK_o, 0,     0,     0, 0x0001, 0x001C, /* LINEAR B SYLLABLE B052 NO */
  GDK_n, GDK_u, 0,     0,     0, 0x0001, 0x001D, /* LINEAR B SYLLABLE B055 NU */

  GDK_n, GDK_w, GDK_a, 0,     0, 0x0001, 0x0045, /* LINEAR B SYLLABLE B048 NWA */

  GDK_o, 0,     0,     0,     0, 0x0001, 0x0003, /* LINEAR B SYLLABLE B061 O */

  GDK_p, GDK_a, 0,     0,     0, 0x0001, 0x001E, /* LINEAR B SYLLABLE B003 PA */
  GDK_p, GDK_e, 0,     0,     0, 0x0001, 0x001F, /* LINEAR B SYLLABLE B072 PE */
  GDK_p, GDK_i, 0,     0,     0, 0x0001, 0x0020, /* LINEAR B SYLLABLE B039 PI */
  GDK_p, GDK_o, 0,     0,     0, 0x0001, 0x0021, /* LINEAR B SYLLABLE B011 PO */
  GDK_p, GDK_t, GDK_e, 0,     0, 0x0001, 0x0047, /* LINEAR B SYLLABLE B062 PTE */
  GDK_p, GDK_u, 0,     0,     0, 0x0001, 0x0022, /* LINEAR B SYLLABLE B050 PU */
  GDK_p, GDK_u, GDK_2, 0,     0, 0x0001, 0x0046, /* LINEAR B SYLLABLE B029 PU2 */

  GDK_q, GDK_a, 0,     0,     0, 0x0001, 0x0023, /* LINEAR B SYLLABLE B016 QA */
  GDK_q, GDK_e, 0,     0,     0, 0x0001, 0x0024, /* LINEAR B SYLLABLE B078 QE */
  GDK_q, GDK_i, 0,     0,     0, 0x0001, 0x0025, /* LINEAR B SYLLABLE B021 QI */
  GDK_q, GDK_o, 0,     0,     0, 0x0001, 0x0026, /* LINEAR B SYLLABLE B032 QO */

  GDK_r, GDK_a, 0,     0,     0, 0x0001, 0x0028, /* LINEAR B SYLLABLE B060 RA */
  GDK_r, GDK_a, GDK_2, 0,     0, 0x0001, 0x0048, /* LINEAR B SYLLABLE B076 RA2 */
  GDK_r, GDK_a, GDK_3, 0,     0, 0x0001, 0x0049, /* LINEAR B SYLLABLE B033 RA3 */
  GDK_r, GDK_e, 0,     0,     0, 0x0001, 0x0029, /* LINEAR B SYLLABLE B027 RE */
  GDK_r, GDK_i, 0,     0,     0, 0x0001, 0x002A, /* LINEAR B SYLLABLE B053 RI */
  GDK_r, GDK_o, 0,     0,     0, 0x0001, 0x002B, /* LINEAR B SYLLABLE B002 RO */
  GDK_r, GDK_o, GDK_2, 0,     0, 0x0001, 0x004A, /* LINEAR B SYLLABLE B068 RO2 */
  GDK_r, GDK_u, 0,     0,     0, 0x0001, 0x002C, /* LINEAR B SYLLABLE B026 RU */

  GDK_s, GDK_a, 0,     0,     0, 0x0001, 0x002D, /* LINEAR B SYLLABLE B031 SA */
  GDK_s, GDK_e, 0,     0,     0, 0x0001, 0x002E, /* LINEAR B SYLLABLE B009 SE */
  GDK_s, GDK_i, 0,     0,     0, 0x0001, 0x002F, /* LINEAR B SYLLABLE B041 SI */
  GDK_s, GDK_o, 0,     0,     0, 0x0001, 0x0030, /* LINEAR B SYLLABLE B012 SO */
  GDK_s, GDK_u, 0,     0,     0, 0x0001, 0x0031, /* LINEAR B SYLLABLE B058 SU */

  GDK_t, GDK_a, 0,     0,     0, 0x0001, 0x0032, /* LINEAR B SYLLABLE B059 TA */
  GDK_t, GDK_a, GDK_2, 0,     0, 0x0001, 0x004B, /* LINEAR B SYLLABLE B066 TA2 */
  GDK_t, GDK_e, 0,     0,     0, 0x0001, 0x0033, /* LINEAR B SYLLABLE B004 TE */
  GDK_t, GDK_i, 0,     0,     0, 0x0001, 0x0034, /* LINEAR B SYLLABLE B037 TI */
  GDK_t, GDK_o, 0,     0,     0, 0x0001, 0x0035, /* LINEAR B SYLLABLE B005 TO */
  GDK_t, GDK_u, 0,     0,     0, 0x0001, 0x0036, /* LINEAR B SYLLABLE B069 TU */

  GDK_t, GDK_w, GDK_e, 0,     0, 0x0001, 0x004C, /* LINEAR B SYLLABLE B087 TWE */
  GDK_t, GDK_w, GDK_o, 0,     0, 0x0001, 0x004D, /* LINEAR B SYLLABLE B091 TWO */

  GDK_u, 0,     0,     0,     0, 0x0001, 0x0004, /* LINEAR B SYLLABLE B010 U */

  GDK_w, GDK_a, 0,     0,     0, 0x0001, 0x0037, /* LINEAR B SYLLABLE B054 WA */
  GDK_w, GDK_e, 0,     0,     0, 0x0001, 0x0038, /* LINEAR B SYLLABLE B075 WE */
  GDK_w, GDK_i, 0,     0,     0, 0x0001, 0x0039, /* LINEAR B SYLLABLE B040 WI */
  GDK_w, GDK_o, 0,     0,     0, 0x0001, 0x003A, /* LINEAR B SYLLABLE B042 WO */

  GDK_z, GDK_a, 0,     0,     0, 0x0001, 0x003C, /* LINEAR B SYLLABLE B017 ZA */
  GDK_z, GDK_e, 0,     0,     0, 0x0001, 0x003D, /* LINEAR B SYLLABLE B074 ZE */
  GDK_z, GDK_o, 0,     0,     0, 0x0001, 0x003F, /* LINEAR B SYLLABLE B020 ZO */
};

static void linearb_class_init         (GtkIMContextSimpleClass  *class);
static void linearb_init               (GtkIMContextSimple       *im_context_simple);

static GType type_linearb = 0;

static const GtkIMContextInfo linearb_info = 
{
  "linearb",             /* ID */
  N_("Linear B"),        /* Human readable name */
  GETTEXT_PACKAGE,       /* Translation domain */
  LOCALEDIR,             /* Dir for bindtextdomain */
  "",                    /* Languages for which this module is the default */  
};

static void
linearb_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (GtkIMContextSimpleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) linearb_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (GtkIMContextSimple),
    0,              /* n_preallocs */
    (GInstanceInitFunc) linearb_init,
  };
  type_linearb = 
    g_type_module_register_type (module,
                                 GTK_TYPE_IM_CONTEXT_SIMPLE,
                                 "GtkIMContextSimpleRunic",
                                 &object_info, 0);
}

static void
linearb_class_init (GtkIMContextSimpleClass *class)
{

}

void 
im_module_exit ()
{

}

static void
linearb_init (GtkIMContextSimple *im_context_simple)
{
  gtk_im_context_simple_add_table (im_context_simple,
				   linearb_compose_seqs,
				   5,
				   G_N_ELEMENTS (linearb_compose_seqs) / 7);
}


static const GtkIMContextInfo *info_list[] = 
{
  &linearb_info,
};


void
im_module_init (GTypeModule *module)
{
  linearb_register_type (module);
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
  if (strcmp (context_id, "linearb") == 0)
    return GTK_IM_CONTEXT_SIMPLE (g_object_new (type_linearb, NULL));
  else
    return NULL;
}
