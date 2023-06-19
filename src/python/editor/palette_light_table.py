
# load image file


# load all colors in image

avail_colors = [
    (155,93,93),
    (217,155,124),
    (248,186,155),
    (124,248,155),
    (62,93,93),
    (0,31,31),
    (62,124,0),
    (62,62,62),
    (217,62,0),
    (31,0,31),
    (155,0,62),
    (62,155,186),
    (93,93,186),
    (217,186,0),
    (0,0,0),
]

# make sure to offset indexes

def get_index_of_color(col):
    #(cr,cg,cb,) = col
    #colc = (cr,cg,cb)
    if len(col) == 4:
        colc = (col[0],col[1],col[2])
    else:
        colc = col
    for idx,col2 in enumerate(avail_colors):
        if col2 == colc:
            return idx+1

    raise Exception("Couldn't find color: {}".format(col))

def get_8pix(p):
    dp = (p<<4|p)
    qp = (dp<<8 | dp)
    ep = (qp << 16 | qp)
    return ep

from PIL import Image


def write_u32(f, lw):
    f.write(lw.to_bytes(4, byteorder="big", signed=False))


#NEAR_START_OFFSET = 2335986
#NEAR_FILE = "/Users/haliewicz/Desktop/near_light_table.png"


MIX_FILE = "src\python\editor\light_remapping_table_mix_rotated.png"
NO_MIX_FILE = "src\python\editor\light_remapping_table_no_mix_rotated.png"

if __name__ == '__main__':

    
    with Image.open(NO_MIX_FILE) as im:
    #with Image.open(MIX_FILE) as im:
        #x,y = im.size
        
        for idx,table in enumerate(["far", "near"]):
            print('// {} table'.format(table))
            dist_row_offset = idx * 5 * 2

            for light_level in range(5):
                print('// {} light level'.format(light_level-2))
                light_level_offset = light_level * 2

                print('0', end=', ')

                row = dist_row_offset + light_level_offset
                for pal_idx in range(15):
                    
                    first_nib = get_index_of_color(im.getpixel((pal_idx, row)))
                    second_nib = get_index_of_color(im.getpixel((pal_idx, row+1)))
                    
                    print('DPIX({},{})'.format(first_nib, second_nib), end=', ')

                print('')




