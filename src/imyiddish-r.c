/* $Id: imyiddish-r.c,v 1.4 2003/09/02 23:26:36 nlevitt Exp $ */
/* 
 * Copyright (C) 2000 Red Hat Software
 * Copyright (C) 2000 SuSE Linux Ltd
 * Copyright (C) 2003 Raphael Finkel
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
 *
 * Original author: Owen Taylor <otaylor@redhat.com>
 * 
 * Modified for Yiddish: Raphael Finkel <raphael@cs.uky.edu>
 *
 */

/* Yiddish conversion routines
 * Raphael Finkel 11/2002
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <gtk/gtkimcontext.h>
#include <gtk/gtkimmodule.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include <ctype.h>

#include <im-extra-intl.h>


GType type_yiddish_translit = 0;

static void yiddish_class_init (GtkIMContextClass *class);
static void yiddish_init (GtkIMContext *im_context);


typedef int bool;
char *nonFinalConversion[128], *conversion[128];
bool twoLetterConversion[128];
  /* true if first letter of two-letter conversion */
bool startVowel[128];
  /* true if this vowel needs an alef in front */

void
initializeConversion()
{
  int index;
  for (index = 0; index < 128; index++)
    {
      /* By default, every character stands for itself. */
      conversion[index] = (char *)malloc(2);
      *(conversion[index]) = index;
      *(conversion[index]+1) = 0;
    }
  conversion['a'] = "אַ";
  conversion['A'] = "א";
  conversion['b'] = "ב";
  conversion['B'] = "בֿ";
  conversion['c'] = "ץ";
  conversion['C'] = "ץ";
  conversion['g'] = "ג";
  conversion['d'] = "ד";
  conversion['h'] = "ה";
  conversion['i'] = "י";
  conversion['I'] = "יִ";
  conversion['v'] = "װ";
  conversion['z'] = "ז";
  conversion['H'] = "ח";
  conversion['t'] = "ט";
  conversion['T'] = "תּ";
  conversion['y'] = "י";
  conversion['x'] = "ך";
  conversion['X'] = "ך";
  conversion['l'] = "ל";
  conversion['m'] = "ם";
  conversion['M'] = "ם";
  conversion['n'] = "ן";
  conversion['N'] = "ן";
  conversion['s'] = "ס";
  conversion['S'] = "ת";
  conversion['e'] = "ע";
  conversion['E'] = "ײ";
  conversion['o'] = "אָ";
  conversion['O'] = "ױ";
  conversion['u'] = "ו";
  conversion['U'] = "וּ";
  conversion['p'] = "פּ";
  conversion['P'] = "פ";
  conversion['w'] = "ש";
  conversion['W'] = "שׂ";
  conversion['f'] = "ף";
  conversion['F'] = "ף";
  conversion['k'] = "ק";
  conversion['K'] = "כּ";
  conversion['r'] = "ר";
  conversion['Y'] = "ײַ";
  conversion['-'] = "־";
  conversion['|'] = "";
  for (index = 0; index <= 128; index++) {
    nonFinalConversion[index] = 0;
    startVowel[index] = 0;
  }
  nonFinalConversion['m'] = "מ";
  nonFinalConversion['n'] = "נ";
  nonFinalConversion['c'] = "צ";
  nonFinalConversion['f'] = "פֿ";
  nonFinalConversion['x'] = "כ";
  twoLetterConversion['s'] = 1; /* sh */
  twoLetterConversion['t'] = 1; /* ts */
  twoLetterConversion['z'] = 1; /* zh */
  twoLetterConversion['a'] = 1; /* ay */
  twoLetterConversion['e'] = 1; /* ey, ei */
  twoLetterConversion['o'] = 1; /* oy */
  twoLetterConversion['d'] = 1; /* dj */
  twoLetterConversion['k'] = 1; /* kh */
  twoLetterConversion['v'] = 1; /* vu */
  twoLetterConversion['u'] = 1; /* uv, ui, uu, uy, uvu */
  twoLetterConversion['y'] = 1; /* yi */
  twoLetterConversion['i'] = 1; /* ii, iy, ie, ia */
  twoLetterConversion['\''] = 1; /* '' */
  twoLetterConversion[','] = 1; /* '' */
  /* vowels that need an aleph before them at start. */
  startVowel['i'] = 1;
  startVowel['u'] = 1;
  startVowel['O'] = 1;
  startVowel['E'] = 1;
} /* initializeConversion */

#define advance(pprev, prev) \
  if (pprev) pprevChar = pprev; else pprevChar = prevChar; \
  if (prev) prevChar = prev; else prevChar = thisChar; \
  /* fprintf(stderr, "state after %d\n", State); */ \
  return

