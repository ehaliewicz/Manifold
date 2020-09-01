from intervaltree import Interval, IntervalTree

span_buffer = None

def clear_span_buffer():
    global span_buffer
    span_buffer = IntervalTree()
    
def does_interval_overlap(x1, x2):
    return span_buffer.overlaps(Interval(x1, x2))
            
    
def get_clipped_draw_spans(x1, x2):
    overlapping_spans = span_buffer.overlap(x1, x2)

    #for span in spans:

    #    if 
    

def insert_interval(x1, x2):
    pass
    #span_buffer[
    

def reset(max_width):
    global span_buf
    span_buf = [(0, max_width)]

def __span_intersects_span(span1, span2):
    (span1_x1, span1_x2) = span1
    (span2_x1, span2_x2) = span2


    if span1_x2 < span2_x1:
        return False

    if span1_x1 > span2_x2:
        return False

    return True
    
    
    
def span_intersects(span):
    for item in span_buf:
        if(__span_intersects_span(span, item)):
            return True
    return False
    
