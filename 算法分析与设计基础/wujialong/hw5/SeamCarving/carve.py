import cv2
import numpy as np
from tqdm import tqdm
import argparse
import os

def get_args():
    parser = argparse.ArgumentParser(description='Seam carving')
    parser.add_argument('--image', '-i', type = str, required=True, help='Path to image')
    parser.add_argument('--scale_x', '-x', type = float, default = 0.5, help='Scale to width, must < 1')
    parser.add_argument('--scale_y', '-y', type = float, default = 0.5, help='Scale to height, must < 1')
    return parser.parse_args()

def get_seam(img):
    grad_x = cv2.Sobel(img, cv2.CV_64F, 1, 0, borderType = cv2.BORDER_REFLECT)
    grad_y = cv2.Sobel(img, cv2.CV_64F, 0, 1, borderType = cv2.BORDER_REFLECT)
    grad = np.sum(np.absolute(grad_x)+np.absolute(grad_y), axis=-1)
    rows, cols = grad.shape[:2]
    f = np.zeros_like(grad)
    s = np.zeros_like(grad).astype(int)
    f[0] = grad[0]
    for i in range(1, rows):
        for j in range(0, cols):
            if j==0:
                f[i,j] = np.min(f[i-1,j:j+2]) + grad[i,j]
                s[i,j] = np.argmin(f[i-1,j:j+2]) + j
            elif j==cols-1:
                f[i,j] = np.min(f[i-1,j-1:j+1]) + grad[i,j]
                s[i,j] = np.argmin(f[i-1,j-1:j+1]) + j-1
            else:
                f[i,j] = np.min(f[i-1,j-1:j+2]) + grad[i,j]
                s[i,j] = np.argmin(f[i-1,j-1:j+2]) + j-1
    seam = [np.argmin(f[-1])]
    for i in range(rows-1,0,-1):
        seam.append(s[i,seam[-1]])
    seam.reverse()
    return seam

def cut_seam(img, seam):
    rows, cols = img.shape[:2]
    mask = np.ones((rows, cols)).astype(np.bool)
    for i in range(rows):
        mask[i,seam[i]] = False
    return img[mask].reshape((rows, cols-1,3))

def cut_columns(img, cut_rate, desc):
    cut = img.copy()
    cols = cut.shape[1]
    for i in tqdm(range(int(cols*cut_rate)), desc):
        seam = get_seam(cut)
        cut = cut_seam(cut, seam)
    return cut

if __name__ == '__main__':
    args = get_args()
    img = cv2.imread(args.image)
    print("Read image: ", args.image)
    print("Image shape: ", img.shape)

    # cut columns
    cut = cut_columns(img, 1-args.scale_x, 'cut columns')

    dirname, extname = os.path.splitext(args.image)
    output_dir = dirname+'_output_cols'+extname
    cv2.imwrite(output_dir, cut)
    print('Output to ', output_dir)

    # cut rows
    cut = np.transpose(cut, (1,0,2))
    cut = cut_columns(cut, 1-args.scale_y, 'cut rows')
    cut = np.transpose(cut, (1,0,2))

    dirname, extname = os.path.splitext(args.image)
    output_dir = dirname+'_output'+extname
    cv2.imwrite(output_dir, cut)
    print('Output to ', output_dir)

