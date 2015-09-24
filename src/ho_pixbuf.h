
/***************************************************************************
 *            ho_pixbuf.h
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

/** @file ho_pixbuf.h
    @brief hebhocr C language header.
    
    hebhocr - library for Hebrew optical character recognition 
*/

#ifndef HO_PIXBUF_H
#define HO_PIXBUF_H 1

/* hebocr pixbuf set/get macros */

/** \def ho_pixbuf_set(m,x,y,ch,val)
  set a pixel
 */
#define ho_pixbuf_set(m,x,y,ch,val) ( \
  ((m)->data)[(x)*(m)->n_channels+(y)*(m)->rowstride+(ch)]=(val))

/** \def ho_pixbuf_get(m,x,y,ch)
  get a pixel
 */
#define ho_pixbuf_get(m,x,y,ch) ( \
  ((m)->data)[(x)*(m)->n_channels+(y)*(m)->rowstride+(ch)])

/** \def ho_pixbuf_getset(m,ml,x,y,ch)
  get a pixel from \ml left hand pixbuf and set a pixel in \m pixbuf
 */
#define ho_pixbuf_getset(m,ml,x,y,ch) ( \
    (((m )->data)[(x)*(m )->n_channels+(y)*(m )->rowstride+(ch)])= \
    (((ml)->data)[(x)*(ml)->n_channels+(y)*(ml)->rowstride+(ch)]) \
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

/** @struct ho_string
  @brief helper string struct for non-null-terminated strings
*/
typedef struct _ho_string {
  int size;
  char *data;
} ho_string;

/** @struct ho_pixbuf
  @brief hebocr pixbuf map struct (copy gtk pixbuf)
*/
typedef struct _ho_pixbuf {
  unsigned char n_channels;
  int height;
  int width;
  int rowstride;
  unsigned char *data;
} ho_pixbuf;

/**
 new ho_pixbuf 
 @param height hight of pixbuf in pixels
 @param width width of pixbuf in pixels

 @return newly allocated ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_new (const int width, const int height);

/**
 free an ho_pixbuf 
 @param pix pointer to an ho_pixbuf

 @return FALSE
 */
int ho_pixbuf_free (ho_pixbuf * pix);

/**
 copy ho_pixbuf
 @param m pointer to a ho_pixbuf image

 @return newly allocated copy ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_copy (const ho_pixbuf * m);

/**
 cut a window from ho_pixbuf
 @param m pointer to a ho_pixbuf image
 @param x x offset of filter window
 @param y y offset of filter window
 @param width width of filter window
 
 @return newly allocated copy ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_cut (const ho_pixbuf * m,
  int x, int y, int width, int height);

/**
 copy pixel data to a ho_pixbuf
 @param pix pointer to a ho_pixbuf image
 @param data the pixel data to copy

 @return FALSE
 */
int ho_pixbuf_set_data (ho_pixbuf * pix, const char *data);

/**
 get pixel data to a ho_pixbuf as a string
 @param pix pointer to a ho_pixbuf image
 @param a string data struct of the pixbuf data

 @return newly allocated copy ho_string
 */
ho_string ho_pixbuf_to_string (ho_pixbuf * pix);

#endif /* HO_PIXBUF_H */
