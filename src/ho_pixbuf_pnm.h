
/***************************************************************************
 *            ho_pixbuf_pnm.h
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

/** @file ho_pixbuf_pnm.h
    @brief hebhocr C language header.
    
    hebhocr - library for Hebrew optical character recognition 
*/

#ifndef HO_PIXBUF_PNM_H
#define HO_PIXBUF_PNM_H 1

#include <stdio.h>

#include "ho_pixbuf.h"

/**
 read ho_pixbuf from pnm file 1 or 24 bpp
 @param file pnm file to read

 @return newly allocated ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_pnm_load (FILE * file);

/**
 writes ho_pixbuf to pnm file
 @param pix ho_pixbuf
 @param file save to file

 @return FALSE
 */
int ho_pixbuf_pnm_save (const ho_pixbuf * pix, FILE * file);

/**
 read ho_pixbuf from pnm file 1 or 24 bpp
 @param filename pnm filename path to read

 @return newly allocated ho_pixbuf
 */
ho_pixbuf *ho_pixbuf_pnm_load_path (const char *filename);

/**
 writes ho_pixbuf to pnm file
 @param pix ho_pixbuf
 @param filename save to filename path

 @return FALSE
 */
int ho_pixbuf_pnm_save_path (const ho_pixbuf * pix, const char *filename);

#endif /* HO_PIXBUF_PNM_H */
