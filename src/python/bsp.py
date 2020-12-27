import util
import draw
import wad

#def draw_bsp_nodes(nodes, surf):
#    root_node = get_root_node(nodes)

#    draw_bsp_node(root_node, surf)


def is_ssect_idx(idx):
    return (idx & (1<<15)) != 0


def is_on_left(node, x, y):
    dx = x - node.partition_x_coord
    dy = y - node.partition_y_coord

    return (((dx * node.dy) - (dy * node.dx)) <= 0)


def get_real_idx(idx):
    masked_idx = (idx & (~(1<<15)))
    return (idx & (~(1<<15))) 

def traverse_bsp_node(node_idx,
                      node_depth,
                      level_data,
                      node_callback,
                      ssector_callback):

    
    if is_ssect_idx(node_idx):
        ssector_callback(level_data['SSECTORS'][get_real_idx(node_idx)])
    else:
        node = level_data['NODES'][get_real_idx(node_idx)]
        node_callback(node, node_depth)
        traverse_bsp_node(node.left_child, node_depth+1,
                          level_data, node_callback, ssector_callback)
        traverse_bsp_node(node.right_child, node_depth+1,
                          level_data, node_callback, ssector_callback)

    
        
def traverse_all_bsp_nodes(level_data,
                           node_callback=lambda node: None,
                           ssect_callback=lambda ssect: None):
    nodes = level_data['NODES']
    root_idx = len(nodes)-1
    traverse_bsp_node(root_idx, 0, level_data, node_callback, ssect_callback)


def bsp_child_onscreen(node, left_child=True):
    
    if left_child:
        v1 = (node.left_box_left, node.left_box_top)
        v2 = (node.left_box_right, node.left_box_top)
        v3 = (node.left_box_right, node.left_box_bottom)
        v4 = (node.left_box_left, node.left_box_bottom)
    else:
        v1 = (node.right_box_left, node.right_box_top)
        v2 = (node.right_box_right, node.right_box_top)
        v3 = (node.right_box_right, node.right_box_bottom)
        v4 = (node.right_box_left, node.right_box_bottom)


    return draw.bbox_on_screen(v1,v2,v3,v4)


def get_bsp_tree_bounds(level_data):
    nodes = level_data['NODES']
    root_node_idx = len(nodes)-1
    node = nodes[root_node_idx]

    xs = [
        node.left_box_left, node.left_box_right,
        node.right_box_left, node.right_box_right,
    ]

    ys = [
        node.left_box_top, node.left_box_bottom,
        node.right_box_top, node.right_box_bottom,
    ]
    
    return [min(xs), max(xs), min(ys), max(ys)]
    
    
def traverse_bsp_front_to_back(level_data,
                               x, y, start_subsector,
                               node_callback=lambda node,visiting_left, visiting_right: None,
                               ssect_callback=lambda ssect: None):
    nodes = level_data['NODES']
    ssectors = level_data['SSECTORS']
    root_node_idx = len(nodes)-1
    #depth = 0

    #stk = [root_node_idx]


    src_sector_idx = wad.get_subsector_sector_idx(start_subsector, level_data)
    
    def recurse(node_idx):
        if is_ssect_idx(node_idx):
            subsector = ssectors[get_real_idx(node_idx)]
            dest_sector_idx = wad.get_subsector_sector_idx(subsector, level_data)
            if True: #((not draw.pvs_check) or
                # (draw.pvs_check and wad.maybe_line_of_sight(src_sector_idx, dest_sector_idx, level_data))):
                real_idx = get_real_idx(node_idx)
                ssect_callback(real_idx, ssectors[real_idx])
            return

        node = nodes[get_real_idx(node_idx)]

        on_left = is_on_left(node, x, y)
        visiting_left = False
        visiting_right = False

        second_child = None
        frustum_culling = draw.bsp_node_frustum_culling
        
        if on_left:
            first_child = node.left_child
            visiting_left = True
            if (not frustum_culling) or bsp_child_onscreen(node, left_child=False):
                visiting_right = True
                second_child = node.right_child
                                

        else:
            first_child = node.right_child
            visiting_right = True
            if (not frustum_culling) or bsp_child_onscreen(node, left_child=True):
                visiting_left = True
                second_child = node.left_child

                

        
        if node_callback:
            node_callback(node, visiting_left, visiting_right)

        recurse(first_child)
        if second_child:
            recurse(second_child)

    recurse(root_node_idx)


                 
    

            
def find_subsector_idx_for_position(level_data,
                                x, y,
                                node_callback=lambda node: None):
    nodes = level_data['NODES']
    node_idx = len(nodes)-1

    depth = 0
    while True:        
        if is_ssect_idx(node_idx):
            return get_real_idx(node_idx)
        else:
            node = nodes[get_real_idx(node_idx)]
            if is_on_left(node, x, y):
                node_callback(node, depth, on_left=True)
                node_idx = node.left_child
            else:
                node_callback(node, depth, on_left=False)
                node_idx = node.right_child
            depth += 1

            
    
def find_subsector_for_position(level_data,
                                x, y,
                                node_callback=lambda node: None):
    idx = find_subsector_idx_for_position(level_data, x ,y, node_callback)
    return level_data['SSECTORS'][idx]


def iterate_cur_pvs(level_data,
                    x, y,
                    cur_ssect_num,
                    node_callback = lambda node, visiting_left, visiting_right: None,
                    ssect_callback = lambda ssect_idx, ssect: None):
    
    global_pvs = level_data['PVS']

    cur_ssector = level_data['SSECTORS'][cur_ssect_num]
    
    cur_pvs = global_pvs[cur_ssect_num]

    def ssector_callback(ssect_idx, ssect):
        if ssect_idx in cur_pvs:
            ssect_callback(ssect_idx, ssect) 
        

    
    traverse_bsp_front_to_back(level_data,
                               x, y, cur_ssector,
                               node_callback,
                               ssect_callback=ssector_callback)

    
    #ssectors = level_data['SSECTORS'] 
    #for ssect_num in cur_pvs:
    #    ssect_callback(ssectors[ssect_num])
        
    
    
