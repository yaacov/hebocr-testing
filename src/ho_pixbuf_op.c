
/***************************************************************************
 *            ho_pixbuf_op.c
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

#include "ho_pixbuf_op.h"

int
ho_pixbuf_op_or (const ho_pixbuf * pix) {
  int x;
  int y;
  unsigned char red;
  unsigned char green;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++) {
      red = ho_pixbuf_get (pix, x, y, 0);
      green = ho_pixbuf_get (pix, x, y, 1);

      ho_pixbuf_set (pix, x, y, 0, red | green);
    }

  return 0;
}

int
ho_pixbuf_op_and (const ho_pixbuf * pix) {
  int x;
  int y;
  unsigned char red;
  unsigned char green;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++) {
      red = ho_pixbuf_get (pix, x, y, 0);
      green = ho_pixbuf_get (pix, x, y, 1);

      ho_pixbuf_set (pix, x, y, 0, red & green);
    }

  return 0;
}

int
ho_pixbuf_op_xor (const ho_pixbuf * pix) {
  int x;
  int y;
  unsigned char red;
  unsigned char green;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++) {
      red = ho_pixbuf_get (pix, x, y, 0);
      green = ho_pixbuf_get (pix, x, y, 1);

      ho_pixbuf_set (pix, x, y, 0, red ^ green);
    }

  return 0;
}

int
ho_pixbuf_op_not (const ho_pixbuf * pix) {
  int x;
  int y;
  unsigned char red;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++) {
      red = ho_pixbuf_get (pix, x, y, 0);

      ho_pixbuf_set (pix, x, y, 0, ~red);
    }

  return 0;
}

int
ho_pixbuf_op_mov (const ho_pixbuf * pix, unsigned char ch1, unsigned char ch2) {
  int x;
  int y;
  unsigned char gray;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++) {
      gray = ho_pixbuf_get (pix, x, y, ch1);

      ho_pixbuf_set (pix, x, y, ch2, gray);
    }

  return 0;
}
