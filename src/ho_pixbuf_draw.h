
/***************************************************************************
 *            ho_pixbuf_draw.h
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

/** @file ho_pixbuf_draw.h
    @brief hebhocr C language header.
    
    hebhocr - library for Hebrew optical character recognition 
*/

#ifndef HO_PIXBUF_DRAW_H
#define HO_PIXBUF_DRAW_H 1

#include <stdio.h>

#include "ho_pixbuf.h"
#include "ho_objmap.h"

/**
 draw objmap object on a pixbuf
 @param pix the color ho_pixbuf
 @param objmap the ho_objmap
 @param x x offset of copy window
 @param y y offset of copy window
 @param width width of copy window
 @param height height of copy window
 @param ch the color channel to use
 @param val the color value to use

 @return 0
 */
int ho_pixbuf_draw_objmap (ho_pixbuf * pix, const ho_objmap * objmap,
  int x, int y, int width, int height, unsigned char ch, unsigned char val);

/**
 draw objmap object on a pixbuf as squares
 @param pix the color ho_pixbuf
 @param objmap the ho_objmap
 @param ch the color channel to use
 @param val the color value to use
 
 @return 0
 */
int ho_pixbuf_draw_objmap_square (ho_pixbuf * pix, const ho_objmap * objmap,
  unsigned char ch, unsigned char val);

/**
 draw objmap object on a pixbuf as squares
 @param pix the color ho_pixbuf
 @param objmap the ho_objmap
 @param ch the color channel to use
 @param val the color value to use
 @param horizontal_margin square margin horizontaly
 @param vertical_margin square margin verticaly
 
 @return 0
 */
int ho_pixbuf_draw_objmap_square_full (ho_pixbuf * pix,
  const ho_objmap * objmap, unsigned char ch, unsigned char val,
  int horizontal_margin, int vertical_margin);

/**
 draw horizontal line on a pixbuf as squares
 @param pix the color ho_pixbuf
 @param x1 start x position
 @param x2 edd x position
 @param y start y position
 @param ch the color channel to use
 @param val the color value to use
 
 @return 0
 */
int ho_pixbuf_draw_hline (ho_pixbuf * pix, int x1, int x2, int y,
  unsigned char ch, unsigned char val);

/**
 draw vertical line on a pixbuf as squares
 @param pix the color ho_pixbuf
 @param x start x position
 @param y1 start x position
 @param y2 end y position
 @param ch the color channel to use
 @param val the color value to use
 
 @return 0
 */
int ho_pixbuf_draw_vline (ho_pixbuf * pix, int x, int y1, int y2,
  unsigned char ch, unsigned char val);

/**
 draw square on a pixbuf as squares
 @param pix the color ho_pixbuf
 @param x1 start x position
 @param x2 end x position
 @param y1 start x position
 @param y2 end y position
 @param ch the color channel to use
 @param val the color value to use
 
 @return 0
 */
int ho_pixbuf_draw_square (ho_pixbuf * pix, int x1, int x2, int y1, int y2,
  unsigned char ch, unsigned char val);

/**
 draw grid on a pixbuf as squares
 @param pix the color ho_pixbuf
 @param step grid steping size
 @param ch the color channel to use
 @param val the color value to use
 
 @return 0
 */
int ho_pixbuf_draw_grid (ho_pixbuf * pix, int step,
  unsigned char ch, unsigned char val);

#endif /* HO_PIXBUF_DRAW_H */
