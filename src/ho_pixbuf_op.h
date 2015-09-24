
/***************************************************************************
 *            ho_pixbuf_op.h
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

/** @file ho_pixbuf_op.h
    @brief hebhocr C language header.
    
    hebhocr - library for Hebrew optical character recognition 
*/

#ifndef HO_PIXBUF_OP_H
#define HO_PIXBUF_OP_H 1

#include <stdio.h>

#include "ho_pixbuf.h"

/**
 do binary operator or on red and green chanels and put result in red chanel
 @param pix the color ho_pixbuf

 @return 0
 */
int ho_pixbuf_op_or (const ho_pixbuf * pix);

/**
 do binary operator and on red and green chanels and put result in red chanel
 @param pix the color ho_pixbuf

 @return 0
 */
int ho_pixbuf_op_and (const ho_pixbuf * pix);

/**
 do binary operator xor on red and green chanels and put result in red chanel
 @param pix the color ho_pixbuf

 @return 0
 */
int ho_pixbuf_op_xor (const ho_pixbuf * pix);

/**
 do operator not on red chanel and put result in red chanel
 @param pix the color ho_pixbuf

 @return 0
 */
int ho_pixbuf_op_not (const ho_pixbuf * pix);

/**
 move ch1 values into ch2
 @param pix the color ho_pixbuf
 @param ch1 chanel to move values from
 @param ch2 chanel to move values into

 @return 0
 */
int ho_pixbuf_op_mov (const ho_pixbuf * pix,
  unsigned char ch1, unsigned char ch2);

#endif /* HO_PIXBUF_OP_H */
