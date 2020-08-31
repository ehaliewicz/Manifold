NUM_PIX = 2
NUM_BITS = NUM_PIX+1

def rel_dst_for_bits(bits):
  tbl = [-1,0,1,2]
  return tbl[bits]

def use_tbl_for_bits(bits):
  return bits > 0


def tbl_lookup(name, bits, is_left_side=False):
  if bits > 0:
    if is_left_side:
      return "table_{}_shift[{}]".format(bits-1, name)
    else:
      return "table_{}[{}]".format(bits-1,name)
  else:
    return name

def compile_routine(bits):
  dsts = {}

  for i in range(NUM_PIX):
    pix_bits = ((0b11<<i)&bits)>>i
    rel_dst = rel_dst_for_bits(pix_bits)
    abs_dst = rel_dst + i
    dsts[abs_dst] = (i, pix_bits)

  reads = {}
  writes = {}
  code = []
  for write_off,(read_off,bits) in dsts.items():
    name = 'fire_{}'.format(read_off, write_off)
    reads[read_off] = (name, bits)
    writes[write_off] = (name,bits)
  
  cur_read_off = 0
  for read_off, (name,bits) in reads.items():
    if cur_read_off < read_off:
      change = read_off - cur_read_off 
      code.append('src_ptr += {};'.format(change))
      cur_read_off += change
    code.append('{} = *src_ptr++;'.format(name))
    cur_read_off += 1


  cur_write_off = 0
  wrote_aligned = False

  for write_off,(name,bits) in writes.items():
    if wrote_aligned:
      wrote_aligned = False
      continue

    aligned = (write_off&0b1 == 0)
    sibling = write_off+1 in writes

    if cur_write_off < write_off:
      if write_off > NUM_PIX:
        diff = write_off - cur_write_off
        tbl_var = tbl_lookup(name, bits)
        code.append('*(dst_ptr-{}) = {};'.format(diff, tbl_var))
        continue
      else:
        change = write_off - cur_write_off
        code.append('dst_ptr += {};'.format(change))
        cur_write_off += change
    if aligned and sibling:
      sibling_name,sibling_bits = writes[write_off+1]
      tbl_var = tbl_lookup(name, bits, True)
      sibling_tbl_var = tbl_lookup(sibling_name, sibling_bits)
      code.append("*((u16*)dst_ptr) = {}|{};".format(tbl_var, sibling_tbl_var))
      wrote_aligned = True
    else:
      tbl_var = tbl_lookup(name, bits)
      if write_off < cur_write_off:
        diff = cur_write_off - write_off
        code.append("*(dst_ptr-{}) = {};".format(diff, tbl_var))
      else:
        if cur_write_off >= NUM_PIX:
          code.append("*dst_ptr = {};".format(tbl_var))
        else:
          code.append("*dst_ptr++ = {};".format(tbl_var))
          cur_write_off += 1



  if cur_read_off != NUM_PIX:
    change = NUM_PIX - cur_read_off
    code.append("src_ptr += {};".format(change))

  if cur_write_off != NUM_PIX:
    change = NUM_PIX - cur_write_off
    code.append("dst_ptr += {};".format(change))

  return code



def compile_switch():
  code = ["switch(rand_bits & 0b{})".format('1'*NUM_BITS) + " {"]

  for bits in range(2**NUM_BITS):
    code.append("    case {}:".format(bits))
    for line in compile_routine(bits):
      code.append("        {}".format(line))
    code.append("        break;")

  code.append("}")
  return "\n".join(code)
