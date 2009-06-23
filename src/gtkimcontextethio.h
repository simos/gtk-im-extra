/* GTK - The GIMP Toolkit
 * Copyright (C) 2000 Red Hat Software
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

#ifndef __GTK_IM_CONTEXT_ETHIOPIC_H__
#define __GTK_IM_CONTEXT_ETHIOPIC_H__

#include <gtk/gtkimcontext.h>
#include <gdk/gdkkeysyms.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define GTK_TYPE_IM_CONTEXT_ETHIOPIC             (gtk_im_context_ethiopic_get_type ())
#define GTK_IM_CONTEXT_ETHIOPIC(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_IM_CONTEXT_ETHIOPIC, GtkIMContextEthiopic))
#define GTK_IM_CONTEXT_ETHIOPIC_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_IM_CONTEXT_ETHIOPIC, GtkIMContextEthiopicClass))
#define GTK_IS_IM_CONTEXT_ETHIOPIC(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_IM_CONTEXT_ETHIOPIC))
#define GTK_IS_IM_CONTEXT_ETHIOPIC_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_IM_CONTEXT_ETHIOPIC))
#define GTK_IM_CONTEXT_ETHIOPIC_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_IM_CONTEXT_ETHIOPIC, GtkIMContextEthiopicClass))


typedef struct _GtkIMContextEthiopic       GtkIMContextEthiopic;
typedef struct _GtkIMContextEthiopicClass  GtkIMContextEthiopicClass;

#define GTK_MAX_COMPOSE_LEN 7
#define GTK_MAX_AHAZE_COMPOSE_LEN 37        /* 2 Me'ilfitat cycles with a coefficient is sufficiently insane */
#define GTK_MAX_AHAZE_ASCII_COMPOSE_LEN 25  /* Upto 9999999999999999999999999 => ፱፼፺፱፻፺፱፼፺፱፻፺፱፼፺፱፻፺፱፼፺፱፻፺፱፼፺፱፻፺፱፼፺፱፻፺፱ */

struct _GtkIMContextEthiopic
{
  GtkIMContext object;

  GSList *tables;
  
  guint *compose_buffer;
  gunichar tentative_match;
  gint tentative_match_len;

  guint in_hex_sequence    : 1;
  guint in_ahaze_sequence  : 1;
  guint in_zemene_sequence : 1;

  /* char **circles; perhaps for the future */
};

struct _GtkIMContextEthiopicClass
{
  GtkIMContextClass parent_class;
};

GType         gtk_im_context_ethiopic_get_type  (void) G_GNUC_CONST;
GtkIMContext *gtk_im_context_ethiopic_new       (void);

void          gtk_im_context_ethiopic_add_table (GtkIMContextEthiopic *context_ethiopic,
                                                 guint16            *data,
                                                 gint                max_seq_len,
                                                 gint                n_seqs);
#ifdef __cplusplus
}
#endif /* __cplusplus */


#define ETHIOPIC_NEGUSSE_S   0x1225
#define ETHIOPIC_W           0x12cd
#define ETHIOPIC_ONE         0x1369
#define ETHIOPIC_TEN         (ETHIOPIC_ONE + 9)
#define ETHIOPIC_HUNDRED     (ETHIOPIC_TEN + 9)
#define ETHIOPIC_TENTHOUSAND (ETHIOPIC_HUNDRED + 1)


#define SYL(a,b) \
  a,  0,  0, 0, 0, 0, b+5, \
  a, 'A', 0, 0, 0, 0, b+3, \
  a, 'E', 0, 0, 0, 0, b+4, \
  a, 'I', 0, 0, 0, 0, b+2, \
  a, 'O', 0, 0, 0, 0, b+6, \
  a, 'U', 0, 0, 0, 0, b+1, \
  a, 'a', 0, 0, 0, 0, b+3, \
  a, 'e', 0, 0, 0, 0, b, \
  a, 'e', 'e', 0, 0, 0, b+4, \
  a, 'i', 0, 0, 0, 0, b+2, \
  a, 'i', 'e', 0, 0, 0, b+4, \
  a, 'o', 0, 0, 0, 0, b+6, \
  a, 'u', 0, 0, 0, 0, b+1,

