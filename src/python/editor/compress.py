
REQUIRED_MIN_MATCH_LEN = 3
from bisect import bisect_left, bisect_right

import math
def bits_to_encode(i):
    n = int(math.log2(i))
    return n+n+1



def compress(bs, max_offset=65536, max_len=65536): #16384, max_len=65536):
    input_len = len(bs)
    output = [None] * input_len * 3


    bits_to_encode_literal_bytes_tbl = [0]
    for i in range(1,max_len+1):
        bits_to_encode_literal_bytes_tbl.append(i*9)
    
    oi = 0

    ii = 0

    byte_buf = 0b00000000
    output_bit_pos = 0

    char_skip_tbl = {}


    for c in range(256):
        char_skip_tbl[c] = 0

    tbl = {}

    
    for i,c in enumerate(bs):
        if c not in tbl:
            tbl[c] = []
        tbl[c].append(i)
        
        
    bits_for_offset_tbl = [0]
    bits_for_length_tbl = [0]
    for i in range(1,max_offset+1):
        bits_for_offset_tbl.append(bits_to_encode(i))
        bits_for_length_tbl.append(bits_to_encode(i))
    
    
    def output_bit(bit,log=True):
        nonlocal byte_buf, output_bit_pos, output, oi
        byte_buf |= (bit << output_bit_pos)
        #if log:
        #    print("outputting bit {}".format(bit))
        output_bit_pos += 1
        if output_bit_pos == 8:
            output[oi] = byte_buf
            oi += 1
            output_bit_pos = 0
            byte_buf = 0

            if oi > len(output):
                output = output + ([None] * len(output))
                

    def output_byte(byte):
        #print("outputting byte {}".format(byte))
        for i in range(8):
            bit = (byte & (0b1<<i))>>i
            output_bit(i,log=False)

    def output_vlint(val):
        #print("outputing vlint {}".format(val))
        n = int(math.log2(val))
       
        for i in range(n):
            output_bit(0, False)

        output_bit(1)
        for i in range(n-1, 0, -1):
            b = val & (1 << i)
            output_bit(b, False)

            
    def flush_bits():
        if output_bit_pos > 0:
            output[oi] = byte_buf
        
        

    last_pct = 0
    while ii < input_len:
        
        pct = int((ii/input_len)*100)
        if pct >= last_pct + 10:
            last_pct = pct
            print("{}%".format(pct))

        got_good_match = False
        best_match_len = -1
        best_match_offset = -1
        
        real_max_offset = ii if ii < max_offset else max_offset
        
        cur_char = bs[ii]

        indexes_of_start_char = tbl[cur_char]
        got_first_valid_idx = False
        skip_idx = char_skip_tbl[cur_char]
        
        for idx_idx in range(skip_idx, len(indexes_of_start_char)):
            index_of_start_char = indexes_of_start_char[idx_idx]
        #for idx_idx, index_of_start_char in enumerate(indexes_of_start_char):
        #for index_of_start_char in indexes_of_start_char:
            
            if index_of_start_char >= ii:
                # don't loop past ii, stop searching for character
                break
            
            
            relative_index = ii - index_of_start_char
            if relative_index > max_offset:
                # if too far back, skip
                continue
            elif not got_first_valid_idx:
                got_first_valid_idx = True
                char_skip_tbl[cur_char] = idx_idx
                
            
            bits_to_encode_cur_char = 9
            #print(relative_index)
            #try:
            bits_to_encode_offset = bits_for_offset_tbl[relative_index]
            #except:
            #    raise Exception(relative_index)

            #min_bits_to_encode_length = 1
            #min_bits_to_encode_pair = bits_to_encode_offset + min_bits_to_encode_length
            #if min_bits_to_encode_pair >= bits_to_encode_cur_char:
            #    # if it's impossible to find a good enough match here, move on
            #    continue

            old_match_base = index_of_start_char
            
            real_max_len = max_len if (input_len - ii) >= max_len else input_len-ii

            for match_len in range(1, real_max_len+1, 1):
                old_match_idx = old_match_base + match_len - 1
                new_match_idx = ii + match_len - 1

                if bs[old_match_idx] != bs[new_match_idx]:
                    break

                bits_to_encode_literal_bytes = bits_to_encode_literal_bytes_tbl[match_len] #(match_len * 9)

                bits_to_encode_match = bits_for_length_tbl[match_len] #bits_to_encode(match_len)
                #bits_to_encode_offset = bits_for_offset_tbl[relative_index]
                #bits_to_encode_offset = bits_to_encode(search_offset)
                bits_to_encode_match_pair = 1 + bits_to_encode_match + bits_to_encode_offset
                if bits_to_encode_match_pair < bits_to_encode_literal_bytes:
                    got_good_match = True
                    if match_len > best_match_len:
                        best_match_len = match_len
                        best_match_offset = relative_index
        """
        
        for search_offset in range(1, real_max_offset):
            
            old_match_base = ii-search_offset

            
            real_max_len = max_len if (input_len - ii) >= max_len else input_len-ii
            
            

            for match_len in range(1, real_max_len+1, 1):
                old_match_idx = old_match_base + match_len - 1
                new_match_idx = ii + match_len - 1

                if bs[old_match_idx] != bs[new_match_idx]:
                    break

                bits_to_encode_literal_bytes = (match_len * 9)

                bits_to_encode_match = bits_to_encode(match_len)
                bits_to_encode_offset = bits_to_encode(search_offset)
                bits_to_encode_match_pair = bits_to_encode_match + bits_to_encode_offset
                if bits_to_encode_match_pair < bits_to_encode_literal_bytes:
                    got_good_match = True
                    if match_len > best_match_len:
                        best_match_len = match_len
                        best_match_offset = search_offset
        """

        if got_good_match:

            output_bit(0)

                
            output_vlint(best_match_offset)
            output_vlint(best_match_len)

            output_byte(best_match_len)

            ii = ii + best_match_len

        else:
            output_bit(1)
            ch = bs[ii]
            output_byte(ch)

            ii = ii +1

            
    flush_bits()
    return output[0:oi]

if __name__ == '__main__':
    #import random
    #random.seed()
    #to_compress = [i%128 for i in range(512)]
    #to_compress = [0,1,2,3,0,1,2,3,0,1]
    import sys
    
    to_compress_file = sys.argv[1]
    with open(to_compress_file, "rb") as f:
        to_compress = f.read()
    #print(to_compress)

    compressed = compress(to_compress)
    
    print("{} bytes compressed to {} bytes".format(len(to_compress), len(compressed)))
    print("{}%".format(len(compressed)/len(to_compress)))

    #print(''.join(bin(i)[2:] for i in compressed))

