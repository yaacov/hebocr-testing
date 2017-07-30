#!/bin/env python
# pylint: disable=import-error,invalid-name

''' hebOCR is a Hebrew character recognition library
'''

from __future__ import print_function
import cv2
import numpy as np

C = 8
FONT = cv2.FONT_HERSHEY_SIMPLEX

class HebOCR(object):
    ''' hebOCR is a Hebrew character recognition library
    '''

    def __init__(self, filename=None, rotate=0):
        self.color_img = None
        self.gray_img = None
        self.bw_img = None
        self.h = None
        self.w = None

        if filename is not None:
            self.load_image(filename, rotate)

    def load_image(self, filename, rotate):
        ''' load color image
        '''
        self.color_img = cv2.imread(filename)

        if self.color_img is None:
            return None, None, None

        # get image dimentions
        rows, cols = self.color_img[:, :, 0].shape

        # rotate image
        rotation_matrix = cv2.getRotationMatrix2D(
            (cols/2, rows/2), rotate, 1)
        self.color_img = cv2.warpAffine(
            self.color_img, rotation_matrix, (cols, rows))

        # convert image to Gray and B/W images
        self.gray_img = cv2.cvtColor(self.color_img, cv2.COLOR_RGB2GRAY)
        self.bw_img = cv2.adaptiveThreshold(self.gray_img, 255,
                                            cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
                                            cv2.THRESH_BINARY, 35, 12)

        # clean bw_img image
        kernel_clean = np.ones((3, 3), np.uint8)
        self.bw_img = cv2.morphologyEx(self.bw_img, cv2.MORPH_CLOSE, kernel_clean)

    def estimate_font_size(self, bw_img):
        ''' find font width and hight aproxymation.
        '''
        bw_neg = cv2.bitwise_not(bw_img)
        _length, _labels, stats, _centroids = cv2.connectedComponentsWithStats(
            bw_neg, C, cv2.CV_32S)

        ws = stats[:, 2][1:]
        hs = stats[:, 3][1:]
        ws = ws[ws > 5]
        hs = hs[hs > 5]
        self.w = int(np.percentile(ws, 80))
        self.h = int(np.percentile(hs, 80))

        return self.w, self.h

    def remove_small_objects(self, bw_img, min_w, min_h):
        ''' remove objects based on size
        '''

        bw_c = bw_img.copy()

        bw_neg = cv2.bitwise_not(bw_img)
        _length, labels, stats, _centroids = cv2.connectedComponentsWithStats(
            bw_neg, C, cv2.CV_32S)

        for stat in stats:
            x, y, w, h, a = stat
            if w < min_w and h < min_h:
                print(x,x + w, y,y + h)
                bw_c[x:x+w,y:y+h] = 255

        cv2.imwrite('lines-{}.png'.format(0), bw_c)

        return bw_c

    def lable_columns(self, bw_img, w=None, h=None):
        ''' find columns
        '''
        w = w or self.w
        h = h or self.h
        bw_c = bw_img.copy()

        kernel = np.ones((3, int(w * 2)), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

        kernel = np.ones((int(h * 5), 3), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

        kernel = np.ones((3, int(w * 2)), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

        kernel = np.ones((int(h), 3), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)

        # Labeling
        bw_neg = cv2.bitwise_not(bw_c)
        _length, labels, stats, _centroids = cv2.connectedComponentsWithStats(
            bw_neg, C, cv2.CV_32S)

        return labels, stats

    def adjust_colums(self, labels, stats, w=None, h=None):
        ''' adjust columns shape
        '''
        w = w or self.w
        h = h or self.h
        rows, cols = labels.shape
        _labels = np.zeros((rows, cols), np.uint8)

        # Filter stats
        _stats = [s for s in stats[1:] if s[2] > (w * 4) and s[3] > (h * 4)]
        _stats = sorted(_stats, key=lambda x: -x[0])

        # Redraw column labels
        for i, stat in enumerate(_stats):
            _labels[:, stat[0]:(stat[0]+stat[2])] = i + 1

        return _labels, _stats

    def lable_lines(self, bw_img, w=None, h=None, _i=0):
        ''' find lines
        '''
        w = w or self.w
        h = h or self.h
        bw_c = bw_img.copy()

        bw_c = self.remove_small_objects(bw_c, int(w * 0.9), int(h * 0.9))

        kernel = np.ones((1, int(w * 0.8)), np.uint8)
        #bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

        kernel = np.ones((1, int(w * 1.5)), np.uint8)
        #bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)

        kernel = np.ones((int(h * 0.1), int(w * 0.1)), np.uint8)
        #bw_c = cv2.dilate(bw_c, kernel, iterations=1)

        self.color_img[:,:,0] = bw_c
        cv2.imwrite('linesbw{}.png'.format(_i), bw_c)
        cv2.imwrite('lines{}.png'.format(_i), self.color_img)

        # Labeling
        bw_neg = cv2.bitwise_not(bw_c)
        _length, labels, stats, _centroids = cv2.connectedComponentsWithStats(
            bw_neg, C, cv2.CV_32S)

        return labels, stats


    def label_words(self, bw_img, line, w=None, h=None, _i=0):
        ''' find worlds
        '''
        w = w or self.w
        h = h or self.h
        bw_c = line.copy()

        kernel = np.ones((int(h * 0.5), int(w * 4)), np.uint8)
        bw_c = cv2.erode(bw_c, kernel, iterations=2)

        kernel = np.ones((3, int(w * 8)), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

        bw_c = cv2.bitwise_or(bw_c, bw_img)

        kernel = np.ones((int(h * 1), 3), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

        kernel = np.ones((3, int(w * 0.5)), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

        # Labeling
        bw_neg = cv2.bitwise_not(bw_c)
        _length, labels, stats, _centroids = cv2.connectedComponentsWithStats(
            bw_neg, C, cv2.CV_32S)

        return labels, stats

def main():
    ''' command line
    '''
    import argparse
    import sys

    # ------------------------------------
    # parse cli
    # ------------------------------------
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--image', type=str, default='in/001.jpg',
                        metavar='FILE', help='image file')
    parser.add_argument('-r', '--rotate', type=int, default=0, metavar='A',
                        help='rotate image A deg')
    args = parser.parse_args()

    # ------------------------------------
    # load image
    # ------------------------------------
    hebocr = HebOCR(args.image, args.rotate)
    if hebocr.color_img is None:
        print('Can\'t load image {}'.format(args.image))
        sys.exit()

    # take a copy of input image
    color_img = hebocr.color_img.copy()
    gray_img = hebocr.gray_img.copy()
    bw_img = hebocr.bw_img.copy()

    # ------------------------------------
    # save input gray_img image
    # ------------------------------------
    bw_filename = './out/00.gray_img.png'
    cv2.imwrite(bw_filename, hebocr.gray_img)
    print('gray_img image saved to: {f}'.format(f=bw_filename))

    w, h = hebocr.estimate_font_size(hebocr.bw_img)
    print('estimated font size: w:{w} h:{h}'.format(w=w, h=h))

    lines = bw_img.copy()

    # ------------------------------------
    # label columns
    # ------------------------------------
    _labels_c, _stats_c = hebocr.lable_columns(bw_img, w, h)
    labels_c, stats_c = hebocr.adjust_colums(_labels_c, _stats_c, w, h)
    for i, stat_c in enumerate(stats_c):
        print("column {}".format(stat_c))

        column = hebocr.bw_img.copy()
        column[labels_c != (i+1)] = 255

        # ------------------------------------
        # label lines
        # ------------------------------------
        labels_l, stats_l = hebocr.lable_lines(column, w, h, i)
        for j, stat_l in enumerate(stats_l[1:]):
            print("lines {}".format(stat_l))
            break

            line = hebocr.bw_img.copy()
            line[labels_l != (j+1)] = 255
            lines[labels_l == (j+1)] = 0

            cv2.putText(color_img, 'L{}'.format(j), (stat_c[0] - 45, stat_l[1] + 20),
                        FONT, 0.6, (255, 0, 0), 1, cv2.LINE_AA)

            # ------------------------------------
            # label words
            # ------------------------------------
            _labels, stats_w = hebocr.label_words(bw_img, line, w, h, j)
            stats_w = sorted(stats_w, key=lambda x: -x[0])
            for k, stat_w in enumerate(stats_w):
                x, y, cw, ch, _a = stat_w
                if cw > (w * 0.8) or ch > (h * 0.5):
                    cv2.imwrite('./fonts/{}.{}.{}.png'.format(i, j, k),
                                gray_img[y:y+ch, x:x+cw])

                    cv2.putText(color_img, 'W{}'.format(k), (x, y), FONT, 0.5,
                                (0, 0, 255), 1, cv2.LINE_AA)
                    cv2.rectangle(color_img, (x, y), (x+cw, y+ch), (120, 0, 120), 1)

    # ------------------------------------
    # save output image
    # ------------------------------------
    color_img[:, :, 0] = lines
    labels_filename = './out/00.labels.png'
    cv2.imwrite(labels_filename, color_img)
    print('labels image saved to: {f}'.format(f=labels_filename))

if __name__ == "__main__":
    main()
