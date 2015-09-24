
/***************************************************************************
 *            ho_objlist.c
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

#include "ho_objlist.h"

ho_objlist *
ho_objlist_new () {
  ho_objlist *new_object_list;

  /* allocate memory for struct */
  new_object_list = (ho_objlist *) malloc (sizeof (ho_objlist));

  /* check for new memory */
  if (!new_object_list)
    return NULL;

  /* init struct values */
  new_object_list->size = 0;
  new_object_list->allocated_size = HO_OBJLIST_ALLOCATE_PAGE_SIZE;

  /* allocate memory for objects */
  new_object_list->objects =
    (ho_obj *) malloc (sizeof (ho_obj) * new_object_list->allocated_size);

  /* if no memeory for objects free the struct */
  if (!new_object_list->objects) {
    free (new_object_list);
    return NULL;
  }

  /* set object 0 to Empty */
  ho_objlist_get_object (new_object_list, 0).index = 0;

  return new_object_list;
}

int
ho_objlist_free (ho_objlist * object_list) {
  if (!object_list)
    return TRUE;

  /* free the objects data array */
  if (object_list->objects)
    free (object_list->objects);

  /* free the struct */
  free (object_list);

  return FALSE;
}

unsigned int
ho_objlist_get_valid_index (ho_objlist * object_list, unsigned int index) {
  /* search for valid index */
  while ((object_list->objects)[index].index != index)
    index = (object_list->objects)[index].index;

  return index;
}

unsigned int
ho_objlist_get_index (ho_objlist * object_list, unsigned int number) {
  unsigned int index;

  /* search for number */
  for (index = object_list->size;
    index > 0 && (object_list->objects)[index].number != number; index--) ;

  return index;
}

int
ho_objlist_add (ho_objlist * object_list, double weight,
  int x, int y, int width, int height, unsigned int number) {
  ho_obj *new_object;
  ho_obj *new_object_list_objects;
  unsigned int new_allocated_size;

  /* we do not want to allocate this memory */
  if (object_list->size > HO_OBJLIST_ALLOCATE_MAX_SIZE)
    return TRUE;

  /* check for allocated space and try to get more memory */
  if ((object_list->size + 2) >= object_list->allocated_size) {
    new_allocated_size =
      object_list->allocated_size + HO_OBJLIST_ALLOCATE_PAGE_SIZE;

    /* try to allocate new memeory for objects */
    new_object_list_objects = (ho_obj *)
      realloc (object_list->objects, sizeof (ho_obj) * new_allocated_size);
    if (!new_object_list_objects)
      return TRUE;

    object_list->objects = new_object_list_objects;
    object_list->allocated_size = new_allocated_size;
  }

  /* add the new object (we start at object 1) */
  (object_list->size)++;
  new_object = &(ho_objlist_get_object (object_list, object_list->size));

  new_object->index = object_list->size;
  new_object->number = number;
  new_object->weight = weight;
  new_object->x = x;
  new_object->y = y;
  new_object->height = height;
  new_object->width = width;

  return FALSE;
}

int
ho_objlist_add_pixel (ho_objlist * object_list,
  unsigned int index, int x, int y) {
  ho_obj *object = &(ho_objlist_get_object (object_list, index));
  int x1 = object->x;
  int x2 = object->x + object->width;
  int y1 = object->y;
  int y2 = object->y + object->height;

  if (x1 > x)
    x1 = x;
  if (y1 > y)
    y1 = y;
  if (x2 <= x)
    x2 = x + 1;
  if (y2 <= y)
    y2 = y + 1;

  object->x = x1;
  object->y = y1;
  object->height = y2 - y1;
  object->width = x2 - x1;

  return FALSE;
}

int
ho_objlist_link (ho_objlist * object_list,
  unsigned int index1, unsigned int index2) {
  ho_obj *object1;
  ho_obj *object2;
  unsigned int index;
  int x11, x12, y11, y12;
  int x21, x22, y21, y22;

  /* just checking :-) */
  if (index1 == index2)
    return TRUE;

  /* always use the lower index as index1 */
  if (index2 < index1) {
    index = index1;
    index1 = index2;
    index2 = index;
  }

  /* get objects */
  object1 = &(ho_objlist_get_object (object_list, index1));
  object2 = &(ho_objlist_get_object (object_list, index2));

  /* set object1 extentions */
  x11 = object1->x;
  x12 = object1->x + object1->width;
  y11 = object1->y;
  y12 = object1->y + object1->height;

  /* set object2 extentions */
  x21 = object2->x;
  x22 = object2->x + object2->width;
  y21 = object2->y;
  y22 = object2->y + object2->height;

  /* set linked object extentions */
  if (x21 < x11)
    x11 = x21;
  if (y21 < y11)
    y11 = y21;
  if (x22 > x12)
    x12 = x22;
  if (y22 > y12)
    y12 = y22;

  /* adjust object2 index */
  object2->index = index1;

  /* adjust object1 extentions */
  object1->x = x11;
  object1->y = y11;
  object1->height = (y12 - y11);
  object1->width = (x12 - x11);

  return FALSE;
}

int
ho_objlist_clear_weight (ho_objlist * object_list) {
  unsigned int i;

  /* set all weights to 0 */
  for (i = 1; i <= (object_list->size); i++)
    (object_list->objects)[i].weight = 0;

  return FALSE;
}

int
ho_objlist_clear_sorting (ho_objlist * object_list) {
  unsigned int i;

  /* set all weights to 0 */
  for (i = 1; i <= (object_list->size); i++) {
    (object_list->objects)[i].paragraph = 0;
    (object_list->objects)[i].line = 0;
    (object_list->objects)[i].number = 0;
  }

  return FALSE;
}

ho_objlist *
ho_objlist_clean (ho_objlist * object_list) {
  ho_objlist *clean_object_list;
  unsigned int i;

  /* allocate temporary list */
  clean_object_list = ho_objlist_new ();
  if (!clean_object_list)
    return TRUE;

  /* copy only valid objects to clean list */
  for (i = 1; i <= (object_list->size); i++) {
    if ((object_list->objects)[i].index == i) {
      /* this index point to a valid object */
      ho_objlist_add (clean_object_list,
        ((object_list->objects)[i]).weight,
        ((object_list->objects)[i]).x,
        ((object_list->objects)[i]).y,
        ((object_list->objects)[i]).width,
        ((object_list->objects)[i]).height, i);
    }
  }

  return clean_object_list;
}
