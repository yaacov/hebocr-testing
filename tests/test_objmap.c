/***************************************************************************
 *            test_pixbuf.h
 *
 *  Thursday, January 20 2011
 *  Copyright  2011  Yaacov Zamir
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

/* compile:
 * gcc test_objmap.c -L../src/.libs/ -lhebocr -I../src/ -o test
 * run:
 * LD_LIBRARY_PATH=../src/.libs/:$LD_LIBRARY_PATH ./test
 */

#include <stdio.h>

#include "hebocr.h"

int
main (int argc, char * argv[]) {
  ho_pixbuf * pix = NULL;
  ho_pixbuf * pix_temp = NULL;
  ho_objmap * obj = NULL;
  int i;
  
  /* try to read pnm files */
  pix = ho_pixbuf_pnm_load_path("./images/p6.pnm");
  ho_pixbuf_pnm_save_path(pix, "t-p6.pnm");
  
  /* run filters */
  ho_pixbuf_filter_gray(pix);

  /* save bw image */
  ho_pixbuf_pnm_save_path(pix, "t-p6-gr.pnm");
  
  ho_pixbuf_filter_polarize(pix, 150);

  /* save bw image */
  ho_pixbuf_pnm_save_path(pix, "t-p6-bw.pnm");

  /* link broken fonts */
  ho_pixbuf_filter_vsmear (pix, 0, 0, 5, 0);
        
  /* create objmap */
  obj = ho_objmap_new(pix->width, pix->height);
  ho_objmap_from_pixbuf (obj, pix, 0);
  
  /* loop on all the objects and dump to disk */
  for (i = 1; i <= ho_objmap_get_size(obj); i++) {

    printf("o%d %d %d %d %d 0\n", i,
      ho_objmap_get_object(obj,i).x,
      pix->height - ho_objmap_get_object(obj,i).y - ho_objmap_get_object(obj,i).height,
      ho_objmap_get_object(obj,i).x + ho_objmap_get_object(obj,i).width,
      pix->height - ho_objmap_get_object(obj,i).y);
  }
  
  /* draw objmap */
  pix_temp = ho_pixbuf_new (pix->width, pix->height);
  ho_pixbuf_draw_objmap_square (pix_temp, obj, 0, 255);
  ho_pixbuf_draw_objmap (pix_temp, obj, 0, 0, pix->width, pix->height, 1, 255);
  ho_pixbuf_draw_objmap (pix_temp, obj, 0, 0, pix->width, pix->height, 2, 255);
  
  /* draw a grid with: step_size=100, channel=1, value=0 */
  ho_pixbuf_draw_grid (pix_temp, 100, 1, 0);
  
  /* save objmap image */
  ho_pixbuf_pnm_save_path(pix_temp, "t-p6-objmap.pnm");
  
  ho_objmap_free(obj);
  ho_pixbuf_free(pix_temp);
  ho_pixbuf_free(pix);

  return 0;
}
