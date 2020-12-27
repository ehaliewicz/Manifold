import bsp, wad
import math
import skgeom


class Leaf(object):
    def __init__(self, old_ssect):
        self.idx = old_ssect.idx
        self.first_seg = old_ssect.first_seg
        self.num_segs = old_ssect.num_segs
        self.portals = []
        
    def num_leaves(self):
        return 1

    def get_leaf(self, idx):
        if self.idx == idx:
            return self
        else:
            return None

    def add_portal(self, portal):
        self.portals.append(portal)


IN_FRONT   = "IN_FRONT"
BEHIND     = "BEHIND"
SPANNING   = "SPANNING"
COINCIDENT = "COINCIDENT"

EPSILON = 0.001

def intersect_portal_by_plane(portal_x1, portal_y1, portal_x2, portal_y2, plane_x1, plane_y1, plane_x2, plane_y2):
    """ returns a (x, y) tuple or None if there is no intersection """
    
    portal_seg = skgeom.Segment2(skgeom.Point2(portal_x1, portal_y1), skgeom.Point2(portal_x2, portal_y2))
    plane_line = skgeom.Segment2(skgeom.Point2(plane_x1, plane_y1), skgeom.Point2(plane_x2, plane_y2)).supporting_line()
    
    inter = skgeom.intersection(plane_line, portal_seg) 
    #print(inter)
    if inter:
        return (inter.x(), inter.y())

    """
    print("{},{}->{},{} intersect with {},{} -> {},{}".format(Ax1, Ay1, Ax2, Ay2, Bx1, By1, Bx2, By2))
    d = (By2 - By1) * (Ax2 - Ax1) - (Bx2 - Bx1) * (Ay2 - Ay1)
    if d:
        uA = ((Bx2 - Bx1) * (Ay1 - By1) - (By2 - By1) * (Ax1 - Bx1)) / d
        uB = ((Ax2 - Ax1) * (Ay1 - By1) - (Ay2 - Ay1) * (Ax1 - Bx1)) / d
    else:
        print("no :(")
        return
    if not(0 <= uA <= 1 and 0 <= uB <= 1):
        print("no 2 :(")
        return
    x = Ax1 + uA * (Ax2 - Ax1)
    y = Ay1 + uA * (Ay2 - Ay1)
 
    return (x, y)
    """

