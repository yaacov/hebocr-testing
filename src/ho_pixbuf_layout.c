
/***************************************************************************
 *            ho_pixbuf_layout.c
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
#include "ho_pixbuf_filter.h"
#include "ho_pixbuf_layout.h"

#include "ho_objmap.h"
#include "ho_pixsum.h"

int
ho_pixbuf_layout_filter (const ho_pixbuf * pix,
  unsigned char ch, int min_width, int max_width,
  int min_height, int max_height) {
  ho_objmap *obj = NULL;

  /* create object map for the filter */
  obj = ho_objmap_new (pix->width, pix->height);
  if (!obj) {
    return TRUE;
  }

  /* filter out very tall or short objects */
  ho_objmap_from_pixbuf (obj, pix, ch);
  ho_objmap_filter_by_height (obj, min_height, max_height);
  ho_objmap_filter_by_width (obj, min_width, max_width);
  
  /* clear the image chanell */
  ho_pixbuf_draw_square (pix, 0, pix->width, 0, pix->height, ch, 255);

  /* draw back only filtered objects */
  ho_pixbuf_draw_objmap (pix, obj, 0, 0, pix->width, pix->height, ch, 0);

  /* free object map */
  ho_objmap_free (obj);

  return FALSE;
}

int
ho_pixbuf_layout_smear (const ho_pixbuf * pix,
  unsigned char ch, int hsmear, int vsmear) {
  /* smear verticaly, color = black, step = height / 2 */
  ho_pixbuf_filter_vsmear (pix, ch, 0, hsmear, 0);

  /* smear horizontaly, color = black, step = height / 2 */
  ho_pixbuf_filter_hsmear (pix, ch, 0, vsmear, 0);

  return FALSE;
}

int
ho_pixbuf_layout_segment_vertical (const ho_pixbuf * pix, ho_pixsum * sums,
  unsigned char ch, int x, int y, int width, int height, double threshold) {
  double fill;
  double avg;
  double min;
  double max;
  double line_fill;
  int i;

  /* init stats */
  fill = ho_pixsum_get (sums, x + width - 1, y + height - 1);
  avg = fill / (double) width;
  min = fill;
  max = 0;

  /* calc stats for each image column */
  for (i = x + 1; i < (x + width); i++) {
    line_fill = ho_pixsum_get (sums, i, y + height - 1) -
      ho_pixsum_get (sums, i - 1, y + height - 1);

    if (min > line_fill)
      min = line_fill;
    if (max < line_fill)
      max = line_fill;
  }

  /* draw the segmented area */
  threshold = (max - min) * threshold + min;
  for (i = x + 1; i < (x + width); i++) {
    line_fill = ho_pixsum_get (sums, i, y + height - 1) -
      ho_pixsum_get (sums, i - 1, y + height - 1);

    if (line_fill - min > threshold) {
      ho_pixbuf_draw_vline (pix, i, y, y + height - 1, ch, 0);
    } else {
      ho_pixbuf_draw_vline (pix, i, y, y + height - 1, ch, 255);
    }
  }

  return FALSE;
}

int
ho_pixbuf_layout_segment_horizontal (const ho_pixbuf * pix, ho_pixsum * sums,
  unsigned char ch, int x, int y, int width, int height, double threshold) {
  double fill;
  double avg;
  double min;
  double max;
  double line_fill;
  int i;

  /* init stats */
  fill = ho_pixsum_get (sums, x + width - 1, y + height - 1);
  avg = fill / (double) width;
  min = fill;
  max = 0;

  /* calc stats for each image column */
  for (i = y + 1; i < (y + height); i++) {
    line_fill = ho_pixsum_get (sums, x + width - 1, i) -
      ho_pixsum_get (sums, x + width - 1, i - 1);

    if (min > line_fill)
      min = line_fill;
    if (max < line_fill)
      max = line_fill;
  }

  /* draw the segmented area */
  threshold = (max - min) * threshold + min;
  for (i = y + 1; i < (y + height); i++) {
    line_fill = ho_pixsum_get (sums, x + width - 1, i) -
      ho_pixsum_get (sums, x + width - 1, i - 1);

    if (line_fill - min > threshold) {
      ho_pixbuf_draw_hline (pix, x, x + width - 1, i, ch, 0);
    } else {
      ho_pixbuf_draw_hline (pix, x, x + width - 1, i, ch, 255);
    }
  }

  return FALSE;
}
