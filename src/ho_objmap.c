
/***************************************************************************
 *            ho_objmap.c
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

#include "ho_objmap.h"
#include "ho_objlist_stat.h"

ho_objmap *
ho_objmap_new (const int width, const int height) {
  ho_objmap *m_new = NULL;

  /* allocate memory for pixbuf */
  m_new = (ho_objmap *) malloc (sizeof (ho_objmap));
  if (!m_new)
    return NULL;

  /* read header */
  m_new->width = width;
  m_new->height = height;

  /* allocate memory for data (and set to zero) */
  m_new->map =
    (unsigned int *) calloc (m_new->height * m_new->width,
    sizeof (unsigned int));
  if (!(m_new->map)) {
    free (m_new);
    return NULL;
  }

  /* allocate initial object list */
  m_new->obj_list = ho_objlist_new ();
  if (!(m_new->obj_list)) {
    free (m_new->map);
    free (m_new);
    return NULL;
  }

  return m_new;
}

int
ho_objmap_free (ho_objmap * m) {
  if (!m)
    return TRUE;

  ho_objlist_free (m->obj_list);
  if (m->map)
    free (m->map);
  free (m);

  return FALSE;
}

ho_objmap *
ho_objmap_copy (const ho_objmap * m) {
  unsigned int i;
  int x, y;
  ho_objmap *m_out;

  m_out = ho_objmap_new (m->width, m->height);
  if (!m_out)
    return NULL;

  /* copy image data */
  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++)
      ho_objmap_getset (m_out, m, x, y);

  /* copy objects data */
  for (i = 1; i <= ho_objmap_get_size (m); i++)
    ho_objlist_add (m_out->obj_list,
      ho_objmap_get_object (m, i).weight,
      ho_objmap_get_object (m, i).x,
      ho_objmap_get_object (m, i).y,
      ho_objmap_get_object (m, i).width,
      ho_objmap_get_object (m, i).height, ho_objmap_get_object (m, i).number);

  return m_out;
}

int
ho_objmap_clear (ho_objmap * m) {
  int x, y;

  /* clear image data */
  for (x = 0; x < m->width; x++)
    for (y = 0; y < m->height; y++)
      ho_objmap_set (m, x, y, 0);

  /* clear objects data */
  m->obj_list->size = 0;

  return FALSE;
}

int
ho_objmap_relax (ho_objmap * objmap) {
  unsigned int index;
  unsigned int new_index;
  int x, y;

  /* make all pixels from one object point to it's valid index */
  for (x = 0; x < objmap->width; x++)
    for (y = 0; y < objmap->height; y++) {
      index = ho_objmap_get (objmap, x, y);
      new_index = ho_objlist_get_valid_index (objmap->obj_list, index);

      /* update pixel index */
      if (index != new_index)
        ho_objmap_set (objmap, x, y, new_index);

      /* update object weight */
      if (new_index != 0)
        (ho_objmap_get_object (objmap, new_index).weight)++;
    }

  /* clean object list and reorder the objmap */
  ho_objmap_reorder (objmap);

  return FALSE;
}

int
ho_objmap_reorder (ho_objmap * objmap) {
  unsigned int index;
  unsigned int new_index;
  int x, y;
  ho_objlist *clean_object_list;

  /* relax and clean the objlist */
  clean_object_list = ho_objlist_clean (objmap->obj_list);
  if (!clean_object_list)
    return TRUE;

  ho_objlist_free (objmap->obj_list);
  objmap->obj_list = clean_object_list;

  /* replace object index with object number */
  for (x = 0; x < objmap->width; x++)
    for (y = 0; y < objmap->height; y++) {

      /* index is the object number */
      index = ho_objmap_get (objmap, x, y);
      new_index = ho_objlist_get_index (objmap->obj_list, index);

      /* update pixel index */
      ho_objmap_set (objmap, x, y, new_index);
    }

  return FALSE;
}

