/* $Id: imyiddish-n.c,v 1.4 2003/06/05 06:10:52 nlevitt Exp $ */
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
 * GTK+ Yiddish input module 
 * modeled after Raphael Finkel's Yiddish input module
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <gtk/gtkimcontext.h>
#include <gtk/gtkimmodule.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>

#include <im-extra-intl.h>


GType type_yiddish = 0;

enum { YIDDISH_MAX_COMPOSE_LEN = 3 };

typedef struct
{
  guint keys[YIDDISH_MAX_COMPOSE_LEN + 1];  /* 0-terminated */
  gchar *normal;
  gchar *initial;
  gchar *final;
  gboolean is_terminator;  /* true if a preceding possible final form
                              should be final */
}
ComposeSequence;

/* like GtkIMContextSimple */
static guint compose_buffer[YIDDISH_MAX_COMPOSE_LEN + 1];
static int n_compose = 0;
static ComposeSequence *preceding_possible_final_form = NULL;

static ComposeSequence yiddish_compose_seqs[] = 
{
  { { GDK_a,          0,              0,     0, }, "אַ",   NULL,  NULL, FALSE, },
  { { GDK_A,          0,              0,     0, }, "א",   NULL,  NULL, FALSE, },
  { { GDK_b,          0,              0,     0, }, "ב",   NULL,  NULL, FALSE, },
  { { GDK_B,          0,              0,     0, }, "בֿ",   NULL,  NULL, FALSE, },
  { { GDK_c,          0,              0,     0, }, "צ",   NULL,  "ץ",  FALSE, },
  { { GDK_C,          0,              0,     0, }, "ץ",   NULL,  NULL, FALSE, },
  { { GDK_g,          0,              0,     0, }, "ג",   NULL,  NULL, FALSE, },
  { { GDK_d,          0,              0,     0, }, "ד",   NULL,  NULL, FALSE, },
  { { GDK_h,          0,              0,     0, }, "ה",   NULL,  NULL, FALSE, },
  { { GDK_i,          0,              0,     0, }, "י",   "אי",  NULL, FALSE, },
  { { GDK_I,          0,              0,     0, }, "יִ",   "איִ",  NULL, FALSE, },
  { { GDK_v,          0,              0,     0, }, "װ",   NULL,  NULL, FALSE, },
  { { GDK_z,          0,              0,     0, }, "ז",   NULL,  NULL, FALSE, },
  { { GDK_H,          0,              0,     0, }, "ח",   NULL,  NULL, FALSE, },
  { { GDK_t,          0,              0,     0, }, "ט",   NULL,  NULL, FALSE, },
  { { GDK_T,          0,              0,     0, }, "תּ",   NULL,  NULL, FALSE, },
  { { GDK_y,          0,              0,     0, }, "י",   NULL,  NULL, FALSE, },
  { { GDK_x,          0,              0,     0, }, "כ",   NULL,  "ך",  FALSE, },
  { { GDK_X,          0,              0,     0, }, "ך",   NULL,  NULL, FALSE, },
  { { GDK_l,          0,              0,     0, }, "ל",   NULL,  NULL, FALSE, },
  { { GDK_m,          0,              0,     0, }, "מ",   NULL,  "ם",  FALSE, },
  { { GDK_M,          0,              0,     0, }, "ם",   NULL,  NULL, FALSE, },
  { { GDK_n,          0,              0,     0, }, "נ",   NULL,  "ן",  FALSE, },
  { { GDK_N,          0,              0,     0, }, "ן",   NULL,  NULL, FALSE, },
  { { GDK_s,          0,              0,     0, }, "ס",   NULL,  NULL, FALSE, },
  { { GDK_S,          0,              0,     0, }, "ת",   NULL,  NULL, FALSE, },
  { { GDK_e,          0,              0,     0, }, "ע",   NULL,  NULL, FALSE, },
  { { GDK_E,          0,              0,     0, }, "ײ",   "אײ",  NULL, FALSE, },
  { { GDK_o,          0,              0,     0, }, "אָ",   NULL,  NULL, FALSE, },
  { { GDK_O,          0,              0,     0, }, "ױ",   "אױ",  NULL, FALSE, },
  { { GDK_u,          0,              0,     0, }, "ו",   "או",  NULL, FALSE, },
  { { GDK_U,          0,              0,     0, }, "וּ",   "אוּ",  NULL, FALSE, },
  { { GDK_p,          0,              0,     0, }, "פּ",   NULL,  NULL, FALSE, },
  { { GDK_P,          0,              0,     0, }, "פ",   NULL,  NULL, FALSE, },
  { { GDK_w,          0,              0,     0, }, "ש",   NULL,  NULL, FALSE, },
  { { GDK_W,          0,              0,     0, }, "שׂ",   NULL,  NULL, FALSE, },
  { { GDK_f,          0,              0,     0, }, "פֿ",   NULL,  "ף",  FALSE, },
  { { GDK_F,          0,              0,     0, }, "ף",   NULL,  NULL, FALSE, },
  { { GDK_k,          0,              0,     0, }, "ק",   NULL,  NULL, FALSE, },
  { { GDK_K,          0,              0,     0, }, "כּ",   NULL,  NULL, FALSE, },
  { { GDK_r,          0,              0,     0, }, "ר",   NULL,  NULL, FALSE, },
  { { GDK_Y,          0,              0,     0, }, "ײַ",   "אײַ",  NULL, FALSE, },
  { { GDK_minus,      0,              0,     0, }, "־",   NULL,  NULL, TRUE,  },
  { { GDK_apostrophe, 0,              0,     0, }, "'",   NULL,  NULL, TRUE,  },
  { { GDK_comma,      0,              0,     0, }, ",",   NULL,  NULL, TRUE,  },
  { { GDK_s,          GDK_h,          0,     0, }, "ש",   NULL,  NULL, FALSE, },
  { { GDK_t,          GDK_s,          0,     0, }, "צ",   NULL,  "ץ",  FALSE, },
  { { GDK_t,          GDK_z,          0,     0, }, "צ",   NULL,  "ץ",  FALSE, },
  { { GDK_z,          GDK_h,          0,     0, }, "זש",  NULL,  NULL, FALSE, },
  { { GDK_a,          GDK_y,          0,     0, }, "ײַ",   "אײַ",  NULL, FALSE, },
  { { GDK_o,          GDK_y,          0,     0, }, "ױ",   "אױ",  NULL, FALSE, },
  { { GDK_o,          GDK_i,          0,     0, }, "ױ",   "אױ",  NULL, FALSE, },
  { { GDK_d,          GDK_j,          0,     0, }, "דזש", NULL,  NULL, FALSE, },
  { { GDK_e,          GDK_y,          0,     0, }, "ײ",   "אײ",  NULL, FALSE, },
  { { GDK_apostrophe, GDK_apostrophe, 0,     0, }, "“",   NULL,  NULL, TRUE,  },
  { { GDK_comma,      GDK_comma,      0,     0, }, "„",   NULL,  NULL, TRUE,  },
  { { GDK_k,          GDK_h,          0,     0, }, "כ",   NULL,  "ך",  FALSE, },
  { { GDK_c,          GDK_h,          0,     0, }, "כ",   NULL,  "ך",  FALSE, },
  { { GDK_u,          GDK_v,          0,     0, }, "וּװ",  "אוּװ", NULL, FALSE, },
  { { GDK_u,          GDK_u,          0,     0, }, "וּו",  "אוּו", NULL, FALSE, },
  { { GDK_u,          GDK_i,          0,     0, }, "ויִ",  "אויִ", NULL, FALSE, },
  { { GDK_u,          GDK_y,          0,     0, }, "וּי",  "אוּי", NULL, FALSE, },
  { { GDK_v,          GDK_u,          0,     0, }, "װוּ",  NULL,  NULL, FALSE, },
  { { GDK_y,          GDK_i,          0,     0, }, "ייִ",  NULL,  NULL, FALSE, },
  { { GDK_i,          GDK_i,          0,     0, }, "יִיִ",  "איִיִ", NULL, FALSE, },
  { { GDK_i,          GDK_y,          0,     0, }, "יִי",  "איִי", NULL, FALSE, },
  { { GDK_E,          GDK_i,          0,     0, }, "ײיִ",  "אײיִ", NULL, FALSE, },
  { { GDK_i,          GDK_e,          0,     0, }, "יִע",  "איִע", NULL, FALSE, },
  { { GDK_i,          GDK_a,          0,     0, }, "יִאַ",  "איִאַ", NULL, FALSE, },
  { { GDK_i,          GDK_o,          0,     0, }, "יִאָ",  "איִאָ", NULL, FALSE, },
  { { GDK_t,          GDK_s,          GDK_h, 0, }, "טש",  NULL,  NULL, FALSE, },
};


