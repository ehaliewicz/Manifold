import itertools, random

def optimize_linedef_entries(entries):
    linedefs_with_vertexes = {}
    for (linedef,v1,v2) in entries: 
        lst = linedefs_with_vertexes.get(v1, list())
        lst.append(linedef)
        linedefs_with_vertexes[v1] = lst
        lst2 = linedefs_with_vertexes.get(v2, list())
        lst2.append(linedef)
        linedefs_with_vertexes[v2] = lst2
    return linedefs_with_vertexes

def count_vertex_transformations(entries):
    prev_vert = None
    transformations = []
    for (l,v1,v2) in entries:
        if v1 == prev_vert:
            # no transformations needed
            pass
        else:
            transformations.append(v1)
        transformations.append(v2)
        prev_vert = v2
    return transformations


def get_linedefs_permutations(entries):
    linedef_to_verts = {}
    for (l,v1,v2) in entries:
        linedef_to_verts[l] = (v1,v2)
    permutations = itertools.permutations(l for (l,_,_) in entries)
    
    res = []
    for perm in permutations:
        perm2 = []
        for l in perm:
            (v1,v2) = linedef_to_verts[l]
            perm2.append((l,v1,v2))
        res.append(perm2)
    return res

def swap_entry(entry):
    (l,v1,v2) = entry
    return (l,v2,v1)
    

def get_vertex_swap_permutations(entries, prefixes=[]):
    #print("got {}".format(entries))
    #print("prefixes: {}".format(prefixes))
    if len(entries) == 0:
        res = prefixes
    elif len(entries) == 1:
        fst = entries[0]
        res = []
        for prefix in prefixes:
            res.append(prefix + [fst])
            res.append(prefix + [swap_entry(fst)])
        
    else:
        fst = entries[0]
        swapped_fst = swap_entry(fst)
        new_prefixes_fst = [pfx+[fst] for pfx in prefixes]
        new_prefixes_swp = [pfx+[swapped_fst] for pfx in prefixes]
        new_prefixes = new_prefixes_fst + new_prefixes_swp
        if len(new_prefixes) == 0:
            new_prefixes = [[fst], [swapped_fst]]
        res = get_vertex_swap_permutations(entries[1:], new_prefixes)

    #print("returning: {}".format(res))
    return res
                

def get_all_possible_permutations(entries):
    res = []
    linedef_perms = get_linedefs_permutations(entries)
    for linedef_entries_permutation in linedef_perms:
        for vertex_perm in get_vertex_swap_permutations(linedef_entries_permutation):
            res.append(vertex_perm)
    return res

def find_best_ordering(entries):
    best_score = None
    best_scoring = None
    for perm in get_all_possible_permutations(entries):
        transformations = count_vertex_transformations(perm)
        score = len(transformations)
        if best_scoring is None or score < best_score:
            best_scoring = perm
            best_score = score

    return best_scoring, best_score
    
    
    

if __name__ == '__main__':
    entries = [(1,'v1','v2'),(2,'v2','v3'), (3,'v0','v1')]
    entries2 = [(1,'v1','v2'),(2,'v2','v3')]

    long_list = [(i, range(50)]
    #print(get_vertex_swap_permutations(entries))
    #print(get_vertex_swap_permutations(entries2))
    
    #print(get_all_possible_permutations(entries))

    print(find_best_ordering(entries))
    
