import random
import draw


def rand_color():
    #while True:
    col = (random.randrange(0, 255),
           random.randrange(0, 255),
           random.randrange(0, 255))
        #if col != GRAY:
        #    break
    return col
