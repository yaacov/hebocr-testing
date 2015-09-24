
/***************************************************************************
 *            ho_objlist_stat.c
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

#include "ho_objlist_stat.h"

int
ho_objlist_stat_minmax_w (ho_objlist * object_list,
  double *min, double *max, double *avg) {
  int i;
  unsigned int size;
  double weight;
  double sum;

  size = object_list->size;
  if (size < 1)
    return TRUE;

  sum = *min = *max = ho_objlist_get_object (object_list, 1).weight;

  /* calculate min max */
  for (i = 2; i <= size; i++) {
    weight = ho_objlist_get_object (object_list, i).weight;

    sum += weight;
    if (*min > weight)
      *min = weight;
    if (*max < weight)
      *max = weight;
  }

  /* calculate avg */
  *avg = sum / size;

  return FALSE;
}

int
ho_objlist_stat_common_w (ho_objlist * object_list,
  double min, double max, double *com) {
  int i;
  int i_com = 0;
  unsigned int size;
  double weight;
  unsigned int histogram[HO_OBJLIST_HISOTGRAM_SIZE];

  size = object_list->size;

  /* clean histogram */
  for (i = 0; i < HO_OBJLIST_HISOTGRAM_SIZE; i++)
    histogram[i] = 0;

  /* create histogram */
  for (i = 1; i <= size; i++) {
    weight = ho_objlist_get_object (object_list, i).weight;

    if (weight > min && weight < max) {
      weight = (HO_OBJLIST_HISOTGRAM_SIZE - 1) * ((weight - min) / (max - min));
      histogram[(unsigned char) weight]++;
    }
  }

  /* get the common weight */
  for (i = 1; i < HO_OBJLIST_HISOTGRAM_SIZE; i++)
    if (histogram[i] > histogram[i_com])
      i_com = i;

  *com = min + (max - min) * ((double) i_com + 0.5) /
    (double) (HO_OBJLIST_HISOTGRAM_SIZE - 1);

  return FALSE;
}

int
ho_objlist_stat_minmax (ho_objlist * object_list,
  int *min_w, int *max_w, int *avg_w, int *min_h, int *max_h, int *avg_h) {
  int i;
  unsigned int size;
  int width, height;
  int sum_w, sum_h;

  size = object_list->size;
  if (size < 1)
    return TRUE;

  sum_w = *min_w = *max_w = ho_objlist_get_object (object_list, 1).width;
  sum_h = *min_h = *max_h = ho_objlist_get_object (object_list, 1).height;

  /* calculate min max */
  for (i = 2; i <= size; i++) {
    width = ho_objlist_get_object (object_list, i).width;
    height = ho_objlist_get_object (object_list, i).height;

    sum_w += width;
    sum_h += height;

    if (*min_w > width)
      *min_w = width;
    if (*max_w < width)
      *max_w = width;

    if (*min_h > height)
      *min_h = height;
    if (*max_h < height)
      *max_h = height;
  }

  /* calculate avg */
  *avg_w = sum_w / size;
  *avg_h = sum_h / size;

  return FALSE;
}

int
ho_objlist_stat_common (ho_objlist * object_list,
  int min_w, int max_w, int *com_w, int min_h, int max_h, int *com_h) {
  int i;
  int i_com_w = 0;
  int i_com_h = 0;
  unsigned int size;
  int width, height;
  unsigned int histogram_w[HO_OBJLIST_HISOTGRAM_SIZE];
  unsigned int histogram_h[HO_OBJLIST_HISOTGRAM_SIZE];

  size = object_list->size;

  /* clean histogram */
  for (i = 0; i < HO_OBJLIST_HISOTGRAM_SIZE; i++) {
    histogram_w[i] = 0;
    histogram_h[i] = 0;
  }

  /* create histogram */
  for (i = 1; i <= size; i++) {
    width = ho_objlist_get_object (object_list, i).width;
    height = ho_objlist_get_object (object_list, i).height;

    if (width > min_w && width < max_w) {
      width = (HO_OBJLIST_HISOTGRAM_SIZE - 1) *
        ((double) (width - min_w) / (double) (max_w - min_w));
      histogram_w[(unsigned char) width]++;
    }

    if (height > min_h && height < max_h) {
      height = (HO_OBJLIST_HISOTGRAM_SIZE - 1) *
        ((double) (height - min_h) / (double) (max_h - min_h));
      histogram_h[(unsigned char) height]++;
    }
  }

  /* get the common weight */
  for (i = 1; i < HO_OBJLIST_HISOTGRAM_SIZE; i++) {
    if (histogram_w[i] > histogram_w[i_com_w])
      i_com_w = i;
    if (histogram_h[i] > histogram_h[i_com_h])
      i_com_h = i;
  }

  *com_w = min_w + (max_w - min_w) *
    ((double) i_com_w + 0.5) / (double) (HO_OBJLIST_HISOTGRAM_SIZE - 1);
  *com_h = min_h + (max_h - min_h) *
    ((double) i_com_h + 0.5) / (double) (HO_OBJLIST_HISOTGRAM_SIZE - 1);

  return FALSE;
}
