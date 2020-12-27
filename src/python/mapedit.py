import tkinter as tk

root = tk.Tk()

def do_nothing():
    pass

SEPARATOR = 'separator'

file_commands = [
    ('New', do_nothing),  #new_map
    ('Open', do_nothing), #open_file,
    ('Save', do_nothing), #save_file,
    SEPARATOR,
    ('Exit', root.quit),

]




def setup_menubar():
    
    for thing in file_commands:
        if type(thing) is tuple:
            (label, cmd) = thing
            filemenu.add(label=name, command=cmd)
        elif thing is SEPARATOR:
            filemenu.add_separator()
        else:
            raise Exception("Wtf")
    root.config(menu=menubar)
            



if __name__ == '__main__':
    menubar = tk.Menu(root)
    filemenu = tk.Menu(menubar, tearoff=0)
    canvas = tk.Canvas(root, borderwidth=3, height=640, width=480)
    
    root.mainloop()