int
ho_objmap_from_pixbuf (ho_objmap * objmap, const ho_pixbuf * pixbuf,
  unsigned char ch) {
  int x, y;
  int k;
  unsigned int sum;
  unsigned int neigbors[4];
  unsigned int min;
  unsigned int index;
  unsigned int new_index;

  /* make sure objmap is empty */
  ho_objmap_clear (objmap);

  /* link all conected black pixels */
  for (x = 1; x < pixbuf->width; x++)
    for (y = 1; y < (pixbuf->height - 1); y++) {
      /* is channel pixel black ? */
      if (ho_pixbuf_get (pixbuf, x, y, ch) == 0) {
        /* get neigbors */
        neigbors[0] = ho_objmap_get (objmap, x - 1, y - 1);
        neigbors[1] = ho_objmap_get (objmap, x - 1, y - 0);
        neigbors[2] = ho_objmap_get (objmap, x - 1, y + 1);
        neigbors[3] = ho_objmap_get (objmap, x - 0, y - 1);

        sum = neigbors[0] + neigbors[1] + neigbors[2] + neigbors[3];

        if (sum == 0) {
          /* no neigbor is black */

          /* create a new object and set it's index */
          ho_objlist_add ((objmap->obj_list), 0, x, y, 1, 1, 0);
          new_index = (objmap->obj_list)->size;
          ho_objmap_set (objmap, x, y, new_index);
        }
        else {
          /* at least one neigbor is black */

          /* find the index of the minimal frind */
          min = HO_OBJLIST_ALLOCATE_MAX_SIZE;
          for (k = 0; k < 4; k++)
            if (neigbors[k]) {
              new_index =
                ho_objlist_get_valid_index (objmap->obj_list, neigbors[k]);
              if (min > new_index)
                min = new_index;
            }

          /* set the current pixel in the objects matrix to that index */
          ho_objmap_set (objmap, x, y, min);
          ho_objlist_add_pixel ((objmap->obj_list), min, x, y);

          /* link all object to the min index */
          for (k = 0; k < 4; k++)
            if (neigbors[k])
              ho_objlist_link ((objmap->obj_list), min, neigbors[k]);
        }
      }
    }

  /* try to relax and clean objmap */
  ho_objmap_relax (objmap);

  return FALSE;
}

int
ho_objmap_dump (ho_objmap * obj) {
  int i;
  double min_weight, max_weight, avg_weight, com_weight;
  int min_width, max_width, avg_width, com_width;
  int min_height, max_height, avg_height, com_height;

  printf ("ho_objmap_dump: %d %d\n", obj->width, obj->height);

  /* dump list of all objects */
  for (i = 1; i <= ho_objmap_get_size (obj); i++)
    printf ("obj %d %d: %d %d %d %d %f\n", i,
      ho_objmap_get_object (obj, i).number,
      ho_objmap_get_object (obj, i).x,
      ho_objmap_get_object (obj, i).y,
      ho_objmap_get_object (obj, i).width,
      ho_objmap_get_object (obj, i).height,
      ho_objmap_get_object (obj, i).weight);

  /* dump objects statistics */
  ho_objlist_stat_minmax_w (obj->obj_list, &min_weight, &max_weight,
    &avg_weight);
  ho_objlist_stat_common_w (obj->obj_list, min_weight, max_weight, &com_weight);

  ho_objlist_stat_minmax (obj->obj_list, &min_width, &max_width, &avg_width,
    &min_height, &max_height, &avg_height);
  ho_objlist_stat_common (obj->obj_list, min_width, max_width, &com_width,
    min_height, max_height, &com_height);

  printf ("\n\n");

  printf ("object list size : %d\n", obj->obj_list->size);

  printf ("weight : %f..%f %f\n", min_weight, max_weight, avg_weight);
  printf ("width : %d..%d %d\n", min_width, max_width, avg_width);
  printf ("height : %d..%d %d\n", min_height, max_height, avg_height);

  printf ("common weight [cell size-%f]: %f\n",
    ((max_weight - min_weight) / (double) HO_OBJLIST_HISOTGRAM_SIZE),
    com_weight);
  printf ("common width [cell size-%d]: %d\n",
    ((max_width - min_width) / HO_OBJLIST_HISOTGRAM_SIZE), com_width);
  printf ("common height [cell size-%d]: %d\n",
    ((max_height - min_height) / HO_OBJLIST_HISOTGRAM_SIZE), com_height);

  return FALSE;
}
