#RLE encoding of a black and white image
from PIL import Image
import numpy as np
from matplotlib import pyplot as plt


def encode_rle(rgb_im):
    # rle encoding
    rle = []
    for x in range(rgb_im.width):
        count = 0
        current_val = 0
        rle_row = []
        for y in range(rgb_im.height):
            r, g, b = rgb_im.getpixel((x, y))
            if r == current_val:
                count = count + 1
            else:
                rle.append(count)
                rle_row.append(count)
                current_val = r
                count = 1
        rle.append(count)
        rle_row.append(count)
        print(x, rle_row, np.sum(rle_row))
    return rle


def decode_rle(RLE, x_dim, y_dim):
    image = np.zeros((y_dim, x_dim, 3))
    color_1 = [1, 1, 0]
    color_2 = [0, 1, 1]
    fg = 0
    x_current = 0
    y_current = 0
    current_color = color_1.copy()
    for count in RLE:
        for i in range(count):
            if fg == 0:
                image[y_current + i, x_current, 0] = 0
                image[y_current + i, x_current, 1] = 0
                image[y_current + i, x_current, 2] = 0
            else:
                image[y_current + i, x_current, 0] = current_color[0]
                image[y_current + i, x_current, 1] = current_color[1]
                image[y_current + i, x_current, 2] = current_color[2]
        fg = 1-fg
        y_current = y_current + count
        if y_current >= y_dim:
            y_current = 0
            x_current = x_current + 1
            beta = x_current/x_dim
            alpha = 1-beta
            current_color[0] = alpha*color_1[0] + beta*color_2[0]
            current_color[1] = alpha*color_1[1] + beta*color_2[1]
            current_color[2] = alpha*color_1[2] + beta*color_2[2]
            fg = 0
    return image

def encode_bitmap(rgb_im):
    """

    :param rgb_im:
    :return:
    """
    bitmap = []
    print("W,H", rgb_im.width, rgb_im.height)
    width = 128
    height = 44
    for y in range(height):
        value = 0
        counter = 0
        for x in range(width):
            r, g, b = rgb_im.getpixel((x, y))
            if r > 0:
                value += 2**counter
            counter = counter + 1
            if counter == 32:
                bitmap.append(value)
                counter = 0
                value = 0
    return bitmap

def encode_bitmap_2bit(rgb_im):
    """

    :param rgb_im:
    :return:
    """
    bitmap = []
    print("W,H", rgb_im.width, rgb_im.height)
    width = 128
    height = 44
    for y in range(height):
        value = 0
        counter = 0
        for x in range(width):
            r, g, b = rgb_im.getpixel((x, y))
            print(r, g, b)
            if r > 0:
                value += 2**counter
            counter = counter + 1
            if counter == 32:
                bitmap.append(value)
                counter = 0
                value = 0
    return bitmap

def decode_bitmap(bitmap, x_dim, y_dim):
    image = np.zeros((y_dim, x_dim, 3))
    print(y_dim, x_dim)
    for y in range(y_dim):
        for x in range(x_dim):
            elem = bitmap[y * int(x_dim/32) + int(x/32)]
            index = x % 32
            data = elem & 2**index
#            print(x, y, y * int(x_dim/32) + int(x/32), index, data)
            if data > 0:
                image[y, x, 0] = 1
    return image

#def compress_list(input):

#1. Read image
IMAGE_NAME = "../images/logo.png"
IMAGE_NAME = "../images/game_over_2bit.png"
im = Image.open(IMAGE_NAME)
rgb_im = im.convert('RGB')


#4. bitmap
bitmap = encode_bitmap_2bit(rgb_im)
print(len(bitmap), bitmap)

im2 = decode_bitmap(bitmap, 128, 44)
plt.imshow(im2)
plt.show()