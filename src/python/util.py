import random
import draw

colors = []

def rand_color():
    #while True:
    while len(colors) < 16:

        col = (random.randint(0, 255),
            random.randint(0, 255),
            random.randint(0, 255))
        colors.append(col)

    idx = random.randint(0, 15)
    return colors[idx]
        #if col != GRAY:
        #    break
    return col
