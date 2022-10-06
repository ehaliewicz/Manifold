from utils import point_in_circle, input_int2, draw_list
import imgui
import undo


class Vertex():
    def __init__(self, x, y, index):
        self.x = x
        self.y = y
        self.index = index

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
    
    if cur_state.cur_vertex is not None:

        cur_vertex = cur_state.cur_vertex
        imgui.text("Vertex {}".format(cur_vertex.index))

        def set_xy(xy):
            undo.push_state(cur_state)
            (x,y) = xy
            cur_state.cur_vertex.x = x
            cur_state.cur_vertex.y = y
            
        input_int2("x,y: ", "##vert{}_xy".format(cur_vertex.index), (cur_vertex.x, cur_vertex.y),
                   set_xy)                  
    
    

    def set_cur_vertex(idx):
        cur_state.cur_vertex = cur_state.map_data.vertexes[idx]

    def delete_vertex(vert):
        if cur_state.cur_vertex == vert:
            cur_state.cur_vertex = None
        deleted_idx = None 
        for idx,vertex in enumerate(cur_state.map_data.vertexes):
            if vert == vertex:
                undo.push_state(cur_state)
                del cur_state.map_data.vertexes[idx] 
                deleted_idx = idx 
                break
        if deleted_idx is not None:
            for idx,vertex in enumerate(cur_state.map_data.vertexes):
                vertex.index = idx

    # disable deleting vertexes for now
    # i'm not sure what it should do if you delete a vertex that's used by a line
    # maybe we should disallow it
    draw_list(cur_state, "Vertexes", "Vertex list", 
              cur_state.map_data.vertexes, 
              set_cur_vertex) #, delete_vertex)

