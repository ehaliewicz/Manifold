package main


type Map struct {
	name string
	sectors []Sector
}

type Sector struct {
	index int
	floorHeight int
	ceilHeight int
	walls []Wall
}

type Wall struct {
	index int
	v1 int
	v2 int
	gotV1 bool
	gotV2 bool
	adjSector int
	isPortal bool
}

type Vertex struct {
	index int
	x int
	gotX bool
	y int
	gotY bool
}

func freshMap() Map {
	return Map{name: "", sectors: nil}
}


func addNewSector(m *Map) *Sector {
	numSects := len(m.sectors)
	sect := Sector{floorHeight: 0, ceilHeight: 0, walls: nil, index: numSects}
	m.sectors = append(m.sectors, sect)
	return &sect
}

func addNewWall(s *Sector) *Wall {
	wall := Wall{gotV1: false, gotV2: false}
	s.walls = append(s.walls, wall)
	return &wall
}


func setVertexVal(w *Wall, vIndex int, val int) {
	if vIndex == 0 {
		w.v1 = val
		w.gotV1 = true
	} else if vIndex == 1 {
		w.v1 = val
		w.gotV2 = true
	} else {
		panic("Tried to set invalid vertex field!")
	}
}
