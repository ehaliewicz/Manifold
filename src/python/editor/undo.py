import copy

undo_stack = []
redo_stack = []

def undo(cur_state, set_cur_state):
    global undo_stack, redo_stack
    if len(undo_stack) > 0:
        redo_stack.append(cur_state)
        if len(redo_stack) > 100:
            redo_stack = redo_stack[1:] # limit to max of 100 redo's
        set_cur_state(undo_stack.pop())

def redo(cur_state, set_cur_state):
    global undo_stack, redo_stack
    if len(redo_stack) > 0:
        undo_stack.append(cur_state)
        if len(undo_stack) > 100:
            undo_stack = undo_stack[1:] # limit to max of 100 undo's
        set_cur_state(redo_stack.pop())

# call before every user action that mutates the map data
def push_state(cur_state):
    global undo_stack, redo_stack
    undo_stack.append(copy.deepcopy(cur_state))
    redo_stack = []
        
def can_undo():
    return len(undo_stack) > 0

def can_redo():
    return len(redo_stack) > 0