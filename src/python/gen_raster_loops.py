
Z_BIT          = 1<<8
WALL_TEX_BIT   = 1<<7
UPPER_TEX_BIT  = 1<<6
LOWER_TEX_BIT  = 1<<5
CEIL_BIT       = 1<<4
FLOOR_BIT      = 1<<3
WALL_BIT       = 1<<2
UPPER_STEP_BIT = 1<<1
LOWER_STEP_BIT = 1<<0

indent = 0
def iprint(*a):
    for _ in range(indent):
        print("    ", end='')
    print(*a)

class Scope(object):
    def __enter__(self):
        global indent
        indent += 1

    def __exit__(self, t, v, tb):
        global indent
        indent -= 1

routines = 0

def make_bsearch_tree(lines):
    if len(lines) == 1:
        return (lines[0], None, None)
    elif len(lines) == 2:
        return (lines[0], None, make_bsearch_tree(lines[1:]))
    else:
        mid = len(lines)//2
        mid_val = lines[mid]
        left = lines[0:mid]
        right = lines[mid+1:]
        return (mid_val,make_bsearch_tree(left), make_bsearch_tree(right))

def flatten_tree(tree):
    if not tree:
        return []
    m = [tree[0]]
    l = flatten_tree(tree[1])
    r = flatten_tree(tree[2])
    return l+m+r


def find_nodes_right_of(sym, tree, right_of, inner_func):
    if not tree:
        inner_func(right_of)
        return
    mid_val = tree[0]
    iprint("if ({} <= {}) ".format(sym, mid_val) + " {")
    with Scope():
        find_nodes_right_of(
            sym, tree[1], 
            [mid_val]+ flatten_tree(tree[2]) + right_of,
            inner_func)
    iprint("} else {")
    with Scope():
        find_nodes_right_of(sym, tree[2], right_of, inner_func)
    iprint("}")
    
def find_nodes_left_of(sym, tree, left_of, inner_func):
    if not tree:
        inner_func(left_of)
        return
    mid_val = tree[0]
    iprint("if ({} >= {})".format(sym, mid_val) + " {")
    with Scope():
        find_nodes_left_of(
            sym, tree[2],
            left_of + flatten_tree(tree[1]) + [mid_val],
            inner_func
        )
    iprint("} else {")
    with Scope():
        find_nodes_left_of(sym, tree[1], left_of, inner_func)
    iprint("}")

def check_draw_ceil(lines_below_y_min, lines_above_y_max, update_clip):
    if "s0" not in lines_below_y_min:
        return 
    if "s0" in lines_above_y_max:
        iprint("draw_vertical_line(y_min, s0, col_ptr, ceil_col); // ceil")
        iprint("*yclip_ptr = s0; // move y_min to bot of ceil")
    else:
        assert update_clip 
        # if we drew lower, we wouldn't get here!
        # if we didn't draw lower, we need to update clip either way
        iprint("draw_vertical_line(y_min, y_max, col_ptr, ceil_col); // ceil")
        # in this case however, the screen is now full 
        if update_clip:
            iprint("set_column_drawn(yclip_ptr);")

def check_draw_upper(lines_below_y_min, lines_above_y_max, tex_upper):
    if "s1" not in lines_below_y_min:
        return 
    if "s0" in lines_below_y_min:
        # s0 is top of visible upper step portion
        top = "s0"
        
    else:
        top = "y_min"
        
    if "s1" in lines_above_y_max:
        # s1 is bottom of visible upper step portion
        bot = "s1"
    else:
        # ymax is bottom of visible upper step portion
        bot = "y_max"
    if tex_upper:
        iprint("draw_tex_line({}, {}, col_ptr, tex_col); // upper step".format(top, bot))
    else:
        iprint("draw_vertical_line({}, {}, col_ptr, upper_col); // upper step".format(top, bot))
    
    iprint("*(yclip_ptr) = {}; // set y_min to bot of upper step".format(bot))

def check_draw_wall(lines_below_y_min, lines_above_y_max, tex_wall):
    if "s4" not in lines_below_y_min:
        return False
    if "s0" not in lines_above_y_max:
        return False 

    if "s0" in lines_below_y_min:
        top = "s0"
    else:
        top = "y_min"
    if "s4" in lines_above_y_max:
        bot = "s4"
    else:
        bot = "y_max"
    
    if tex_wall:
        iprint("draw_tex_line({}, {}, col_ptr, tex_col); // wall".format(top, bot))
    else:
        iprint("draw_vertical_line({}, {}, col_ptr, wall_col); // wall".format(top, bot))
    return True


def check_draw_lower(lines_below_y_min, lines_above_y_max, tex_lower):
    if "s3" not in lines_above_y_max:
        print("s3 not above y_max")
        return False 
    
    if "s3" in lines_below_y_min:
        top = "s3"
    else:
        top = "y_min"

    if "s4" in lines_above_y_max:
        bot = "s4"
    else:
        bot = "y_max"    

    if tex_lower:
        iprint("draw_tex_line({}, {}, col_ptr, tex_col); // lower".format(top, bot))
    else:
        iprint("draw_vertical_line({}, {}, col_ptr, lower_col); // lower".format(top, bot))
    iprint("*(yclip_ptr+1) = {}; // set y_max to top of lower step".format(top))
    return True