#define SYLW1(a,b) \
  a,  0,  0, 0, 0, 0, b+5, \
  a, 'A', 0, 0, 0, 0, b+3, \
  a, 'E', 0, 0, 0, 0, b+4, \
  a, 'I', 0, 0, 0, 0, b+2, \
  a, 'O', 0, 0, 0, 0, b+6, \
  a, 'U', 0, 0, 0, 0, b+1, \
  a, 'W', 0, 0, 0, 0, b+7, \
  a, 'W', 'A', 0, 0, 0, b+7, \
  a, 'W', 'a', 0, 0, 0, b+7,
#define SYLW2(a,b) \
  a, 'a', 0, 0, 0, 0, b+3, \
  a, 'e', 0, 0, 0, 0, b, \
  a, 'e', 'e', 0, 0, 0, b+4, \
  a, 'i', 0, 0, 0, 0, b+2, \
  a, 'i', 'e', 0, 0, 0, b+4, \
  a, 'o', 0, 0, 0, 0, b+6, \
  a, 'u', 0, 0, 0, 0, b+1, \
  a, 'w', 'w',   0, 0, 0, b+7, \
  a, 'w', 'w', 'a', 0, 0, b+7,

#define SYLW(a,b) \
  SYLW1(a,b)\
  SYLW2(a,b)

#define SYLWW(a,b) \
  a,  0,  0, 0, 0, 0, b+5, \
  a, 'A', 0, 0, 0, 0, b+3, \
  a, 'E', 0, 0, 0, 0, b+4, \
  a, 'I', 0, 0, 0, 0, b+2, \
  a, 'O', 0, 0, 0, 0, b+6, \
  a, 'O', 'O', 0, 0, 0, b+8, \
  a, 'O', 'o', 0, 0, 0, b+8, \
  a, 'U', 0, 0, 0, 0, b+1, \
  a, 'W', 0, 0, 0, 0, b+11, \
  a, 'W', '\'', 0, 0, 0, b+13, \
  a, 'W', 'A', 0, 0, 0, b+11, \
  a, 'W', 'E', 0, 0, 0, b+12, \
  a, 'W', 'I', 0, 0, 0, b+10, \
  a, 'W', 'U', 0, 0, 0, b+13, \
  a, 'W', 'a', 0, 0, 0, b+11, \
  a, 'W', 'e', 0, 0, 0, b+8, \
  a, 'W', 'e', 'e', 0, 0, b+12, \
  a, 'W', 'i', 0, 0, 0, b+10, \
  a, 'W', 'i', 'e', 0, 0, b+12, \
  a, 'W', 'u', 0, 0, 0, b+13, \
  a, 'a', 0, 0, 0, 0, b+3, \
  a, 'e', 0, 0, 0, 0, b, \
  a, 'e', 'e', 0, 0, 0, b+4, \
  a, 'i', 0, 0, 0, 0, b+2, \
  a, 'i', 'e', 0, 0, 0, b+4, \
  a, 'o', 0, 0, 0, 0, b+6, \
  a, 'o', 'o', 0, 0, 0, b+8, \
  a, 'o', 'o', 'o', 0, 0, b+6, /* circle end */\
  a, 'u', 0, 0, 0, 0, b+1, \
  a, 'w', 'w', 0, 0, 0, b+11, \
  a, 'w', 'w', '\'', 0, 0, b+13, \
  a, 'w', 'w', 'E', 0, 0, b+12, \
  a, 'w', 'w', 'a', 0, 0, b+11, \
  a, 'w', 'w', 'e', 0, 0, b+8, \
  a, 'w', 'w', 'e', 'e', 0, b+12, \
  a, 'w', 'w', 'i', 0, 0, b+10, \
  a, 'w', 'w', 'i', 'e', 0, b+12, \
  a, 'w', 'w', 'u', 0, 0, b+13,

