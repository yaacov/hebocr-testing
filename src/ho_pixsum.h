
/***************************************************************************
 *            ho_pixsum.h
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

/** @file ho_pixsum.h
    @brief hebhocr C language header.
    
    hebhocr - library for Hebrew optical character recognition 
*/

#ifndef HO_PIXSUM_H
#define HO_PIXSUM_H 1

#include "ho_pixbuf.h"

/* hebocr pixbuf set/get macros */

/** \def ho_pixsum_set(m,x,y,val)
  set a pixel
 */
#define ho_pixsum_set(m,x,y,val) ( \
  ((m)->data)[(x)+(y)*(m)->rowstride]=(val))

/** \def ho_pixsum_get(m,x,y)
  get a pixel
 */
#define ho_pixsum_get(m,x,y) ( \
  ((m)->data)[(x)+(y)*(m)->rowstride])

/** \def ho_pixbuf_getset(m,ml,x,y)
  get a pixel from \ml left hand pixsum and set a pixel in \m pixsum
 */
#define ho_pixsum_getset(m,ml,x,y) ( \
    (((m )->data)[(x)+(y)*(m )->rowstride])= \
    (((ml)->data)[(x)+(y)*(ml)->rowstride]) \
  )

#ifndef TRUE
#	define TRUE -1
#endif

#ifndef FALSE
#	define FALSE 0
#endif

#ifndef NULL
#	define NULL ((void*)0)
#endif

/** @struct ho_pixsum
  @brief hebocr pixsum map struct
*/
typedef struct _ho_pixsum {
  int height;
  int width;
  int rowstride;
  double *data;
} ho_pixsum;

/**
 new ho_pixbuf 
 @param height hight of pixbuf in pixels
 @param width width of pixbuf in pixels

 @return newly allocated ho_pixsum
 */
ho_pixsum *ho_pixsum_new (const int width, const int height);

/**
 free an ho_pixsum 
 @param pix pointer to an ho_pixsum

 @return FALSE
 */
int ho_pixsum_free (ho_pixsum * pix);

/**
 copy ho_pixsum
 @param m pointer to a ho_pixsum image

 @return newly allocated copy ho_pixsum
 */
ho_pixsum *ho_pixsum_copy (const ho_pixsum * m);

/**
 calculate ho_pixsum from ho_pixbuf
 @param m_out pointer to an ho_pixsum image
 @param pix pointer to a ho_pixmap image
 @param ch the chanell to sum
 
 @return 0
 */
ho_pixsum *ho_pixsum_from_pixbuf (ho_pixsum * m_out, const ho_pixbuf * pix,
  unsigned char ch);

#endif /* HO_PIXSUM_H */
