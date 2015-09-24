
/***************************************************************************
 *            ho_objlist_stat.h
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

/** @file ho_objlist_stat.h
    @brief hebhocr C language header.
    
    hebhocr - library for Hebrew optical character recognition 
*/

#ifndef HO_OBJLIST_STAT_H
#define HO_OBJLIST_STAT_H 1

#include "ho_objlist.h"

#define HO_OBJLIST_HISOTGRAM_SIZE 10

/**
 get the minimum and maximum object weight
 @param object_list pointer to an ho_objlist
 @param min output min weignt
 @param max output max weight
 @param avg output average weignt
 
 @return FALSE
 */
int
ho_objlist_stat_minmax_w (ho_objlist * object_list,
  double *min, double *max, double *avg);

/**
 get the common object weight
 @param object_list pointer to an ho_objlist
 @param min input min weignt
 @param max input max weight
 @param com output common weight
 
 @return FALSE
 */
int
ho_objlist_stat_common_w (ho_objlist * object_list,
  double min, double max, double *com);

/**
 get the minimum and maximum object height and width
 @param object_list pointer to an ho_objlist
 @param min_w output min width
 @param max_w output max width
 @param avg_w output average width
 @param min_h output min height
 @param max_h output max height
 @param avg_h output average height
 
 @return lowest FALSE
 */
int
ho_objlist_stat_minmax (ho_objlist * object_list,
  int *min_w, int *max_w, int *avg_w, int *min_h, int *max_h, int *avg_h);

/**
 get the common object height and width
 @param object_list pointer to an ho_objlist
 @param min_w input min width
 @param max_w input min width
 @param com_w output common width
 @param min_h input min height
 @param max_h input min height
 @param com_h output common height
 
 @return lowest FALSE
 */
int
ho_objlist_stat_common (ho_objlist * object_list,
  int min_w, int max_w, int *com_w, int min_h, int max_h, int *com_h);

#endif /* HO_OBJLIST_STAT_H */
