
/***************************************************************************
 *            ho_pixbuf_filter.c
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

#include "ho_pixbuf_filter.h"

int
ho_pixbuf_filter_get_min_max (const ho_pixbuf * pix,
  unsigned char ch, unsigned char *min, unsigned char *max,
  int x, int y, int width, int height) {
  int x1;
  int y1;
  unsigned char gray;

  *min = 255;
  *max = 0;

  if (x < 0)
    x = 0;
  if (y < 0)
    y = 0;
  if ((x + width) > pix->width)
    width = pix->width - x;
  if ((y + height) > pix->height)
    height = pix->height - y;

  /* calculate min and max */
  for (x1 = 0; x1 < width; x1++)
    for (y1 = 0; y1 < height; y1++) {
      gray = ho_pixbuf_get (pix, x + x1, y + y1, ch);

      if (*min > gray)
        *min = gray;
      if (*max < gray)
        *max = gray;
    }

  return 0;
}

int
ho_pixbuf_filter_get_histogram (const ho_pixbuf * pix,
  unsigned char ch, double *histogram, int x, int y, int width, int height) {
  int x1;
  int y1;
  int i;
  double image_factor;
  unsigned char gray;

  if (x < 0)
    x = 0;
  if (y < 0)
    y = 0;
  if ((x + width) > pix->width)
    width = pix->width - x;
  if ((y + height) > pix->height)
    height = pix->height - y;

  for (i = 1; i < 256; i++)
    histogram[i] = 0;

  /* calculate histograms */
  for (x1 = 0; x1 < width; x1++)
    for (y1 = 0; y1 < height; y1++) {
      gray = ho_pixbuf_get (pix, x + x1, y + y1, ch);
      histogram[gray]++;
    }

  /* equalize and cumulate histograms */
  image_factor = 255.0 / (double) (width * height);
  histogram[0] *= image_factor;
  for (i = 1; i < 256; i++) {
    histogram[i] = histogram[i - 1] + histogram[i] * image_factor;
  }

  return 0;
}

int
ho_pixbuf_filter_channel (const ho_pixbuf * pix, unsigned char ch) {
  int x;
  int y;
  unsigned char gray;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++) {
      gray = ho_pixbuf_get (pix, x, y, ch);

      ho_pixbuf_set (pix, x, y, 0, gray);
      ho_pixbuf_set (pix, x, y, 1, gray);
      ho_pixbuf_set (pix, x, y, 2, gray);
    }

  return 0;
}

int
ho_pixbuf_filter_gray (const ho_pixbuf * pix) {
  int x;
  int y;
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char gray;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++) {
      red = ho_pixbuf_get (pix, x, y, 0);
      green = ho_pixbuf_get (pix, x, y, 1);
      blue = ho_pixbuf_get (pix, x, y, 2);
      gray = (unsigned char)
        (0.3 * (double) red + 0.59 * (double) green + 0.11 * (double) blue);

      ho_pixbuf_set (pix, x, y, 0, gray);
      ho_pixbuf_set (pix, x, y, 1, gray);
      ho_pixbuf_set (pix, x, y, 2, gray);
    }

  return 0;
}

int
ho_pixbuf_filter_polarize (const ho_pixbuf * pix, unsigned char threshold) {
  int x;
  int y;
  unsigned char red;
  unsigned char green;
  unsigned char blue;

  for (x = 0; x < pix->width; x++)
    for (y = 0; y < pix->height; y++) {
      red = ho_pixbuf_get (pix, x, y, 0);
      green = ho_pixbuf_get (pix, x, y, 1);
      blue = ho_pixbuf_get (pix, x, y, 2);

      ho_pixbuf_set (pix, x, y, 0, (red < threshold) ? 0 : 255);
      ho_pixbuf_set (pix, x, y, 1, (green < threshold) ? 0 : 255);
      ho_pixbuf_set (pix, x, y, 2, (blue < threshold) ? 0 : 255);
    }

  return 0;
}

