import cv2
import numpy as np
import argparse
import sys
import logging

logger = logging.getLogger()
logger.setLevel(logging.INFO)

# ------------------------------------
# parse cli
# ------------------------------------
parser = argparse.ArgumentParser()
parser.add_argument('-i', '--image', type=str, default='in.jpg', metavar='FILE', help='image file')
parser.add_argument('-r', '--rotate', type=int, default=0, metavar='A', help='rotate image A deg')
args = parser.parse_args()

C = 8
font = cv2.FONT_HERSHEY_SIMPLEX

def load_image(filename, rotate):
    ''' load color image
    '''

    img = cv2.imread( filename )

    if img is None:
        logging.error('can\'t load image file')
        sys.exit()

    # rotate image
    rows, cols = img[:,:,0].shape
    M = cv2.getRotationMatrix2D( (cols/2, rows/2), rotate, 1 )
    img = cv2.warpAffine (img, M, (cols, rows) )

    # convert image to Gray and B/W images
    gray = cv2.cvtColor( img, cv2.COLOR_RGB2GRAY )
    bw = cv2.adaptiveThreshold( gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 35, 12 )

    # clean bw image
    kernel_clean = np.ones( (3, 3), np.uint8 )
    bw = cv2.morphologyEx(bw, cv2.MORPH_CLOSE, kernel_clean)

    return img, gray, bw

def estimate_font_size(bw):
    ''' find font width and hight aproxymation.
    '''

    _, bw1 = cv2.threshold(bw, 220, 255, cv2.THRESH_BINARY_INV);
    length, labels, stats, centroids = cv2.connectedComponentsWithStats(bw1, C, cv2.CV_32S)

    ws = stats[:,2][1:]
    hs = stats[:,3][1:]
    ws = ws[ws > 5]
    hs = hs[hs > 5]
    w = int(np.percentile(ws, 80))
    h = int(np.percentile(hs, 80))

    return w, h

def lable_columns(bw, w, h):
    ''' find columns
    '''
    kernel = np.ones( (3, int(w * 2)), np.uint8 )
    bw_c = cv2.morphologyEx(bw, cv2.MORPH_OPEN, kernel)

    kernel = np.ones( (int(h * 5), 3), np.uint8 )
    bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

    kernel = np.ones( (3, int(w * 2)), np.uint8 )
    bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

    kernel = np.ones( (int(h), 3), np.uint8 )
    bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)

    # Labeling
    bw_neg = cv2.bitwise_not(bw_c)
    length, labels, stats, centroids = cv2.connectedComponentsWithStats(bw_neg, C, cv2.CV_32S)

    # Filter stats
    s = []
    for i, stat in enumerate(stats[1:]):
        x,y,cw,ch,a = stat
        if cw > (w * 4) and ch > (h * 4):
            s.append(stat)

    # Redraw column labels
    rows, cols = labels.shape
    l = np.zeros((rows, cols), np.uint8)
    sc = sorted(s, key=lambda x: -x[0])
    for i, stat in enumerate(sc):
        x,y,cw,ch,a = stat
        l[:, x:(x+cw)] = i + 1

    return l, sc

def lable_lines(bw, w, h, i):
    ''' find lines
    '''
    #cv2.imwrite('{}.lines.in.png'.format(i), bw)

    kernel = np.ones( (3, int(w * 6)), np.uint8 )
    bw_c = cv2.morphologyEx(bw, cv2.MORPH_OPEN, kernel)

    kernel = np.ones( (3, int(w * 0.3)), np.uint8 )
    bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)

    kernel = np.ones( (int(h * 0.5), 3), np.uint8 )
    bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)

    kernel = np.ones( (3, int(w * 6)), np.uint8 )
    bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

    if h > 25:
        kernel = np.ones( (int(h * 0.5), 3), np.uint8 )
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)

        kernel = np.ones( (int(h * 0.5), int(w * 0.5)), np.uint8 )
        bw_c = cv2.dilate(bw_c, kernel, iterations = 1)

        kernel = np.ones( (3, int(w * 6)), np.uint8 )
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

        kernel = np.ones( (3, int(w * 6)), np.uint8 )
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)
    else:
        kernel = np.ones( (3, int(w * 6)), np.uint8 )
        bw_c = cv2.morphologyEx(bw, cv2.MORPH_OPEN, kernel)

        kernel = np.ones( (3, int(w * 6)), np.uint8 )
        bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_CLOSE, kernel)

    kernel = np.ones( (int(h * 0.4), int(w * 0.4)), np.uint8 )
    bw_c = cv2.erode(bw_c, kernel, iterations = 1)

    cv2.imwrite('{}.lines.png'.format(i), bw_c)

    # Labeling
    bw_neg = cv2.bitwise_not(bw_c)
    length, labels, stats, centroids = cv2.connectedComponentsWithStats(bw_neg, C, cv2.CV_32S)

    return labels, stats


def label_words(bw, line, w, h, i):
    ''' find worlds
    '''
    #cv2.imwrite('{}.words.in.png'.format(i), line)

    kernel = np.ones( (int(h * 0.5), int(w * 4)), np.uint8 )
    bw_c = cv2.erode(line, kernel, iterations = 2)

    kernel = np.ones( (3, int(w * 8)), np.uint8 )
    bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

    bw_c = cv2.bitwise_or(bw_c, bw)

    kernel = np.ones( (int(h * 1), 3), np.uint8 )
    bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

    kernel = np.ones( (3, int(w * 0.5)), np.uint8 )
    bw_c = cv2.morphologyEx(bw_c, cv2.MORPH_OPEN, kernel)

    #cv2.imwrite('{}.words.png'.format(i), bw_c)

    # Labeling
    bw_neg = cv2.bitwise_not(bw_c)
    length, labels, stats, centroids = cv2.connectedComponentsWithStats(bw_neg, C, cv2.CV_32S)

    return labels, stats

img, gray, bw = load_image(args.image, args.rotate)

bw_filename = '00.hocr.bw.png'
cv2.imwrite(bw_filename, gray)
logging.info( 'b/w image saved to: {f}'.format(f=bw_filename) )

w, h = estimate_font_size(bw)
logging.info( 'estimated font size: w:{w} h:{h}'.format(w=w, h=h) )

# loop on all columns
labels_c, stats_c = lable_columns(bw, w, h)
for i, sc in enumerate(stats_c):
    #print "column", sc

    column = bw.copy()
    column[labels_c != (i+1)] = 255

    # loop on all lines
    labels_l, stats_l = lable_lines(column, w, h, i)
    for j, s in enumerate(stats_l[1:]):
        #print "lines", s

        line = bw.copy()
        line[labels_l != (j+1)] = 255

        cv2.putText(img,'L-{}-{}'.format(i, j), (sc[0], s[1]), font, 0.8, (255, 0, 0), 2, cv2.LINE_AA)

        labels, stats = label_words(bw, line, w, h, j)

        sw = sorted(stats[1:], key=lambda x: -x[0])
        for k, stat in enumerate(sw):
            x,y,cw,ch,a = stat
            cv2.imwrite('fonts/{}.{}.{}.png'.format(i, j, k), gray[y:y+ch, x:x+cw])

            cv2.putText(img,'W{}'.format(k), (x, y), font, 0.5, (0, 0, 255), 1, cv2.LINE_AA)
            cv2.rectangle(img, (x, y), (x+cw, y+ch), (120,0,120), 3)


# -------------------------------------

labels_filename = '00.labels.png'
cv2.imwrite( labels_filename, img )
logging.info( 'labels image saved to: {f}'.format(f=labels_filename) )
