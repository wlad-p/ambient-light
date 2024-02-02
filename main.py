import numpy as np
import cv2
import matplotlib.pyplot as plt

img = cv2.imread('example.png')
img_width = 960
img_height = 540
img = cv2.resize(img, (img_width, img_height))
ksize = (80, 80)
img = cv2.blur(img, ksize)

def init_arrays():
    left = []
    top = []
    right = []
    bottom = []

    for i in range(0,17):
        left.append(i)
    for i in range(17, 46):
        top.append(i)
    for i in range(46,62):
        right.append(i)
    for i in range(62, 91):
        bottom.append(i)

    return left, top, right, bottom
        
left, top, right, bottom = init_arrays()

# left
section_size_y = img_height / len(left)
section_size_x = img_width * 0.25



cv2.imshow('image',img)
cv2.waitKey(0)
cv2.destroyAllWindows()