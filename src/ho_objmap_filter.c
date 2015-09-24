
/***************************************************************************
 *            ho_objmap_filter.c
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "ho_objmap_filter.h"

int
ho_objmap_filter_by_width (ho_objmap * obj, int min, int max) {
  unsigned int i;

  /* filter the obj list */
  for (i = 1; i <= ho_objmap_get_size (obj); i++)
    if (ho_objmap_get_object (obj, i).width < min ||
      ho_objmap_get_object (obj, i).width > max) {
      ho_objmap_get_object (obj, i).index = 0;
    }

  /* reorder the matrix */
  return ho_objmap_reorder (obj);
}

int
ho_objmap_filter_by_height (ho_objmap * obj, int min, int max) {
  unsigned int i;

  /* filter the obj list */
  for (i = 1; i <= ho_objmap_get_size (obj); i++)
    if (ho_objmap_get_object (obj, i).height < min ||
      ho_objmap_get_object (obj, i).height > max) {
      ho_objmap_get_object (obj, i).index = 0;
    }

  /* reorder the matrix */
  return ho_objmap_reorder (obj);
}

int
ho_objmap_filter_by_width_and_height (ho_objmap * obj,
  int min_w, int max_w, int min_h, int max_h) {
  unsigned int i;

  /* filter the obj list */
  for (i = 1; i <= ho_objmap_get_size (obj); i++)
    if ((ho_objmap_get_object (obj, i).width < min_w ||
        ho_objmap_get_object (obj, i).width > max_w) &&
      (ho_objmap_get_object (obj, i).height < min_h ||
        ho_objmap_get_object (obj, i).height > max_h)) {
      ho_objmap_get_object (obj, i).index = 0;
    }

  /* reorder the matrix */
  return ho_objmap_reorder (obj);
}

int
ho_objmap_filter_by_width_and_height_ratio (ho_objmap * obj,
  double min_ratio, double max_ratio) {
  unsigned int i;
  double ratio;

  /* filter the obj list */
  for (i = 1; i <= ho_objmap_get_size (obj); i++) {
    /* calculate current item w/h ratio */
    ratio = 0.0;
    if (ho_objmap_get_object (obj, i).height != 0.0)
      ratio = ho_objmap_get_object (obj, i).width /
        ho_objmap_get_object (obj, i).height;

    /* filter object */
    if (ratio < min_ratio || ratio > max_ratio) {
      ho_objmap_get_object (obj, i).index = 0;
    }
  }

  /* reorder the matrix */
  return ho_objmap_reorder (obj);
}

int
ho_objmap_filter_by_weight (ho_objmap * obj, int min, int max) {
  unsigned int i;

  /* filter the obj list */
  for (i = 1; i <= ho_objmap_get_size (obj); i++)
    if (ho_objmap_get_object (obj, i).weight < min ||
      ho_objmap_get_object (obj, i).weight > max) {
      ho_objmap_get_object (obj, i).index = 0;
    }

  /* reorder the matrix */
  return ho_objmap_reorder (obj);
}
