def get_y_var_names(num):
    return [("y{}".format(i), "dy{}_dx".format(i)) for i in range(num)]


def gen_rasterizer(bitmap):
    draw_upper_step = (bitmap & 0b1000) >> 3
    draw_lower_step = (bitmap & 0b0100) >> 2
    draw_ceiling = (bitmap & 0b0010) >> 1
    draw_floor = (bitmap & 0b0001)

    trace_upper_line = draw_upper_step | draw_ceiling
    trace_lower_line = draw_lower_step | draw_floor

    update_upper_clipping = trace_upper_line
    update_lower_clipping = trace_lower_line

    trace_neighbor_upper_line = draw_upper_step
    trace_neighbor_lower_line = draw_lower_step
    num_y_vars = (trace_upper_line + trace_neighbor_upper_line + trace_lower_line + trace_neighbor_lower_line)

    y_var_names = get_y_var_names(num_y_vars)

    print("for(int x = x0; x < x1; x++){")
    for y_var,y_var_dx in y_var_names:
        print("    {} += {};".format(y_var, y_var_dx))
    print("}")


for bitmap in range(0b0001, 0b1111):
    gen_rasterizer(bitmap)
