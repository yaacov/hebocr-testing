
/***************************************************************************
 *            ho_objmap_filter.h
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

/** @file ho_objmap_filter.h
    @brief hebhocr C language header.
    
    hebhocr - library for Hebrew optical character recognition 
*/

#ifndef HO_OBJMAP_FILTER_H
#define HO_OBJMAP_FILTER_H 1

#include <ho_objlist.h>
#include <ho_objmap.h>

/**
 filter objects by width size
 @param obj the object map
 @param min minimum width to pass the filter
 @param max maximum width to pass the filter
 
 @return 0
 */
int ho_objmap_filter_by_width (ho_objmap * obj, int min, int max);

/**
 filter objects by height size
 @param obj the object map
 @param min minimum height to pass the filter
 @param max maximum height to pass the filter
 
 @return 0
 */
int ho_objmap_filter_by_height (ho_objmap * obj, int min, int max);

/**
 filter objects by width and height sizes
 @param obj the object map
 @param min_w minimum width to pass the filter
 @param max_w maximum width to pass the filter
 @param min_h minimum height to pass the filter
 @param max_h maximum height to pass the filter
 
 @return 0
 */
int ho_objmap_filter_by_width_and_height (ho_objmap * obj,
  int min_w, int max_w, int min_h, int max_h);

/**
 filter objects by width / height ratio
 @param obj the object map
 @param min_ratio minimum ratio to pass the filter
 @param max_ratio maximum ratio to pass the filter
 
 @return 0
 */
int ho_objmap_filter_by_width_and_height_ratio (ho_objmap * obj,
  double min_ratio, double max_ratio);

/**
 filter objects by weight
 @param obj the object map
 @param min minimum weight to pass the filter
 @param max maximum weight to pass the filter
 
 @return 0
 */
int ho_objmap_filter_by_weight (ho_objmap * obj, int min, int max);

#endif /* HO_OBJMAP_FILTER_H */
