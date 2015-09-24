
/***************************************************************************
 *            ho_pixbuf_draw.c
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

#include "ho_pixbuf_draw.h"

int
ho_pixbuf_draw_objmap (ho_pixbuf * pix, const ho_objmap * objmap,
  int x, int y, int width, int height, unsigned char ch, unsigned char val) {
  int x1, y1;
  unsigned char new_val;
  unsigned int index;

  /* copy data */
  for (x1 = 0; x1 < width && x1 < pix->width; x1++) {
    for (y1 = 0; y1 < height && y1 < pix->height; y1++) {
      index = ho_objmap_get (objmap, x1, y1);

      if (index) {

        if (val == 255)
          new_val = (unsigned char) ((index * 10 * (ch + 1)) % 200 + 25);
        else
          new_val = val;

        ho_pixbuf_set (pix, x + x1, y + y1, ch, new_val);
      }
    }
  }

  return FALSE;
}

int
ho_pixbuf_draw_objmap_square (ho_pixbuf * pix, const ho_objmap * objmap,
  unsigned char ch, unsigned char val) {
  return ho_pixbuf_draw_objmap_square_full (pix, objmap, ch, val, 0, 0);
}

int
ho_pixbuf_draw_objmap_square_full (ho_pixbuf * pix, const ho_objmap * objmap,
  unsigned char ch, unsigned char val,
  int horizontal_margin, int vertical_margin) {
  int i;
  int x1, x2, y1, y2;
  unsigned char new_val;

  for (i = 1; i <= ho_objmap_get_size (objmap); i++) {
    x1 = ho_objmap_get_object (objmap, i).x;
    y1 = ho_objmap_get_object (objmap, i).y;
    x2 = x1 + ho_objmap_get_object (objmap, i).width;
    y2 = y1 + ho_objmap_get_object (objmap, i).height;

    if (val == 255)
      new_val = (unsigned char) ((i * 10 * (ch + 1)) % 200 + 25);
    else
      new_val = val;

    ho_pixbuf_draw_square (pix,
      x1 - horizontal_margin,
      x2 + horizontal_margin,
      y1 - vertical_margin, y2 + vertical_margin, ch, new_val);
  }

  return FALSE;
}

int
ho_pixbuf_draw_hline (ho_pixbuf * pix, int x1, int x2, int y,
  unsigned char ch, unsigned char val) {
  int x;

  if (x1 < 0)
    x1 = 0;

  /* set line data */
  for (x = x1; x < x2 && x < pix->width; x++)
    ho_pixbuf_set (pix, x, y, ch, val);

  return FALSE;
}

int
ho_pixbuf_draw_vline (ho_pixbuf * pix, int x, int y1, int y2,
  unsigned char ch, unsigned char val) {
  int y;

  if (y1 < 0)
    y1 = 0;

  /* set line data */
  for (y = y1; y < y2 && y < pix->height; y++)
    ho_pixbuf_set (pix, x, y, ch, val);

  return FALSE;
}

int
ho_pixbuf_draw_square (ho_pixbuf * pix, int x1, int x2, int y1, int y2,
  unsigned char ch, unsigned char val) {
  int x, y;

  if (x1 < 0)
    x1 = 0;
  if (y1 < 0)
    y1 = 0;

  /* fill the square */
  for (x = x1; x < x2 && x < pix->width; x++)
    for (y = y1; y < y2 && y < pix->height; y++)
      ho_pixbuf_set (pix, x, y, ch, val);

  return FALSE;
}

int
ho_pixbuf_draw_grid (ho_pixbuf * pix, int step,
  unsigned char ch, unsigned char val) {
  int x, y;

  /* draw horizontal lines */
  for (x = step; x < pix->width; x += step)
    ho_pixbuf_draw_vline (pix, x, 0, pix->height, ch, val);

  /* draw vertical lines */
  for (y = step; y < pix->height; y += step)
    ho_pixbuf_draw_hline (pix, 0, pix->width, y, ch, val);

  return FALSE;
}
