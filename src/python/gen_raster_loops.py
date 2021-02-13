
    
    

    
def gen_raster_routine(bitmask):
    indent = 0
    def line(l):
        print("{}{}".format(" "*indent*4, l))

    def gen_dy_for_floor():
        if draw_floor:
            line("s16 portal_bot_dy_dx = ((x2_ybot - x1_ybot) << 16) / dx;")
        
    def gen_dy_for_ceiling():
        if draw_ceiling:
            line("s16 portal_top_dy_dx = ((x2_ytop - x1_ytop) << 16) / dx;")

    def gen_dy_for_bot_step():
        if draw_bot_step:
            line("s16 portal_bot_ndy_dx = ((x2_nybot - x1_nybot) << 16) / dx;")
        
    def gen_dy_for_top_step():
        if draw_top_step:
            line("s16 portal_top_ndy_dx = ((x2_nytop - x1_nytop) << 16) / dx;")


            
    def gen_init_y_for_floor():
        if draw_floor:
            line("s16 floor_y_fix = x1_ybot << 16;")
            line("s16 floor_y_int;")

    def gen_init_y_for_ceiling():
        if draw_ceiling:
            line("s16 ceiling_y_fix = x1_ytop << 16;")
            line("s16 ceiling_y_int;")
        
    def gen_init_y_for_bot_step():
        if draw_bot_step:
            line("s16 bot_step_y_fix = x1_nybot << 16;")
            line("s16 bot_step_y_int;")
        
    def gen_init_y_for_top_step():
        if draw_top_step:
            line("s16 top_step_y_fix = x1_nytop << 16;")
            line("s16 top_step_y_int;")


            
    def gen_skip_for_ceiling():
        if draw_ceiling:
            line("ceiling_y_fix += (skip * portal_top_dy_dx);")

    def gen_skip_for_floor():
        if draw_floor:
            line("floor_y_fix += (skip * portal_bot_dy_dx);")

    def gen_skip_for_bot_step():
        if draw_bot_step:
            line("bot_step_y_fix += (skip * portal_bot_ndy_dx);")

    def gen_skip_for_top_step():
        if draw_top_step:
            line("top_step_y_fix += (skip * portal_top_ndy_dx);")



    def gen_get_int_y_ceiling():
        if draw_ceiling:
            line("ceiling_y_int = ceiling_y_fix>>16;")
            line("u8 clamped_ceiling_y_int = clamp(ceiling_y_int, min_drawable_y, max_drawable_y);")

    def gen_get_int_y_top_step():
        if draw_top_step:
            line("top_step_y_int = top_step_y_fix>>16;")
            line("u8 clamped_top_step_y_int = clamp(top_step_y_int, min_drawable_y, max_drawable_y);")

    def gen_get_int_y_bot_step():
        if draw_bot_step:
            line("bot_step_y_int = bot_step_y_fix>>16;")
            line("u8 clamped_bot_step_y_int = clamp(bot_step_y_int, min_drawable_y, max_drawable_y);")
            
    def gen_get_int_y_floor():
        if draw_floor:
            line("floor_y_int = floor_y_fix>>16;")
            line("u8 clamped_floor_y_int = clamp(floor_y_int, min_drawable_y, max_drawable_y);")
            

    def gen_draw_ceiling():
        if draw_ceiling:
            line("draw_native_vline_unrolled(min_drawable_y, clamped_ceiling_y_int-1, ceil_col);")

    def gen_draw_floor():
        if draw_floor:
            line("draw_native_vline_unrolled(clamped_floor_y+1, max_drawable_y, floor_col);")
            
    def gen_draw_top_step():
        if draw_top_step:
            if draw_ceiling:
                line("draw_native_vline_unrolled(clamped_ceiling_y_int, clamped_top_step_y_int-1, upper_col);")
            else:
                line("draw_native_vline_unrolled(min_drawable_y, clamped_top_step_y_int-1, upper_col);")

    def gen_draw_bot_step():
        if draw_bot_step:
            if draw_floor:
                # use bot of portal
                line("draw_native_vline_unrolled(clamped_bot_step_y_int+1, clamped_floor_y_int, lower_col);")
            else:
                # use max_drawable_y
                line("draw_native_vline_unrolled(clamped_bot_step_y_int+1, max_drawable_y, lower_col);")


    def gen_inc_for_ceiling():
        if draw_ceiling:
            line("ceiling_y_fix += portal_top_dy_dx;")

    def gen_inc_for_floor():
        if draw_floor:
            line("floor_y_fix += portal_top_dy_dx;")

    def gen_inc_for_top_step():
        if draw_top_step:
            line("top_step_y_fix += portal_top_ndy_dx;")

    def gen_inc_for_bot_step():
        if draw_bot_step:
            line("bot_step_y_fix += portal_bot_ndy_dx;")
            

    def gen_update_clipping_by_ceiling():
        if draw_ceiling:
            line("*y_clip_ptr++ = clamped_ceiling_y_int;")
    
    def gen_update_clipping_by_bot_step():
        if draw_bot_step:
            line("*y_clip_ptr++ = clamped_bot_step_y_int;")

    def gen_update_clipping_by_top_step():
        if draw_top_step:
            line("*y_clip_ptr++ = clamped_top_step_y_int;")

    def gen_update_clipping_by_floor():
        if draw_floor:
            line("*y_clip_ptr++ = clamped_floor_y_int;")
            
        
    draw_ceiling = 0b0010 & bitmask
    draw_floor   = 0b0001 & bitmask
    draw_top_step = 0b1000 & bitmask
    draw_bot_step = 0b0100 & bitmask

    line("void draw_bitmask_{0:b}()".format(bitmask) + "{")
    indent += 1

    line("if (x2 <= x1) { return; }")
    line("u16 dx = x2-x1;")

    gen_dy_for_ceiling()
    gen_dy_for_floor()
    gen_dy_for_top_step()
    gen_dy_for_bot_step()
    
    # skip off-screen portion of wall
    line("u16 beginx = max(x1, window_min);")
    line("u16 endx = min(x2, window_max);")

    line("u16 skip = x1-beginx;")

    gen_init_y_for_ceiling()
    gen_init_y_for_floor()
    gen_init_y_for_top_step()
    gen_init_y_for_bot_step()
        
        
    line("if (skip) {")
    indent += 1
    gen_skip_for_ceiling()
    gen_skip_for_floor()
    gen_skip_for_top_step()
    gen_skip_for_bot_step()
    indent -= 1
    line("}")

    line("for(u16 x = beginx; x <= endx; x++) {")
    indent += 1

    line("u8 min_drawable_y = *y_clip_ptr;")
    line("u8 max_drawable_y = *(y_clip_ptr+1);")
    
    gen_get_int_y_ceiling()
    gen_get_int_y_top_step()
    gen_get_int_y_bot_step()
    gen_get_int_y_floor()

    
    if draw_top_step:
        gen_update_clipping_by_top_step()
    elif draw_ceiling:
        gen_update_clipping_by_ceiling()
    else:
        line("y_clip_ptr++;")

    if draw_bot_step:
        gen_update_clipping_by_bot_step()
    elif draw_floor:
        gen_update_clipping_by_floor()
    else:
        line("y_clip_ptr++;")
    
    gen_inc_for_ceiling()
    gen_inc_for_floor()
    gen_inc_for_top_step()
    gen_inc_for_bot_step()

    gen_draw_ceiling()
    gen_draw_top_step()
    gen_draw_bot_step()
    gen_draw_floor()

    
    indent -= 1
    line("}")
    

    #if draw_ceiling:
    #    print("
    
    
        
    indent -= 1
    line("}\n")


def gen_raster_routines():
    for bitmask in range(0b1111+1):
        gen_raster_routine(bitmask)


if __name__ == '__main__':
    gen_raster_routines()

