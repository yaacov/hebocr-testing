
/***************************************************************************
 *            ho_pixbuf_layout.h
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

/** @file ho_pixbuf_layout.h
    @brief hebhocr C language header.
    
    hebhocr - library for Hebrew optical character recognition 
*/

#ifndef HO_PIXBUF_LAYOUT_H
#define HO_PIXBUF_LAYOUT_H 1

#include "ho_pixbuf.h"
#include "ho_pixsum.h"

/**
 return filtered image of on pixbuf
 @param pix the color ho_pixbuf
 @param ch color channel to filter
 @param min_width min width in pixels of filtered in objects
 @param max_width max width in pixels of filtered in objects
 @param min_height min height in pixels of filtered in objects
 @param max_height max height in pixels of filtered in objects
 * 
 @return 0
 */
int ho_pixbuf_layout_filter (const ho_pixbuf * pix,
  unsigned char ch, int min_width, int max_width,
  int min_height, int max_height);

/**
 return smeared image of on pixbuf
 @param pix the color ho_pixbuf
 @param ch color channel to filter
 @param hsmear horizontal smear in pixesl
 @param vsmear horizontal smear in pixesl
 
 @return 0
 */
int ho_pixbuf_layout_smear (const ho_pixbuf * pix,
  unsigned char ch, int hsmear, int vsmear);

/**
 return vertically segmented image of on pixbuf
 @param pix the color ho_pixbuf
 @param ch color channel to filter
 @param x the left pos of segemented window
 @param y the bottom pos of segemented window
 @param width the width of segemented window
 @param height the height of segemented window
 @param threshold the threshold for segmentation
 
 @return 0
 */
int ho_pixbuf_layout_segment_vertical (const ho_pixbuf * pix, ho_pixsum * sums,
  unsigned char ch, int x, int y, int width, int height, double threshold);

/**
 return horizontaly segmented image of on pixbuf
 @param pix the color ho_pixbuf
 @param ch color channel to filter
 @param x the left pos of segemented window
 @param y the bottom pos of segemented window
 @param width the width of segemented window
 @param height the height of segemented window
 @param threshold the threshold for segmentation
 
 @return 0
 */
int ho_pixbuf_layout_segment_horizontal (const ho_pixbuf * pix,
  ho_pixsum * sums, unsigned char ch, int x, int y, int width, int height,
  double threshold);

#endif /* HO_PIXBUF_LAYOUT_H */
