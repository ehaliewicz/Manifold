# load and save wad files

# directory

# BWAD -> base wad 

from io import BufferedRandom
from typing import NewType
from file_utils import read_u8, read_u16, read_u32

WadHeader = NewType(dict[str, int])
WadChunk = NewType(tuple[str, bytes])


Wad = NewType(list[WadChunk])

NAME_FIELD_LEN = 32
def read_name(file: BufferedRandom) -> str:
    return str(file.read(NAME_FIELD_LEN))


def read_wad_header(wad_file: BufferedRandom) -> WadHeader:
    # already read the tag and asserted that it was 'BWAD'
    num_directories = read_u16(wad_file)
    header = {}
    for i in range(num_directories):
        # name and offset (length can be in the directory itself?)
        # name is up to 32 characters
        nm = read_name(wad_file)
        off = read_u32(wad_file)
        header[nm] = off
    return WadHeader(header)

def read_wad_chunks(wad_file: BufferedRandom, header: WadHeader) -> list[WadChunk]:
    chunks: list[WadChunk] = []
    for nm, off in header.items():
        wad_file.seek(off)
        chunk_len = read_u32(wad_file)
        chunks.append(WadChunk(tuple(nm, wad_file.read(chunk_len))))
    return chunks



def read_wad(wad_file: BufferedRandom) -> Wad:
    tag = wad_file.read(4)

    assert tag == 'BWAD'
    header = read_wad_header(wad_file)
    chunks = read_wad_chunks(wad_file, header)

    return Wad(chunks)


def export_to_rom(header, directories, wad_bytes):
    pass 