int thisChar, prevChar, pprevChar;
enum {initial, seenFinal, seenDigraphStart, seenTS} State = initial;

void enqueue(char *string); /* forward */

void
keypressConvert(unsigned int thisChar)
{
  /* finite-state machine, programmed as a case statement
   * calls enqueue() as many times as needed
   * advance(0, 0);
   * DEBUG(fprintf(stdout, "input: %c\r\n", thisChar));
   */
  while (1)
    { /* keep playing with this character until we advance to next */
      /* fprintf(stderr, "state before %d\n", State); */
      switch (State)
        {
        case initial:
          /* DEBUG(fprintf(stdout, "state: initial\r\n")); */
          if (startVowel[thisChar] && !isalpha(prevChar) &&
              prevChar != '\b')
            enqueue("א");
          if (nonFinalConversion[thisChar])
            {
              State = seenFinal;
            }
          else if (twoLetterConversion[thisChar])
            {
              State = seenDigraphStart;
            }
          enqueue(conversion[thisChar]);
          advance(0,0);
          break;
        case seenFinal:
          /* fprintf(stdout, "state: seenFinal\r\n"); */
          if (isalpha(thisChar) || thisChar == '|')
            {
              /* need to fix final to nonfinal. */
              enqueue("\b");
              enqueue(nonFinalConversion[prevChar]);
              State = initial;
            }
          else
            { /* our decision to use final form was ok. */
              State = initial;
            }
          break;
        case seenDigraphStart:
          /* DEBUG(fprintf(stdout, "state: seenDigraphStart\r\n")); */
          State = initial;
          if (prevChar == 's' && thisChar == 'h')
            {
              enqueue("\bש");
              advance(pprevChar,'S');
            }
          else if (prevChar == 't' && (thisChar == 's' || thisChar == 'z'))
            {
            enqueue("\bץ");
            State = seenTS;
            advance(pprevChar, 'c');
            }
          else if (prevChar == 'z' && thisChar == 'h')
            {
              enqueue("\bזש");
              advance(pprevChar, 'D');
            }
          else if (prevChar == 'a' && thisChar == 'y')
            {
              enqueue("\b\b"); /* delcombine mode on */
            if (!isalpha(pprevChar) && pprevChar != '\b')
                enqueue("א");
            enqueue("ײַ");
            advance(pprevChar, 'I');
            }
          else if (prevChar == 'o' && (thisChar == 'y' || thisChar == 'i'))
            {
              enqueue("\b\b"); /* delcombine mode on */
              if (!isalpha(pprevChar) && pprevChar != '\b')
                  enqueue("א");
              enqueue("ױ");
              advance(pprevChar, 'O');
            }
          else if (prevChar == 'd' && thisChar == 'j')
            {
              enqueue("זש");
              advance(pprevChar, 'Z');
            }
          else if (prevChar == 'e' && thisChar == 'y')
            {
              enqueue("\b");
              if (!isalpha(pprevChar) && pprevChar != '\b')
                  enqueue("א");
              enqueue("ײ");
              State = seenDigraphStart;
              advance(pprevChar, 'E');
              }
          else if (prevChar == '\'' && thisChar == '\'')
            {
              enqueue("\b“");
              advance(pprevChar, '"');
            }
          else if (prevChar == ',' && thisChar == ',')
            {
              enqueue("\b„");
              advance(pprevChar, '"');
            }
          else if (prevChar == 'k' && thisChar == 'h')
            {
              enqueue("\bך");
              State = seenFinal;
              advance(pprevChar, 'x');
            }
          else if (prevChar == 'u' && thisChar == 'v')
            {
              enqueue("\bוּװ");
              State = seenDigraphStart;
              advance(pprevChar, 'v');
            }
          else if (prevChar == 'u' && thisChar == 'u')
            {
              enqueue("\bוּו");
              State = initial;
              advance(pprevChar, 'u');
            }
          else if (prevChar == 'u' && thisChar == 'i')
            {
              enqueue("יִ");
              State = initial;
              advance(pprevChar, 'i');
            }
          else if (prevChar == 'u' && thisChar == 'y')
            {
              enqueue("\bוּי");
              State = initial;
              advance(pprevChar, 'i');
            }
          else if (prevChar == 'v' && thisChar == 'u')
            {
              enqueue("וּ");
              State = initial;
              advance(pprevChar, 'u');
            }
          else if (prevChar == 'y' && thisChar == 'i')
            {
              enqueue("יִ");
              State = initial;
              advance(pprevChar, 'i');
            }
          else if (prevChar == 'e' && thisChar == 'i')
            {
              enqueue("יִ");
              State = initial;
              advance(pprevChar, 'i');
            }
          else if (prevChar == 'i' && thisChar == 'i')
            {
              enqueue("\bיִיִ");
              State = initial;
              advance(pprevChar, 'i');
            }
          else if (prevChar == 'i' && thisChar == 'y')
            {
              enqueue("\bיִי");
              advance(pprevChar, 'i');
            }
          else if (prevChar == 'E' && thisChar == 'i')
            {
              enqueue("יִ");
              advance(pprevChar, 'i');
            } 
          else if (prevChar == 'i' && thisChar == 'e')
            {
              enqueue("\bיִע");
              State = initial;
              advance(pprevChar, 'e');
            }
          else if (prevChar == 'i' && thisChar == 'a')
            {
              enqueue("\bיִאַ");
              State = initial;
              advance(pprevChar, 'a');
            }
          else if (prevChar == 'i' && thisChar == 'o')
            {
              enqueue("\bיִאָ");
              State = initial;
              advance(pprevChar, 'o');
            } 
          break;
        case seenTS:
          /* fprintf(stderr, "in state seenTS\n"); */
          if (thisChar == 'h')
            {
              enqueue("\bטש");
              State = initial;
              advance('s', 'h');
            }
          else
            {
              State = seenFinal;
            }
          break;
        } /* switch(State) */
    } /* while (1) */
} /* keypressConvert */

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

  type_yiddish_translit = 
    g_type_module_register_type ( module,
                                  GTK_TYPE_IM_CONTEXT,
                                  "GtkIMContextYiddishRaphael",
                                  &object_info, 0);
} /* yiddish_register_type */