#define NUM_PUNCT \
  '\'', '\'', 0, 0, 0, 0, GDK_apostrophe, \
  ',',  0,  0, 0, 0, 0, 0x1363, \
  ',',  ',',  0, 0, 0, 0, ',', \
  ',',',',',', 0, 0, 0, 0x1363, /* circle end */ \
  '-',  0,  0, 0, 0, 0, '-', \
  '-',  ':',  0, 0, 0, 0, 0x1365, \
  ':',  0,  0, 0, 0, 0, 0x1361, \
  ':',  '-',  0, 0, 0, 0, 0x1366, \
  ':',  ':',  0, 0, 0, 0, 0x1362, \
  ':',  ':',  ':', 0, 0, 0, ':', \
  ':',  ':',  ':', ':', 0, 0, 0x1361, /* circle end */ \
  ':',  '|',  ':', 0, 0, 0, 0x1368, \
  ';',  0,  0, 0, 0, 0, 0x1364, \
  ';',  ';',  0, 0, 0, 0, ';', \
  ';',  ';',  ';', 0, 0, 0, 0x1364,  /* circle end */ \
  '<',  0,  0, 0, 0, 0, '<', \
  '<',  '<',  0, 0, 0, 0, 0x00AB, \
  '>',  0,  0, 0, 0, 0, '>', \
  '>',  '>',  0, 0, 0, 0, 0x00BB,