int
ho_pixbuf_filter_convert (const ho_pixbuf * pix,
  unsigned char ch, double *convert_array,
  int x, int y, int width, int height) {
  int x1;
  int y1;
  unsigned char old_color;
  unsigned char new_color;

  if (x < 0)
    x = 0;
  if (y < 0)
    y = 0;
  if ((x + width) > pix->width)
    width = pix->width - x;
  if ((y + height) > pix->height)
    height = pix->height - y;

  for (x1 = 0; x1 < width; x1++)
    for (y1 = 0; y1 < height; y1++) {
      old_color = ho_pixbuf_get (pix, x + x1, y + y1, ch);
      new_color = (unsigned char) (convert_array[old_color]);

      ho_pixbuf_set (pix, x + x1, y + y1, ch, new_color);
    }

  return 0;
}

int
ho_pixbuf_filter_linear (const ho_pixbuf * pix, unsigned char ch,
  int x, int y, int width, int height, int margin) {
  double convert_array[256];
  int i;
  unsigned char min;
  unsigned char max;
  double a;
  double value;

  /* create conversion array for this chanel */
  ho_pixbuf_filter_get_min_max (pix, 0, &min, &max,
    x - margin, y - margin, width + margin, height + margin);
  a = 255.0 / ((double) (max - min));
  for (i = 0; i < 256; i++) {
    value = (i - min) * a;
    if (value < 0)
      value = 0;
    if (value > 255)
      value = 255;

    convert_array[i] = value;
  }

  /* run conversion filter */
  ho_pixbuf_filter_convert (pix, ch, convert_array, x, y, width, height);

  return 0;
}

int
ho_pixbuf_filter_histequal (const ho_pixbuf * pix, unsigned char ch,
  int x, int y, int width, int height, int margin) {
  double convert_array[256];
  int i;
  unsigned char min;
  unsigned char max;
  double a;
  double value;

  /* create conversion array for this chanel */
  ho_pixbuf_filter_get_histogram (pix, ch, convert_array,
    x - margin, y - margin, width + margin, height + margin);

  /* run conversion filter */
  ho_pixbuf_filter_convert (pix, ch, convert_array, x, y, width, height);

  return 0;
}

int
ho_pixbuf_filter_vsmear (const ho_pixbuf * pix, unsigned char ch,
  unsigned char val, int step, int moment) {
  int x, y;
  int dist;
  unsigned char current_val;
  unsigned char last_val;

  for (x = 0; x < pix->width; x++) {
    dist = 0;
    last_val = val;

    /* wait for first color */
    for (y = 0; ho_pixbuf_get (pix, x, y, ch) != val && y < pix->height; y++) ;

    /* found first color */
    for (; y < pix->height; y++) {
      current_val = ho_pixbuf_get (pix, x, y, ch);

      /* found color */
      if (current_val == val) {

        /* if this is the first pixel in this color patch and the gap is small, 
           fill the gap */
        if (last_val != val && dist < step) {
          ho_pixbuf_draw_vline (pix, x, y - dist - moment, y + moment, ch, val);
        }
        dist = 0;
      }

      dist++;
      last_val = current_val;
    }
  }

  return 0;
}

int
ho_pixbuf_filter_hsmear (const ho_pixbuf * pix, unsigned char ch,
  unsigned char val, int step, int moment) {
  int x, y;
  int dist;
  unsigned char current_val;
  unsigned char last_val;

  for (y = 0; y < pix->height; y++) {
    dist = 0;
    last_val = val;

    /* wait for first color */
    for (x = 0; ho_pixbuf_get (pix, x, y, ch) != val && x < pix->width; x++) ;

    /* found first color */
    for (; x < pix->width; x++) {
      current_val = ho_pixbuf_get (pix, x, y, ch);

      /* found color */
      if (current_val == val) {

        /* if this is the first pixel in this color patch and the gap is small, 
           fill the gap */
        if (last_val != val && dist < step) {
          ho_pixbuf_draw_hline (pix, x - dist - moment, x + moment, y, ch, val);
        }

        dist = 0;
      }

      dist++;
      last_val = current_val;
    }
  }

  return 0;
}
