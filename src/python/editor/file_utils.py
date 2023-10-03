
def _read_longword_(f, signed: bool) -> int:
    return int.from_bytes(f.read(4), "big", signed)

def read_s32(f) -> int:
    return _read_longword_(f, True)

def read_u32(f) -> int:
    return _read_longword_(f, False)

def _write_longword_(f, lw: int, signed: bool) -> int:
    off = f.tell()
    f.write(lw.to_bytes(4, byteorder="big", signed=signed))
    return off 

def write_s32(f, lw: int) -> int:
    return _write_longword_(f, lw, True)
def write_u32(f, lw: int) -> int:
    return _write_longword_(f, lw, False)


def _read_word_(f, signed: bool) -> int:
    return int.from_bytes(f.read(2), "big", signed)

def read_s16(f) -> int:
    return _read_word_(f, True)
def read_u16(f) -> int:
    return _read_word_(f, False)

def _write_word_(f, w: int, signed: bool) -> int:
    off = f.tell()
    f.write(w.to_bytes(2, byteorder="big", signed=signed))
    return off 

def write_u16(f, w: int) -> int:
    assert w >= 0 and w < 65536
    _write_word_(f, w, False)

def write_s16(f, w: int) -> int:
    assert w >= -32768 and w < 32768
    _write_word_(f, w, True)


def _read_byte_(f, signed: bool) -> int:
    return int.from_bytes(f.read(1), byteorder="big", signed=signed)
def _write_byte_(f, b: int, signed: bool) -> int:
    off = f.tell()
    f.write(b.to_bytes(1, byteorder="big", signed=signed))
    return off

def read_s8(f) -> int:
    return _read_byte_(f, True)
def read_u8(f) -> int:
    return _read_byte_(f, False)
def write_s8(f, b: int) -> int:
    return _write_byte_(f, b, True)
def write_u8(f, b: int) -> int:
    return _write_byte_(f, b, False)