#define ET_LOWER_H \
  'h',  0,  0, 0, 0, 0, 0x1200+5, \
  'h', 'A', 0, 0, 0, 0, 0x1200+3, \
  'h', 'E', 0, 0, 0, 0, 0x1200+4, \
  'h', 'I', 0, 0, 0, 0, 0x1200+2, \
  'h', 'O', 0, 0, 0, 0, 0x1200+6, \
  'h', 'U', 0, 0, 0, 0, 0x1200+1, \
  'h', 'W', 0, 0, 0, 0, 0x1280+11, \
  'h', 'W', '\'', 0, 0, 0, 0x1280+13, \
  'h', 'W', 'A', 0, 0, 0, 0x1280+11, \
  'h', 'W', 'E', 0, 0, 0, 0x1280+12, \
  'h', 'W', 'I', 0, 0, 0, 0x1280+10, \
  'h', 'W', 'U', 0, 0, 0, 0x1280+13, \
  'h', 'W', 'a', 0, 0, 0, 0x1280+11, \
  'h', 'W', 'e', 0, 0, 0, 0x1280+8, \
  'h', 'W', 'e', 'e', 0, 0, 0x1280+12, \
  'h', 'W', 'i', 0, 0, 0, 0x1280+10, \
  'h', 'W', 'i', 'e', 0, 0, 0x1280+12, \
  'h', 'W', 'u', 0, 0, 0, 0x1280+13, \
  'h', 'a', 0, 0, 0, 0, 0x1200+3, \
  'h', 'e', 0, 0, 0, 0, 0x1200, \
  'h', 'e', 'e', 0, 0, 0, 0x1200+4, \
  'h', 'h', 0, 0, 0, 0, 0x1280+5, \
  'h', 'h', 'A', 0, 0, 0, 0x1280+3, \
  'h', 'h', 'E', 0, 0, 0, 0x1280+4, \
  'h', 'h', 'I', 0, 0, 0, 0x1280+2, \
  'h', 'h', 'O', 0, 0, 0, 0x1280+6, \
  'h', 'h', 'O', 'O', 0, 0, 0x1280+8, \
  'h', 'h', 'U', 0, 0, 0, 0x1280+1, \
  'h', 'h', 'W', 0, 0, 0, 0x1280+11, \
  'h', 'h', 'W', '\'', 0, 0, 0x1280+13, \
  'h', 'h', 'W', 'A', 0, 0, 0x1280+11, \
  'h', 'h', 'W', 'E', 0, 0, 0x1280+12, \
  'h', 'h', 'W', 'I', 0, 0, 0x1280+10, \
  'h', 'h', 'W', 'U', 0, 0, 0x1280+13, \
  'h', 'h', 'W', 'a', 0, 0, 0x1280+11, \
  'h', 'h', 'W', 'e', 0, 0, 0x1280+8, \
  'h', 'h', 'W', 'e', 'e', 0, 0x1280+12, \
  'h', 'h', 'W', 'i', 0, 0, 0x1280+10, \
  'h', 'h', 'W', 'u', 0, 0, 0x1280+13, \
  'h', 'h', 'a', 0, 0, 0, 0x1280+3, \
  'h', 'h', 'e', 0, 0, 0, 0x1280, \
  'h', 'h', 'e', 'e', 0, 0, 0x1280+4, \
  'h', 'h', 'i', 0, 0, 0, 0x1280+2, \
  'h', 'h', 'i', 'e', 0, 0, 0x1280+4, \
  'h', 'h', 'o', 0, 0, 0, 0x1280+6, \
  'h', 'h', 'o', 'o', 0, 0, 0x1280+8, \
  'h', 'h', 'o', 'o', 'o', 0, 0x1280+6, /* circle end */ \
  'h', 'h', 'u', 0, 0, 0, 0x1280+1, \
  'h', 'h', 'w', 'w',   0, 0, 0x1280+11, \
  'h', 'h', 'w', 'w', 'a', 0, 0x1280+11, \
  'h', 'h', 'w', 'w', 0, 0, 0x1280+11, \
  'h', 'h', 'w', 'w', '\'', 0, 0x1280+13, \
  'h', 'h', 'w', 'w', 'E', 0, 0x1280+12, \
  'h', 'h', 'w', 'w', 'a', 0, 0x1280+11, \
  'h', 'h', 'w', 'w', 'e', 0, 0x1280+8, \
  'h', 'h', 'w', 'w', 'e', 'e', 0x1280+12,  /* this used to break */ \
  'h', 'h', 'w', 'w', 'i', 0, 0x1280+10, \
  'h', 'h', 'w', 'w', 'u', 0, 0x1280+13, \
  'h', 'i', 0, 0, 0, 0, 0x1200+2, \
  'h', 'i', 'e', 0, 0, 0, 0x1200+4, \
  'h', 'o', 0, 0, 0, 0, 0x1200+6, \
  'h', 'u', 0, 0, 0, 0, 0x1200+1, \
  'h', 'w', 'w',   0, 0, 0, 0x1280+11, \
  'h', 'w', 'w', 'a', 0, 0, 0x1280+11, \
  'h', 'w', 'w', 0, 0, 0, 0x1280+11, \
  'h', 'w', 'w', '\'', 0, 0, 0x1280+13, \
  'h', 'w', 'w', 'E', 0, 0, 0x1280+12, \
  'h', 'w', 'w', 'a', 0, 0, 0x1280+11, \
  'h', 'w', 'w', 'e', 0, 0, 0x1280+8, \
  'h', 'w', 'w', 'e', 'e', 0, 0x1280+12, \
  'h', 'w', 'w', 'i', 0, 0, 0x1280+10, \
  'h', 'w', 'w', 'u', 0, 0, 0x1280+13,

