import imgui
import numpy as np

def unit_vector(vector):
    """ Returns the unit vector of the vector.  """
    return vector / np.linalg.norm(vector)

def angle_between(v1, v2):
    """ 
    Returns the angle in radians between vectors 'v1' and 'v2'::
    
    >>> angle_between((1, 0, 0), (0, 1, 0))
    1.5707963267948966
    >>> angle_between((1, 0, 0), (1, 0, 0))
    0.0
    >>> angle_between((1, 0, 0), (-1, 0, 0))
    3.141592653589793
    """
    v1_u = unit_vector(v1)
    v2_u = unit_vector(v2)
    rads = np.arccos(np.clip(np.dot(v1_u, v2_u), -1.0, 1.0))
    return rads * 57.29578

def cross(v1, w1_v2, w2_v2):
    x1 = w1_v2.x-v1.x
    y1 = w1_v2.y-v1.y

    x2 = w2_v2.x-v1.x
    y2 = w2_v2.y-v1.y

    return (x1 * y2 - y1 * x2);


def is_convex(sector, map_data):
    if len(sector.walls) < 3:
        return True 
    prev_wall = sector.walls[-1]
    prev = 0
    for idx,wall in enumerate(sector.walls):
        cv1 = wall.v1
        cv2 = wall.v2

        curr = cross(cv1, prev_wall.v1, cv2);
        if curr != 0:
            if curr*prev < 0:
                return False
            else:
                prev = curr

    return True

    



def draw_tree_mode(cur_state):
    disabled = cur_state.map_data.bsp
    if not disabled:
        if imgui.button("blah"):
            cur_state.map_data.bsp = True

            
