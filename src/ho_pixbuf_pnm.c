
/***************************************************************************
 *            ho_pixbuf_pnm.c
 *
 *  Fri Aug 12 20:13:33 2005
 *  Copyright  2005,2007,2009,2011  Yaacov Zamir
 *  <kobi.zamir@gmail.com>
 ****************************************************************************/

/*  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "ho_pixbuf_pnm.h"

unsigned char
ho_pbm_getc (FILE * file) {
  unsigned char ch;
  int comment = FALSE;

  do {
    ch = getc (file);
    if (ch == '\n')
      comment = FALSE;
    else if (ch == '#')
      comment = TRUE;
  } while (comment);

  return ch;
}

int
ho_pbm_getint (FILE * file) {
  unsigned char ch;
  int i = 0;

  /* pass on white spaces */
  do {
    ch = ho_pbm_getc (file);
  } while (ch == ' ' || ch == '\n' || ch == '\t');

  /* read a number */
  do {
    i = (i * 10) + (ch - '0');
    ch = ho_pbm_getc (file);
  } while (ch >= '0' && ch <= '9');

  return i;
}

int
ho_pbm_getbit (FILE * file) {
  static unsigned char byte = 0;
  static unsigned char mask = 0;
  int return_bit;

  /* if file = 0, reset mask */
  if (!file) {
    mask = 0;
    return 0;
  }

  /* if we need a new byte read it from file */
  if (mask == 0) {
    mask = 0x80;
    byte = getc (file);
  }

  /* get the bit */
  return_bit = (byte & mask) ? 0 : 255;

  /* update mask */
  mask >>= 1;

  return return_bit;
}

ho_pixbuf *
ho_pixbuf_pnm_load (FILE * file) {
  char ch1, ch2;
  ho_pixbuf *pix = NULL;
  unsigned char use_stdin = 0;
  unsigned char n_channels = 0;
  unsigned char val = 0;
  int width = 0;
  int height = 0;
  int x, y, rowstride;

  if (!file)
    return NULL;

  /* read magic number "P?" for pbm file */
  ch1 = ho_pbm_getc (file);
  ch2 = ho_pbm_getc (file);
  if (ch1 != 'P' || (ch2 != '6' && ch2 != '5' && ch2 != '4')) {
    /* bad magic */
    return NULL;
  }

  /* read header */
  width = ho_pbm_getint (file);
  height = ho_pbm_getint (file);

  /* create a new pixbuf */
  pix = ho_pixbuf_new (width, height);
  if (!pix)
    return NULL;

  if (ch2 == '4') {
    /* one bit image */
    rowstride = 8 * (width / 8 + 1);

    for (y = 0; y < height; y++) {
      /* reset mask to 0 on a new line */
      ho_pbm_getbit (0);

      /* get all line data */
      for (x = 0; x < rowstride; x++) {
        val = ho_pbm_getbit (file);

        /* if pixel in image range */
        if (x < width) {
          ho_pixbuf_set (pix, x, y, 0, val);
          ho_pixbuf_set (pix, x, y, 1, val);
          ho_pixbuf_set (pix, x, y, 2, val);
        }
      }
    }
  }
  else if (ch2 == '5') {
    /* check for color channel depth of 8bit */
    if (ho_pbm_getint (file) == 255)
      for (y = 0; y < height; y++)
        for (x = 0; x < width; x++) {
          val = getc (file);

          ho_pixbuf_set (pix, x, y, 0, val);
          ho_pixbuf_set (pix, x, y, 1, val);
          ho_pixbuf_set (pix, x, y, 2, val);
        }
  }
  else {
    /* check for color channel depth of 8bit */
    if (ho_pbm_getint (file) == 255)
      fread (pix->data, 1, pix->height * pix->rowstride, file);
  }

  /* return the new pixbuf to user */
  return pix;
}

int
ho_pixbuf_pnm_save (const ho_pixbuf * pix, FILE * file) {
  if (!file)
    return TRUE;

  /* print header (3 8bit color channels ) */
  fprintf (file, "P6 %d %d 255\n", pix->width, pix->height);

  /* this might be a huge write... */
  fwrite (pix->data, 1, pix->height * pix->rowstride, file);

  return FALSE;
}

ho_pixbuf *
ho_pixbuf_pnm_load_path (const char *filename) {
  ho_pixbuf *pix = NULL;
  FILE *file = NULL;

  file = fopen (filename, "r");
  if (!file)
    return NULL;

  pix = ho_pixbuf_pnm_load (file);
  fclose (file);

  return pix;
}

int
ho_pixbuf_pnm_save_path (const ho_pixbuf * pix, const char *filename) {
  FILE *file = NULL;

  file = fopen (filename, "wb");
  if (!file)
    return TRUE;

  ho_pixbuf_pnm_save (pix, file);
  fclose (file);

  return FALSE;
}
