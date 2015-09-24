
/***************************************************************************
 *            ho_objmap.h
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

/** @file ho_objmap.h
    @brief hebhocr C language header.
    
    hebhocr - library for Hebrew optical character recognition 
*/

#ifndef HO_OBJMAP_H
#define HO_OBJMAP_H 1

#include <ho_pixbuf.h>
#include <ho_objlist.h>

/* hocr objmap set/get macros */

/** \def ho_objmap_set(m,x,y,val)
  set a pixel in pixbuf
 */
#define ho_objmap_set(m,x,y,val) (((m)->map)[(x) + (y) * (m)->width] = (val))

/** \def ho_objmap_get(m,x,y)
  set a pixel from pixbuf
 */
#define ho_objmap_get(m,x,y) (((m)->map)[(x) + (y) * (m)->width])

/** \def ho_objmap_getset(m_out,m_in,x,y)
  get a pixel from \m_out left hand objmap and set a pixel in \m objmap
 */
#define ho_objmap_getset(m_out,m_in,x,y) ( \
    ((m_out)->map)[(x) + (y) * (m_out)->width] = \
    ((m_in)->map)[(x) + (y) * (m_in)->width])

/** \def ho_objmap_get_size(m)
  get the number of objects
 */
#define ho_objmap_get_size(m) ((m)->obj_list->size)

/** \def ho_objmap_get_object(m,i)
  get an object
 */
#define ho_objmap_get_object(m,i) ((m)->obj_list->objects[(i)])

/** @struct ho_objmap
  @brief hebocr object map struct
*/
typedef struct _ho_objmap {
  int height;
  int width;
  unsigned int *map;
  ho_objlist *obj_list;
} ho_objmap;

/**
 new ho_pixbuf 
 @param height hight of pixbuf in pixels
 @param width width of pixbuf in pixels
 
 @return newly allocated ho_pixbuf
 */
ho_objmap *ho_objmap_new (const int width, const int height);

/**
 free an ho_objmap
 @param m pointer to an ho_objmap
 
 @return FALSE
 */
int ho_objmap_free (ho_objmap * m);

/**
 copy ho_objmap
 @param m pointer to a ho_pixbuf image

 @return newly allocated copy ho_objmap
 */
ho_objmap *ho_objmap_copy (const ho_objmap * m);

/**
 clear ho_objmap
 @param m pointer to a ho_pixbuf image

 @return FALSE
 */
int ho_objmap_clear (ho_objmap * m);

/**
 relax and clean an ho_objmap using it's objlist
 @param objmap pointer to an ho_objmap
 
 @return FALSE
 */
int ho_objmap_relax (ho_objmap * objmap);

/**
 reorder object map using it's object numbers
 @param objmap pointer to an ho_objmap
 
 @return FALSE
 */
int ho_objmap_reorder (ho_objmap * objmap);

/**
 calculate ho_objmap from ho_pixbuf
 @param objmap pointer to an ho_objmap image
 @param pixbuf pointer to an ho_pixbuf image
 @param ch the color channel to use
 
 @return FALSE
 */
int ho_objmap_from_pixbuf (ho_objmap * objmap,
  const ho_pixbuf * pixbuf, unsigned char ch);

/**
 dump debug information to stdout
 @param objmap pointer to an ho_objmap
 
 @return FALSE
 */
int ho_objmap_dump (ho_objmap * obj);

#endif /* HO_OBJMAP_H */
