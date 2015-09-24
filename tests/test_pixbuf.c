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
 * gcc test_pixbuf.c -L../src/.libs/ -lhebocr -I../src/ -o test
 * run:
 * LD_LIBRARY_PATH=../src/.libs/:$LD_LIBRARY_PATH ./test
 */

#include <stdio.h>

#include "hebocr.h"

int
main (int argc, char * argv[]) {
  int x;
  int y;
  int width;
  int height;
  int window_size;
  int window_margin;
  
  ho_pixbuf * pix = NULL;
  ho_pixbuf * pix_temp = NULL;

  /* try to read pnm files */
  pix = ho_pixbuf_pnm_load_path("./images/p4.pbm");
  ho_pixbuf_pnm_save_path(pix, "t-p4.pnm");
  ho_pixbuf_free(pix);

  pix = ho_pixbuf_pnm_load_path("./images/p5.pnm");
  ho_pixbuf_pnm_save_path(pix, "t-p5.pnm");
  ho_pixbuf_free(pix);

  pix = ho_pixbuf_pnm_load_path("./images/p6.pnm");
  ho_pixbuf_pnm_save_path(pix, "t-p6.pnm");

  /* get image size */
  x = 0;
  y = 0;
  width = pix->width;
  height = pix->height;
  window_size = 300;
  window_margin = 30;
  
  /* run filters */
  pix_temp = ho_pixbuf_copy (pix);
  ho_pixbuf_filter_gray(pix_temp);
  ho_pixbuf_pnm_save_path(pix_temp, "t-p6-gray.pnm");
  ho_pixbuf_free(pix_temp);

  pix_temp = ho_pixbuf_copy (pix);
  ho_pixbuf_filter_polarize(pix_temp, 150);
  ho_pixbuf_pnm_save_path(pix_temp, "t-p6-polarize.pnm");
  ho_pixbuf_free(pix_temp);

  pix_temp = ho_pixbuf_copy (pix);
  ho_pixbuf_filter_linear(pix_temp, 0, x, y, width, height, window_margin);
  ho_pixbuf_filter_linear(pix_temp, 1, x, y, width, height, window_margin);
  ho_pixbuf_filter_linear(pix_temp, 2, x, y, width, height, window_margin);
  ho_pixbuf_pnm_save_path(pix_temp, "t-p6-linear.pnm");
  ho_pixbuf_free(pix_temp);
  
  pix_temp = ho_pixbuf_copy (pix);
  ho_pixbuf_filter_histequal(pix_temp, 0, x, y, width, height, window_margin);
  ho_pixbuf_filter_histequal(pix_temp, 1, x, y, width, height, window_margin);
  ho_pixbuf_filter_histequal(pix_temp, 2, x, y, width, height, window_margin);
  ho_pixbuf_pnm_save_path(pix_temp, "t-p6-histequal.pnm");
  ho_pixbuf_free(pix_temp);
  
  ho_pixbuf_free(pix);

  return 0;
}