static const guint16 yiddish_compose_ignore[] = 
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


/* handles null case */
static void
commit_preceding_possible_final_form (GtkIMContext *context, 
                                      gboolean is_final)
{
  if (preceding_possible_final_form != NULL)
    {
      if (is_final) 
        g_signal_emit_by_name (context, "commit", 
                               preceding_possible_final_form->final);
      else
        g_signal_emit_by_name (context, "commit", 
                               preceding_possible_final_form->normal);

      preceding_possible_final_form = NULL;
    }
}


/* returns the composed string iff keys exactly matches the compose
 * sequence keys */
static ComposeSequence *
find_complete_compose_sequence (guint *keys)
{
  gint i, j;

  for (i = 0;  i < G_N_ELEMENTS (yiddish_compose_seqs);  i++)
    for (j = 0;  j <= YIDDISH_MAX_COMPOSE_LEN;  j++)
      {
        if (keys[j] != yiddish_compose_seqs[i].keys[j])
          break;
        else if (keys[j] == 0 && keys[j] == yiddish_compose_seqs[i].keys[j])
          return yiddish_compose_seqs + i;
      }

  return NULL;
}


/* returns the composed string iff keys is a substring thang of the compose
 * sequence keys */
static ComposeSequence *
find_incomplete_compose_sequence (guint *keys)
{
  gint i, j;

  for (i = 0;  i < G_N_ELEMENTS (yiddish_compose_seqs);  i++)
    for (j = 0;  j <= YIDDISH_MAX_COMPOSE_LEN;  j++)
      {
        if (keys[j] == 0 && yiddish_compose_seqs[i].keys[j] != 0)
          return yiddish_compose_seqs + i;
        else if (keys[j] != yiddish_compose_seqs[i].keys[j])
          break;
      }

  return NULL;
}


