/***************************************************************************
 *            test_layout.h
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
 * gcc test_layout.c -L../src/.libs/ -lhebocr -I../src/ -o test
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
  ho_pixsum * sums = NULL;
  
  int i;
  
  /* read image and convert to bw */
  /* ---------------------------- */
  
  /* try to read pnm files */
  pix = ho_pixbuf_pnm_load_path("./images/p6.pnm");
  ho_pixbuf_pnm_save_path(pix, "t-p6.pnm");
  
  /* convert image to gray and then to b/w */
  ho_pixbuf_filter_gray(pix);
  ho_pixbuf_filter_polarize(pix, 150);
  ho_pixbuf_pnm_save_path(pix, "t-bw-p6.pnm");
    
  /* run layout filters */
  /* ------------------ */
  
  /* filter by size */
  ho_pixbuf_layout_filter (pix, 0, 10, 100, 10, 100);
  
  /* smear image */
  //ho_pixbuf_filter_hsmear (pix, 0, 0, 10, 0);
  //ho_pixbuf_filter_vsmear (pix, 0, 0, 10, 0);
  //ho_pixbuf_op_mov (pix, 0, 1);
  
  /* create a sums transforamtion of pix */
  sums = ho_pixsum_new(pix->width, pix->height);
  ho_pixsum_from_pixbuf(sums, pix, 0);

  /* segment the image verticaly using the sums map */
  ho_pixbuf_layout_segment_vertical (pix, sums, 0, 
    0, 0, pix->width, pix->height, 0.05);
    
  /* connect small gaps between vertical segments, using smear */
  /* ho_pixbuf_filter_hsmear */
  /*    (pix = pix, chanel = 0, value = 255, gap = 5, moment = 0) */
  ho_pixbuf_filter_hsmear (pix, 0, 0, 5, 0);
  
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
      
    ho_pixbuf_layout_segment_horizontal (pix, sums, 0, 
      ho_objmap_get_object(obj,i).x, ho_objmap_get_object(obj,i).y, 
      ho_objmap_get_object(obj,i).width, ho_objmap_get_object(obj,i).height, 
      0.12);
  }
  
  
  //ho_pixbuf_filter_vsmear (pix, 1, 255, 5, 0);
  
  //ho_pixbuf_layout_filter_by_height (pix, 1, 10, 120);
  //ho_pixbuf_layout_smear(pix, 1, 10, 120);
  
  /* draw a grid with: step_size=100, channel=1, value=0 */
  ho_pixbuf_draw_grid (pix, 100, 2, 0);
  
  /* save layout image */
  ho_pixbuf_pnm_save_path(pix, "t-p6-layout.pnm");
    
  ho_objmap_free(obj);
  ho_pixbuf_free(pix_temp);
  ho_pixbuf_free(pix);

  return 0;
}