#define ET_LOWER_S \
  's',  0,  0, 0, 0, 0, 0x1230+5, \
  's', 'A', 0, 0, 0, 0, 0x1230+3, \
  's', 'E', 0, 0, 0, 0, 0x1230+4, \
  's', 'I', 0, 0, 0, 0, 0x1230+2, \
  's', 'O', 0, 0, 0, 0, 0x1230+6, \
  's', 'U', 0, 0, 0, 0, 0x1230+1, \
  's', 'W', 0, 0, 0, 0, 0x1230+7, \
  's', 'W', 'A', 0, 0, 0, 0x1230+7, \
  's', 'W', 'a', 0, 0, 0, 0x1230+7, \
  's', 'a', 0, 0, 0, 0, 0x1230+3, \
  's', 'e', 0, 0, 0, 0, 0x1230, \
  's', 'e', 'e', 0, 0, 0, 0x1230+4, \
  's', 'i', 0, 0, 0, 0, 0x1230+2, \
  's', 'i', 'e', 0, 0, 0, 0x1230+4, \
  's', 'o', 0, 0, 0, 0, 0x1230+6, \
  's', 's', 0, 0, 0, 0, 0x1220+5, \
  's', 's', 'A', 0, 0, 0, 0x1220+3, \
  's', 's', 'E', 0, 0, 0, 0x1220+4, \
  's', 's', 'I', 0, 0, 0, 0x1220+2, \
  's', 's', 'O', 0, 0, 0, 0x1220+6, \
  's', 's', 'U', 0, 0, 0, 0x1220+1, \
  's', 's', 'W', 0, 0, 0, 0x1220+7, \
  's', 's', 'W', 'A', 0, 0, 0x1220+7, \
  's', 's', 'W', 'a', 0, 0, 0x1220+7, \
  's', 's', 'a', 0, 0, 0, 0x1220+3, \
  's', 's', 'e', 0, 0, 0, 0x1220, \
  's', 's', 'e', 'e', 0, 0, 0x1220+4, \
  's', 's', 'i', 0, 0, 0, 0x1220+2, \
  's', 's', 'i', 'e', 0, 0, 0x1220+4, \
  's', 's', 'o', 0, 0, 0, 0x1220+6, \
  's', 's', 'u', 0, 0, 0, 0x1220+1, \
  's', 's', 'w', 'w', 0, 0, 0x1220+7, \
  's', 's', 'w', 'w', 'a', 0, 0x1220+7, \
  's', 'u', 0, 0, 0, 0, 0x1230+1, \
  's', 'w', 'w',   0, 0, 0, 0x1230+7, \
  's', 'w', 'w', 'a', 0, 0, 0x1230+7,

#define UPPER_S \
  'S',  0,  0, 0, 0, 0, 0x1338+5, \
  'S', 'A', 0, 0, 0, 0, 0x1338+3, \
  'S', 'E', 0, 0, 0, 0, 0x1338+4, \
  'S', 'I', 0, 0, 0, 0, 0x1338+2, \
  'S', 'O', 0, 0, 0, 0, 0x1338+6, \
  'S', 'S', 0, 0, 0, 0, 0x1340+5, \
  'S', 'S', 'A', 0, 0, 0, 0x1340+3, \
  'S', 'S', 'E', 0, 0, 0, 0x1340+4, \
  'S', 'S', 'I', 0, 0, 0, 0x1340+2, \
  'S', 'S', 'O', 0, 0, 0, 0x1340+6, \
  'S', 'S', 'U', 0, 0, 0, 0x1340+1, \
  'S', 'S', 'a', 0, 0, 0, 0x1340+3, \
  'S', 'S', 'e', 0, 0, 0, 0x1340, \
  'S', 'S', 'e', 'e', 0, 0, 0x1340+4, \
  'S', 'S', 'i', 0, 0, 0, 0x1340+2, \
  'S', 'S', 'i', 'e', 0, 0, 0x1340+4, \
  'S', 'S', 'o', 0, 0, 0, 0x1340+6, \
  'S', 'S', 'u', 0, 0, 0, 0x1340+1, \
  'S', 'U', 0, 0, 0, 0, 0x1338+1, \
  'S', 'W', 0, 0, 0, 0, 0x1338+7, \
  'S', 'W', 'A', 0, 0, 0, 0x1338+7, \
  'S', 'W', 'a', 0, 0, 0, 0x1338+7, \
  'S', 'a', 0, 0, 0, 0, 0x1338+3, \
  'S', 'e', 0, 0, 0, 0, 0x1338, \
  'S', 'e', 'e', 0, 0, 0, 0x1338+4, \
  'S', 'i', 0, 0, 0, 0, 0x1338+2, \
  'S', 'i', 'e', 0, 0, 0, 0x1338+4, \
  'S', 'o', 0, 0, 0, 0, 0x1338+6, \
  'S', 'u', 0, 0, 0, 0, 0x1338+1, \
  'S', 'w', 'w',   0, 0, 0, 0x1338+7, \
  'S', 'w', 'w', 'a', 0, 0, 0x1338+7,

#endif /* __GTK_IM_CONTEXT_ETHIOPIC_H__ */
