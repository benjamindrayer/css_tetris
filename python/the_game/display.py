import numpy as np
import pygame
import time
import importlib
from PIL import Image



class Display:
    """Display class
    """

    def __init__(self, size_x, size_y, factor=8):
        """
        Initializes display of size x,y
        :param size_x:
        :param size_y:
        :param display_type: 0 -> Display on screen
                             1 -> Display on LED-matrix
                             2 -> Use both
        """
        self.image = np.zeros((size_x, size_y, 3))
        self.size_x = size_x
        self.size_y = size_y
        self.factor = factor
        pygame.init()
        self.scr = pygame.display.set_mode((self.size_x * self.factor, self.size_y * self.factor))
        self.scr.fill((0, 0, 0))

    def set_pixel(self, x, y, color):
        """Set pixel to color
        :param x:
        :param y:
        :param color:
        :return:
        """
        self.image[x, y, 0] = color[0]
        self.image[x, y, 1] = color[1]
        self.image[x, y, 2] = color[2]

    def clear_screen(self, color=None):
        """Sets the whole screen to a given color
        :param color:
        :return:
        """
        if not color:
            self.image[:, :, :] = 0
        else:
            self.image[:, :, 0] = color[0]
            self.image[:, :, 1] = color[1]
            self.image[:, :, 2] = color[2]

    def fade_to_image(self, image):
        """Fading from current image to given image
        :param image:
        :return:
        """
        old_image = np.copy(self.image)
        n_steps = 20
        for i in range(n_steps):
            factor = i / (n_steps - 1)
            self.image = old_image * (1 - factor) + image * factor
            self.show()
            time.sleep(0.2)


    def show(self, mask=[]):
        """Show the current image
        :return:
        """
        if mask:
            for x in range(mask[0], mask[1]):
                for y in range(mask[2], mask[3]):
                    color = (self.image[x, y, 0], self.image[x, y, 1], self.image[x, y, 2])
                    #                pygame.draw.circle(self.scr, color, ((x + 0.5) * self.factor, (y + 0.5) * self.factor),
                    #                                   self.factor / 2 - 1)
                    pygame.draw.rect(self.scr, color,
                                     pygame.Rect(x * self.factor, y * self.factor, self.factor, self.factor))
        else:
            for x in range(self.size_x):
                for y in range(self.size_y):
                    color = (self.image[x, y, 0], self.image[x, y, 1], self.image[x, y, 2])
    #                pygame.draw.circle(self.scr, color, ((x + 0.5) * self.factor, (y + 0.5) * self.factor),
    #                                   self.factor / 2 - 1)
                    pygame.draw.rect(self.scr, color, pygame.Rect(x*self.factor, y*self.factor, self.factor, self.factor) )
        pygame.display.update()

    def showImage(self, image):
        for x in range(self.size_x):
            for y in range(self.size_y):
                self.image[x,y,0] = image.get_at((x,y)).r
                self.image[x,y,1] = image.get_at((x,y)).g
                self.image[x,y,2] = image.get_at((x,y)).b


    def showImageMasked(self, image, mask):
        for x in range(mask[0], mask[1]):
            for y in range(mask[2], mask[3]):
                self.image[x,y,0] = image.get_at((x,y)).r
                self.image[x,y,1] = image.get_at((x,y)).g
                self.image[x,y,2] = image.get_at((x,y)).b
        self.show(mask=mask)