class Normal(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y


    def dot_product(self, other):
        return (self.x * other.x) + (self.y * other.y)
        


class Line(object):
    def __init__(self, x1, y1, x2, y2):
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2
        self.partition_id = None

    def __str__(self):
        return "<Line: {},{} -> {},{}>".format(self.x1, self.y1, self.x2, self.y2)
    
    def dot_product(self,other):
        return (self.x1 * other.x1) + (self.x2 + other.x2) + (self.y1 + other.y1) + (self.y2 * other.y2)

    def get_distance(self):
        dx = self.x2 - self.x1
        dy = self.y2 - self.y1
        return math.sqrt((dx * dx)+(dy * dy))

    
    def classify_against_plane(self, node):
        
        plane_line = node.get_plane_line()
        self_seg = skgeom.Segment2(skgeom.Point2(self.x1, self.y1),
                                   skgeom.Point2(self.x2, self.y2))
        
        inter = skgeom.intersection(plane_line, self_seg)
        
        if inter:
            if isinstance(inter, skgeom.Segment2):
                return COINCIDENT
            else:
                return SPANNING
        else:
            num_positive = 0
            num_negative = 0
            #print("-- classifying point --")
            for point in [Point(self.x1,self.y1), Point(self.x2, self.y2)]:
                #proj = plane_line.projection(skgeom.Point2(point.x, point.y))
                #print("projection: {}".format(proj))
                val = point.classify_against_plane(node)
                if val == IN_FRONT:
                    #print("one point in front")
                    num_positive += 1
                elif val == BEHIND:
                    #print("one point behind")
                    num_negative += 1
            #print("-- done --\n")

            
            if num_positive == 0 and num_negative > 0:
                return BEHIND
            if num_positive > 0 and num_negative > 0:
                return SPANNING
                #print(self)
                #print(node)
                #raise Exception("This should not happen")
            #    return SPANNING

            if num_positive == 2:
                return IN_FRONT
            elif num_negative == 2:
                return BEHIND

            
            return COINCIDENT

    def split_by_plane(self, node):
        
        plane = node.get_plane_line()

        inter = skgeom.intersection(plane, skgeom.Segment2(skgeom.Point2(self.x1, self.y1),
                                                           skgeom.Point2(self.x2, self.y2)))


        #print(inter)
        

        px1 = node.partition_x_coord
        px2 = px1 + node.dx
        py1 = node.partition_y_coord
        py2 = py1 + node.dy
        
        (ix, iy) = intersect_portal_by_plane(self.x1, self.y1, self.x2, self.y2,
                                             px1, py1, px2, py2)
                                             #plane.x1, plane.y1, plane.x2, plane.y2)

        p1 = Point(self.x1, self.y1)
        p2 = Point(self.x2, self.y2)

        p1_class = p1.classify_against_plane(node)
        p2_class = p2.classify_against_plane(node)

        p1_to_intersection = Line(self.x1, self.y1, ix, iy)
        intersection_to_p2 = Line(ix, iy, self.x2, self.y2)
        if p1_class == IN_FRONT and p2_class == BEHIND:
            return (p1_to_intersection, intersection_to_p2)
        elif p1_class == BEHIND and p2_class == IN_FRONT:
            return (intersection_to_p2, p1_to_intersection)
        else:
            raise Exception("This shouldnt happen")
        
                    
        
        
        

class Point(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y
        
    def classify_against_plane(self, node):

        #plane_line = node.get_plane_line()
        
        #sign = plane_line.oriented_side(skgeom.Point2(self.x, self.y))
        #if sign == skgeom.Sign.NEGATIVE:
        #    return BEHIND
        #else:
        #    return IN_FRONT
                
        dx = self.x - node.partition_x_coord
        dy = self.y - node.partition_y_coord

        val = (((dx * node.dx) - (dy * node.dy)))# <= 0))
        print("val: {}".format(val))
        if val <= EPSILON:
            return BEHIND
        else:
            return IN_FRONT
       
    
        
        
        
                
                

class Node(object):

    def __init__(self, nodes, ssectors, idx):
        #print([(node.idx,
        #        (bsp.is_ssect_idx(node.left_child), bsp.get_real_idx(node.left_child)),
        #        (bsp.is_ssect_idx(node.right_child), bsp.get_real_idx(node.right_child)),
        #) for node in nodes])
        #sys.exit()
        
        old_node = nodes[bsp.get_real_idx(idx)]
        self.idx = idx
        
        self.partition_x_coord = old_node.partition_x_coord 
        self.partition_y_coord = old_node.partition_y_coord
        self.dx = old_node.dx
        self.dy = old_node.dy
        self.right_box_top = old_node.right_box_top
        self.right_box_bottom =  old_node.right_box_bottom
        self.right_box_left = old_node.right_box_left
        self.right_box_right = old_node.right_box_right
        self.left_box_top = old_node.left_box_top
        self.left_box_bottom = old_node.left_box_bottom
        self.left_box_left = old_node.left_box_left
        self.left_box_right = old_node.left_box_right


        right_idx = old_node.right_child
        if bsp.is_ssect_idx(right_idx):
            self.right_child = Leaf(ssectors[bsp.get_real_idx(right_idx)])
        else:
            self.right_child = Node(nodes, ssectors, bsp.get_real_idx(right_idx))
            

        left_idx = old_node.left_child
        if bsp.is_ssect_idx(left_idx):
            self.left_child = Leaf(ssectors[bsp.get_real_idx(left_idx)])
        else:
            self.left_child = Node(nodes, ssectors, bsp.get_real_idx(left_idx))


    def __str__(self):
        return "<Node: {},{} + {},{}>".format(self.partition_x_coord, self.partition_y_coord,
                                              self.dx, self.dy)
        
    def num_partitions(self):
        parts = 1
        if isinstance(self.right_child, Node):
            parts += self.right_child.num_partitions()

        if isinstance(self.left_child, Node):
            parts += self.left_child.num_partitions()

        return parts

    def num_leaves(self):
        leaves = 0
        return (self.right_child.num_leaves() +
                self.left_child.num_leaves())


    def get_leaf(self, idx):
        return (self.right_child.get_leaf(idx) or
                self.left_child.get_leaf(idx))
    
            
            
        

    def get_node_by_idx(self,idx):
        if self.idx == idx:
            return self
        else:
            if isinstance(self.right_child, Node):
                r_node = self.right_child.get_node_by_idx(idx)
                if r_node:
                    return r_node

            if isinstance(self.left_child, Node):
                l_node = self.left_child.get_node_by_idx(idx)
                if l_node:
                    return l_node

            return None

    
    def create_portal_for_partition(self):
        x1 = self.partition_x_coord
        x2 = x1 + self.dx
        y1 = self.partition_y_coord
        y2 = y1 + self.dy

        return Line(x1=x1,y1=y1, x2=x2,y2=y2)

    def get_plane_normal(self):
        ox = self.partition_x_coord
        oy = self.partition_y_coord
        return Normal(-self.dy, self.dx)

    
    def get_plane_line(self):
        px = self.partition_x_coord
        py = self.partition_y_coord
        return skgeom.Segment2(skgeom.Point2(px, px + self.dx),
                               skgeom.Point2(py, py + self.dy)).supporting_line()


    def add_portal(self, portal):
        # are we on the left or right side of this
        
        res = portal.classify_against_plane(self)
        

        if res == COINCIDENT:
            self.left_child.add_portal(portal)
            self.right_child.add_portal(portal)
        elif res == IN_FRONT:
            #print("got in front portal")
            self.right_child.add_portal(portal)
        elif res == BEHIND:
            print("got behind portal")
            self.left_child.add_portal(portal)
        else:
            raise Exception("Got {} when classifying already-clipped portal against node!".format(res))
        
            
        
        
        
class PortalList(object):
    def __init__(self):
        self.portals = []

class SectorManager(object):
    def __init__(self, nodes, ssectors):
                
        root_idx = len(nodes)-1
        print(nodes[root_idx])
        self.bsp_root = Node(nodes, ssectors, root_idx)
        

    def generate_portal_sector_graph(self):
        print("Creating sectors....")
        self.create_sectors()
        print("Creating portal list....")
        self.create_portal_list()
        print("Adding portals to sectors....")
        self.add_portals_to_sectors()
        print("Finding true portals....")
        self.find_true_portals()
        
        
    def create_sectors(self):
        num_sectors = self.bsp_root.num_leaves()
        sectors = []
        for i in range(num_sectors):
            sectors.append(self.bsp_root.get_leaf(i))
        self.sectors = sectors


    def create_portal_list(self):
        num_partitions = self.bsp_root.num_partitions()
        num_portals = num_partitions
        portal_list = []

        # num partitions is the number of root or internal nodes (not leaves) 
        for x in range(num_partitions):
            node_for_partition = self.bsp_root.get_node_by_idx(x)
            cur_portal = node_for_partition.create_portal_for_partition()
            cur_portal.partition_id = x
            portal_list.append(cur_portal)
            

        for x in range(num_partitions):
            node = self.bsp_root.get_node_by_idx(x)
            i = num_portals - 1
            has_more_portals = True
            while has_more_portals:
                portal = portal_list[i]
                retval = portal.classify_against_plane(node)
                if retval == SPANNING:
                    (pfront, pback) = portal.split_by_plane(node)
                    pfront.partition_id = portal.partition_id
                    pback.partition_id = portal.partition_id
                    del portal_list[i]
                    portal_list.append(pfront)
                    portal_list.append(pback)
                    num_portals += 1

                if i != 0:
                    i -= 1
                else:
                    has_more_portals = False

        self.portal_list = portal_list
            
            
    def add_portals_to_sectors(self):
        for portal in self.portal_list:
            self.bsp_root.add_portal(portal)
            

    
    def find_true_portals(self):
        pass

    
