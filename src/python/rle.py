# rle compress image
# scale RLE spans 

from tokenize import cookie_re
from PIL import Image

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
            if cur_run_len == 32:
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
    scale_table = [0]
    for ipx in range(1, 32):
        for opx in range(1, 513):
            scale_fix = int((opx*65536)/ipx)
            scale_table.append(scale_fix)


    columns = []

    def get_pix(lx,rx,y):
        lp = px[lx,y]
        rp = px[rx,y]
        p = lp<<12|lp<<8|rp<<4|rp
        return p

    rle_compressed_columns = []


    rle_compressed_columns = [rle_comp_seq((get_pix(col,col+1,row) for row in range(im.height)), 512, lambda p: p==0) for col in range(0, im.width, 2)]

    num_spans = 0
    for col in rle_compressed_columns:
        num_spans += len(col)

    res = [(x*2,col) for x,col in enumerate(rle_compressed_columns)]
    #print(res)


    print(("scaled output size total: ", num_spans*511*2)) # 28 kilobytes... wow
    return res


            

if __name__ == '__main__':
    print(compile_image("./res/enemy1.png"))