static gchar *
get_appropriate_string (ComposeSequence *comp_seq, gboolean is_initial)
{
  if (comp_seq == NULL)
    return NULL;
  else if (is_initial && comp_seq->initial != NULL)
    return comp_seq->initial;
  else
    return comp_seq->normal;
}


/* is this a character that could appear in a yiddish word */
static gboolean
is_yiddish_word_character (gunichar uc)
{
  return (((uc >= 0x0590 && uc <= 0x5ff) || (uc >= 0xfb1d && uc <= 0xfb4f))
          && g_unichar_isdefined (uc) && ! g_unichar_ispunct (uc));

}


static gboolean
at_initial_position (GtkIMContext *context)
{
  gchar *text;
  gchar *prevp;
  gint cursor_index;
  gunichar uc;

  if (! gtk_im_context_get_surrounding (context, &text, &cursor_index))
    return FALSE;

  prevp = g_utf8_find_prev_char (text, text + cursor_index);
  if (prevp == NULL)
    return TRUE;

  uc = g_utf8_get_char_validated (prevp, text + cursor_index - prevp);
  g_return_val_if_fail (uc != (gunichar)(-1) && uc != (gunichar)(-2), 
                        FALSE);

  if (is_yiddish_word_character (uc))
    return FALSE;
  else
    return TRUE;
}


