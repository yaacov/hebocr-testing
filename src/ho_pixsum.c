
/***************************************************************************
 *            ho_pixsum.c
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

#include "ho_pixsum.h"

ho_pixsum *
ho_pixsum_new (const int width, const int height) {
  ho_pixsum *pix = NULL;

  /* allocate memory for pixbuf */
  pix = (ho_pixsum *) malloc (sizeof (ho_pixsum));
  if (!pix)
    return NULL;

  /* read header */
  pix->width = width;
  pix->height = height;
  pix->rowstride = pix->width;

  /* allocate memory for data */
  pix->data = malloc (sizeof (double) * pix->height * pix->rowstride);
  if (!(pix->data)) {
    free (pix);
    return NULL;
  }

  /* set data to 0 */
  memset ((void *) (pix->data), 0,
    sizeof (double) * pix->height * pix->rowstride);

  return pix;
}

int
ho_pixsum_free (ho_pixsum * pix) {
  if (!pix)
    return TRUE;

  if (pix->data)
    free (pix->data);

  free (pix);

  return FALSE;
}

ho_pixsum *
ho_pixsum_copy (const ho_pixsum * m) {
  ho_pixsum *m_out;
  int x, y;

  /* allocate new pixbuf */
  m_out = ho_pixsum_new (m->width, m->height);
  if (!m_out)
    return NULL;

  /* copy data */
  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++) {
      ho_pixsum_getset (m_out, m, x, y);
    }

  return m_out;
}

ho_pixsum *
ho_pixsum_from_pixbuf (ho_pixsum * m_out, const ho_pixbuf * pix,
  unsigned char ch) {
  int x;
  int y;
  double sum;

  /* (0, 0) is itself */
  sum = 255 - ho_pixbuf_get (pix, 0, 0, ch);
  ho_pixsum_set (m_out, 0, 0, sum);

  /* first row only sum left */
  for (x = 1; x < pix->width; x++) {
    sum = ho_pixsum_get (m_out, x - 1, 0) + 255 - ho_pixbuf_get (pix, x, 0, ch);

    ho_pixsum_set (m_out, x, 0, sum);
  }

  /* first column only sum up */
  for (y = 1; y < pix->height; y++) {
    sum = ho_pixsum_get (m_out, 0, y - 1) + 255 - ho_pixbuf_get (pix, 0, y, ch);

    ho_pixsum_set (m_out, 0, y, sum);
  }

  /* sum all the rest of the pixels */
  for (x = 1; x < pix->width; x++)
    for (y = 1; y < pix->height; y++) {
      sum = ho_pixsum_get (m_out, x - 1, y) +
        ho_pixsum_get (m_out, x, y - 1) -
        ho_pixsum_get (m_out, x - 1, y - 1) +
        255 - ho_pixbuf_get (pix, x, y, ch);

      ho_pixsum_set (m_out, x, y, sum);
    }

  return 0;
}
