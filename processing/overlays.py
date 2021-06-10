import cv2
import numpy as np

from collections import namedtuple
from matplotlib import pyplot as plt
from typing import Callable

Layout = namedtuple('Layout', ['x', 'y', 'width', 'height'])

class Overlay:
  def __new__(cls, layout: Layout):
    new = object.__new__(cls)
    new.layout = layout
    new.upper_left = (layout.x, layout.y)
    new.lower_right = (layout.x + layout.width, layout.y + layout.height)
    # Creates a blank canvas
    new.canvas = np.zeros((layout.height, layout.width, 1), dtype = "uint8")
    return new

  def draw(self, image):
    image[self.upper_left[1]:self.lower_right[1],self.upper_left[0]:self.lower_right[0]] = self.canvas

class TextOverlay:
  pass

class PlotOverlay(Overlay):
  def __init__(self, layout: Layout):
    # Setup a figure with the right size.
    in_per_pix = 1/plt.rcParams['figure.dpi']
    self.fig, self.ax = plt.subplots(tight_layout=True)
    #self.fig.set_size_inches(layout.width*in_per_pix, layout.height*in_per_pix)

  def update(self):
    # redraw the canvas
    self.fig.canvas.draw()

    # convert canvas to image
    bytes = np.frombuffer(self.fig.canvas.tostring_rgb(), dtype=np.uint8)
    bytes = bytes.reshape(self.fig.canvas.get_width_height()[::-1] + (3,))
    bytes = cv2.resize(bytes, (self.layout.width, self.layout.height))

    # img is rgb, convert to opencv's default bgr
    self.canvas = cv2.cvtColor(bytes, cv2.COLOR_RGB2BGR)