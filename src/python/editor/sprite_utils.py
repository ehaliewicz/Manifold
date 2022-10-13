# rle compress image
# scale RLE spans 

from PIL import Image

SPRITE_SIZE = 64

def rle_comp_seq(seq, max_len, is_empty, debug=False):
    skip_items = 0
    run_items = [] 

    runs = []
    got_items = False

    def finish_span_helper():
        nonlocal skip_items, run_items, got_items
        skip = skip_items
        run = run_items
        skip_items = 0
        run_items = []
        if skip != 0 or len(run) != 0:
            if len(run) != 0:
                got_items = True
            runs.append((skip,len(run),run))

    for item in seq:
        cur_run_len = len(run_items)
        in_run = cur_run_len != 0

        if is_empty(item):
            if in_run:
                finish_span_helper()

            skip_items += 1
            if skip_items == max_len:
                finish_span_helper()
        else:
            # if we were just skipping empty pixels so far, start keeping track of solid pixels
            run_items.append(item)
            if cur_run_len == max_len:
                finish_span_helper()
    
    finish_span_helper()

    #for idx,run in enumerate(runs):
    #    if all((x[1] == 0 for x in run)):
    #        runs[idx] = 
    if got_items:
        while runs[-1][1] == 0:
            runs.pop()

        return runs
    else:
        return []



def compile_image(path):
    im = Image.open(path)
    px = im.load()

    # all possible span sizes up to 32 input pixels have scales from 1 to 512 output pixels

    # 16.16 fixed point
    # indexed by (output<<5)|input
    #scale_table = [0]
    #for ipx in range(1, 32):
    #    for opx in range(1, 513):
    #        scale_fix = int((opx*65536)/ipx)
    #        scale_table.append(scale_fix)


    columns = []

    def get_word_pix(x1,x2,x3,x4,y):
        p1 = px[x1,y]
        p2 = px[x2,y]
        p3 = px[x3,y]
        p4 = px[x4,y]

        p = p1<<12|p2<<8|p3<<4|p4
        return p   
    
    def get_doubled_pix(x,y):
        p = px[x,y]
        return p<<4|p

    rle_compressed_columns = []

    # gathers four separate pixels into a word column, not going to work when scaling up, due to texel duplication
    for col in range(0, im.width):
        pix_row = []
        for row in range(SPRITE_SIZE):
            pix_row.append(get_doubled_pix(col, row))
        rle_compressed_columns.append(
            rle_comp_seq(
                pix_row,
                256,
                lambda p: p==0
            )
        )
    """
    for col in range(0, im.width, 4):
        pix_row = []
        for row in range(SPRITE_SIZE):
            pix_row.append(get_word_pix(col, col+1, col+2, col+3, row))

        rle_compressed_columns.append(
            rle_comp_seq(
                pix_row,
                256,
                lambda p: p==0
            )
        )
    """
    #rle_compressed_columns = [
    #    rle_comp_seq(
    #        (get_pix(col,col+1,row) for row in range(SIZE)), 
    #        256, 
    #        lambda p: p==0
    #    ) for col in range(0, im.width, 2)]

    num_spans = 0
    for col in rle_compressed_columns:
        num_spans += len(col)

    #res = [(x*2,col) for x,col in enumerate(rle_compressed_columns)]
    #print(res)


    #print(("scaled output size total: ", num_spans*511*2)) # 28 kilobytes... wow
    return rle_compressed_columns

def half(n):
    ni = n//2
    nf = n/2 
    assert ni == nf 
    return ni

def generate_c_for_column(column,name):
    column_c = ""
    c_texels = []
    c_spans = []
    for span in column:
        (skip,length,texels) = span
        for texel in texels:
            c_texels.append("{0:#04x}".format(texel))
        c_spans.append(str(skip))
        c_spans.append(str(length))
    
    if len(c_spans) != 0:
        c_spans_out = "const u16 col_{}_spans[{}] = ".format(name, len(c_spans)) + "{" + ",".join(c_spans) + "};"
        c_texels_out = "const u8 col_{}_texels[{}] = ".format(name, len(c_texels)) + "{" + ",".join(c_texels) + "};"

        column = "{" + ".num_spans = {}, .spans = col_{}_spans, .texels = col_{}_texels".format(half(len(c_spans)), name, name) + "}"
    else:
        c_spans_out = ""
        c_texels_out = ""
        column = "{.num_spans = 0, .spans = NULL, .texels = NULL}"
    return c_spans_out, c_texels_out, column
    #return ",".join(c_texels), ",".join(c_spans)


if __name__ == '__main__':

    name = "test_texture"
    #columns = compile_image("./res/textures/sprites.png")
    #columns = compile_image("./res/textures/doobguy_3d_palette.png")
    columns = compile_image("./res/textures/doobguy.png")
    output = ""
    array_cols = []



    for idx, column in enumerate(columns):
        spns,txls,col = generate_c_for_column(column, idx)
        if spns != "":
            output += spns
            output += "\n"
        if txls != "":
            output += txls
            output += "\n"
        array_cols.append(col)

    output += "const rle_object test_obj = {\n"
    output += ".num_columns = {},\n".format(len(columns))
    output += ".columns = {\n"
    output += ",\n".join(array_cols)
    output += "\n}\n"
    output += "};\n"
    print(output)