static void     
yiddish_get_preedit_string (GtkIMContext   *context,
                            gchar         **str,
                            PangoAttrList **attrs,
                            gint           *cursor_pos)
{
  ComposeSequence *comp_seq;
  gboolean is_initial;
  gchar *string;
  gint len;

  is_initial = at_initial_position (context) 
               && preceding_possible_final_form == NULL;

  comp_seq = find_complete_compose_sequence (compose_buffer);
  if (comp_seq == NULL)
    string = "";
  else
    string = get_appropriate_string (comp_seq, is_initial);

  if (preceding_possible_final_form != NULL)
    *str = g_strdup_printf ("%s%s", preceding_possible_final_form->normal, 
                            string);
  else
    *str = g_strdup (string);

  len = strlen (*str);

  if (attrs)
    {
      *attrs = pango_attr_list_new ();

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
yiddish_reset (GtkIMContext *context)
{
  clear_compose_buffer ();
  preceding_possible_final_form = NULL;
  g_signal_emit_by_name (context, "preedit-changed");
}


static gboolean
no_sequence_matches (GtkIMContext *context, GdkEventKey *event)
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
yiddish_filter_keypress (GtkIMContext *context,
                         GdkEventKey  *event)
{
  ComposeSequence *comp_seq;
  gboolean is_initial;
  gint i;

  if (event->type == GDK_KEY_RELEASE)
    return FALSE;

  /* don't filter key events with accelerator modifiers held down */
  if (event->state 
      & (gtk_accelerator_get_default_mod_mask () & ~GDK_SHIFT_MASK))
    return FALSE;

  for (i = 0;  i < G_N_ELEMENTS (yiddish_compose_ignore);  i++)
    if (event->keyval == yiddish_compose_ignore[i])
      return FALSE;

  is_initial = at_initial_position (context) 
               && preceding_possible_final_form == NULL;

  /* '|' commits what we have */
  if (event->keyval == GDK_bar && (n_compose > 0 
                                   || preceding_possible_final_form != NULL))
    {
      commit_preceding_possible_final_form (context, FALSE); /* non-final */

      comp_seq = find_complete_compose_sequence (compose_buffer);
      if (comp_seq != NULL)
        {
          g_signal_emit_by_name (context, "commit", 
                                 get_appropriate_string (comp_seq, is_initial));
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
      if (comp_seq->final != NULL) /* has a final form, don't commit yet */
        {
          commit_preceding_possible_final_form (context, 
                                                comp_seq->is_terminator);
          clear_compose_buffer ();
          preceding_possible_final_form = comp_seq;
          g_signal_emit_by_name (context, "preedit-changed");
          return TRUE;
        }
      else
        {
          commit_preceding_possible_final_form (context, 
                                                comp_seq->is_terminator);
          g_signal_emit_by_name (context, "commit", 
                                 get_appropriate_string (comp_seq, is_initial));
          clear_compose_buffer ();
          g_signal_emit_by_name (context, "preedit-changed");
          return TRUE;
        }
    }

  /* if we reach this point, the sequence *with the key just pressed*
   * cannot be a complete or incomplete match, so: commit the old sequence,
   * then reprocess this key */

  n_compose--;
  compose_buffer[n_compose] = 0;

  if (n_compose > 0)
    {
      comp_seq = find_complete_compose_sequence (compose_buffer);
      commit_preceding_possible_final_form (context, comp_seq->is_terminator);
      if (comp_seq->final != NULL)
        {
          clear_compose_buffer ();
          preceding_possible_final_form = comp_seq;
        }
      else
        {
          g_signal_emit_by_name (context, "commit", 
                                 get_appropriate_string (comp_seq, is_initial));
          clear_compose_buffer ();
          g_signal_emit_by_name (context, "preedit-changed");
        }

      return yiddish_filter_keypress (context, event);
    }
  else
    commit_preceding_possible_final_form (context, TRUE);

  return no_sequence_matches (context, event);
}


static void
yiddish_class_init (GtkIMContextClass *clazz)
{
  clazz->filter_keypress = yiddish_filter_keypress;
  clazz->get_preedit_string = yiddish_get_preedit_string;
  clazz->reset = yiddish_reset;
}


void 
im_module_exit ()
{
}


static void
yiddish_init (GtkIMContext *im_context)
{
}


static void
yiddish_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (GtkIMContextClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) yiddish_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (GtkIMContext),
    0,
    (GInstanceInitFunc) yiddish_init,
  };

  type_yiddish = 
    g_type_module_register_type (module,
                                 GTK_TYPE_IM_CONTEXT,
                                 "GtkIMContextYiddishNoah",
                                 &object_info, 0);
}


static const GtkIMContextInfo yiddish_info = 
{
  "yiddish-n",     /* ID */
  N_("Yiddish נ"), /* Human readable name */
  GETTEXT_PACKAGE, /* Translation domain */
  LOCALEDIR,       /* Dir for bindtextdomain */
  "yi",            /* Languages for which this module is the default */
};


static const GtkIMContextInfo *info_list[] = 
{
  &yiddish_info,
};


void
im_module_init (GTypeModule *module)
{
  yiddish_register_type (module);
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
  if (strcmp (context_id, "yiddish-n") == 0)
    return GTK_IM_CONTEXT (g_object_new (type_yiddish, NULL));
  else
    return NULL;
}



