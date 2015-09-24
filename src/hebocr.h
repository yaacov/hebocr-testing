
/***************************************************************************
 *            hebocr.h
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

/** @mainpage hebOCR
 *
 * @ref README @ref HACKING @ref ChangeLog
 *
 * Library for Hebrew optical character recognition 
 *
 * http://code.google.com/r/kobizamir-hebocr/
 *
 * @section README
 * @include README
 * 
 * @section HACKING
 * @include HACKING
 * 
 * @section ChangeLog
 * @include ChangeLog
 *
 */

/** @file hebocr.h
    @brief hebhocr C language header.
    
    hebhocr - library for Hebrew optical character recognition 
*/

#ifndef HOCR_H
#define HOCR_H 1

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

/* color image map */
#include <ho_pixbuf.h>
#include <ho_pixbuf_pnm.h>
#include <ho_pixbuf_filter.h>
#include <ho_pixbuf_op.h>
#include <ho_pixbuf_draw.h>
#include <ho_pixbuf_layout.h>

/* connected objects list */
#include <ho_objlist.h>
#include <ho_objlist_stat.h>

/* connected objects map */
#include <ho_objmap.h>
#include <ho_objmap_filter.h>

/* image sums map */
#include <ho_pixsum.h>

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif                          /* HOCR_H */
