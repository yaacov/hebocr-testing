#!/bin/env python
# pylint: disable=invalid-name,import-error

''' hebOCR is a Hebrew character recognition library
'''

import cv2
import numpy as np

C = 8
FONT = cv2.FONT_HERSHEY_SIMPLEX

class HebOCR(object):
    ''' hebOCR is a Hebrew character recognition library
    '''

    def __init__(self, filename=None, rotate=0):
        self.img = None
        self.gray = None
        self.bw = None
        self.h = None
        self.w = None

        if filename is not None:
            self.load_image(filename, rotate)

    def load_image(self, filename, rotate):
        ''' load color image
        '''
        self.img = cv2.imread(filename)

        if self.img is None:
            return None, None, None

        # rotate image
        self.rows, self.cols = self.img[:, :, 0].shape
        M = cv2.getRotationMatrix2D((self.cols/2, self.rows/2), rotate, 1)
        self.img = cv2.warpAffine(self.img, M, (self.cols, self.rows))

        # recalc rows and cols after rotate
        self.rows, self.cols = self.img[:, :, 0].shape

        # convert image to Gray and B/W images
        self.gray = cv2.cvtColor(self.img, cv2.COLOR_RGB2GRAY)
        self.bw = cv2.adaptiveThreshold(self.gray, 255,
                                        cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
                                        cv2.THRESH_BINARY, 35, 12)

        # clean bw image
        kernel_clean = np.ones((3, 3), np.uint8)
        self.bw = cv2.morphologyEx(self.bw, cv2.MORPH_CLOSE, kernel_clean)

    def estimate_font_size(self, bw):
        ''' find font width and hight aproxymation.
        '''
        bw_neg = cv2.bitwise_not(bw)
        _length, _labels, stats, _centroids = cv2.connectedComponentsWithStats(
            bw_neg, C, cv2.CV_32S)

        ws = stats[:, 2][1:]
        hs = stats[:, 3][1:]
        ws = ws[ws > 5]
        hs = hs[hs > 5]
        self.w = int(np.percentile(ws, 80))
        self.h = int(np.percentile(hs, 80))

        return self.w, self.h

    def lable_columns(self, bw, w=None, h=None):
        ''' find columns
        '''
        w = w or self.w
        h = h or self.h

        kernel = np.ones((3, int(w * 2)), np.uint8)
        bw_c = cv2.morphologyEx(bw, cv2.MORPH_OPEN, kernel)

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
        # pylint: disable=too-many-locals
        w = w or self.w
        h = h or self.h
        rows, cols = labels.shape

        # Filter stats
        _stats = []
        for stat in stats[1:]:
            _x, _y, cw, ch, _a = stat
            if cw > (w * 4) and ch > (h * 4):
                _stats.append(stat)

        # Redraw column labels
        labels_out = np.zeros((rows, cols), np.uint8)
        stats_out = sorted(_stats, key=lambda x: -x[0])
        for i, stat in enumerate(stats_out):
            x, _y, cw, _ch, _a = stat
            labels_out[:, x:(x+cw)] = i + 1

        return labels_out, stats_out

    def lable_lines(self, bw, w=None, h=None, _i=0):
        ''' find lines
        '''
        w = w or self.w
        h = h or self.h

        #cv2.imwrite('{}.lines.in.png'.format(i), bw)

        kernel = np.ones((3, int(w * 6)), np.uint8)
        bw_c = cv2.morphologyEx(bw, cv2.MORPH_OPEN, kernel)

        kernel = np.ones((3, int(w * 0.3)), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)

        kernel = np.ones((int(h * 0.5), 3), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)

        kernel = np.ones((3, int(w * 6)), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

        if h > 25:
            kernel = np.ones((int(h * 0.5), 3), np.uint8)
            bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)

            kernel = np.ones((int(h * 0.5), int(w * 0.5)), np.uint8)
            bw_c = cv2.dilate(bw_c, kernel, iterations=1)

            kernel = np.ones((3, int(w * 6)), np.uint8)
            bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

            kernel = np.ones((3, int(w * 6)), np.uint8)
            bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)
        else:
            kernel = np.ones((3, int(w * 6)), np.uint8)
            bw_c = cv2.morphologyEx(bw, cv2.MORPH_OPEN, kernel)

            kernel = np.ones((3, int(w * 6)), np.uint8)
            bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)

        kernel = np.ones((int(h * 0.4), int(w * 0.4)), np.uint8)
        bw_c = cv2.erode(bw_c, kernel, iterations=1)

        #cv2.imwrite('{}.lines.png'.format(i), bw_c)

        # Labeling
        bw_neg = cv2.bitwise_not(bw_c)
        _length, labels, stats, _centroids = cv2.connectedComponentsWithStats(
            bw_neg, C, cv2.CV_32S)

        return labels, stats


    def label_words(self, bw, line, w=None, h=None, _i=0):
        ''' find worlds
        '''
        w = w or self.w
        h = h or self.h

        #cv2.imwrite('{}.words.in.png'.format(i), line)

        kernel = np.ones((int(h * 0.5), int(w * 4)), np.uint8)
        bw_c = cv2.erode(line, kernel, iterations=2)

        kernel = np.ones((3, int(w * 8)), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

        bw_c = cv2.bitwise_or(bw_c, bw)

        kernel = np.ones((int(h * 1), 3), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

        kernel = np.ones((3, int(w * 0.5)), np.uint8)
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

        #cv2.imwrite('{}.words.png'.format(i), bw_c)

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
    if hebocr.img is None:
        print('Can\'t load image {}'.format(args.image))
        sys.exit()

    # take a copy of input image
    img = hebocr.img.copy()
    gray = hebocr.gray.copy()
    bw = hebocr.bw.copy()

    # ------------------------------------
    # save input gray image
    # ------------------------------------
    bw_filename = './out/00.gray.png'
    cv2.imwrite(bw_filename, hebocr.gray)
    print('gray image saved to: {f}'.format(f=bw_filename))

    w, h = hebocr.estimate_font_size(hebocr.bw)
    print('estimated font size: w:{w} h:{h}'.format(w=w, h=h))

    # ------------------------------------
    # label columns
    # ------------------------------------
    _labels_c, _stats_c = hebocr.lable_columns(bw, w, h)
    labels_c, stats_c = hebocr.adjust_colums(_labels_c, _stats_c, w, h)
    for i, sc in enumerate(stats_c):
        print("column {}".format(sc))

        column = hebocr.bw.copy()
        column[labels_c != (i+1)] = 255

        # ------------------------------------
        # label lines
        # ------------------------------------
        labels_l, stats_l = hebocr.lable_lines(column, w, h, i)
        for j, s in enumerate(stats_l[1:]):
            print("lines {}".format(s))

            line = hebocr.bw.copy()
            line[labels_l != (j+1)] = 255

            cv2.putText(img, 'L-{}-{}'.format(i, j), (sc[0], s[1]), FONT, 0.8,
                        (255, 0, 0), 2, cv2.LINE_AA)

            # ------------------------------------
            # label words
            # ------------------------------------
            _labels, stats = hebocr.label_words(bw, line, w, h, j)
            sw = sorted(stats[1:], key=lambda x: -x[0])
            for k, stat in enumerate(sw):
                x, y, cw, ch, _a = stat

                _ret, th = cv2.threshold(gray[y:y+ch, x:x+cw], 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
                cv2.imwrite('./fonts/{}.{}.{}.png'.format(i, j, k), th)

                cv2.putText(img, 'W{}'.format(k), (x, y), FONT, 0.5,
                            (0, 0, 255), 1, cv2.LINE_AA)
                cv2.rectangle(img, (x, y), (x+cw, y+ch), (120, 0, 120), 3)

    # ------------------------------------
    # save output image
    # ------------------------------------
    labels_filename = './out/00.labels.png'
    cv2.imwrite(labels_filename, img)
    print('labels image saved to: {f}'.format(f=labels_filename))

if __name__ == "__main__":
    main()
