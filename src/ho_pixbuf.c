
/***************************************************************************
 *            ho_pixbuf.c
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

#include "ho_pixbuf.h"

ho_pixbuf *
ho_pixbuf_new (const int width, const int height) {
  ho_pixbuf *pix = NULL;

  /* allocate memory for pixbuf */
  pix = (ho_pixbuf *) malloc (sizeof (ho_pixbuf));
  if (!pix)
    return NULL;

  /* read header */
  pix->n_channels = 3;
  pix->width = width;
  pix->height = height;
  pix->rowstride = pix->width * pix->n_channels;

  /* allocate memory for data */
  pix->data = malloc (pix->height * pix->rowstride);
  if (!(pix->data)) {
    free (pix);
    return NULL;
  }

  /* set data to white */
  memset ((void *) (pix->data), 255, pix->height * pix->rowstride);

  return pix;
}

int
ho_pixbuf_free (ho_pixbuf * pix) {
  if (!pix)
    return TRUE;

  if (pix->data)
    free (pix->data);

  free (pix);

  return FALSE;
}

ho_pixbuf *
ho_pixbuf_copy (const ho_pixbuf * m) {
  ho_pixbuf *m_out;
  int x, y;

  /* allocate new pixbuf */
  m_out = ho_pixbuf_new (m->width, m->height);
  if (!m_out)
    return NULL;

  /* copy data */
  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++) {
      ho_pixbuf_getset (m_out, m, x, y, 0);
      ho_pixbuf_getset (m_out, m, x, y, 1);
      ho_pixbuf_getset (m_out, m, x, y, 2);
    }

  return m_out;
}

ho_pixbuf *
ho_pixbuf_cut (const ho_pixbuf * m, int x, int y, int width, int height) {
  ho_pixbuf *m_out;
  int x1, y1;
  unsigned char val;

  /* allocate new pixbuf */
  m_out = ho_pixbuf_new (width, height);
  if (!m_out)
    return NULL;

  /* sanity check */
  if (x < 0)
    x = 0;
  if (y < 0)
    y = 0;

  /* copy data */
  for (x1 = 0; x1 < width && (x + x1) < m->width; x1++)
    for (y1 = 0; y1 < height && (y + y1) < m->height; y1++) {
      val = ho_pixbuf_get (m, x + x1, y + y1, 0);
      ho_pixbuf_set (m_out, x1, y1, 0, val);

      val = ho_pixbuf_get (m, x + x1, y + y1, 1);
      ho_pixbuf_set (m_out, x1, y1, 1, val);

      val = ho_pixbuf_get (m, x + x1, y + y1, 2);
      ho_pixbuf_set (m_out, x1, y1, 2, val);
    }

  return m_out;
}

int
ho_pixbuf_set_data (ho_pixbuf * pix, const char *data) {
  /* copy the pixels */
  memcpy (pix->data, data, (pix->height * pix->rowstride));

  return FALSE;
};

ho_string
ho_pixbuf_to_string (ho_pixbuf * pix) {
  ho_string out;

  /* allocate new string data */
  out.data = (char *) malloc (pix->height * pix->rowstride);

  if (!out.data) {
    out.data = NULL;
    out.size = 0;

    return out;
  }

  /* copy data */
  memcpy (out.data, pix->data, (pix->height * pix->rowstride));
  out.size = pix->height * pix->rowstride;

  return out;
}
