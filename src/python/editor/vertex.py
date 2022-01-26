from utils import point_in_circle, input_int2, draw_list
import imgui


class Vertex():
    def __init__(self, x, y, idx):
        self.x = x
        self.y = y
        self.idx = idx

    def __str__(self):
        return "x: {} y: {}".format(self.x, self.y)

    def __eq__(self, other):
        if isinstance(other, Vertex):
            return self.x == other.x and self.y == other.y
        else:
            return False
        
    def point_collides(self,x,y):
        radius = 5
        x1 = self.x
        y1 = self.y
        x2 = x
        y2 = y

        return point_in_circle(x1,y1,x2,y2,radius)

    
def draw_vert_mode(cur_state):
    
    #if imgui.button("New vertex"):
    #    cur_state.cur_vertex = add_new_vertex()
    
    
    if cur_state.cur_vertex != -1:

        cur_vertex_index = cur_state.cur_vertex
        imgui.text("Vertex {}".format(cur_vertex_index))

        vert = cur_state.map_data.vertexes[cur_vertex_index]
        def set_xy(xy):
            (x,y) = xy
            cur_state.map_data.vertexes[cur_vertex_index].x = x
            cur_state.map_data.vertexes[cur_vertex_index].y = y
            
        input_int2("x,y: ", "##vert{}_xy".format(cur_vertex_index), (vert.x, vert.y),
                   set_xy)                  
    
    

    def set_cur_vertex(idx):
        cur_state.cur_vertex = idx

    draw_list(cur_state, "Vertex", "Vertex list", cur_state.map_data.vertexes, set_cur_vertex)

