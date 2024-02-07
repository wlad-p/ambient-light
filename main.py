import numpy as np
import cv2
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans

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

def visualize_colors(cluster, centroids):
    # Get the number of different clusters, create histogram, and normalize
    labels = np.arange(0, len(np.unique(cluster.labels_)) + 1)
    (hist, _) = np.histogram(cluster.labels_, bins = labels)
    hist = hist.astype("float")
    hist /= hist.sum()

    # Create frequency rect and iterate through each cluster's color and percentage
    rect = np.zeros((50, 300, 3), dtype=np.uint8)
    colors = sorted([(percent, color) for (percent, color) in zip(hist, centroids)])
    start = 0
    for (percent, color) in colors:
        print(color, "{:0.2f}%".format(percent * 100))
        end = start + (percent * 300)
        cv2.rectangle(rect, (int(start), 0), (int(end), 50), \
                      color.astype("uint8").tolist(), -1)
        start = end
    return rect

def left(leds,img, img_height, img_width):
    section_size_y = int(img_height / len(leds))
    section_size_x = int(img_width * 0.25)

    for i in range(0,len(leds)):
        section = img[ i*section_size_y : (i+1)*section_size_y, 0 : section_size_x]
        section = cv2.cvtColor(section, cv2.COLOR_RGB2BGR)

        reshape = img.reshape((section.shape[0] * section.shape[1], 3))
        cluster = KMeans(n_clusters=5).fit(reshape)
        visualize = visualize_colors(cluster, cluster.cluster_centers_)
        visualize = cv2.cvtColor(visualize, cv2.COLOR_RGB2BGR)

    


def main():

    img = cv2.imread('example.png')
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    img_width = 960
    img_height = 540
    img = cv2.resize(img, (img_width, img_height))
    ksize = (30,30)
    img = cv2.blur(img, ksize)

    leds_left, leds_top, leds_right, leds_bottom = init_arrays()
    
    left(leds_left, img, img_height, img_width)


    img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)

    cv2.imshow('image',img)
    cv2.imshow('colors', visualize)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()