for i in range(257):
    lpix = i>>4
    rpix = i&0b1111
    lshad = rshad = 0
    if lpix > 0:
        lshad = 0b1110
    if rpix > 0:
        rshad = 0b1110
    print((lshad<<4)|rshad, end=',')
    if i != 0 and i % 16 == 0:
        print("")