def check_draw_floor(lines_below_y_min, lines_above_y_max, update_clip):
    if "s4" not in lines_above_y_max:
        return False
    
    if "s4" in lines_below_y_min:
        iprint("draw_vertical_line(s4, y_max, col_ptr, floor_col); // floor")
        if update_clip:
            iprint("*(yclip_ptr+1) = s4; // move y_max to top of floor")
    else:
        assert update_clip
        iprint("draw_vertical_line(y_min, y_max, col_ptr, floor_col); // floor")
        if update_clip:
            iprint("set_column_drawn(yclip_ptr);")
    return True

for i in range(0b1000000000):

    lines_traced = 0

    increasing_z = Z_BIT & i
    draw_wall = WALL_BIT & i
    draw_upper = UPPER_STEP_BIT & i
    draw_lower = LOWER_STEP_BIT & i

    tex_wall = WALL_TEX_BIT & i
    tex_upper = UPPER_TEX_BIT & i
    tex_lower = LOWER_TEX_BIT & i

    draw_floor = FLOOR_BIT & i
    draw_ceil = CEIL_BIT & i

    if tex_wall and not draw_wall:
        #print("tex wall but not draw wall")
        continue

    if tex_upper and not draw_upper:
        #print("tex upper but not draw upper")
        continue

    if tex_lower and not draw_lower:
        #print("tex lower but not draw lower")
        continue

    if draw_wall and (draw_upper or draw_lower):
        #print("draw wall and step")
        continue

    lines = []
    if draw_ceil:
        lines.append('s0')
    if draw_upper:
        lines.append('s1')
    if draw_lower:
        lines.append('s2')
    if draw_floor:
        lines.append('s3')

    if len(lines) == 0:
        continue

    args = ['s16 x1', 's16 x2']
    for line in lines:
        args.append("s32 {}_fix".format(line))
        args.append("s32 d{}_fix".format(line))
    args.append('wall_fill_params* params')

    needs_texture = tex_lower or tex_upper or tex_wall

    print("void routine_{}({})".format(i, ", ".join(args)) + " {")
    print("// draw wall {}, lines {}".format(1 if draw_wall else 0, lines))
    with Scope():
        if increasing_z:
            iprint("calc_increasing_z_light_levels();")
        else:
            iprint("calc_decreasing_z_light_levels();")

        if draw_floor:
            iprint("u16 floor_col = params->floor_col;")
        if draw_ceil:
            iprint("u16 ceil_col = params->ceil_col;")
        if needs_texture:
            iprint("calc_tex_coords();")
            iprint("u16* tex_col_ptr = tex_col_buffer;")
            iprint("u16* base_tex = params->base_tex;")

        if draw_upper and (not tex_upper):
            iprint("u16 upper_col = params->upper_col;")
        if draw_lower and (not tex_lower):
            iprint("u16 lower_col = params->lower_col;")
        if draw_wall and (not tex_wall):
            iprint("u16 wall_col = params->wall_col;")

        iprint("u8** offset_ptr = &offset_buf[x1];")
        iprint("u8* yclip_ptr = &y_clip_buffer[x1<<1];")
        iprint("u16 cnt = x2-x1;")
        iprint("while (cnt--) {")
        with Scope():
            for line in lines:
                iprint("s16 {} = {}_fix>>16;".format(line, line))

            iprint("u8* col_ptr = *offset_ptr++;")
            if needs_texture:
                iprint("u16 tex_idx = *tex_col_ptr++;")

            iprint("u8 y_min = *yclip_ptr;")
            iprint("u8 y_max = *(yclip_ptr+1);")

            iprint("if(!is_column_drawn(y_min, y_max)) {")

            
            with Scope():
                iprint("if(y_min >= y_max) { __builtin_unreachable(); }")
                if needs_texture:
                    iprint("u16* tex_col = &base_tex[tex_idx];")
                btree = make_bsearch_tree(lines)
                def inner1(lines_gte_y_min):
                    def inner2(lines_lte_y_max):
                        #draw_lines = (set(lines_gte_y_min)
                        #              .intersection(set(lines_lte_y_max)))
                        
                        drew_upper = False
                        drew_lower = False
                        if draw_upper:
                            drew_upper = check_draw_upper(lines_gte_y_min, lines_lte_y_max, tex_upper)
                        if draw_wall:
                            check_draw_wall(lines_gte_y_min, lines_lte_y_max, tex_wall)
                        if draw_lower:
                            drew_lower = check_draw_lower(lines_gte_y_min, lines_lte_y_max, tex_lower)

                        if draw_ceil:
                            check_draw_ceil(lines_gte_y_min, lines_lte_y_max, (not drew_upper))
                        if draw_floor:
                            check_draw_floor(lines_gte_y_min, lines_lte_y_max, (not drew_lower))
                    

                    find_nodes_left_of("y_max", btree, [], inner2)
                    """
                    else:
                        drew_upper = False
                        drew_lower = False
                        if draw_upper:
                            drew_upper = check_draw_upper(lines_gte_y_min, set(), tex_upper)
                        if draw_lower:
                            drew_lower = check_draw_lower(lines_gte_y_min, set(), tex_lower)
                        if draw_wall:
                            check_draw_wall(lines_gte_y_min, set(), tex_wall)
                        if draw_ceil:
                            check_draw_ceil(lines_gte_y_min, set(), (not drew_upper))
                        if draw_floor:
                            check_draw_floor(lines_gte_y_min, set(), (not drew_lower))
                    """
                        

                find_nodes_right_of("y_min", btree, [], inner1)
            
            iprint("}")

            for line in lines:
                iprint("{}_fix += d{}_fix;".format(line, line))
            iprint("yclip_ptr += 2;")


        iprint("}")

    routines += 1
    print("}")
    exit()
print(routines)

