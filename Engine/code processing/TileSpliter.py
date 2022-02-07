# -*- coding: utf-8 -*-
"""
Created on Thu Dec 30 11:53:32 2021

@author: HP
"""
PATH = "Forest.png"
from PIL import Image
sprites = [Image.new("RGBA", (32, 32), (0,0,0,0)) for x in range(8*6)]
current = 0
with Image.open(PATH) as im:
    curI = 0
    curJ = 0
    for curI in range(0,256,32):
        for curJ in range(0, 192, 32):
            for i in range(curI,curI + 32):
                for j in range(curJ, curJ + 32):
                    col = im.getpixel((i,j))
                    sprites[current].putpixel((i - curI, j - curJ), col)      
            sprites[current].save("sprite_{}.png".format(current))
            current += 1

