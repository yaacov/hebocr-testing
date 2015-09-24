
/***************************************************************************
 *            ho_pixbuf_filter.h
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

/** @file ho_pixbuf_filter.h
    @brief hebhocr C language header.
    
    hebhocr - library for Hebrew optical character recognition 
*/

#ifndef HO_PIXBUF_FILTER_H
#define HO_PIXBUF_FILTER_H 1

#include <stdio.h>

#include "ho_pixbuf.h"

/**
 take one channel from an RGB pixbuf
 @param pix the color ho_pixbuf
 @param ch the color channel to use

 @return 0
 */
int ho_pixbuf_filter_channel (const ho_pixbuf * pix, unsigned char ch);

/**
 convert RGB pixbuf to gray
 @param pix the color ho_pixbuf

 @return 0
 */
int ho_pixbuf_filter_gray (const ho_pixbuf * pix);

/**
 convert RGB pixbuf to lineart
 @param pix the color ho_pixbuf

 @return 0
 */
int ho_pixbuf_filter_polarize (const ho_pixbuf * pix, unsigned char threshold);

/**
 run conversion filter on pixbuf
 @param pix the color ho_pixbuf
 @param ch color chanel to filter
 @param convert_array the conversion array to use (type: double[256])
 @param x x offset of filter window
 @param y y offset of filter window
 @param width width of filter window
 @param height height of filter window

 @return 0
 */
int ho_pixbuf_filter_convert (const ho_pixbuf * pix,
  unsigned char ch, double *convert_array, int x, int y, int width, int height);

/**
 run linear filter on pixbuf
 @param pix the color ho_pixbuf
 @param ch color chanel to filter
 @param x x offset of filter window
 @param y y offset of filter window
 @param width width of filter window
 @param height height of filter window

 @return 0
 */
int ho_pixbuf_filter_linear (const ho_pixbuf * pix, unsigned char ch,
  int x, int y, int width, int height, int margin);

/**
 run histogram equalization filter on pixbuf
 @param pix the color ho_pixbuf
 @param ch color chanel to filter
 @param x x offset of filter window
 @param y y offset of filter window
 @param width width of filter window
 @param height height of filter window
 @param margin margin of filter window
 
 @return 0
 */
int ho_pixbuf_filter_histequal (const ho_pixbuf * pix, unsigned char ch,
  int x, int y, int width, int height, int margin);

/**
 run vertical smear filter on pixbuf
 @param pix the color ho_pixbuf
 @param ch color chanel to filter
 @param val color value to filter
 @param step smear step size
 @param moment smear moment size
 
 @return 0
 */
int ho_pixbuf_filter_vsmear (const ho_pixbuf * pix, unsigned char ch,
  unsigned char val, int step, int moment);

/**
 run horizontal smear filter on pixbuf
 @param pix the color ho_pixbuf
 @param ch color chanel to filter
 @param val color value to filter
 @param step smear step size
 @param moment smear moment size
 
 @return 0
 */
int ho_pixbuf_filter_hsmear (const ho_pixbuf * pix, unsigned char ch,
  unsigned char val, int step, int moment);

#endif /* HO_PIXBUF_FILTER_H */
