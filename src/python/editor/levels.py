from tkinter import filedialog, messagebox

def export_maps_to_rom(
        map_loader_func # dumb hack to get around circular imports
    ):
    f = filedialog.askopenfile(mode="r")

    textures = set()
    tracks = set()
    with open(f) as maplist:
        maps = maplist.readlines()
        for map in maps:
            map_data = map_loader_func(map)
            level_textures = gather_textures_from_level(map_data)
            textures = textures.union(level_textures)

            tracks.add(map_data.music_path)

    # write textures to WAD area

    # write music to WAD area

    # export maps to ROM with offsets to compiled textures and music
    
def gather_textures_from_level(map_data):
    textures = set()
    for wall in map_data.sectors.walls:
        textures.add(wall.texture_file)
    return textures
    
    

        # get textures
        # get 