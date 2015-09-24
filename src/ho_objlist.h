
/***************************************************************************
 *            ho_objlist.h
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

/** @file ho_objlist.h
    @brief hebhocr C language header.
    
    hebhocr - library for Hebrew optical character recognition 
*/

#ifndef HO_OBJLIST_H
#define HO_OBJLIST_H 1

#define HO_OBJLIST_ALLOCATE_PAGE_SIZE 1000
#define HO_OBJLIST_ALLOCATE_MAX_SIZE 40000000

/* hebocr objlist set/get macros */
#define ho_objlist_get_object(object_list,index) ( \
  (object_list->objects)[index])

#ifndef TRUE
#	define TRUE -1
#endif

#ifndef FALSE
#	define FALSE 0
#endif

#ifndef NULL
#	define NULL ((void*)0)
#endif

/** @struct ho_obj
  @brief hebocr object struct
*/
typedef struct _ho_obj {
  unsigned int index;
  unsigned int paragraph;
  unsigned int line;
  unsigned int number;
  double weight;
  int x;
  int y;
  int width;
  int height;
} ho_obj;

/** @struct ho_objlist
  @brief hebocr object list struct
*/
typedef struct _ho_objlist {
  unsigned int size;
  unsigned int allocated_size;
  ho_obj *objects;
} ho_objlist;

/**
 new ho_objlist 
 
 @return newly allocated ho_objlist
 */
ho_objlist *ho_objlist_new ();

/**
 free an ho_objlist 
 @param object_list pointer to an ho_objlist

 @return FALSE
 */
int ho_objlist_free (ho_objlist * object_list);

/**
 get the lowest index linked to input index
 @param object_list pointer to an ho_objlist
 @param index starting index
 
 @return lowest linked index
 */
unsigned int
ho_objlist_get_valid_index (ho_objlist * object_list, unsigned int index);

/**
 get the index linked to object number
 @param object_list pointer to an ho_objlist
 @param number the object number to search
 
 @return lowest linked index
 */
unsigned int
ho_objlist_get_index (ho_objlist * object_list, unsigned int number);

/**
 add an object to ho_objlist 
 @param object_list pointer to an ho_objlist
 @param weight object weight
 @param x object x offset
 @param y object y offset
 @param width object width
 @param height object height
 @param number object number
 
 @return FALSE
 */
int ho_objlist_add (ho_objlist * object_list, double weight,
  int x, int y, int width, int height, unsigned int number);

/**
 add a pixel to ho_objlist object 
 @param object_list pointer to an ho_objlist
 @param index object index
 @param x pixel x offset
 @param y pixel y offset
 
 @return FALSE
 */
int ho_objlist_add_pixel (ho_objlist * object_list,
  unsigned int index, int x, int y);

/**
 link two ho_objlist objects 
 @param object_list pointer to an ho_objlist
 @param index1 first object index
 @param index2 second object index
 
 @return FALSE
 */
int ho_objlist_link (ho_objlist * object_list,
  unsigned int index1, unsigned int index2);

/**
 set all objects weight to 0
 @param object_list pointer to an ho_objlist
 
 @return FALSE
 */
int ho_objlist_clear_weight (ho_objlist * object_list);

/**
 set all objects paragraph, line and number indexes to 0
 @param object_list pointer to an ho_objlist
 
 @return FALSE
 */
int ho_objlist_clear_sorting (ho_objlist * object_list);

/**
 clean ho_objlist 
 @param object_list pointer to an ho_objlist
 
 @return newly allocated clean ho_objlist
 */
ho_objlist *ho_objlist_clean (ho_objlist * object_list);

#endif /* HO_OBJLIST_H */