GtkIMContext *myContext;

void
enqueue(char *string)
{
  /* fprintf(stderr, "enqueueing [%s]\n", string); */
  while (*string == '\b')
    {
      gtk_im_context_delete_surrounding(myContext, -1, 1);
      string += 1;
    }
  if (*string == 0)
    {
      return;
    }
  else if (*string == '\r')
    {
      /* fprintf(stderr, "saw cr, suppressing\n"); */
    }
  else
    {
      g_signal_emit_by_name (myContext, "commit", string);
    }
} /* enqueue */

gboolean
my_filter_keypress(GtkIMContext *context, GdkEventKey *event){
  if (event->type != GDK_KEY_PRESS)
    return(FALSE); /* not for me */
  if (event->state & ~GDK_SHIFT_MASK)
    return(FALSE); /* special bits not for me */
  /* fprintf(stderr, "I see 0x%x [%s] \n", *(event->string), event->string); */
  if (0xd == *(event->string) || (
        0x20 <= *(event->string) && *(event->string) <= 0x7e))
    {
      keypressConvert(*(event->string));
      return (*(event->string) == 0xd) ? FALSE : TRUE;
    }
  /* fprintf(stderr, "I am not dealing with it\n"); */
  return(FALSE); /* let someone else handle it. */
} /* my_filter_keypress */

void
my_context_reset(GtkIMContext *context)
{
  /* fprintf(stderr, "resetting my context to initial state\n"); */
  State = initial;
} /* my_context_reset */

static void
yiddish_class_init (GtkIMContextClass *class)
{
  class->filter_keypress = my_filter_keypress;
  class->reset = my_context_reset;
  initializeConversion();
  State = initial;
  /* fprintf(stderr, "initialized yiddish class\n"); */
} /* yiddish_class_init */

static void
yiddish_init (GtkIMContext *im_context)
{
  my_context_reset(im_context);
}

static const GtkIMContextInfo yiddish_info = 
{
  "yiddish-r",     /* ID */
  N_("Yiddish ר"), /* Human readable name */
  GETTEXT_PACKAGE, /* Translation domain */
  LOCALEDIR,       /* Dir for bindtextdomain */
  "yi",            /* Languages for which this module is the default */
};


static const GtkIMContextInfo *info_list[] =
{
  &yiddish_info
};

void
im_module_init (GTypeModule *module)
{
  yiddish_register_type (module);
} /* im_module_init */

void 
im_module_exit (void)
{
} /* im_module_exit */

void 
im_module_list (const GtkIMContextInfo ***contexts, int *n_contexts)
{
  *contexts = info_list;
  *n_contexts = G_N_ELEMENTS (info_list);
} /* im_module_list */

GtkIMContext *
im_module_create (const gchar *context_id)
{
  if (strcmp (context_id, "yiddish-r") == 0)
    {
      myContext = GTK_IM_CONTEXT (g_object_new (type_yiddish_translit, NULL));
      return(myContext);
    }
  else
    return NULL;
} /* im_module_create */

/* vi:set sw=2 ts=2: */
